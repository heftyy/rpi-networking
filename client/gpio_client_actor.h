#pragma once
#include <iostream>
#include <atan/actor/remote_actor.h>
#include <gpio_settings.h>
#include "gpio_pins_message.h"

class gpio_client_actor : public remote_actor
{
public:
	gpio_client_actor(std::string& remote_actor_ref, actor_system& actor_system, std::function<void(gpio_pins_message)> receive_pins_function, int actor_sleep_ms = 10)
		: remote_actor("gpio_client_actor", actor_system, actor_ref(remote_actor_ref), actor_sleep_ms), receive_pins_function_(receive_pins_function)
	{
		//actor_system_.add_actor(std::shared_ptr<test_actor>(this));
	}
	~gpio_client_actor()
	{
		std::cout << "gpio_client_actor destructor" << std::endl;
	}

private:
	std::function<void(gpio_pins_message)> receive_pins_function_;

	void on_receive(message msg)
	{
		std::cout << "[GPIO_CLIENT_ACTOR] on_receive thread id = " << std::this_thread::get_id() << std::endl;
		std::cout << "gpio_client_actor received message from " << msg.sender.actor_name << std::endl;
		std::cout << "message was " << std::string(msg.data.begin(), msg.data.end()) << std::endl;

		if (msg.type == GPIO_PINS_LIST)
		{
			gpio_pins_message pins_msg = gpio_pins_message(msg.data);
			receive_pins_function_(pins_msg);
		}

		std::string msg_string = std::string(msg.data.begin(), msg.data.end());
		//reply(msg_string, msg.sender);
	}
};
