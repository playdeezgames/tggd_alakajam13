#pragma once
#include <vector>
namespace mazes
{
	struct Directions
	{
		static size_t NextColumn(size_t, size_t, const Direction&);
		static size_t NextRow(size_t, size_t, const Direction&);
		static const std::vector<Direction>& All();
		static Direction Opposite(const Direction&);
	};
}