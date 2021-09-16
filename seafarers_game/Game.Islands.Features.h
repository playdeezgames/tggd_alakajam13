#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Feature.h"
namespace game::islands
{
	struct Features
	{
		static bool Read(const common::XY<double>&, const game::Feature&);
		static void Write(const common::XY<double>&, const game::Feature&, bool);
		static void Reset(const game::Difficulty&);
	};
}