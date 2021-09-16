#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.GamblingHand.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
#include <Visuals.CardSprites.h>
#include <Visuals.Menus.h>
#include <Visuals.Messages.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_START;
	static const std::string LAYOUT_NAME = "State.InPlay.GambleStart";
	static const std::string MENU_ID = "Wager";
	static const std::string IMAGE_FIRST_CARD = "FirstCard";
	static const std::string IMAGE_SECOND_CARD = "SecondCard";
	static const std::string TEXT_WAGER = "Wager";

	static double currentWager = 0;
	double GambleStart::GetCurrentWager()
	{
		return currentWager;
	}

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static common::XY<double> GetDockedLocation()
	{
		return game::avatar::Docked::GetDockedLocation().value();
	}

	static void OnNoBet()
	{
		auto ante = game::islands::DarkAlley::GetAnte(GetDockedLocation()).value();
		game::avatar::Statistics::ChangeMoney(-ante);
		//TODO: if avatar now has less than minimum bet... leave gambling!
		visuals::Messages::Write(
			{
				"==NO BET!==",
				{
					{
						{19,9},
						std::format("Pay the ante ({:.4f})", ante),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,11},
						"and are dealt two new cards.",
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					},
					{
						{19,13},
						std::format("Current funds: {:.4f}", game::avatar::Statistics::GetMoney()),
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					}
				}
			});
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	enum class WagerItem
	{
		BET,
		CHANGE_BET,
		NO_BET,
		LEAVE
	};

	const std::map<WagerItem, std::function<void()>> activators =
	{
		{ WagerItem::BET, application::UIState::GoTo(::UIState::IN_PLAY_GAMBLE_FINISH) },
		{ WagerItem::CHANGE_BET, []() {} },//TODO: make this work
		{ WagerItem::NO_BET, OnNoBet },
		{ WagerItem::LEAVE, OnLeave }
	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const size_t FIRST_CARD_INDEX = 0;
	const size_t SECOND_CARD_INDEX = 1;

	static void RefreshCards()
	{
		auto hand = game::islands::dark_alley::GamblingHand::Read(GetDockedLocation());
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_FIRST_CARD, visuals::CardSprites::GetSpriteForCard(hand[FIRST_CARD_INDEX]));
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_SECOND_CARD, visuals::CardSprites::GetSpriteForCard(hand[SECOND_CARD_INDEX]));
	}

	static void RefreshBet()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_WAGER, std::format("Wager {:.4f}(ante={:.4f})", currentWager, game::islands::DarkAlley::GetAnte(GetDockedLocation()).value()));
	}

	static void Refresh()
	{
		RefreshBet();
		RefreshCards();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		game::islands::dark_alley::GamblingHand::Deal(GetDockedLocation());
		currentWager = game::islands::DarkAlley::GetMinimumWager(GetDockedLocation()).value();
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) }
	};

	void GambleStart::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
