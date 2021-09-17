#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.World.h"
#include <map>
namespace game::World
{
	const int CURRENT_VERSION = 1;

	int GetVersion()
	{
		return data::game::World::Read().value().version;
	}

	struct WorldProperties
	{
	};

	const std::map<game::Difficulty, WorldProperties> worldProperties =
	{
		{game::Difficulty::EASY, {}},
		{game::Difficulty::NORMAL, {}},
		{game::Difficulty::HARD, {}},
		{game::Difficulty::HARDCORE, {}}
	};

	void Reset(const game::Difficulty& difficulty)
	{
		auto properties = worldProperties.find(difficulty)->second;
		data::game::World data = 
		{
			CURRENT_VERSION
		};
		data::game::World::Write(data);
	}
}