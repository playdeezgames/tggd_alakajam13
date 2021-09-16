#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar
{
	struct AtSea
	{
		enum class MoveResult
		{
			MOVED,
			CLAMPED
		};

		static void Reset(const game::Difficulty&);

		static double GetHeading();
		static void SetHeading(double);

		static double GetSpeed();
		static void SetSpeed(double);

		static common::XY<double> GetLocation();

		static MoveResult Move();
		static void ApplyTurnEffects();
	};
}
