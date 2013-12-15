#pragma once

#include <atan\actor\local_actor.h>
#include "gpio_pins_message.h"
#include "gpio_settings.h"

class gpio_pwm_actor : public local_actor
{
public:
	gpio_pwm_actor(std::string actor_name, actor_system& actor_system, int value, int range, int pin, int actor_sleep_ms = 10)
	: local_actor(actor_name, actor_system, actor_sleep_ms), value_(value), range_(range), pin_(pin)
	{
	}

	~gpio_pwm_actor()
	{
		gpio_pwm_stop();
	}

private:
	std::unique_ptr<interruptible_thread> pwm_thread_;
	int value_;
	int range_;
	int pin_;

	void on_receive(message& msg)
	{
		if (msg.type == GPIO_PWM_START)
		{
			gpio_pwm_start();
		}
		else if (msg.type == GPIO_PWM_STOP)
		{
			gpio_pwm_stop();
		}
		else if (msg.type == GPIO_PWM_UPDATE)
		{
			auto received_pin = std::unique_ptr<gpio_pin>(new gpio_pin(msg.data));
			if (this->pin_ == received_pin->pin)
			{
				this->value_ = received_pin->value;
				this->range_ = received_pin->range;
			}
		}
	}

	void gpio_pwm_start()
	{
		pwm_thread_ = std::unique_ptr<interruptible_thread>(new interruptible_thread());
		pwm_thread_->start_thread([this]() {
			pwm_thread_->check_for_interrupt();
			/*
			std::chrono::milliseconds sleep_duration(gpio_sleep_ms_);
			std::this_thread::sleep_for(sleep_duration);
			std::cout << "PWM" << std::endl;
			*/
		});
		std::cout << "[GPIO_ACTOR] task finished" << std::endl;
	}

	void gpio_pwm_stop()
	{
		if (pwm_thread_)
		{
			pwm_thread_->stop();
			pwm_thread_.release();
		}
	}
};