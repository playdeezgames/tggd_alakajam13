#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Achievements.h>
#include <Game.Audio.Mux.h>
#include <format>
#include <tuple>
#include "States.h"
#include "UIState.h"
#include <Visuals.Texts.h>
namespace state
{
	static const std::string LAYOUT_NAME = "State.Statistics";
	static const std::string TEXT_MOVES_MADE = "MovesMade";
	static const std::string TEXT_GAMES_PLAYED = "GamesPlayed";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void SetStatisticText(const std::string& textId, const std::string& caption, const game::Statistic& statistic)
	{
		auto value = game::Statistics::Read(statistic);
		if (value)
		{
			visuals::Texts::SetText(LAYOUT_NAME, textId, std::format("{}: {}", caption, value.value()));
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, textId, std::format("{}: -", caption));
		}
	}

	static const std::vector<std::tuple<std::string, std::string, game::Statistic>> statisticsList =
	{
		{ TEXT_MOVES_MADE, "Moves Made", game::Statistic::MOVES_MADE},
		{ TEXT_GAMES_PLAYED, "Games Played", game::Statistic::GAMES_PLAYED}
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		for (auto& item : statisticsList)
		{
			SetStatisticText(std::get<0>(item), std::get<1>(item), std::get<2>(item));
		}
	}

	void Statistics::Start()
	{
		::application::OnEnter::AddHandler(::UIState::STATISTICS, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::STATISTICS, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::STATISTICS, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::STATISTICS, LAYOUT_NAME);
	}
}
