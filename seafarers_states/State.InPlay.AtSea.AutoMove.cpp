#include <Application.UIState.h>
#include <format>
#include <Game.Avatar.AtSea.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const size_t TICKS_TOTAL = 250;
	static size_t ticksLeft = TICKS_TOTAL;
	enum class AutoMoveState
	{
		OFF,
		ON,
		STARTING
	};
	static AutoMoveState autoMoveState = AutoMoveState::OFF;
	bool AtSea::IsAutoMoveEngaged()
	{
		return autoMoveState != AutoMoveState::OFF;
	}
	void AtSea::ToggleAutoMove()
	{
		if (autoMoveState == AutoMoveState::OFF)
		{
			ticksLeft = TICKS_TOTAL;
			autoMoveState = (game::Islands::CanDock()) ? (AutoMoveState::STARTING) : (AutoMoveState::ON);
		}
		else
		{
			autoMoveState = AutoMoveState::OFF;
		}
	}

	void AtSea::DoAutomoveTimer(const unsigned int& ticks)
	{
		if (autoMoveState != AutoMoveState::OFF)
		{
			if (ticks >= ticksLeft)
			{
				ticksLeft = TICKS_TOTAL;
				if (game::avatar::AtSea::Move() == game::avatar::AtSea::MoveResult::CLAMPED)
				{
					autoMoveState = AutoMoveState::OFF;
				}
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				ticksLeft -= ticks;
			}
		}
	}

	static void AddArrivalMessage()
	{
		auto island = game::Islands::GetDockableIslands().front();
		if (island.visits.has_value())
		{
			visuals::Messages::Write({
					"==ARRIVAL!==",
					{
						{
							{19,10},
							std::format("You arrive at {}.", island.name),
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			return;
		}
		visuals::Messages::Write({
				"==ARRIVAL!==",
				{
					{
						{19,10},
						"You arrive at an unknown island.",
						game::Colors::GRAY,
						visuals::HorizontalAlignment::CENTER
					}
				}
		});
	}

	void AtSea::UpdateAutoMoveState()
	{
		if (game::Islands::CanDock())
		{
			if (autoMoveState == AutoMoveState::ON)
			{
				autoMoveState = AutoMoveState::OFF;
				AddArrivalMessage();
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
		}
		else
		{
			if (autoMoveState == AutoMoveState::STARTING)
			{
				autoMoveState = AutoMoveState::ON;
			}
		}
	}
}