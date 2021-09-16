#pragma once
#include <Common.XY.h>
#include <optional>
namespace game::islands
{
	struct DarkAlley
	{
		static std::optional<double> GetRuffianBrawling(const common::XY<double>&);
		static std::optional<double> GetMinimumWager(const common::XY<double>&);
		static std::optional<double> GetAnte(const common::XY<double>&);
	};
}
