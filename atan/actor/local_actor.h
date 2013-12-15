#pragma once

#include "actor.h"
#include "../actor_system/actor_system_errors.h"

class local_actor : public actor
{
public:
	local_actor(const std::string& name, actor_system& actor_system, int actor_sleep_ms = 10) 
		: actor(name, actor_system, actor_sleep_ms) {}

	void tell(const message& msg)
	{
		add_message(msg);
	}

	void tell(int type, std::string msg = "", std::shared_ptr<actor> sender_actor = nullptr)
	{
		atan_error(ATAN_WRONG_ACTOR_METHOD, "network tell in local_actor");
	}
};