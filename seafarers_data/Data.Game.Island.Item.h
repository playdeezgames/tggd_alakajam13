#pragma once
#include <Common.XY.h>
#include <set>
namespace data::game::island
{
	struct Item
	{
		static void Set(const common::XY<double>&, int);
		static void Clear(const common::XY<double>&, int);
		static bool Get(const common::XY<double>&, int);
		static std::set<int> GetAll(const common::XY<double>&);
		static void ClearAll();
	};
}
