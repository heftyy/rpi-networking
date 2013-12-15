#pragma once

#include <wx/wx.h>
#include <thread>
#include <atan/actor_system/actor_system.h>
#include "gui_frame.h"
//#include "gui_slider.h"
#include "../gpio_client_actor.h"
#include "sliders_panel.h"
#include "gui_sliders_panel.h"

#define SLIDER_HEIGHT 20
#define SLIDER_WIDTH 150

class gui_main : public wxApp
{
public:
	const static int ID_MAIN = 100;

	gui_main()
	{
		actor_system_ = std::unique_ptr<actor_system>(new actor_system("client_system", 8558));
		std::string rpi_actor_ref = "gpio_actor$raspberry_pi@127.0.0.1:8556";
		client_actor_ = std::shared_ptr<gpio_client_actor>(new gpio_client_actor(rpi_actor_ref, *actor_system_, [this](gpio_pins_message msg)
		{
			//render_pins(msg.pins);
		}));
		actor_system_->add_actor(client_actor_);
	}

	virtual bool OnInit()
	{
		frame_ = new gui_frame(_("hddda"), wxPoint(50, 50), wxSize(400, 800));

		/*
		slidersPage *p = new slidersPage(frame_);
		p->render();
		*/

		if (!connect_to_rpi()) this->Exit();
		request_pins();

		frame_->Show(true);
		return true;
	}

	bool connect_to_rpi()
	{
		std::string address = frame_->get_ip_dialog();
		if (address.length() == 0)
		{
			return false;
		}

		message response = client_actor_->future(GPIO_CONNECT);
		if (response.type != GPIO_CONNECTED)
		{
			frame_->alert("Tried to connect to " + address, "Failed");
			return false;
		}
		return true;
	}

	void request_pins()
	{
		message msg = client_actor_->future(GPIO_REQUEST_PINS, 1000);
		gpio_pins_message pins_msg(msg.data);

		gui_sliders_panel *sliders_panel = new gui_sliders_panel(frame_, [this](int pin_id, int value)
		{
			std::cout << "pin_id = " << pin_id << " value = " << value << std::endl;
			this->update_pin_pwm(pin_id, value);
		});
		sliders_panel->render(pins_msg.pins);
	}

	void update_pin_pwm(int pin_id, int value)
	{
		std::vector<gpio_pin> pins;
		gpio_pin pin;
		pin.pin = pin_id;
		pin.value = value;
		pin.pwm = true;
		pin.range = 100;
		pin.mode = GPIO_MODE_OUT;
		pins.push_back(pin);
		gpio_pins_message pins_msg(pins);
		client_actor_->tell(GPIO_SET_PINS, pins_msg.get_serialized());
	}

	virtual int OnExit()
	{
		return 0;
	}

private:
	gui_frame* frame_;
	std::shared_ptr<gpio_client_actor> client_actor_;
	std::unique_ptr<actor_system> actor_system_;
};