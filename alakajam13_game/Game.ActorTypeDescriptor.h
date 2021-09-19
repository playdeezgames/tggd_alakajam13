#pragma once
#include <string>
#include <optional>
namespace game
{
	struct ActorTypeDescriptor
	{
		std::string sprite;
		bool playControlled;
		std::string miniSprite;		
		std::optional<std::string> powerImage;
	};
}
