#include <Common.RNG.h>
#include "Game.Fishes.h"
#include <map>
namespace game
{
	static const std::map<Fish, FishDescriptor> descriptors =
	{
		{
			Fish::MARLIN,
			{
				"marlin",
				[]() { return common::RNG::FromRange(1,3); },
				[]() { return common::RNG::FromRange(10.0,20.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{1,0},{2,0},{3,0}
				},
				{ 4, 1 }
			}
		},
		{
			Fish::COD,
			{
				"cod",
				[]() { return common::RNG::FromRange(5,11); },
				[]() { return common::RNG::FromRange(20.0,40.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{0,1},{0,2},{0,3}
				},
				{ 1, 4 }
			}
		},
		{
			Fish::TUNA,
			{
				"tuna",
				[]() { return common::RNG::FromRange(2,5); },
				[]() { return common::RNG::FromRange(15.0,30.0); },
				[]() { return common::RNG::FromRange(0.1,0.2); },
				{
					{0,0},{0,1},{1,0},{1,1}
				},
				{ 2, 2 }
			}
		}
	};

	static std::list<Fish> fishes;

	std::list<Fish> Fishes::All()
	{
		if (fishes.empty())
		{
			for (auto& descriptor : descriptors)
			{
				fishes.push_back(descriptor.first);
			}
		}
		return fishes;
	}

	const FishDescriptor& Fishes::Read(const Fish& fish)
	{
		return descriptors.find(fish)->second;
	}
}