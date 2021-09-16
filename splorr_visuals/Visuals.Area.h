#pragma once
#include <Common.XY.h>
#include <optional>
#include "Visuals.AreaMenu.h"
namespace visuals
{
	struct Area
	{
		std::string areaId;
		common::XY<int> xy;
		common::XY<size_t> size;
		bool Contains(const common::XY<int>&) const;
		std::optional<AreaMenu> menu;
	};
}