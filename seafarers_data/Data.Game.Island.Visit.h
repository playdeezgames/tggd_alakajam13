#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island
{
	struct Visit
	{
		common::XY<double> location;
		int visits;
		std::optional<int> lastVisit;
		static void Write(const Visit&);
		static std::optional<Visit> Read(const common::XY<double>&);
		static void Clear();
	};

}