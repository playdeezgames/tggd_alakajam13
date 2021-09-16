#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.Destination.h>
#include <Data.Game.Avatar.DestinationName.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Island.Item.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.Quest.h>
#include <Data.Game.Island.Visit.h>
#include "Game.Commodities.h"
#include "Game.Islands.h"
#include "Game.Items.h"
#include "Game.World.h"
#include <map>
#include <set>
#include <sstream>
#include <vector>
namespace game
{
	const size_t RETRY_COUNT = 500;

	static std::list<common::XY<double>> GenerateLocations()
	{
		double minimumIslandDistance = game::World::GetMinimumIslandDistance();
		auto worldSize = game::World::GetSize();
		size_t retry = 0;
		std::list<common::XY<double>> locations;
		while (retry < RETRY_COUNT)
		{
			double x = common::RNG::FromRange(0.0, worldSize.GetX());
			double y = common::RNG::FromRange(0.0, worldSize.GetY());
			bool found = true;
			for (auto& location : locations)
			{
				double distance = common::Heading::Distance(location, { x, y });
				if (distance < minimumIslandDistance)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				locations.push_back({ x, y });
				retry = 0;
			}
			else
			{
				retry++;
			}
		}
		return locations;
	}

	const std::map<std::string, size_t> CONSONANT_GENERATOR =
	{
		{"h", 1},
		{"k", 1},
		{"l", 1},
		{"m", 1},
		{"p", 1}
	};
	const std::map<std::string, size_t> VOWEL_GENERATOR =
	{
		{"a", 1},
		{"e", 1},
		{"i", 1},
		{"o", 1},
		{"u", 1}
	};
	const std::map<size_t, size_t> LENGTH_GENERATOR =
	{
		{3, 1},
		{4, 3},
		{5, 9},
		{6, 27},
		{7, 9},
		{8, 3},
		{9, 1}
	};
	const std::map<bool, size_t> IS_VOWEL_GENERATOR =
	{
		{true, 1},
		{false, 1}
	};

	static std::string GenerateName()
	{
		auto length = common::RNG::FromGenerator(LENGTH_GENERATOR, (size_t)0);
		auto isVowel = common::RNG::FromGenerator(IS_VOWEL_GENERATOR, false);
		std::stringstream ss;
		while (length > 0)
		{
			if (isVowel)
			{
				ss << common::RNG::FromGenerator(VOWEL_GENERATOR, (std::string)"");
			}
			else
			{
				ss << common::RNG::FromGenerator(CONSONANT_GENERATOR, (std::string)"");
			}
			length--;
			isVowel = !isVowel;
		}
		return ss.str();;
	}

	const std::vector<std::string> permanentNames =
	{
		"antwerp"
	};

	static std::set<std::string> GenerateNames(size_t size)
	{
		std::set<std::string> names;
		for (auto& name : permanentNames)
		{
			names.insert(name);
		}
		while (names.size() < size)
		{
			names.insert(GenerateName());
		}
		return names;
	}

	static void ClearData()
	{
		data::game::Island::Clear();
		data::game::island::Visit::Clear();
		data::game::island::Known::Clear();
		data::game::avatar::Destination::ClearAll();
		data::game::avatar::DestinationName::Clear();
		data::game::island::Market::Clear();
		data::game::island::Item::ClearAll();
	}

	static void GenerateMarkets(const common::XY<double>& location)
	{
		for (auto& commodity : game::Commodities::All())
		{
			data::game::island::Market data =
			{
				common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
				common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
				0,
				0
			};
			data::game::island::Market::Write(location, (int)commodity, data);
		}
	}

	static void GenerateItems(const common::XY<double>& location)
	{
		for (auto& item : game::Items::All())
		{
			auto descriptor = game::Items::Read(item);
			if (common::RNG::FromRange(0u, descriptor.present + descriptor.notPresent) < descriptor.present)
			{
				data::game::island::Item::Set(location, (int)item);
			}
		}
	}

	static std::string GeneratePatronDemigod()
	{
		auto demigods = data::game::Demigod::All();
		std::map<std::string, size_t> table;
		for (auto& demigod : demigods)
		{
			table[demigod.name] = demigod.patronWeight;
		}
		return common::RNG::FromGenerator(table, std::string(""));
	}

	void GenerateIslands()
	{
		ClearData();
		auto locations = GenerateLocations();
		auto names = GenerateNames(locations.size());
		while (!locations.empty())
		{
			data::game::Island data =
			{
				locations.front(),
				*names.begin(),
				common::RNG::FromRange(0.001,0.005),//TODO: should be a world setting?
				GeneratePatronDemigod()
			};
			data::game::Island::Write(data);
			locations.pop_front();
			names.erase(names.begin());
			GenerateMarkets(data.location);
			GenerateItems(data.location);
		}
	}
}
