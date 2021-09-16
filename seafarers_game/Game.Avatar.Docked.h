#pragma once
#include <Common.XY.h>
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar
{
	struct Docked
	{
		enum class DockResult
		{
			DOCKED,
			COMPLETED_QUEST,
			ALREADY_DOCKED
		};
		static void Reset(const game::Difficulty&);
		static std::optional<DockResult> Dock();
		static std::optional<common::XY<double>> GetDockedLocation();
	};

}
