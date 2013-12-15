#pragma once

#include <wx/wx.h>
#include <thread>
#include "gui_frame.h"
#include "gui_slider.h"
#include "../gpio_client_actor.h"

class gui_main : public wxApp 
{
public:

	virtual bool OnInit() 
	{
		std::cout << "gui_main_init" << std::endl;

		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		frame_ = new gui_frame(_("hddda"), wxPoint(50, 50), wxSize(400, 340));

		std::string address = frame_->get_ip_dialog();
		if (address.length() == 0)
		{
			return false;
		}

		message response = client_actor->future(GPIO_CONNECT);
		if (response.type != GPIO_CONNECTED)
		{
			frame_->alert("Tried to connect to "+address, "Failed");
		}

		slider_ = new gui_slider(frame_, wxPoint(40, 40), wxSize(180, 20), 0, 100, wxHORIZONTAL, [](int value)
		{
			std::cout << value << std::endl;
		});

		frame_->Show(true);
		
		return true;
	}

	virtual int OnExit()
	{
		return 0;
	}

private:
	gui_frame* frame_;
	gui_slider* slider_;
	std::unique_ptr<gpio_client_actor> client_actor;
};