#include <iostream>
#include <atan/actor_system/actor_system.h>
#include "gpio_actor.h"

int main(int argc, char* argv[])
{
	try
	{
		actor_system actor_system("raspberry_pi", 8556);
		auto actor = std::shared_ptr<gpio_actor>(new gpio_actor(actor_system));
		actor_system.add_actor(actor);

		std::string input;
		while (1)
		{
			std::cin >> input;
			if (input.compare("quit") == 0)
			{
				break;
			}
		}

		actor_system.stop();
	}
	catch (std::runtime_error &e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
