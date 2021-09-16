#include <map>
#include "Visuals.CardSprites.h"
namespace visuals
{
	static const std::map<cards::Card, std::string> cardSprites =
	{
		{{cards::Rank::DEUCE, cards::Suit::CLUB}, "Card2C" },
		{{cards::Rank::DEUCE, cards::Suit::DIAMOND}, "Card2D" },
		{{cards::Rank::DEUCE, cards::Suit::HEART}, "Card2H" },
		{{cards::Rank::DEUCE, cards::Suit::SPADE}, "Card2S" },
		{{cards::Rank::THREE, cards::Suit::CLUB}, "Card3C" },
		{{cards::Rank::THREE, cards::Suit::DIAMOND}, "Card3D" },
		{{cards::Rank::THREE, cards::Suit::HEART}, "Card3H" },
		{{cards::Rank::THREE, cards::Suit::SPADE}, "Card3S" },
		{{cards::Rank::FOUR, cards::Suit::CLUB}, "Card4C" },
		{{cards::Rank::FOUR, cards::Suit::DIAMOND}, "Card4D" },
		{{cards::Rank::FOUR, cards::Suit::HEART}, "Card4H" },
		{{cards::Rank::FOUR, cards::Suit::SPADE}, "Card4S" },
		{{cards::Rank::FIVE, cards::Suit::CLUB}, "Card5C" },
		{{cards::Rank::FIVE, cards::Suit::DIAMOND}, "Card5D" },
		{{cards::Rank::FIVE, cards::Suit::HEART}, "Card5H" },
		{{cards::Rank::FIVE, cards::Suit::SPADE}, "Card5S" },
		{{cards::Rank::SIX, cards::Suit::CLUB}, "Card6C" },
		{{cards::Rank::SIX, cards::Suit::DIAMOND}, "Card6D" },
		{{cards::Rank::SIX, cards::Suit::HEART}, "Card6H" },
		{{cards::Rank::SIX, cards::Suit::SPADE}, "Card6S" },
		{{cards::Rank::SEVEN, cards::Suit::CLUB}, "Card7C" },
		{{cards::Rank::SEVEN, cards::Suit::DIAMOND}, "Card7D" },
		{{cards::Rank::SEVEN, cards::Suit::HEART}, "Card7H" },
		{{cards::Rank::SEVEN, cards::Suit::SPADE}, "Card7S" },
		{{cards::Rank::EIGHT, cards::Suit::CLUB}, "Card8C" },
		{{cards::Rank::EIGHT, cards::Suit::DIAMOND}, "Card8D" },
		{{cards::Rank::EIGHT, cards::Suit::HEART}, "Card8H" },
		{{cards::Rank::EIGHT, cards::Suit::SPADE}, "Card8S" },
		{{cards::Rank::NINE, cards::Suit::CLUB}, "Card9C" },
		{{cards::Rank::NINE, cards::Suit::DIAMOND}, "Card9D" },
		{{cards::Rank::NINE, cards::Suit::HEART}, "Card9H" },
		{{cards::Rank::NINE, cards::Suit::SPADE}, "Card9S" },
		{{cards::Rank::TEN, cards::Suit::CLUB}, "Card10C" },
		{{cards::Rank::TEN, cards::Suit::DIAMOND}, "Card10D" },
		{{cards::Rank::TEN, cards::Suit::HEART}, "Card10H" },
		{{cards::Rank::TEN, cards::Suit::SPADE}, "Card10S" },
		{{cards::Rank::JACK, cards::Suit::CLUB}, "CardJC" },
		{{cards::Rank::JACK, cards::Suit::DIAMOND}, "CardJD" },
		{{cards::Rank::JACK, cards::Suit::HEART}, "CardJH" },
		{{cards::Rank::JACK, cards::Suit::SPADE}, "CardJS" },
		{{cards::Rank::QUEEN, cards::Suit::CLUB}, "CardQC" },
		{{cards::Rank::QUEEN, cards::Suit::DIAMOND}, "CardQD" },
		{{cards::Rank::QUEEN, cards::Suit::HEART}, "CardQH" },
		{{cards::Rank::QUEEN, cards::Suit::SPADE}, "CardQS" },
		{{cards::Rank::KING, cards::Suit::CLUB}, "CardKC" },
		{{cards::Rank::KING, cards::Suit::DIAMOND}, "CardKD" },
		{{cards::Rank::KING, cards::Suit::HEART}, "CardKH" },
		{{cards::Rank::KING, cards::Suit::SPADE}, "CardKS" },
		{{cards::Rank::ACE, cards::Suit::CLUB}, "CardAC" },
		{{cards::Rank::ACE, cards::Suit::DIAMOND}, "CardAD" },
		{{cards::Rank::ACE, cards::Suit::HEART}, "CardAH" },
		{{cards::Rank::ACE, cards::Suit::SPADE}, "CardAS" }
	};

	const std::string& CardSprites::GetSpriteForCard(const cards::Card& card)
	{
		return cardSprites.find(card)->second;
	}
}