#include <Cards.Card.h>
#include <Cards.Deck.h>
#include <Data.Game.Island.DarkAlley.GamblingHand.h>
#include "Game.Islands.DarkAlley.GamblingHand.h"
#include "Game.Islands.Features.h"
namespace game::islands::dark_alley
{
	bool GamblingHand::Deal(const common::XY<double>& location)
	{
		if (!game::islands::Features::Read(location, game::Feature::DARK_ALLEY))
		{
			return false;
		}
		cards::Deck<cards::Card> deck(cards::All());
		deck.Shuffle();
		data::game::island::dark_alley::GamblingHand data =
		{
			cards::ToInt(deck.Draw().value()),
			cards::ToInt(deck.Draw().value()),
			cards::ToInt(deck.Draw().value())
		};
		data::game::island::dark_alley::GamblingHand::Write(location, data);
		return true;
	}

	std::vector<cards::Card> GamblingHand::Read(const common::XY<double>& location)
	{
		std::vector<cards::Card> result;
		auto hand = data::game::island::dark_alley::GamblingHand::Read(location);
		if (hand)
		{
			result.push_back(cards::OfInt(hand.value().firstCard));
			result.push_back(cards::OfInt(hand.value().secondCard));
			result.push_back(cards::OfInt(hand.value().thirdCard));
		}
		return result;
	}

	bool GamblingHand::IsWinner(const common::XY<double>& location)
	{
		auto hand = Read(location);
		if(!hand.empty())
		{
			auto first = std::get<0>(hand[0]);
			auto second = std::get<0>(hand[1]);
			auto third = std::get<0>(hand[2]);
			if (first > second)
			{
				return (third > second) && (first > third);
			}
			else if (second > first)
			{
				return (third > first) && (second > third);
			}
		}
		return false;
	}

}