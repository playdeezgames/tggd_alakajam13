#pragma once
#include <Common.XY.h>
#include "Game.Ship.h"
#include <map>
namespace game::islands
{
	struct Ships
	{
		static std::map<game::Ship, double> GetPurchasePrices(const common::XY<double>&);
		static std::map<game::Ship, double> GetSalePrices(const common::XY<double>&);
		static double GetSalePrice(const common::XY<double>&, const game::Ship&);
	};
}