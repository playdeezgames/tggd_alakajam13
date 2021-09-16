#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::MAIN_MENU;
	static const std::string LAYOUT_NAME = "State.MainMenu";
	static const std::string MENU_ID = "Main";
	static const std::string SUBLAYOUT_MODDED = "Sublayout.Modded";
	static const std::string TEXT_MODDED = "modded";
	static const std::string MODDED = "Modded";

	enum class MainMenuItem
	{
		START,
		ABOUT,
		STATISTICS,
		OPTIONS,
		QUIT
	};

	static void GoToAbout()
	{
		::application::UIState::Write(::UIState::ABOUT);
	}

	static const std::map<MainMenuItem, std::function<void()>> activators =
	{
		{ MainMenuItem::START, ::application::UIState::GoTo(::UIState::START_GAME) },
		{ MainMenuItem::OPTIONS, ::application::UIState::PushTo(::UIState::OPTIONS)  },
		{ MainMenuItem::STATISTICS, ::application::UIState::GoTo(::UIState::STATISTICS)  },
		{ MainMenuItem::ABOUT, GoToAbout },
		{ MainMenuItem::QUIT, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT)  },
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) }
	};

	static void OnEnter()
	{
		if (data::json::Stores::IsModded())
		{
			visuals::Texts::SetText(SUBLAYOUT_MODDED, TEXT_MODDED, MODDED);
		}
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void MainMenu::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}