#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Islands.h>
#include <Game.Islands.Features.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.Docked";
	static const std::string MENU_ID = "Order";
	static const std::string MENU_ITEM_SHIPYARD = "Shipyard";
	static const std::string MENU_ITEM_DARK_ALLEY = "DarkAlley";
	static const std::string MENU_ITEM_TEMPLE = "Temple";
	static const std::string TEXT_ISLAND_NAME = "island-name";
	static const std::string TEXT_ISLAND_VISITS = "island-visits";
	static const std::string FORMAT_NAME = "Island Name: {}";
	static const std::string FORMAT_VISITS = "Visits: {}";
	static const std::string FORMAT_TEMPLE = "Temple of {}";

	enum class OrderMenuItem
	{
		UNDOCK,
		JOBS,
		TRADE,
		SHIPYARD,
		DARK_ALLEY,
		TEMPLE
	};

	static void OnUndock()
	{
		game::Avatar::DoAction(game::avatar::Action::UNDOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnJob()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_JOB_BOARD);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnTrade()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnShipyard()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_SHIPYARD);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDarkAlley()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DARK_ALLEY);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnTemple()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_TEMPLE);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{ OrderMenuItem::UNDOCK, OnUndock },
		{ OrderMenuItem::JOBS, OnJob },
		{ OrderMenuItem::TRADE, OnTrade },
		{ OrderMenuItem::SHIPYARD, OnShipyard },
		{ OrderMenuItem::DARK_ALLEY, OnDarkAlley },
		{ OrderMenuItem::TEMPLE, OnTemple }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnUndock },
		{::Command::RED, OnUndock }
	};

	static const std::map<std::string, game::Feature> featureMenuItems =
	{
		{MENU_ITEM_SHIPYARD, game::Feature::SHIPYARD},
		{MENU_ITEM_DARK_ALLEY, game::Feature::DARK_ALLEY}
	};

	static void RefreshIslandDetails()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto island = game::Islands::Read(location).value();
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_TEMPLE, std::format(FORMAT_TEMPLE, island.patronDemigod));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ISLAND_NAME, std::format(FORMAT_NAME,island.name));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ISLAND_VISITS, std::format(FORMAT_VISITS, island.visits.value_or(0)));
		for (auto featureMenuItem : featureMenuItems)
		{
			visuals::MenuItems::SetEnabled(
				LAYOUT_NAME, 
				featureMenuItem.first, 
				game::islands::Features::Read(location, featureMenuItem.second));
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		RefreshIslandDetails();
	}

	void Docked::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DOCKED, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_DOCKED, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DOCKED, LAYOUT_NAME);
	}
}