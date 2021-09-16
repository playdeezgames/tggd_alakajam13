#pragma once
#include <Cards.Card.h>
#include <string>
namespace visuals
{
	struct CardSprites
	{
		static const std::string& GetSpriteForCard(const cards::Card&);
	};
}