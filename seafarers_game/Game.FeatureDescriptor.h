#pragma once
#include <string>
namespace game
{
	struct FeatureDescriptor
	{
		std::string name;
		size_t minimumCount;//at least this many have to exist in a given world, up to the number of islands present
		double coveragePercentage;//the percentage of islands with this feature
	};
}
