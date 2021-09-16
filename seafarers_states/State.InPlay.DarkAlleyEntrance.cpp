#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseMotion.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Audio.Sfx.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.FightCard.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.CardSprites.h>
#include <Visuals.Images.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.DarkAlleyEntrance";
	static const std::string SPRITE_CARD_BACK = "CardBackRed";
	static const std::string TEXT_ENEMY_BRAWLING = "EnemyBrawling";
	static const std::string TEXT_BRAWLING = "Brawling";
	static const std::string TEXT_HEALTH = "Health";
	static const std::string TEXT_BUTTON = "Button";
	static const std::string AREA_BUTTON_HOVER = "ButtonHover";
	static const std::string TEXT_FIGHT_STATE = "FightState";

	static std::optional<size_t> hoverCard = std::nullopt;
	
	enum class FightResult
	{
		WIN,
		LOSE
	};

	static std::optional<FightResult> fightResult = std::nullopt;
	static size_t hitsTaken = 0;

	const auto GetDockedLocation = []() { return game::avatar::Docked::GetDockedLocation().value();	};

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, size_t> cardAreas =
	{
		{"Card0Hover", 0},
		{"Card1Hover", 1},
		{"Card2Hover", 2},
		{"Card3Hover", 3},
		{"Card4Hover", 4},
		{"Card5Hover", 5},
		{"Card6Hover", 6},
		{"Card7Hover", 7},
		{"Card8Hover", 8},
		{"Card9Hover", 9},
		{"Card10Hover", 10},
		{"Card11Hover", 11}
	};

	struct CardPosition
	{
		std::string adjacentText;
		std::string cardImage;
		std::string cardSelectImage;
	};

	static const std::map<size_t, CardPosition> cardPositions =
	{
		{
			0, 
			{
				"Card0Adjacent",
				"Card0",
				"Card0Select"
			}
		},
		{
			1,
			{
				"Card1Adjacent",
				"Card1",
				"Card1Select"
			}
		},
		{
			2,
			{
				"Card2Adjacent",
				"Card2",
				"Card2Select"
			}
		},
		{
			3,
			{
				"Card3Adjacent",
				"Card3",
				"Card3Select"
			}
		},
		{
			4,
			{
				"Card4Adjacent",
				"Card4",
				"Card4Select"
			}
		},
		{
			5,
			{
				"Card5Adjacent",
				"Card5",
				"Card5Select"
			}
		},
		{
			6,
			{
				"Card6Adjacent",
				"Card6",
				"Card6Select"
			}
		},
		{
			7,
			{
				"Card7Adjacent",
				"Card7",
				"Card7Select"
			}
		},
		{
			8,
			{
				"Card8Adjacent",
				"Card8",
				"Card8Select"
			}
		},
		{
			9,
			{
				"Card9Adjacent",
				"Card9",
				"Card9Select"
			}
		},
		{
			10,
			{
				"Card10Adjacent",
				"Card10",
				"Card10Select"
			}
		},
		{
			11,
			{
				"Card11Adjacent",
				"Card11",
				"Card11Select"
			}
		}
	};


	static void RefreshCardSelect()
	{
		for (auto cardPosition : cardPositions)
		{
			visuals::Images::SetVisible(
				LAYOUT_NAME, 
				cardPosition.second.cardSelectImage,
				(hoverCard.has_value() && hoverCard.value() == cardPosition.first));
		}
	}

	static void RefreshCards()
	{
		auto fightCards = game::islands::dark_alley::FightCard::Read(GetDockedLocation());
		for (auto& fightCard : fightCards)
		{
			auto& cardImage = cardPositions.find(fightCard.first)->second.cardImage;
			auto& sprite = visuals::CardSprites::GetSpriteForCard(fightCard.second.card);
			visuals::Images::SetSprite(LAYOUT_NAME, cardImage, (fightCard.second.shown) ? (sprite) :(SPRITE_CARD_BACK));
			visuals::Texts::SetText(
				LAYOUT_NAME, 
				cardPositions.find(fightCard.first)->second.adjacentText, 
				(!fightCard.second.shown || fightCard.second.success) ?
				("") :
				(std::format("{}", fightCard.second.adjacent)));
		}
	}

	static const auto GetRuffianBrawling = []() { return game::islands::DarkAlley::GetRuffianBrawling(GetDockedLocation()).value(); };

	static void RefreshStatistics()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_ENEMY_BRAWLING, 
			std::format(
				game::avatar::Statistics::FORMAT_BRAWLING, 
				GetRuffianBrawling()));
		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_BRAWLING, 
			std::format(
				game::avatar::Statistics::FORMAT_BRAWLING, 
				game::avatar::Statistics::GetBrawling()));
		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_HEALTH, 
			std::format(
				game::avatar::Statistics::FORMAT_HEALTH, 
				game::avatar::Statistics::GetHealth()));
	}

	static std::string GetButtonText()
	{
		if (fightResult)
		{
			return "Continue....";
		}
		return "Retreat!";
	}

	static void RefreshButton()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_BUTTON,
			GetButtonText());
	}

	static std::string GetFightStateText()
	{
		if (fightResult)
		{
			switch (fightResult.value())
			{
			case FightResult::LOSE:
				return "Defeated!";
			case FightResult::WIN:
				return "You win!";
			}
		}
		return "Pick a face card to defeat enemy";
	}

	static void RefreshFightState()
	{
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_FIGHT_STATE,
			GetFightStateText());
	}

	static void Refresh()
	{
		RefreshButton();
		RefreshFightState();
		RefreshStatistics();
		RefreshCards();
		RefreshCardSelect();
	}

	static void SetButtonHover(bool hover)
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_BUTTON, hover ? game::Colors::CYAN : game::Colors::GRAY);
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>&)
	{
		SetButtonHover(false);
		auto iter = cardAreas.find(areaName);
		if (iter != cardAreas.end())
		{
			hoverCard = iter->second;
			RefreshCardSelect();
			return;
		}
		if (areaName == AREA_BUTTON_HOVER)
		{
			SetButtonHover(true);
		}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>&)
	{
		hoverCard = std::nullopt;
		SetButtonHover(false);
		RefreshCardSelect();
	}

	static void HandleRuffianDefeated()
	{
		game::audio::Sfx::Play(game::audio::GameSfx::ENEMY_HIT);
		fightResult = FightResult::WIN;
	}

	static void HandleTakeDamage()
	{
		game::avatar::Statistics::ChangeHealth(-GetRuffianBrawling());
		if (game::avatar::Statistics::IsDead())
		{
			fightResult = FightResult::LOSE;
		}
		game::audio::Sfx::Play(game::audio::GameSfx::HIT);
		hitsTaken++;
	}

	static void HandleFightCard(const game::islands::dark_alley::FightCard& fightCard)
	{
		if (fightCard.success)
		{
			HandleRuffianDefeated();
			return;
		}
		HandleTakeDamage();
	}

	static void IncreaseInfamy()
	{
		const double INFAMY_DELTA = 0.1;
		game::avatar::Statistics::ChangeInfamy((hitsTaken==0) ? (INFAMY_DELTA) : (INFAMY_DELTA/2.0));
	}

	static void IncreaseBrawling()
	{
		const double BRAWLING_DELTA = 0.1;
		game::avatar::Statistics::ChangeBrawling((hitsTaken > 0) ? (BRAWLING_DELTA) : (BRAWLING_DELTA / 2.0));
	}

	static bool HandleButton()
	{
		if (fightResult)
		{
			switch (fightResult.value())
			{
			case FightResult::LOSE:
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
				break;
			case FightResult::WIN:
				IncreaseInfamy();
				IncreaseBrawling();
				game::Avatar::DoAction(game::avatar::Action::DEFEAT_RUFFIAN);
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
				break;
			}
			return true;
		}
		game::avatar::Statistics::ChangeMoney(-game::avatar::Statistics::GetMoney()/2.0);
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (hoverCard && !fightResult.has_value())
		{
			auto fightCard = game::islands::dark_alley::FightCard::Pick(GetDockedLocation(), hoverCard.value());
			if (fightCard)
			{
				HandleFightCard(fightCard.value());
			}
			Refresh();
			return true;
		}
		if (areaName == AREA_BUTTON_HOVER)
		{
			return HandleButton();
		}
		return false;
	}

	static void ResetDisplay()
	{
		for (auto& cardPosition : cardPositions)
		{
			visuals::Texts::SetText(LAYOUT_NAME, cardPosition.second.adjacentText, "");
			visuals::Images::SetSprite(LAYOUT_NAME, cardPosition.second.cardImage, SPRITE_CARD_BACK);
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		game::islands::dark_alley::FightCard::Generate(GetDockedLocation());
		hoverCard = std::nullopt;
		fightResult = std::nullopt;
		hitsTaken = 0;
		ResetDisplay();
		Refresh();
	}

	void DarkAlleyEntrance::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, LAYOUT_NAME);
	}
}
