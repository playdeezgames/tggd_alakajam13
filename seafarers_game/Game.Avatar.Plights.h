#pragma once
#include "Game.Avatar.Plight.h"
#include "Game.Avatar.PlightType.h"
#include "Game.Avatar.PlightDescriptor.h"
#include <optional>
#include <set>
namespace game::avatar
{
	struct Plights
	{
		static bool Has(const Plight&);
		static void ApplyTurnEffects();
		static const PlightDescriptor& Read(const Plight&);
		static Plight Generate(const PlightType&);
		static void Inflict(const Plight&);
		static std::set<Plight> InflictedWith();
	};
}
