#pragma once

#include <wiringPi.h>
#include <softPwm.h>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "gpio_error.h"
#include "gpio_pins_message.h"
#include "gpio_pwm_actor.h"

class gpio_server
{
public:
	gpio_server(actor_system& actor_system) : actor_system_(actor_system)
	{
		wiringPiSetup();
		int pins[] = { 0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14 };
		int pin_labels[] = { 17, 18, 21, 22, 12, 24, 25, 8, 7, 10, 9, 11 };
		for (int i = 0; i < 12; i++)
		{
			this->pins.push_back(gpio_pin(pins[i], pin_labels[i]));
		}
	}

	gpio_pins_message get_pins()
	{
		gpio_pins_message message(pins);
		return message;
	}

	void pwm(actor_system& actor_system, int pin, int value, int range = 100)
	{
		auto gpio_pin = get_pin(pin);
		gpio_pin.pin = pin;
		gpio_pin.value = value;
		gpio_pin.range = range;		
		if (!gpio_pin.pwm_actor)
		{
			pinMode(pin, WIRING_PI_OUTPUT);
			gpio_pin.pwm_actor = std::shared_ptr<gpio_pwm_actor>(new gpio_pwm_actor("pwm_actor" + pin, actor_system, value, range, pin));
			gpio_pin.pwm_actor->tell(GPIO_PWM_START);
			gpio_pin.pwm = true;
		}
		else if (value == 0 || range == 0)
		{
			gpio_pin.pwm_actor->tell(GPIO_PWM_STOP);
			gpio_pin.pwm = false;
		}
		else
		{
			gpio_pin.pwm_actor->tell(GPIO_PWM_UPDATE, gpio_pin.get_serialized());
		}
	}

	void write(int pin, bool value)
	{
		if (is_pin_available(pin))
		{
			pinMode(pin, WIRING_PI_OUTPUT);
		}
	}

	int read(int pin)
	{
		if (is_pin_available(pin))
		{
			pinMode(pin, WIRING_PI_INPUT);
			return digitalRead(pin);
		}
	}
private:
	actor_system& actor_system_;
	std::vector<gpio_pin> pins;

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