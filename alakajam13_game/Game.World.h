#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game::World
{
	int GetVersion();
	void Reset(const game::Difficulty&);
}
