#pragma once
#include "Game.Demigod.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <list>
namespace game
{
	enum class OfferingResult
	{
		COOLING_DOWN,
		SUCCESS,
		BLESSING,
		CURSE,
		FAILURE
	};
	struct Demigods
	{
		static void Reset(const Difficulty&);
		static OfferingResult MakeOffering(const std::string&, const Item&);
		static void ApplyTurnEffects();
	};
}