#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island::dark_alley
{
	struct FightCard
	{
		int rank;
		int suit;
		int adjacent;
		bool shown;

		static std::optional<FightCard> Read(const common::XY<double>&, size_t);
		static void Write(const common::XY<double>&, size_t, const FightCard&);
		static void Clear(const common::XY<double>&);
	};
}
