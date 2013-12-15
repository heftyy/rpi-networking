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
	friend class boost::serialization::access;
	int pin;
	int pin_label;
	int value;
	int range;
	int mode;
	bool pwm;
	std::shared_ptr<gpio_pwm_actor> pwm_actor;

	gpio_pin() : pin(-1), mode(-1), pwm(false) {}
	gpio_pin(int pin, int pin_label) : pin(pin), pin_label(pin_label), value(0), range(0), pwm(false), pwm_actor(nullptr), mode(-1) {}

	gpio_pin(std::string received_data)
	{
		std::istringstream is(received_data);

		boost::archive::text_iarchive in_archive(is);
		gpio_pin *pin = new gpio_pin;
		in_archive >> pin;
		std::memcpy(this, pin, sizeof(gpio_pin));
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