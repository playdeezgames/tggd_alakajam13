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
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_GAMBLE_FINISH;
	static const std::string LAYOUT_NAME = "State.InPlay.GambleFinish";
	static const std::string MENU_ID = "Proceed";
	static const std::string IMAGE_FIRST_CARD = "FirstCard";
	static const std::string IMAGE_SECOND_CARD = "SecondCard";
	static const std::string IMAGE_THIRD_CARD = "ThirdCard";
	static const std::string TEXT_RESULT = "Result";
	static const std::string TEXT_WIN_LOSE = "WinLose";

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	enum class ProceedItem
	{
		PLAY_AGAIN,
		LEAVE
	};

	static const std::map<ProceedItem, std::function<void()>> activators =
	{
		{ ProceedItem::PLAY_AGAIN, application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ProceedItem::LEAVE, OnLeave }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static common::XY<double> GetDockedLocation()
	{
		return game::avatar::Docked::GetDockedLocation().value();
	}

	static const size_t FIRST_CARD_INDEX = 0;
	static const size_t SECOND_CARD_INDEX = 1;
	static const size_t THIRD_CARD_INDEX = 2;

	static void RefreshCards()
	{
		auto hand = game::islands::dark_alley::GamblingHand::Read(GetDockedLocation());
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_FIRST_CARD, visuals::CardSprites::GetSpriteForCard(hand[FIRST_CARD_INDEX]));
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_SECOND_CARD, visuals::CardSprites::GetSpriteForCard(hand[SECOND_CARD_INDEX]));
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_THIRD_CARD, visuals::CardSprites::GetSpriteForCard(hand[THIRD_CARD_INDEX]));
	}

	static void Refresh()
	{
		RefreshCards();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		if (game::islands::dark_alley::GamblingHand::IsWinner(GetDockedLocation()))
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_RESULT, "You win!");
			game::avatar::Statistics::ChangeMoney(GambleStart::GetCurrentWager());
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_RESULT, "You lose!");
			game::avatar::Statistics::ChangeMoney(GambleStart::GetCurrentWager());
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_WIN_LOSE, std::format("You now have {:.4f}", game::avatar::Statistics::GetMoney()));
		//TODO: hide "play again" if avatar has less than the minimum wager
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

	void GambleFinish::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
