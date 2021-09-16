#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace game
{
	struct Island
	{
		common::XY<double> relativeLocation;
		common::XY<double> absoluteLocation;
		std::string name;
		std::optional<int> visits;
		std::string patronDemigod;
	};
}