#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Quest.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.ConfirmReplaceJob";
	static const std::string MENU_ID = "ConfirmReplace";

	enum class ConfirmReplaceJobItem
	{
		NO,
		YES
	};

	static void ReplaceJob()
	{
		game::avatar::Quest::AbandonQuest();
		game::avatar::Quest::AcceptQuest(game::avatar::Docked::GetDockedLocation().value());
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnNo()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmReplaceJobItem, std::function<void()>> activators =
	{
		{ ConfirmReplaceJobItem::NO, OnNo },
		{ ConfirmReplaceJobItem::YES, ReplaceJob }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, OnNo },
		{ ::Command::RED, OnNo }
	};

	void ConfirmReplaceJob::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, LAYOUT_NAME);
	}
}