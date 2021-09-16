#include <Cards.Card.h>
#include <Common.RNG.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.DarkAlley.FightCard.h>
#include <functional>
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.DarkAlley.FightCard.h"
#include <list>
#include <set>
namespace game::islands::dark_alley
{
	static const size_t CARD_ROWS = 3;
	static const size_t CARD_COLUMNS = 4;
	static const size_t CARD_COUNT = CARD_ROWS * CARD_COLUMNS;

	static const std::list<cards::Rank> FACE_CARD_RANKS =
	{
		cards::Rank::JACK,
		cards::Rank::QUEEN,
		cards::Rank::KING
	};

	static bool IsFaceCard(const cards::Card& card)
	{
		auto rank = std::get<0>(card);
		return std::find(FACE_CARD_RANKS.begin(), FACE_CARD_RANKS.end(), rank) != FACE_CARD_RANKS.end();
	}

	static const std::list<cards::Rank> NONFACE_CARD_RANKS =
	{
		cards::Rank::ACE,
		cards::Rank::DEUCE,
		cards::Rank::THREE,
		cards::Rank::FOUR,
		cards::Rank::FIVE,
		cards::Rank::SIX,
		cards::Rank::SEVEN,
		cards::Rank::EIGHT,
		cards::Rank::NINE,
		cards::Rank::TEN
	};

	static const std::list<cards::Suit> CARD_SUITS =
	{
		cards::Suit::CLUB,
		cards::Suit::DIAMOND,
		cards::Suit::HEART,
		cards::Suit::SPADE
	};

	static const std::map<size_t, std::list<size_t>> ADJACENCIES =
	{
		{0, {1,4,5}},
		{1, {0,2,4,5,6}},
		{2, {1,3,5,6,7}},
		{3, {2,6,7}},
		{4, {0,1,5,8,9}},
		{5, {0,1,2,4,6,8,9,10}},
		{6, {1,2,3,5,7,9,10,11}},
		{7, {2,3,6,10,11}},
		{8, {4,5,9}},
		{9, {4,5,6,8,10}},
		{10, {5,6,7,9,11}},
		{11, {6,7,10}}
	};

	static void WriteCards(const common::XY<double>& location, const std::map<size_t, FightCard>& fightCards)
	{
		for (auto& fightCard : fightCards)
		{
			data::game::island::dark_alley::FightCard::Write(location, fightCard.first,
				{
					(int)std::get<0>(fightCard.second.card),
					(int)std::get<1>(fightCard.second.card),
					fightCard.second.adjacent,
					fightCard.second.shown
				});
		}
	}

	static size_t DetermineFaceCardCount(const data::game::island::DarkAlley& darkAlley)
	{
		auto brawling = game::avatar::Statistics::GetBrawling();
		size_t faceCardCount =
			(size_t)(brawling / (brawling + darkAlley.ruffianBrawlingStrength) * (double)CARD_COUNT);
		return
			(faceCardCount < 1) ? (1) :
			(faceCardCount > CARD_COUNT - 1) ? (CARD_COUNT - 1) :
			(faceCardCount);
	}

	static cards::Card GenerateCard(const std::list<cards::Rank>& ranks)
	{
		auto rank = common::RNG::FromList(ranks).value();
		auto suit = common::RNG::FromList(CARD_SUITS).value();
		return std::make_tuple(rank, suit);
	}

	static void PlaceFaceCards(std::map<size_t, FightCard>& fightCards, std::set<cards::Card>& cards, const data::game::island::DarkAlley& darkAlley)
	{
		size_t faceCardCount = DetermineFaceCardCount(darkAlley);
		while (faceCardCount > 0)
		{
			auto card = GenerateCard(FACE_CARD_RANKS);
			if (!cards.contains(card))
			{
				cards.insert(card);
				size_t index;
				do
				{
					index = common::RNG::FromRange(0u, CARD_COUNT);
				} while (fightCards.find(index) != fightCards.end());
				fightCards[index] =
				{
					card,
					0,
					false
				};
				faceCardCount--;
			}
		}
	}

	static void PlaceNonfaceCards(std::map<size_t, FightCard>& fightCards, std::set<cards::Card>& cards)
	{
		for (size_t index = 0; index < CARD_COUNT; ++index)
		{
			if (fightCards.find(index) == fightCards.end())
			{
				auto card = GenerateCard(NONFACE_CARD_RANKS);
				while (cards.contains(card))
				{
					card = GenerateCard(NONFACE_CARD_RANKS);
				}
				cards.insert(card);
				auto adjacencies = ADJACENCIES.find(index)->second;
				int count = 0;
				for (auto adjacency : adjacencies)
				{
					auto neighbor = fightCards.find(adjacency);
					if (neighbor != fightCards.end() && IsFaceCard(neighbor->second.card))
					{
						count++;
					}
				}
				fightCards[index] =
				{
					card,
					count,
					false
				};
			}
		}
	}

	static void DoGenerate(const common::XY<double>& location, const data::game::island::DarkAlley& darkAlley)
	{
		std::map<size_t, FightCard> fightCards;
		std::set<cards::Card> cards;
		PlaceFaceCards(fightCards, cards, darkAlley);
		PlaceNonfaceCards(fightCards, cards);
		WriteCards(location, fightCards);
	}

	void FightCard::Generate(const common::XY<double>& location)
	{
		data::game::island::dark_alley::FightCard::Clear(location);
		auto darkAlley = data::game::island::DarkAlley::Read(location);
		if (darkAlley)
		{
			DoGenerate(location, darkAlley.value());
		}
	}

	static FightCard DataToFightCard(const data::game::island::dark_alley::FightCard& data)
	{
		auto card = std::make_tuple((cards::Rank)data.rank, (cards::Suit)data.suit);
		return 
		{
				card,
				data.adjacent,
				data.shown,
				IsFaceCard(card)
		};
	}

	std::map<size_t, FightCard> FightCard::Read(const common::XY<double>& location)
	{
		std::map<size_t, FightCard> result;
		for (size_t index = 0; index < CARD_COUNT; ++index)
		{
			auto data = data::game::island::dark_alley::FightCard::Read(location, index);
			result[index] = DataToFightCard(data.value());
		}
		return result;
	}

	std::optional<FightCard> FightCard::Pick(const common::XY<double>& location, size_t index)
	{
		auto data = data::game::island::dark_alley::FightCard::Read(location, index);
		if (data)
		{
			auto cardData = data.value();
			if (cardData.shown)
			{
				return std::nullopt;
			}
			cardData.shown = true;
			data::game::island::dark_alley::FightCard::Write(location, index, cardData);
			return DataToFightCard(cardData);
		}
		return std::nullopt;
	}
}