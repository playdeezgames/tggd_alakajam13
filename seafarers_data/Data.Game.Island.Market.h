#pragma once
#include <Common.XY.h>
#include <map>
#include <optional>
namespace data::game::island
{
	struct Market
	{
		double supply;
		double demand;
		double purchases;
		double sales;
		static void Write(const common::XY<double>&, int, const Market&);
		static std::optional<Market> Read(const common::XY<double>&, int);
		static std::map<int, Market> All(const common::XY<double>&);
		static void Clear();
	};

}