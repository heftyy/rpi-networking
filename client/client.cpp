#include <vector>
#include <iostream>
#include <atomic>
#include <memory>
#include <atan/actor_system/actor_system.h>
//#include "gpio_client_actor.h"
#include "gui/gui_main.h"

int main(int argc, char** argv)
{
	wxApp::SetInstance(new gui_main());
	wxEntryStart(argc, argv);
	wxTheApp->OnInit();

	std::cout << "main test" << std::endl;

	wxTheApp->OnRun();
	wxTheApp->OnExit();
	wxEntryCleanup();

	/*
	std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;

	try
	{
		actor_system as("client_system", 8558);
		std::string rpi_actor_ref = "gpio_actor$raspberry_pi@127.0.0.1:8556";
		auto actor = std::shared_ptr<gpio_client_actor>(new gpio_client_actor(rpi_actor_ref, as, [](std::string msg)
		{
			std::cout << msg << std::endl;
		}));
		as.add_actor(actor);

		message response = actor->future(GPIO_CONNECT, 1000);
		if (response.type != GPIO_CONNECTED)
		{
			std::cout << "type is not GPIO_CONNECTED" << std::endl;
		}
		else if (response.type == GPIO_CONNECTED)
		{
			std::cout << "connect successed" << std::endl;
		}

		actor->tell(GPIO_REQUEST_PINS);

		_sleep(2000);
		//as.get_actor("test_actor");
	}
	catch (std::runtime_error)
	{
		std::cerr << "runtime error" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Got an exception!";
	}
	*/

	std::cout << "PROGRAM STOP" << std::endl;
	return 0;
}