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
#include "msg_pusher.hpp"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/ref.hpp>

#include <libhexabus/device.hpp>
#include <libhexabus/endpoint_registry.hpp>

#include <libklio/store-factory.hpp>

using namespace msg_pusher;

namespace bf = boost::filesystem;

std::string _device_name = "Pusher";
uint32_t eid = 2;

Pusher::Pusher(boost::asio::io_service& io, const std::vector<std::string>& interfaces, const std::vector<std::string>& addresses,
		const std::string& url, const std::string& id, const std::string& token, int interval, bool debug)
	: _device(io, interfaces, addresses, interval)
	, _debug(debug)
{
	klio::StoreFactory store_factory;

	//TODO: url handling
	_store = store_factory.create_msg_store();

	klio::SensorFactory sensor_factory;
	_sensor = sensor_factory.create_msg_sensor(
		"uid123",
		id,
		"test",
		"description",
		"watt",
		"Europe/Berlin",
		token);
	_store->add_sensor(_sensor);

	_init();
}

void Pusher::_init() {
	_device.onReadName(boost::bind(&Pusher::loadDeviceName, this));
	_device.onWriteName(boost::bind(&Pusher::saveDeviceName, this, _1));

	hexabus::EndpointRegistry ep_registry;
	hexabus::EndpointRegistry::const_iterator ep_it;

	ep_it = ep_registry.find(eid);
	hexabus::TypedEndpointFunctions<uint32_t>::Ptr powerEP = ep_it != ep_registry.end()
		? hexabus::TypedEndpointFunctions<uint32_t>::fromEndpointDescriptor(ep_it->second)
		: hexabus::TypedEndpointFunctions<uint32_t>::Ptr(new hexabus::TypedEndpointFunctions<uint32_t>(eid, "HexabusPlug+ Power meter (W)"));
	powerEP->onRead(boost::bind(&Pusher::get_last_reading, this));
	_device.addEndpoint(powerEP);
}

uint32_t Pusher::get_last_reading() {
	try {
		klio::reading_t reading = _store->get_last_reading(_sensor);
		std::cout << reading.first << ": " << reading.second << std::endl;
		return reading.second;
	} catch (klio::GenericException e) {
		std::cerr << "Error during fetch of sensor data: " << e.what() << std::endl;
	}
	return 0;
}

std::string Pusher::loadDeviceName()
{
	_debug && std::cout << "loading device name" << std::endl;
	std::string name;
	name = _device_name;

	return name;
}

void Pusher::saveDeviceName(const std::string& name)
{
	_debug && std::cout << "saving device name \"" << name << "\"" << std::endl;
	if ( name.size() > 30 )
	{
		std::cerr << "cannot save a device name of length > 30" << std::endl;
		return;
	}

	_device_name = name;
}
