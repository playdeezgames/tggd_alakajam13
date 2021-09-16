#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
namespace game
{
	struct Fishery
	{
		common::XY<double> location;
		double radius;
		Fish fish;
	};
}
