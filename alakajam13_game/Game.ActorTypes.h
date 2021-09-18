#pragma once
#include "Game.ActorType.h"
#include "Game.ActorTypeDescriptor.h"
namespace game
{
	struct ActorTypes
	{
		static const ActorTypeDescriptor& Read(const ActorType&);
	};
}
