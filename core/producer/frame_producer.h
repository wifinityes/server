/*
* Copyright (c) 2011 Sveriges Television AB <info@casparcg.com>
*
* This file is part of CasparCG (www.casparcg.com).
*
* CasparCG is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CasparCG is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with CasparCG. If not, see <http://www.gnu.org/licenses/>.
*
* Author: Robert Nagy, ronag89@gmail.com
*/

#pragma once

#include "../monitor/monitor.h"

#include <common/forward.h>
#include <common/spl/memory.h>
#include <common/enum_class.h>

#include <stdint.h>
#include <limits>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

FORWARD1(caspar, class executor);
FORWARD1(boost, template<typename T> class unique_future);

namespace caspar { namespace core {
	
struct frame_producer : public monitor::observable
					  , boost::noncopyable
{
	struct flags_def
	{
		enum type
		{
			none		= 0,
			alpha_only	= 2,
			deinterlace	= 4,
		};
	};
	typedef enum_class<flags_def> flags;

	virtual ~frame_producer(){}	

	virtual std::wstring print() const = 0; // nothrow
	virtual std::wstring name() const = 0;
	virtual boost::property_tree::wptree info() const = 0;

	virtual boost::unique_future<std::wstring> call(const std::wstring&);

	virtual void leading_producer(const spl::shared_ptr<frame_producer>&) {}  // nothrow
		
	virtual uint32_t nb_frames() const {return std::numeric_limits<uint32_t>::max();}
	
	virtual spl::shared_ptr<class draw_frame> receive(int fBlags) = 0;
	virtual spl::shared_ptr<class draw_frame> last_frame() const;
	
	static const spl::shared_ptr<frame_producer>& empty(); // nothrow

	// monitor::observable

	virtual void subscribe(const monitor::observable::observer_ptr& o) {}
	virtual void unsubscribe(const monitor::observable::observer_ptr& o) {}
};

typedef std::function<spl::shared_ptr<core::frame_producer>(const spl::shared_ptr<struct frame_factory>&, const std::vector<std::wstring>&)> producer_factory_t;
void register_producer_factory(const producer_factory_t& factory); // Not thread-safe.

spl::shared_ptr<core::frame_producer> create_producer(const spl::shared_ptr<frame_factory>&, const std::vector<std::wstring>& params);
spl::shared_ptr<core::frame_producer> create_producer(const spl::shared_ptr<frame_factory>&, const std::wstring& params);

spl::shared_ptr<core::frame_producer> wrap_producer(spl::shared_ptr<core::frame_producer> producer);
		
}}
