#pragma once
#include "Game.Difficulty.h"
#include "Game.Ship.h"
namespace game::avatar
{
	struct Ship
	{
		static void Reset(const game::Difficulty&);
		static void Write(const game::Ship&);
		static game::Ship Read();
		static double AvailableTonnage();
	};
}
