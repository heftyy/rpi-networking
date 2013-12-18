#pragma once

#include <wiringPi.h>
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
	gpio_server()
	{
		wiringPiSetup();
		int pins[] = { 0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14 };
		int pin_labels[] = { 17, 18, 21, 22, 12, 24, 25, 8, 7, 10, 9, 11 };
		std::vector<gpio_pin> v;
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
		int pin_iter = get_pin_iter(pin);
		pins[pin_iter].pin = pin;
		pins[pin_iter].value = value;
		pins[pin_iter].range = range;
		if (!pins[pin_iter].pwm_actor)
		{
			pinMode(pin, GPIO_MODE_OUT);
			pins[pin_iter].pwm_actor = std::shared_ptr<gpio_pwm_actor>(new gpio_pwm_actor("pwm_actor" + pin, actor_system, value, range, pin));
			pins[pin_iter].pwm_actor->tell(GPIO_PWM_START);
			pins[pin_iter].pwm = true;
		}
		else if (value == 0 || range == 0)
		{
			pins[pin_iter].pwm_actor->tell(GPIO_PWM_STOP);
			pins[pin_iter].pwm = false;
		}
		else
		{
			pins[pin_iter].pwm_actor->tell(GPIO_PWM_UPDATE, pins[pin_iter].get_serialized());
		}
	}

	void write(int pin, bool value)
	{
		if (is_pin_available(pin))
		{
			pinMode(pin, GPIO_MODE_OUT);
			digitalWrite(pin, value);
		}
	}

	int read(int pin)
	{
		if (is_pin_available(pin))
		{
			pinMode(pin, GPIO_MODE_IN);
			return digitalRead(pin);
		}
	}
private:
	std::vector<gpio_pin> pins;

	bool is_pin_available(int pin)
	{
		for (unsigned int i = 0; i < pins.size(); i++)
		{
			if (pins[i].pin == pin) return true;
		}
		return false;
	}

	int get_pin_iter(int pin)
	{
		for (unsigned int i = 0; i < pins.size(); i++)
		{
			if (pins[i].pin == pin) return i;
		}
	}
};