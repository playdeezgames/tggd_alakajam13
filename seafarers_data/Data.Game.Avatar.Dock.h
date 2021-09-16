#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::avatar
{
	struct Dock
	{
		static void Write(const common::XY<double>&);
		static void Clear();
		static std::optional<common::XY<double>> Read();
	};
}