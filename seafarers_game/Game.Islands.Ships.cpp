#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.Ships.h"
namespace game::islands
{
	static double GetPurchasePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& shipDescriptor = game::Ships::Read(ship);
		return Commodities::GetPurchasePrice(location, shipDescriptor.commodities);
	}



	double Ships::GetSalePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& shipDescriptor = game::Ships::Read(ship);
		return Commodities::GetSalePrice(location, shipDescriptor.commodities);
	}

	std::map<game::Ship, double> Ships::GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetPurchasePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}

	std::map<game::Ship, double> Ships::GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetSalePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}
}