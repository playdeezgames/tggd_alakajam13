#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.World.h"
#include <map>
namespace game::World
{
	const int CURRENT_VERSION = 1;

	common::XY<double> GetSize()
	{
		return data::game::World::Read().value().size;
	}

	int GetVersion()
	{
		return data::game::World::Read().value().version;
	}

	double GetMinimumIslandDistance()
	{
		return data::game::World::Read().value().minimumIslandDistance;

	}

	double GetViewDistance()
	{
		return data::game::World::Read().value().viewDistance;
	}

	double GetDockDistance()
	{
		return data::game::World::Read().value().dockDistance;
	}

	struct WorldProperties
	{
		double size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
	};

	const std::map<game::Difficulty, WorldProperties> worldProperties =
	{
		{game::Difficulty::EASY, {100.0, 10.0, 10.0, 1.0}},
		{game::Difficulty::NORMAL, {150.0, 15.0, 10.0, 1.0}},
		{game::Difficulty::HARD, {200.0, 20.0, 10.0, 1.0}},
		{game::Difficulty::HARDCORE, {250.0, 25.0, 10.0, 1.0}}
	};

	void Reset(const game::Difficulty& difficulty)
	{
		auto properties = worldProperties.find(difficulty)->second;
		data::game::World data = 
		{
			CURRENT_VERSION,
			{properties.size, properties.size},
			properties.minimumIslandDistance,
			properties.viewDistance,
			properties.dockDistance,
			common::RNG::FromRange(0.0, common::Heading::DEGREES)
		};
		data::game::World::Write(data);
	}

	double GetWindHeading()
	{
		return data::game::World::Read().value().windHeading;
	}

	void SetWindHeading(double heading)
	{
		auto data = data::game::World::Read();
		if (data)
		{
			data.value().windHeading = heading;
			data::game::World::Write(data.value());
		}
	}
}