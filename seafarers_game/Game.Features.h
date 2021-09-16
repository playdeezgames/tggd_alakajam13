#pragma once
#include "Game.Feature.h"
#include "Game.FeatureDescriptor.h"
#include <list>
namespace game
{
	struct Features
	{
		static const game::FeatureDescriptor& Read(const game::Feature&);
		static const std::list<game::Feature>& All();
	};
}
