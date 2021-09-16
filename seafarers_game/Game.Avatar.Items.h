#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <map>
namespace game::avatar
{
	struct Items
	{
		static std::map<game::Item, size_t> All();
		static size_t Read(const game::Item&);
		static bool Has(const Item&);
		static void Add(const game::Item&, const size_t&);
		static void Remove(const game::Item&, const size_t&);
		static void Reset(const game::Difficulty&);
		static double TotalTonnage();
	};
}
