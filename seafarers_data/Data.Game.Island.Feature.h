#pragma once
#include <Common.XY.h>
namespace data::game::island
{
	struct Feature
	{
		static void Clear();
		static void Write(const common::XY<double>&, int);
		static void Clear(const common::XY<double>&, int);
		static bool Read(const common::XY<double>&, int);
	};
}
