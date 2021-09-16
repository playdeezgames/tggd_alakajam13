#pragma once
#include <Common.XY.h>
#include <functional>
#include "Game.Fish.h"
#include <list>
#include <string>
namespace game
{
	struct FishDescriptor
	{
		std::string name;
		std::function<size_t()> countGenerator;
		std::function<double()> radiusGenerator;
		std::function<double()> speedGenerator;
		std::list<common::XY<int>> shape;
		common::XY<size_t> size;
	};
}
