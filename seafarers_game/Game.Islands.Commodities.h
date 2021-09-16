#pragma once
#include <Common.XY.h>
#include "Game.Commodity.h"
#include <map>
namespace game::islands
{
	struct Commodities
	{
		static double GetPurchasePrice(const common::XY<double>&, const std::map<Commodity, double>&);
		static double GetSalePrice(const common::XY<double>&, const std::map<Commodity, double>&);
	};
}