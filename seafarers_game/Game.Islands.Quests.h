#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::islands
{
	struct Quests
	{
		static void Update(const common::XY<double>&);
		static std::optional<game::Quest> Read(const common::XY<double>&);
	};
}
