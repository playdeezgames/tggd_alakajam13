#include "UIState.h"
#include "States.h"
#include <string>
#include <Application.OnEnter.h>
#include <Application.Command.h>
#include <Application.Renderer.h>
#include <Game.Audio.Mux.h>
#include <Application.UIState.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_BOARD;
	static const std::string LAYOUT_NAME = "State.InPlay.Board";

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	static const std::map<::Command, std::function<void()>> commandTable =
	{
		{::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY)}
	};

	void Board::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Command::SetHandlers(CURRENT_STATE, commandTable);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}