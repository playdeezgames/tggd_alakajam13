#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Destination
	{
		static void Write(int, const common::XY<double>&);
		static std::optional<common::XY<double>> Read(int);
		static void Clear(int);
		static void ClearAll();
	};
}