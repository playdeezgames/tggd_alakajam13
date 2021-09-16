#pragma once
#include "Game.Avatar.PlightType.h"
#include <optional>
#include <string>
namespace game::avatar
{
	struct PlightDescriptor
	{
		std::string name;
		PlightType type;
		std::optional<size_t> durationMinimum;
		std::optional<size_t> durationMaximum;
		size_t generationWeight;
	};
}
