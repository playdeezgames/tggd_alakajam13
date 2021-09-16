#include <Common.RNG.h>
#include "Game.Ships.h"
#include <map>
namespace game
{
	static const std::map<game::Ship, game::ShipDescriptor> ships =
	{
		{
			game::Ship::RAFT, 
			{
				"raft",
				{
					{game::ship::Property::SPEED_FACTOR, 1.0},
					{game::ship::Property::TONNAGE, 50.0}
				},
				{
					{game::Commodity::WOOD, 100.0},
					{game::Commodity::LABOR, 50.0}
				},
				{
					{
						ShipStatistic::PORT_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::STARBOARD_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::FOULING_RATE,
						{
							0.001,
							0.001,
							0.001
						}
					}
				},
				1
			}
		},
		{
			game::Ship::JUNK, 
			{
				"junk",
				{
					{game::ship::Property::SPEED_FACTOR, 1.25},
					{game::ship::Property::TONNAGE, 100.0}
				},
				{
					{game::Commodity::WOOD, 200.0},
					{game::Commodity::LABOR, 75.0}
				},
				{
					{
						ShipStatistic::PORT_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::STARBOARD_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::FOULING_RATE,
						{
							0.001,
							0.001,
							0.001
						}
					}
				},
				0
			}
		}
	};

	static std::list<game::Ship> shipList;

	const game::ShipDescriptor& Ships::Read(const game::Ship & ship)
	{
		return ships.find(ship)->second;
	}

	const std::list<game::Ship>& Ships::All()
	{
		if (shipList.empty())
		{
			for (auto& ship : ships)
			{
				shipList.push_back(ship.first);
			}
		}
		return shipList;
	}

	std::map<game::Ship, size_t> initialShipGenerator;

	game::Ship Ships::GenerateForAvatar()
	{
		if (initialShipGenerator.empty())
		{
			for (auto ship : All())
			{
				auto descriptor = Read(ship);
				if (descriptor.initialShipGenerationWeight > 0)
				{
					initialShipGenerator[ship] = descriptor.initialShipGenerationWeight;
				}
			}
		}
		return common::RNG::FromGenerator(initialShipGenerator, game::Ship::RAFT);
	}
}