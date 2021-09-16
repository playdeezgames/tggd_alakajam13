#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Quest.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.ConfirmAbandonJob";
	static const std::string MENU_ID = "ConfirmAbandon";

	enum class ConfirmAbandonJobItem
	{
		NO,
		YES
	};

	static void AbandonJob()
	{
		game::avatar::Quest::AbandonQuest();
		application::UIState::Pop();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmAbandonJobItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonJobItem::NO, ::application::UIState::Pop },
		{ ConfirmAbandonJobItem::YES, AbandonJob }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::Pop },
		{ ::Command::RED, ::application::UIState::Pop }
	};

	void ConfirmAbandonJob::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB, LAYOUT_NAME);
	}
}