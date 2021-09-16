#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Merchant.h"
#include <list>
namespace game
{
	struct Merchants
	{
		static void Spawn(const common::XY<double>&);
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&);
		static std::list<Merchant> All();
	};
}