#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CONFIRM_ABANDON;
	static const std::string LAYOUT_NAME = "State.ConfirmAbandon";
	static const std::string MENU_ID = "ConfirmAbandon";

	enum class ConfirmAbandonItem
	{
		NO,
		YES
	};

	static void CancelAbandon()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmAbandonItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonItem::NO, CancelAbandon },
		{ ConfirmAbandonItem::YES, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	void ConfirmAbandon::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}