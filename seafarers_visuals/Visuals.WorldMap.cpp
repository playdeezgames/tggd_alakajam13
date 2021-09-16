#include <Application.Engine.h>
#include <Common.Heading.h>
#include <Common.XY.h>
#include <functional>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Destinations.h>
#include <Game.Avatar.Quest.h>
#include <Game.Islands.h>
#include <Game.World.h>
#include <json.hpp>
#include <map>
#include "Visuals.Data.Properties.h"
#include "Visuals.Sprites.h"
#include "Visuals.WorldMap.h"
namespace visuals
{
	static const std::string SPRITE_WORLD_MAP_SHIP = "WorldMapShip";
	static const std::string SPRITE_WORLD_MAP_NEW_DESTINATION = "WorldMapNewDestination";
	static const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_1 = "WorldMapCurrentDestination1";
	static const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_2 = "WorldMapCurrentDestination2";
	static const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_3 = "WorldMapCurrentDestination3";
	static const std::string SPRITE_WORLD_MAP_CURRENT_DESTINATION_4 = "WorldMapCurrentDestination4";
	static const std::string SPRITE_WORLD_MAP_QUEST_ISLAND = "WorldMapQuestIsland";
	static const std::string SPRITE_WORLD_MAP_ISLAND = "WorldMapIsland";
	static const std::string SPRITE_WORLD_MAP_HOVER_ISLAND = "WorldMapHoverIsland";

	struct InternalWorldMap
	{
		common::XY<int> xy;
		common::XY<size_t> size;
		std::optional<common::XY<int>> destination;
		std::optional<common::XY<double>> hoverIsland;
	};

	static std::vector<InternalWorldMap> worldMaps;
	static std::map<std::string, std::map<std::string, size_t>> worldMapTable;

	static common::XY<int> Plot(const InternalWorldMap& worldMap, const common::XY<double> worldSize, const common::XY<double>& location)
	{
		return common::XY<int>(
			(int)(location.GetX() / worldSize.GetX() * worldMap.size.GetX() + worldMap.xy.GetX()),
			(int)(location.GetY() / worldSize.GetY() * worldMap.size.GetY() + worldMap.xy.GetY()));
	}

	static common::XY<double> Pick(const InternalWorldMap& worldMap, const common::XY<double> worldSize, const common::XY<int>& location)
	{
		return common::XY<double>(
			location.GetX() * worldSize.GetX() / worldMap.size.GetX(),
			location.GetY() * worldSize.GetY() / worldMap.size.GetY());
	}

	static void DrawAvatar(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto avatarLocation = game::avatar::AtSea::GetLocation();
		common::XY<int> plot = Plot(worldMap, worldSize, avatarLocation);
		visuals::Sprites::Draw(SPRITE_WORLD_MAP_SHIP, renderer, plot, { 0xff,0xff,0xff,0xff });
	}

	static void DrawNewDestination(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		if (worldMap.destination)
		{
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_NEW_DESTINATION, renderer, worldMap.destination.value() + worldMap.xy, { 0xff,0xff,0xff,0xff });
		}
	}

	static const std::map<game::avatar::Destination, std::string> destinationIdSprites =
	{
		{game::avatar::Destination::ONE, SPRITE_WORLD_MAP_CURRENT_DESTINATION_1},
		{game::avatar::Destination::TWO, SPRITE_WORLD_MAP_CURRENT_DESTINATION_2},
		{game::avatar::Destination::THREE, SPRITE_WORLD_MAP_CURRENT_DESTINATION_3},
		{game::avatar::Destination::FOUR, SPRITE_WORLD_MAP_CURRENT_DESTINATION_4}
	};

	static void DrawCurrentDestination(const std::shared_ptr<application::Engine::Renderer>& renderer, const game::avatar::Destination& destinationId, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto currentDestination = game::avatar::Destinations::GetDestination(destinationId);
		if (currentDestination)
		{
			common::XY<int> plot = Plot(worldMap, worldSize, currentDestination.value());
			visuals::Sprites::Draw(destinationIdSprites.find(destinationId)->second, renderer, plot, { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawCurrentDestinations(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		for (auto destinationId : game::avatar::Destinations::All())
		{
			DrawCurrentDestination(renderer, destinationId, worldMap, worldSize);
		}
	}

	static double DrawKnownIsland(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		InternalWorldMap& worldMap, 
		const common::XY<double> worldSize, 
		double closest,
		const std::optional<game::Quest>& quest,//TODO: does not need entire quest model, just the location would do
		const game::Island& knownIsland)
	{
		common::XY<int> plot = Plot(worldMap, worldSize, knownIsland.absoluteLocation);
		if (worldMap.destination)
		{
			auto distance = common::Heading::Distance({ (double)plot.GetX(),(double)plot.GetY() }, { (double)worldMap.destination.value().GetX() + worldMap.xy.GetX(), (double)worldMap.destination.value().GetY() + worldMap.xy.GetY() });
			if (distance < closest)
			{
				closest = distance;
				worldMap.hoverIsland = knownIsland.absoluteLocation;
			}
		}
		if (quest.has_value() && quest.value().destination == knownIsland.absoluteLocation)
		{
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_QUEST_ISLAND, renderer, plot, { 0xff,0xff,0xff,0xff });
		}
		else
		{
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_ISLAND, renderer, plot, { 0xff,0xff,0xff,0xff });
		}
		return closest;
	}

	static void DrawKnownIslands(const std::shared_ptr<application::Engine::Renderer>& renderer, InternalWorldMap& worldMap, const common::XY<double> worldSize)
	{
		auto closest = common::Heading::Distance({(double)worldMap.size.GetX(), (double)worldMap.size.GetY()}, {0.0,0.0});
		auto knownIslands = game::Islands::GetKnownIslands();
		worldMap.hoverIsland = std::nullopt;
		auto quest = game::avatar::Quest::Read();
		for (auto& knownIsland : knownIslands)
		{
			closest = DrawKnownIsland(renderer, worldMap, worldSize, closest, quest, knownIsland);
		}
		if (worldMap.hoverIsland)
		{
			common::XY<int> plot = Plot(worldMap, worldSize, worldMap.hoverIsland.value());
			visuals::Sprites::Draw(SPRITE_WORLD_MAP_HOVER_ISLAND, renderer, plot, { 0xff,0xff,0xff,0xff });
		}
	}

	static void DrawInternalWorldMap(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t worldMapIndex)
	{
		auto& worldMap = worldMaps[worldMapIndex];
		auto worldSize = game::World::GetSize();
		DrawKnownIslands(renderer, worldMap, worldSize);
		DrawAvatar(renderer, worldMap, worldSize);
		DrawCurrentDestinations(renderer, worldMap, worldSize);
		DrawNewDestination(renderer, worldMap, worldSize);
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> WorldMap::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto worldMapIndex = worldMaps.size();
		worldMaps.push_back({
			common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y]),
			common::XY<size_t>(model[visuals::data::Properties::WIDTH], model[visuals::data::Properties::HEIGHT]),
			std::nullopt
			});
		if (model.count(visuals::data::Properties::WORLD_MAP_ID)>0)
		{
			worldMapTable[layoutName][model[visuals::data::Properties::WORLD_MAP_ID]] = worldMapIndex;
		}

		return [worldMapIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalWorldMap(renderer, worldMapIndex);
		};
	}

	void WorldMap::SetDestination(const std::string& layoutName, const std::string& worldMapId, const std::optional<common::XY<int>>& destination)
	{
		size_t index = worldMapTable.find(layoutName)->second.find(worldMapId)->second;
		worldMaps[index].destination = destination;
	}

	std::optional<common::XY<double>> WorldMap::GetDestination(const std::string& layoutName, const std::string& worldMapId)
	{
		auto layoutIter = worldMapTable.find(layoutName);
		if (layoutIter != worldMapTable.end())
		{
			auto worldMapIter = layoutIter->second.find(worldMapId);
			if (worldMapIter != layoutIter->second.end())
			{
				size_t index = worldMapIter->second;
				auto worldMap = worldMaps[index];
				if (worldMap.destination)
				{
					return Pick(worldMaps[index], game::World::GetSize(), worldMap.destination.value());
				}
			}
		}
		return std::nullopt;
	}

	std::optional<common::XY<double>> WorldMap::GetHoverIsland(const std::string& layoutName, const std::string& worldMapId)
	{
		auto layoutIter = worldMapTable.find(layoutName);
		if (layoutIter != worldMapTable.end())
		{
			auto worldMapIter = layoutIter->second.find(worldMapId);
			if (worldMapIter != layoutIter->second.end())
			{
				size_t index = worldMapIter->second;
				const auto& worldMap = worldMaps[index];
				return worldMap.hoverIsland;
			}
		}
		return std::nullopt;
	}
}
