#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Islands.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.IslandTrade";
	static const std::string MENU_ID = "Trade";

	enum class TradeMenuItem
	{
		BUY,
		SELL,
		LEAVE
	};

	static void OnBuy()
	{
		game::Avatar::DoAction(game::avatar::Action::MARKET_BUY);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnSell()
	{
		game::Avatar::DoAction(game::avatar::Action::MARKET_SELL);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<TradeMenuItem, std::function<void()>> activators =
	{
		{ TradeMenuItem::BUY, OnBuy },
		{ TradeMenuItem::SELL, OnSell },
		{ TradeMenuItem::LEAVE, OnLeave }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnLeave },
		{::Command::RED, OnLeave }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void IslandTrade::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_TRADE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_TRADE, LAYOUT_NAME);
	}
}