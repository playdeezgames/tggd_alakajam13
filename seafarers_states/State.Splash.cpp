#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.Update.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
namespace state
{
	static const size_t TICKS_TOTAL = 3000;
	static const std::string LAYOUT_NAME = "State.Splash";
	static size_t ticksLeft = TICKS_TOTAL;

	static void RunOutTimer()
	{
		ticksLeft = 0;
	}

	static void OnUpdate(const unsigned int& ticks)
	{
		ticksLeft = (ticksLeft > ticks) ? (ticksLeft - ticks) : (0);
		if (ticksLeft == 0)
		{
			::application::UIState::Write(::UIState::MAIN_MENU);
			ticksLeft = TICKS_TOTAL;
		}
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		RunOutTimer();
		return true;
	}

	void Splash::Start()
	{
		::application::OnEnter::AddHandler(::UIState::SPLASH, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::SPLASH, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::SPLASH, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::SPLASH, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::SPLASH, OnUpdate);
	}
}
