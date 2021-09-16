#pragma once
#include "Game.Item.h"
#include "Game.ItemDescriptor.h"
#include <list>
namespace game
{
	struct Items
	{
		static const game::ItemDescriptor& Read(const game::Item&);
		static const std::list<game::Item>& All();
	};
}
