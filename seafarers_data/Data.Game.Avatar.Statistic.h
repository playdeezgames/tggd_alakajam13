#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Statistic
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, const Statistic&);
		static std::optional<Statistic> Read(int);
	};
}
