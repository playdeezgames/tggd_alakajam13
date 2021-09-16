#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game::World
{
	common::XY<double> GetSize();
	int GetVersion();
	double GetMinimumIslandDistance();
	double GetViewDistance();
	double GetDockDistance();
	double GetWindHeading();
	void SetWindHeading(double);
	void Reset(const game::Difficulty&);
}
