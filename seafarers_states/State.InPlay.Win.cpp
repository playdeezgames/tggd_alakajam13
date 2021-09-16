#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_WIN;
	static const std::string LAYOUT_NAME = "State.InPlay.Win";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Win::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, OnMouseButtonUp);
		::application::Command::SetHandler(CURRENT_STATE, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
