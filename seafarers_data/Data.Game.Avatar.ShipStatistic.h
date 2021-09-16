#pragma once
#include <optional>
namespace data::game::avatar
{
	struct ShipStatistic
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, const ShipStatistic&);
		static std::optional<ShipStatistic> Read(int);
		static void Clear();
	};
}
