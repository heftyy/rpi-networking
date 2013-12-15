#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "gpio_error.h"
#include "gpio_pins_message.h"
#include "gpio_pwm_actor.h"

class gpio_client
{
public:
	std::vector<gpio_pin> pins;

	gpio_client(actor_system& actor_system) : actor_system_(actor_system) {}

	void set_pins(gpio_pins_message& pins_message)
	{
		this->pins = pins_message.pins;
	}

	gpio_pins_message get_pins()
	{
		gpio_pins_message message(pins);
		return message;
	}

private:
	actor_system& actor_system_;

	bool is_pin_available(int pin)
	{
		bool safe = false;
		std::for_each(this->pins.begin(), this->pins.end(), [pin, &safe](const gpio_pin& gpio_pin)
		{
			if (pin == gpio_pin.pin) safe = true;
		});
		return false;
	}

	gpio_pin get_pin(int pin)
	{
		gpio_pin return_pin;
		std::for_each(this->pins.begin(), this->pins.end(), [pin, &return_pin](const gpio_pin& gpio_pin)
		{
			if (pin == gpio_pin.pin) return_pin = gpio_pin;
		});
		//
		if (return_pin.pin == -1) throw gpio_error(std::string("error, pin ") + std::string(pin + "") + std::string(" is not safe"));
		return return_pin;
	}
};