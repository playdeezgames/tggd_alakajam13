#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct World
	{
		int version;
		static void Write(const World&);
		static std::optional<World> Read();
	};
}