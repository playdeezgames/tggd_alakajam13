#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Common.RNG.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Texts.h>
#include <Game.Actors.h>
#include <format>
namespace state
{
	static const std::string LAYOUT_NAME = "State.GameOver";
	static const std::string TEXT_TIP_1 = "Tip1";
	static const std::string TEXT_TIP_2 = "Tip2";
	static const std::string TEXT_TIP_3 = "Tip3";

	static void RunOutTimer()
	{
		application::UIState::Write(::UIState::MAIN_MENU);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::LOSE);
		int score = 0;
		for (const auto& actor : game::Actors::All())
		{
			auto iter = actor.statistics.find(game::Statistic::TURDS_DEPOSITED);
			if (iter != actor.statistics.end())
			{
				score += iter->second;
			}
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TIP_3, std::format("{}", score));
	}

	void GameOver::Start()
	{
		::application::OnEnter::AddHandler(::UIState::GAME_OVER, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::GAME_OVER, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::GAME_OVER, RunOutTimer);
		::application::Renderer::SetRenderLayout(::UIState::GAME_OVER, LAYOUT_NAME);
	}
}
