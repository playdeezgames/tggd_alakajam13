#include <Common.Heading.h>
#include <Common.XY.h>
#include <functional>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Destinations.h>
#include <Game.Avatar.Quest.h>
#include <Game.Colors.h>
#include <Game.Fisheries.h>
#include <Game.Islands.h>
#include <Game.Merchants.h>
#include <Game.World.h>
#include <Visuals.Colors.h>
#include <Visuals.Data.Properties.h>
#include <Visuals.Fonts.h>
#include "Visuals.Navigator.h"
#include <Visuals.Sprites.h>
namespace visuals
{
	static const std::string PROPERTY_TEXT_OFFSET_X = "text-offset-x";
	static const std::string PROPERTY_TEXT_OFFSET_Y = "text-offset-y";

	static const std::string SPRITE_MERCHANT_SHIP = "AtSeaMerchantShip";
	static const std::string SPRITE_CURRENT_HEADING = "AtSeaCurrentHeading";
	static const std::string SPRITE_WIND_DIRECTION = "AtSeaWindDirection";
	static const std::string SPRITE_SHIP = "AtSeaShip";
	static const std::string SPRITE_ISLAND = "AtSeaIsland";
	static const std::string SPRITE_QUEST_DESTINATION = "AtSeaQuestDestination";
	static const std::string SPRITE_DESTINATION_1 = "AtSeaDestination1";
	static const std::string SPRITE_DESTINATION_2 = "AtSeaDestination2";
	static const std::string SPRITE_DESTINATION_3 = "AtSeaDestination3";
	static const std::string SPRITE_DESTINATION_4 = "AtSeaDestination4";
	static const std::string SPRITE_FISHERY = "AtSeaFishery";

	static const std::string FONT_ISLAND_NAME = "font5x7";

	struct InternalNavigator
	{
		common::XY<int> xy;
		size_t radius;
		common::XY<int> textOffset;
	};

	static std::vector<InternalNavigator> navigators;
	static std::map<std::string, std::map<std::string, size_t>> navigatorTable;

	static std::function<common::XY<int>(const common::XY<double>&)> MakePlotter(const InternalNavigator& navigator)
	{
		return [navigator](const common::XY<double>& location)
		{
			auto avatarLocation = game::avatar::AtSea::GetLocation();
			auto difference = (location - avatarLocation);
			auto scaledDifference = difference * (double)navigator.radius / game::World::GetViewDistance();
			common::XY<int> plottedDifference = {(int)scaledDifference.GetX(), (int)scaledDifference.GetY()};
			return common::XY<int>(plottedDifference.GetX()+navigator.xy.GetX(), plottedDifference.GetY() + navigator.xy.GetY());
		};
	}

	static std::function<bool(const common::XY<int>&)> MakeDrawFilter(const InternalNavigator& navigator)
	{
		return [navigator](const common::XY<int>& plot)
		{
			auto difference = plot - navigator.xy;
			return difference.GetMagnitude() < navigator.radius;
		};
	}

	static void DrawIslands(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		const InternalNavigator& navigator)
	{
		auto islands = game::Islands::GetViewableIslands();
		for (auto& island : islands)
		{
			auto plot = plotter(island.absoluteLocation);
			Sprites::Draw(SPRITE_ISLAND, renderer, plot, visuals::Colors::Read(game::Colors::WHITE));
			auto textPlot = plot + navigator.textOffset;
			auto islandName = (island.visits.has_value()) ? (island.name) : (game::Islands::UNKNOWN);
			visuals::Fonts::WriteText(
				FONT_ISLAND_NAME,
				renderer, 
				textPlot + common::XY<int>(1,1),
				islandName,
				game::Colors::BLACK,
				visuals::HorizontalAlignment::CENTER);
			visuals::Fonts::WriteText(
				FONT_ISLAND_NAME, 
				renderer, 
				textPlot, 
				islandName, 
				game::Colors::GRAY, 
				visuals::HorizontalAlignment::CENTER);
		}
	}

	static void DrawMerchants(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		std::function<bool(const common::XY<int>&)> drawFilter)
	{
		auto merchants = game::Merchants::All();
		for (auto& merchant : merchants)
		{
			auto plot = plotter(merchant.location);
			if (drawFilter(plot))
			{
				Sprites::Draw(SPRITE_MERCHANT_SHIP, renderer, plot, visuals::Colors::Read(game::Colors::WHITE));
			}
		}
	}

	static void DrawCurrentHeading(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		const InternalNavigator& navigator)
	{
		auto heading = game::avatar::AtSea::GetHeading();
		Sprites::Draw(
			SPRITE_CURRENT_HEADING, 
			renderer, 
			navigator.xy.Convert<int>(), 
			visuals::Colors::Read(game::Colors::WHITE),
			heading);
	}

	static void DrawShip(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		const InternalNavigator& navigator)
	{
		Sprites::Draw(
			SPRITE_SHIP,
			renderer,
			navigator.xy.Convert<int>(),
			visuals::Colors::Read(game::Colors::WHITE));
	}


	static void DrawWindDirection(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		const InternalNavigator& navigator)
	{
		auto windHeading = common::Heading::ToRadians(game::World::GetWindHeading() + common::Heading::DEGREES / 2.0);
		common::XY<int> plot = 
		{ 
			(int)(navigator.xy.GetX() + std::cos(windHeading) * navigator.radius), 
			(int)(navigator.xy.GetY() + std::sin(windHeading) * navigator.radius) 
		};
		Sprites::Draw(
			SPRITE_WIND_DIRECTION,
			renderer,
			plot,
			visuals::Colors::Read(game::Colors::WHITE));
	}

	static void DrawQuestDestination(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		const InternalNavigator& navigator)
	{
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			auto clampedDistance = common::Heading::ClampDistance(quest.value().destination - game::avatar::AtSea::GetLocation(), game::World::GetViewDistance());
			auto plot = plotter(clampedDistance+ game::avatar::AtSea::GetLocation());
			Sprites::Draw(
				SPRITE_QUEST_DESTINATION,
				renderer,
				plot,
				visuals::Colors::Read(game::Colors::WHITE));
		}
	}

	static const std::map<game::avatar::Destination, std::string> destinationSprites = 
	{
		{game::avatar::Destination::ONE, SPRITE_DESTINATION_1},
		{game::avatar::Destination::TWO, SPRITE_DESTINATION_2},
		{game::avatar::Destination::THREE, SPRITE_DESTINATION_3},
		{game::avatar::Destination::FOUR, SPRITE_DESTINATION_4}
	};

	static void DrawAvatarDestination(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		const InternalNavigator& navigator,
		const game::avatar::Destination& destinationId)
	{
		auto destination = game::avatar::Destinations::GetDestination(destinationId);
		if (destination)
		{
			auto clampedDistance = common::Heading::ClampDistance(destination.value() - game::avatar::AtSea::GetLocation(), game::World::GetViewDistance() + 0.5);//TODO: magic number
			auto plot = plotter(clampedDistance + game::avatar::AtSea::GetLocation());
			Sprites::Draw(
				destinationSprites.find(destinationId)->second,
				renderer,
				plot,
				visuals::Colors::Read(game::Colors::WHITE));
		}
	}


	static void DrawAvatarDestinations(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		const InternalNavigator& navigator)
	{
		for (auto destinationId : game::avatar::Destinations::All())
		{
			DrawAvatarDestination(renderer, plotter, navigator, destinationId);
		}
	}

	static void DrawFisheries(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		std::function<common::XY<int>(const common::XY<double>&)> plotter,
		std::function<bool(const common::XY<int>&)> drawFilter,
		const InternalNavigator& navigator)
	{
		auto fisheries = game::Fisheries::All();
		for (auto& fishery : fisheries)
		{
			auto plot = plotter(fishery.location);
			if (drawFilter(plot))
			{
				Sprites::Draw(SPRITE_FISHERY, renderer, plot, visuals::Colors::Read(game::Colors::WHITE));
			}
		}
	}

	static void DrawInternalNavigator(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t navigatorIndex)
	{
		auto& navigator = navigators[navigatorIndex];
		auto plotter = MakePlotter(navigator);
		auto filter = MakeDrawFilter(navigator);
		DrawFisheries(renderer, plotter, filter, navigator);
		DrawCurrentHeading(renderer, navigator);
		DrawIslands(renderer, plotter, navigator);
		DrawMerchants(renderer, plotter, filter);
		DrawShip(renderer, navigator);
		DrawWindDirection(renderer, navigator);
		DrawAvatarDestinations(renderer, plotter, navigator);
		DrawQuestDestination(renderer, plotter, navigator);
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Navigator::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto navigatorIndex = navigators.size();
		navigators.push_back({
			common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y]),
			model[visuals::data::Properties::RADIUS],
			common::XY<int>(model[PROPERTY_TEXT_OFFSET_X], model[PROPERTY_TEXT_OFFSET_Y])
			});
		if (model.count(visuals::data::Properties::WORLD_MAP_ID) > 0)
		{
			navigatorTable[layoutName][model[visuals::data::Properties::WORLD_MAP_ID]] = navigatorIndex;
		}

		return [navigatorIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalNavigator(renderer, navigatorIndex);
		};
	}
}
