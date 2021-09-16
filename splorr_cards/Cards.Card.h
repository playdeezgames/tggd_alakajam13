#pragma once
#include "Cards.Rank.h"
#include "Cards.Suit.h"
#include <tuple>
#include <vector>
namespace cards
{
	typedef std::tuple<Rank, Suit> Card;
	const std::vector<Card>& All();
	int ToInt(const Card&);
	Card OfInt(int);
}