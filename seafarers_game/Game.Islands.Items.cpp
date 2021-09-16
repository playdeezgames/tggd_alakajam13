#include <Data.Game.Island.Item.h>
#include "Game.Islands.Commodities.h"
#include "Game.Islands.Items.h"
#include "Game.Items.h"
namespace game::islands
{
	static double GetItemPurchasePrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		auto& itemDescriptor = game::Items::Read(item);
		return Commodities::GetPurchasePrice(location, itemDescriptor.commodities);
	}

	static double GetItemSellPrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		auto& itemDescriptor = game::Items::Read(item);
		return Commodities::GetSalePrice(location, itemDescriptor.commodities);
	}

	std::map<game::Item, double> Items::GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		auto itemsAvailable = data::game::island::Item::GetAll(location);
		for (auto& item : itemsAvailable)
		{
			double price = GetItemPurchasePrice(location, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}

	std::map<game::Item, double> Items::GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		auto items = game::Items::All();
		for (auto& item : items)
		{
			double price = GetItemSellPrice(location, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}
}