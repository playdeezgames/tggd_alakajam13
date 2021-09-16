#include "Game.Items.h"
#include <map>
namespace game
{
	static const std::map<game::Item, game::ItemDescriptor> itemTable =
	{
		{game::Item::RATIONS,
			{
				"hardtack",
				{
					{game::Commodity::WHEAT, 0.1}
				},
				1,
				0,
				0.02,
				0
			}
		},
		{game::Item::SACK_OF_WHEAT,
			{
				"sack of wheat",
				{
					{game::Commodity::WHEAT, 1.0}
				},
				1,
				0,
				0.1,
				0
			}
		},
		{game::Item::CLOTH,
			{
				"cloth",
				{
					{game::Commodity::WOOL, 0.25},
					{game::Commodity::LABOR, 0.1}
				},
				3,
				1,
				0.25,
				0
			}
		},
		{game::Item::FISH,
			{
				"fish",
				{
					{game::Commodity::MEAT, 0.2}
				},
				5,
				1,
				0.02,
				0
			}
		},
		{game::Item::JOOLS,
			{
				"jools",
				{
					{game::Commodity::JOOLS, 1.0}
				},
				1,
				0,
				0.0,
				0
			}
		},
		{game::Item::TROUSERS,
			{
				"trousers",
				{
					{game::Commodity::WOOL, 1.0},
					{game::Commodity::LABOR, 0.75}
				},
				1,
				1,
				0.1,
				1
			}
		},
		{game::Item::FISHING_POLE,
			{
				"fishing pole",
				{
					{game::Commodity::WOOD, 0.25},
					{game::Commodity::SILK, 0.25}
				},
				1,
				1,
				0.3,
				1
			}
		},
		{game::Item::BAIT,
			{
				"bait",
				{
					{game::Commodity::MEAT, 0.01}
				},
				1,
				1,
				0.001,
				10
			}
		}
	};

	static std::list<game::Item> itemList;

	const game::ItemDescriptor& Items::Read(const game::Item& item)
	{
		return itemTable.find(item)->second;
	}

	const std::list<game::Item>& Items::All()
	{
		if (itemList.empty())
		{
			for (auto entry : itemTable)
			{
				itemList.push_back(entry.first);
			}
		}
		return itemList;
	}
}