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
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Islands.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_CAREEN_SELECT;
	static const std::string LAYOUT_NAME = "State.InPlay.AtSeaCareenSelect";
	static const std::string MENU_ID = "Careen";
	static const std::string MENU_ITEM_PORT = "Port";
	static const std::string MENU_ITEM_STARBOARD = "Starboard";
	static const std::string FORMAT_PORT = "Port({:.0f}%)";
	static const std::string FORMAT_STARBOARD = "Starboard({:.0f}%)";

	enum class CareenMenuItem
	{
		STARBOARD,
		PORT,
		BELAY
	};

	static void OnStarboard()
	{
		game::Avatar::DoAction(game::avatar::Action::CAREEN_TO_STARBOARD);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnPort()
	{
		game::Avatar::DoAction(game::avatar::Action::CAREEN_TO_PORT);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnBelay()
	{
		application::UIState::Pop();
	}

	static const std::map<CareenMenuItem, std::function<void()>> activators =
	{
		{ CareenMenuItem::STARBOARD, OnStarboard },
		{ CareenMenuItem::PORT, OnPort },
		{ CareenMenuItem::BELAY, OnBelay }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnBelay },
		{::Command::RED, OnBelay }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_PORT, std::format(FORMAT_PORT, game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::PORT)));
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_STARBOARD, std::format(FORMAT_STARBOARD, game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::STARBOARD)));
	}

	void AtSeaCareenSelect::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}