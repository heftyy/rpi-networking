#pragma once

#include <iostream>
#include <atomic>
#include <atan/interruptible_thread.h>
#include <atan/actor/local_actor.h>
#include <atan/actor_system/actor_system.h>
#include "gpio_server.h"
#include "gpio_settings.h"

class gpio_actor: public local_actor
{
public:
	gpio_actor(actor_system& actor_system, int actor_sleep_ms = 10) : local_actor("gpio_actor", actor_system, actor_sleep_ms)
	{
		gpio_ = std::unique_ptr<gpio_server>(new gpio_server());
	}

	~gpio_actor()
	{
	}

private:
	std::unique_ptr<gpio_server> gpio_;

	void on_receive(const message& msg)
	{
		if (msg.type == GPIO_REQUEST_PINS)
		{
			gpio_pins_message pins = gpio_->get_pins();
			reply(GPIO_PINS_LIST, pins.get_serialized(), msg.sender);
		}
		else if (msg.type == GPIO_SET_PINS)
		{
			gpio_pins_message pins_message = gpio_pins_message(msg.data);
			for (auto pin : pins_message.pins)
			{
				if (pin.pwm)
				{
					gpio_->pwm(actor_system_, pin.pin, pin.value, pin.range);
				}
				else
				{
					gpio_->write(pin.pin, pin.value > 0 ? true : false);
				}
			}
		}
		else if (msg.type == GPIO_CONNECT)
		{
			reply(GPIO_CONNECTED, "", msg.sender);
		}
	}
};
