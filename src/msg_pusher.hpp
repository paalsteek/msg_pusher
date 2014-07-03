/**
 * This file is part of msg_pusher.
 *
 * (c) Fraunhofer ITWM - Stephan Platz <platz@itwm.fhg.de>, 2014
 *
 * msg_pusher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * msg_pusher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with msg_pusher. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _MSG_PUSHER_HPP
#define _MSG_PUSHER_HPP

#include <boost/asio/io_service.hpp>

#include <libhexabus/device.hpp>

#include <libklio/sensor.hpp>
#include <libklio/msg/msg-store.hpp>

namespace msg_pusher {
	class Pusher {
		public:
			typedef boost::shared_ptr<Pusher> Ptr;
			Pusher(boost::asio::io_service& io, const std::vector<std::string>& interfaces, const std::vector<std::string>& addresses, const std::string& url, const std::string& id, const std::string& token, int interval = 60, bool debug = false);
			virtual ~Pusher() {};

			std::string loadDeviceName();
			void saveDeviceName(const std::string& name);

			uint32_t get_last_reading();

		private:
			void _init();

		private:
			hexabus::Device _device;
			klio::MSGStore::Ptr _store;
			klio::Sensor::Ptr _sensor;
			bool _debug;
	};
}

#endif // _HEXABUS_SERVER_HPP
