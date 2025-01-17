#pragma once

#include <vector>
#include <array>
#include <sstream>
#include <memory>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "gpio_pin.h"

class gpio_pins_message
{
public:
	friend class boost::serialization::access;
	std::vector<gpio_pin> pins;

	gpio_pins_message() {}
	gpio_pins_message(std::vector<gpio_pin>& pins) : pins(pins) {}
	gpio_pins_message(std::string& received_data)
	{
		std::istringstream is(received_data);

		boost::archive::text_iarchive in_archive(is);
		gpio_pins_message* pins = new gpio_pins_message;
		in_archive >> pins;
		this->pins = pins->pins;
	}

	std::string get_serialized()
	{
		std::ostringstream archive_stream;
		boost::archive::text_oarchive out_archive(archive_stream);
		out_archive << this;
		std::string serialized_data;
		return archive_stream.str();
	}

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & pins;
	}
};