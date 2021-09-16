#include "Game.Commodities.h"
#include <map>
namespace game
{
	static const std::map<game::Commodity, game::CommodityDescriptor> commodityTable =
	{
		{game::Commodity::WHEAT, { "wheat", 1.0, 0.05, 0.05, 0.15}},
		{game::Commodity::WOOD, { "wood", 7.5, 0.05, 0.05, 0.15}},
		{game::Commodity::WOOL, { "wool", 2.5, 0.05, 0.05, 0.15}},
		{game::Commodity::LABOR, { "labor", 1.0, 0.05, 0.05, 0.15}},
		{game::Commodity::MEAT, { "meat", 5.0, 0.05, 0.05, 0.15}},
		{game::Commodity::JOOLS, { "jools", 100.0, 0.05, 0.05, 0.15}},
		{game::Commodity::SILK, { "silk", 50.0, 0.05, 0.05, 0.15}}
	};

	static std::list<game::Commodity> commodityList;

	const game::CommodityDescriptor& Commodities::Read(const game::Commodity& commodity)
	{
		return commodityTable.find(commodity)->second;
	}

	const std::list<game::Commodity>& Commodities::All()
	{
		if (commodityList.empty())
		{
			for (auto entry : commodityTable)
			{
				commodityList.push_back(entry.first);
			}
		}
		return commodityList;
	}
}
