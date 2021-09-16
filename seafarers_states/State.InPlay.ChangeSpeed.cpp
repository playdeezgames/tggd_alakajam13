#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.AtSea.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.ChangeSpeed";
	static const std::string MENU_ID = "ChangeSpeed";

	enum class ChangeSpeedItem
	{
		ALL_STOP,
		AHEAD_ONE_THIRD,
		AHEAD_TWO_THIRDS,
		AHEAD_FULL,
		AHEAD_FLANK,
		BELAY
	};

	static void OnLeave()
	{
		::application::UIState::Pop();
	}

	static std::function<void()> SetSpeed(double speed)
	{
		return [speed]()
		{
			game::avatar::AtSea::SetSpeed(speed);
			OnLeave();
		};
	}

	static const double SPEED_ALL_STOP = 0.0;
	static const double SPEED_ONE_THIRD = 0.3;
	static const double SPEED_TWO_THIRDS = 0.6;
	static const double SPEED_FULL = 0.9;
	static const double SPEED_FLANK = 1.0;

	static const std::map<ChangeSpeedItem, std::function<void()>> activators =
	{
		{ ChangeSpeedItem::ALL_STOP, SetSpeed(SPEED_ALL_STOP) },
		{ ChangeSpeedItem::AHEAD_ONE_THIRD, SetSpeed(SPEED_ONE_THIRD) },
		{ ChangeSpeedItem::AHEAD_TWO_THIRDS, SetSpeed(SPEED_TWO_THIRDS) },
		{ ChangeSpeedItem::AHEAD_FULL, SetSpeed(SPEED_FULL) },
		{ ChangeSpeedItem::AHEAD_FLANK, SetSpeed(SPEED_FLANK) },
		{ ChangeSpeedItem::BELAY, ::application::UIState::Pop }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	void ChangeSpeed::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CHANGE_SPEED, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CHANGE_SPEED, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CHANGE_SPEED, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CHANGE_SPEED, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CHANGE_SPEED, LAYOUT_NAME);
	}
}