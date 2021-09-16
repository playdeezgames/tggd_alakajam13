#pragma once
#include "Game.Avatar.State.h"
#include <string>
namespace game::avatar
{
	struct StateTransition
	{
		std::string logColor;
		std::string logText;
		State dockedState;
	};
}

