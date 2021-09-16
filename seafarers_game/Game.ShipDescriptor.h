#pragma once
#include "Game.Commodity.h"
#include "Game.Ship.Property.h"
#include "Game.ShipStatistic.h"
#include "Game.ShipStatisticDescriptor.h"
#include <map>
#include <string>
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Property, double> properties;
		std::map<game::Commodity, double> commodities;
		std::map<ShipStatistic, ShipStatisticDescriptor> statistics;
		size_t initialShipGenerationWeight;
	};
}