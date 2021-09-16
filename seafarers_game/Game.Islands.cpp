#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.Destination.h>
#include <Data.Game.Island.Item.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.Quest.h>
#include <Data.Game.Island.Visit.h>
#include <functional>
#include "Game.Avatar.AtSea.h"
#include "Game.Commodities.h"
#include "Game.Islands.h"
#include "Game.Items.h"
#include "Game.Merchants.h"
#include "Game.World.h"
#include <map>
#include <set>
#include <sstream>
#include <vector>
namespace game
{
	const std::string Islands::UNKNOWN = "????";

	void GenerateIslands();

	void Islands::Reset(const game::Difficulty&)
	{
		GenerateIslands();
	}

	static void AddIsland(std::list<Island>& result, const data::game::Island& island, const common::XY<double>& avatarLocation)
	{
		auto visitData = data::game::island::Visit::Read(island.location);
		data::game::island::Known::Write(island.location);
		result.push_back(
			{
				(island.location - avatarLocation),
				island.location,
				island.name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt),
				island.patronDemigod
			});
	}

	static void AddIslandWhenCloseEnough(std::list<Island>& result, const data::game::Island& island, const common::XY<double>& avatarLocation, std::function<bool(const data::game::Island&, double)> filter)
	{
		auto distance = common::Heading::Distance(avatarLocation, island.location);
		if (filter(island, distance))
		{
			AddIsland(result, island, avatarLocation);
		}
	}

	static std::list<Island> GetIslandsInRange(std::function<bool(const data::game::Island&, double)> filter)
	{
		std::list<Island> result;
		auto avatarLocation = game::avatar::AtSea::GetLocation();
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			AddIslandWhenCloseEnough(result, island, avatarLocation, filter);
		}
		return result;
	}

	static std::function<bool(const data::game::Island&, double)> FixedDistance(double maximumDistance)
	{
		return [maximumDistance](const data::game::Island&, double distance)
		{
			return distance <= maximumDistance;
		};
	}

	std::list<Island> Islands::GetViewableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetViewDistance()));
	}

	std::list<Island> Islands::GetDockableIslands()
	{
		return GetIslandsInRange(FixedDistance(game::World::GetDockDistance()));
	}

	bool Islands::CanDock()
	{
		return !GetDockableIslands().empty();
	}

	void Islands::AddVisit(const common::XY<double>& location, const int& turn)
	{
		auto visitData = data::game::island::Visit::Read(location);
		if (visitData)
		{
			auto islandVisits = visitData.value();
			if (islandVisits.lastVisit != turn)
			{
				islandVisits.visits = islandVisits.visits + 1;
				islandVisits.lastVisit = turn;
				data::game::island::Visit::Write(islandVisits);
				return;
			}
		}
		data::game::island::Visit::Write({
			location,
			1,
			turn});
	}

	void Islands::SetKnown(const common::XY<double>& location, const int& turn)
	{
		if (!data::game::island::Visit::Read(location))
		{
			data::game::island::Visit::Write({
				location,
				0,
				turn });
		}
	}

	std::optional<Island> Islands::Read(const common::XY<double>& location)//TODO: different model for docked?
	{
		auto data = data::game::Island::Read(location);
		if (data)
		{
			auto visitData = data::game::island::Visit::Read(data.value().location);
			return std::optional<Island>({
				{0.0, 0.0},
				data.value().location,
				data.value().name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt),
				data.value().patronDemigod
				});
		}
		return std::nullopt;
	}

	std::list<Island> Islands::GetKnownIslands()
	{
		auto knownLocations = data::game::island::Known::All();
		std::list<Island> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(knownLocation);
			if (model)
			{
				auto visits = data::game::island::Visit::Read(knownLocation);
				if (visits)
				{
					result.push_back(model.value());
				}
				else
				{
					auto obfuscated = model.value();
					obfuscated.name = game::Islands::UNKNOWN;
					result.push_back(obfuscated);
				}
			}
		}
		return result;
	}

	static void SpawnMerchant(const data::game::Island& island)
	{
		if (common::RNG::FromRange(0.0, 1.0) < island.merchantSpawnRate)
		{
			Merchants::Spawn(island.location);
		}
	}

	void Islands::ApplyTurnEffects()
	{
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			SpawnMerchant(island);
		}
	}
}
