#pragma once
#include "Game.Commodity.h"
#include "Game.CommodityDescriptor.h"
#include <list>
namespace game
{
	struct Commodities
	{
		static const game::CommodityDescriptor& Read(const game::Commodity&);
		static const std::list<game::Commodity>& All();
	};
}
