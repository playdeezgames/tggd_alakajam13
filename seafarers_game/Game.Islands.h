#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Island.h"
#include <list>
#include <optional>
#include <string>
namespace game
{
	struct Islands
	{
		static void Reset(const game::Difficulty&);
		static std::list<Island> GetViewableIslands();
		static std::list<Island> GetDockableIslands();
		static bool CanDock();
		static std::list<Island> GetKnownIslands();
		static void AddVisit(const common::XY<double>&, const int&);
		static void SetKnown(const common::XY<double>&, const int&);
		static std::optional<Island> Read(const common::XY<double>&);
		static const std::string UNKNOWN;
		static void ApplyTurnEffects();
	};
}
