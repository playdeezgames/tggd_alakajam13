#pragma once
#include "Game.ActorType.h"
#include <Common.XY.h>
namespace game
{
	struct Actor
	{
		ActorType actorType;
		common::XY<int> location;
	};
}
