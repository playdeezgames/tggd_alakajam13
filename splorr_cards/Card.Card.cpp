#include "Cards.Card.h"
namespace cards
{
	const std::vector<Card> fullDeck =
	{
		{ Rank::DEUCE, Suit::CLUB},
		{ Rank::DEUCE, Suit::DIAMOND},
		{ Rank::DEUCE, Suit::HEART},
		{ Rank::DEUCE, Suit::SPADE},
		{ Rank::THREE, Suit::CLUB},
		{ Rank::THREE, Suit::DIAMOND},
		{ Rank::THREE, Suit::HEART},
		{ Rank::THREE, Suit::SPADE},
		{ Rank::FOUR, Suit::CLUB},
		{ Rank::FOUR, Suit::DIAMOND},
		{ Rank::FOUR, Suit::HEART},
		{ Rank::FOUR, Suit::SPADE},
		{ Rank::FIVE, Suit::CLUB},
		{ Rank::FIVE, Suit::DIAMOND},
		{ Rank::FIVE, Suit::HEART},
		{ Rank::FIVE, Suit::SPADE},
		{ Rank::SIX, Suit::CLUB},
		{ Rank::SIX, Suit::DIAMOND},
		{ Rank::SIX, Suit::HEART},
		{ Rank::SIX, Suit::SPADE},
		{ Rank::SEVEN, Suit::CLUB},
		{ Rank::SEVEN, Suit::DIAMOND},
		{ Rank::SEVEN, Suit::HEART},
		{ Rank::SEVEN, Suit::SPADE},
		{ Rank::EIGHT, Suit::CLUB},
		{ Rank::EIGHT, Suit::DIAMOND},
		{ Rank::EIGHT, Suit::HEART},
		{ Rank::EIGHT, Suit::SPADE},
		{ Rank::NINE, Suit::CLUB},
		{ Rank::NINE, Suit::DIAMOND},
		{ Rank::NINE, Suit::HEART},
		{ Rank::NINE, Suit::SPADE},
		{ Rank::TEN, Suit::CLUB},
		{ Rank::TEN, Suit::DIAMOND},
		{ Rank::TEN, Suit::HEART},
		{ Rank::TEN, Suit::SPADE},
		{ Rank::JACK, Suit::CLUB},
		{ Rank::JACK, Suit::DIAMOND},
		{ Rank::JACK, Suit::HEART},
		{ Rank::JACK, Suit::SPADE},
		{ Rank::QUEEN, Suit::CLUB},
		{ Rank::QUEEN, Suit::DIAMOND},
		{ Rank::QUEEN, Suit::HEART},
		{ Rank::QUEEN, Suit::SPADE},
		{ Rank::KING, Suit::CLUB},
		{ Rank::KING, Suit::DIAMOND},
		{ Rank::KING, Suit::HEART},
		{ Rank::KING, Suit::SPADE},
		{ Rank::ACE, Suit::CLUB},
		{ Rank::ACE, Suit::DIAMOND},
		{ Rank::ACE, Suit::HEART},
		{ Rank::ACE, Suit::SPADE}
	};

	const std::vector<Card>& All()
	{
		return fullDeck;
	}

	const int RANK_COUNT = 13;

	int ToInt(const Card& card)
	{
		return (int)std::get<1>(card) * RANK_COUNT + (int)std::get<0>(card);
	}

	Card OfInt(int value)
	{
		return std::make_tuple((Rank)(value % RANK_COUNT), (Suit)(value / RANK_COUNT));
	}
}