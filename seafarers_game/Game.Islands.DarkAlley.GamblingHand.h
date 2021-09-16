#pragma once
#include <Cards.Card.h>
#include <Common.XY.h>
#include <vector>
namespace game::islands::dark_alley
{
	struct GamblingHand
	{
		static bool Deal(const common::XY<double>&);
		static std::vector<cards::Card> Read(const common::XY<double>&);
		static bool IsWinner(const common::XY<double>&);
	};
}