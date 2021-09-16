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
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_LAYOUT = ::UIState::IN_PLAY_TEMPLE;
	static const std::string LAYOUT_NAME = "State.InPlay.Temple";
	static const std::string MENU_ID = "Temple";
	static const std::string FORMAT_TEMPLE = "==Temple of {}==";
	static const std::string TEXT_CAPTION = "Caption";

	enum class TempleMenuItem
	{
		MAKE_OFFERING,
		LEAVE
	};

	static void OnMakeOffering()
	{
		::application::UIState::Write(::UIState::IN_PLAY_MAKE_OFFERING);
	}

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<TempleMenuItem, std::function<void()>> activators =
	{
		{ TempleMenuItem::MAKE_OFFERING, OnMakeOffering },
		{ TempleMenuItem::LEAVE, OnLeave }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnLeave },
		{::Command::RED, OnLeave }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto island = game::Islands::Read(location).value();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CAPTION, std::format(FORMAT_TEMPLE, island.patronDemigod));
	}

	void Temple::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_LAYOUT, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_LAYOUT, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_LAYOUT, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_LAYOUT, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_LAYOUT, LAYOUT_NAME);
	}
}