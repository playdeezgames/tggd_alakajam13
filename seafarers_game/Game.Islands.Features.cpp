#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.Feature.h>
#include <functional>
#include "Game.Features.h"
#include "Game.Islands.Features.h"
#include <vector>
namespace game::islands
{
	bool Features::Read(const common::XY<double>& xy, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(xy, (int)feature);
	}

	void Features::Write(const common::XY<double>& xy, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(xy, (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(xy, (int)feature);
		}
	}

	static void InitializeDarkAlley(const common::XY<double>& location)
	{
		data::game::island::DarkAlley::Write(location, {
			common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
			common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
			common::RNG::FromRange(1.0,4.0) + common::RNG::FromRange(1.0,4.0)
			});
	}

	static const std::map<game::Feature, std::function<void(const common::XY<double>&)>> featureInitializers =
	{
		{game::Feature::DARK_ALLEY, InitializeDarkAlley}
	};

	static void InitializeFeature(const game::Feature& feature, const common::XY<double>& location)
	{
		auto initializer = featureInitializers.find(feature);
		if (initializer != featureInitializers.end())
		{
			initializer->second(location);
		}
	}

	static void GenerateFeature(const game::Feature& feature, const std::list<data::game::Island>& islands)
	{
		auto& descriptor = game::Features::Read(feature);
		std::vector<data::game::Island> candidates;
		for (auto island : islands)
		{
			candidates.push_back(island);
		}
		size_t islandCount = (size_t)(descriptor.coveragePercentage * (double)islands.size());
		if (islandCount < descriptor.minimumCount)
		{
			islandCount = descriptor.minimumCount;
		}
		if (islandCount > islands.size())
		{
			islandCount = islands.size();
		}
		while (islandCount > 0)
		{
			auto index = common::RNG::FromRange(0u, candidates.size());
			auto candidate = candidates[index];
			candidates[index] = candidates.back();
			candidates.pop_back();
			data::game::island::Feature::Write(candidate.location, (int)feature);
			InitializeFeature(feature, candidate.location);
			islandCount--;
		}
	}

	static void GenerateAllFeatures()
	{
		auto allIslands = data::game::Island::All();
		for (auto feature : game::Features::All())
		{
			GenerateFeature(feature, allIslands);
		}
	}

	void Features::Reset(const game::Difficulty&)
	{
		data::game::island::Feature::Clear();
		GenerateAllFeatures();
	}
}