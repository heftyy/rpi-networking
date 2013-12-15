#pragma once

#include <vector>
#include <array>
#include <sstream>
#include <memory>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

class gpio_pwm_actor;

/**
	value = the time pin will be on in specified range 
	range = hz
*/
class gpio_pin
{
public:
	int pin;
	int pin_label;
	int value;
	int range;
	int mode;
	bool pwm;
	std::shared_ptr<gpio_pwm_actor> pwm_actor;

	gpio_pin() : pin(-1), pwm(false) {}
	gpio_pin(const gpio_pin& gpio_pin) {}
	gpio_pin(int pin, int pin_label) : pin(pin), pin_label(pin_label), value(0), range(0), pwm(false), pwm_actor(nullptr) {}

	gpio_pin(std::string received_data)
	{
		std::istringstream is(received_data);

		boost::archive::text_iarchive in_archive(is);
		gpio_pin gpio_pin;
		in_archive >> gpio_pin;
		std::memcpy(this, &gpio_pin, sizeof(class gpio_pin));
		/*
		this->pin = gpio_pin.pin;
		this->pin_label = gpio_pin.pin_label;
		this->value = gpio_pin.value;
		this->range = gpio_pin.range;
		this->mode = gpio_pin.mode;
		this->pwm = gpio_pin.pwm;
		*/
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
		ar & pin;
		ar & pin_label;
		ar & value;
		ar & range;
		ar & mode;
		ar & pwm;
	}
};

class gpio_pins_message
{
public:
	friend class boost::serialization::access;
	std::vector<gpio_pin> pins;
	gpio_pins_message() {}
	gpio_pins_message(std::vector<gpio_pin>& pins) : pins(pins) {}
	gpio_pins_message(std::string received_data)
	{
		std::istringstream is(received_data);

		boost::archive::text_iarchive in_archive(is);
		gpio_pins_message pins;
		in_archive >> pins;
		this->pins = pins.pins;
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