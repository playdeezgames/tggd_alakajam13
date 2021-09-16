#include <map>
#include "Mazes.Direction.h"
#include "Mazes.Directions.h"
#include <string>
namespace mazes
{
	static const std::vector<Direction> allDirections = 
	{
		Direction::NORTH,
		Direction::EAST,
		Direction::SOUTH,
		Direction::WEST
	};

	const std::vector<Direction>& Directions::All()
	{
		return allDirections;
	}

	static const std::map<Direction, Direction> opposites =
	{
		{Direction::EAST, Direction::WEST},
		{Direction::SOUTH, Direction::NORTH},
		{Direction::WEST, Direction::EAST},
		{Direction::NORTH, Direction::SOUTH}
	};

	Direction Directions::Opposite(const Direction& direction)
	{
		return opposites.find(direction)->second;
	}

	static const std::map<Direction, int> deltaColumns =
	{
		{Direction::EAST, 1},
		{Direction::WEST, -1},
		{Direction::NORTH, 0},
		{Direction::SOUTH, 0}
	};

	size_t Directions::NextColumn(size_t column, size_t row, const Direction& direction)
	{
		return column + deltaColumns.find(direction)->second;
	}

	static const std::map<Direction, int> deltaRows =
	{
		{Direction::EAST, 0},
		{Direction::WEST, 0},
		{Direction::NORTH, -1},
		{Direction::SOUTH, 1}
	};

	size_t Directions::NextRow(size_t column, size_t row, const Direction& direction)
	{
		return row + deltaRows.find(direction)->second;
	}
}