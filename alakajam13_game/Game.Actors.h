#pragma once
#include "Game.Actor.h"
#include "Game.Difficulty.h"
#include <optional>
#include <vector>
namespace game
{
	struct Actors
	{
		static void Next();
		static void AddActor(const Actor&);
		static const Actor& GetCurrent();
		static void Reset(const Difficulty&);
		static const std::vector<Actor>& All();
		static bool MoveActor(const common::XY<int>&);
	};
}