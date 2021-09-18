#pragma once
#include "Game.ActorType.h"
#include <Common.XY.h>
#include "Game.Statistic.h"
#include <map>
namespace game
{
	struct Actor
	{
		ActorType actorType;
		common::XY<int> location;
		std::map<Statistic, int> statistics;
	};
}
