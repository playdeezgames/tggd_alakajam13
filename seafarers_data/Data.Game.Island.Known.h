#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
namespace data::game::island
{
	struct Known
	{
		static void Write(const common::XY<double>&);
		static bool Read(const common::XY<double>&);
		static void Clear();
		static std::list<common::XY<double>> All();
	};
}