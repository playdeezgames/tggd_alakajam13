#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Visuals.Confirmations.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.Confirm";
	static const std::string MENU_ID = "Confirm";
	static const std::string TEXT_PROMPT = "Prompt";

	enum class ConfirmItem
	{
		NO,
		YES
	};

	static visuals::Confirmation confirmation;

	static void OnYes()
	{
		confirmation.onYes();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnNo()
	{
		confirmation.onNo();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmItem, std::function<void()>> activators =
	{
		{ ConfirmItem::NO, OnNo },
		{ ConfirmItem::YES, OnYes }
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

	static void Refresh()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_PROMPT, confirmation.prompt);
	}

	static void OnEnter()
	{
		if (visuals::Confirmations::HasConfirmation())
		{
			confirmation = visuals::Confirmations::Read();
			Refresh();
			game::audio::Mux::Play(game::audio::Theme::MAIN);//TODO: have this come from the confirmation object?
			return;
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	void Confirm::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CONFIRM, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CONFIRM, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CONFIRM, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CONFIRM, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CONFIRM, LAYOUT_NAME);
	}
}