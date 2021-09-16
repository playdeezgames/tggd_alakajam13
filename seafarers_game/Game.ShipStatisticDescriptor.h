#pragma once
#include <optional>
namespace game
{
	struct ShipStatisticDescriptor
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};
}