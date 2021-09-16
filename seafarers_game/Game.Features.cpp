#include "Game.Features.h"
#include <map>
namespace game
{
	static const std::map<game::Feature, game::FeatureDescriptor> featureDescriptors =
	{
		{game::Feature::SHIPYARD,
			{
				"shipyard",
				1,
				0.5
			}
		},
		{game::Feature::DARK_ALLEY,
			{
				"dark alley",
				1,
				0.6
			}
		}
	};
	static std::list<game::Feature> featureList;

	const game::FeatureDescriptor& Features::Read(const game::Feature& feature)
	{
		return featureDescriptors.find(feature)->second;
	}

	const std::list<game::Feature>& Features::All()
	{
		if (featureList.empty())
		{
			for (auto entry : featureDescriptors)
			{
				featureList.push_back(entry.first);
			}
		}
		return featureList;
	}
}
