#include <Data.Game.Island.Market.h>
#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
namespace game::islands
{
	static double GetPurchasePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		auto& commodityDescriptor = game::Commodities::Read(commodity);
		auto market = data::game::island::Market::Read(location, (int)commodity).value();
		return 
			commodityDescriptor.basePrice *
			(market.demand + (double)market.purchases * commodityDescriptor.demandFactor) /
			(market.supply + (double)market.sales * commodityDescriptor.supplyFactor);
	}

	static double GetSalePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		auto& commodityDescriptor = game::Commodities::Read(commodity);
		return 
			GetPurchasePrice(location, commodity) *
			(1.0 - commodityDescriptor.discount);
	}

	double Commodities::GetPurchasePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		double price = 0.0;
		for (auto entry : table)
		{
			auto commodity = entry.first;
			auto commodityAmount = entry.second;
			price +=
				commodityAmount * game::islands::GetPurchasePrice(location, entry.first);
		}
		return price;
	}

	double Commodities::GetSalePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		double price = 0.0;
		for (auto entry : table)
		{
			auto commodity = entry.first;
			auto commodityAmount = entry.second;
			price +=
				commodityAmount * game::islands::GetSalePrice(location, entry.first);
		}
		return price;
	}
}