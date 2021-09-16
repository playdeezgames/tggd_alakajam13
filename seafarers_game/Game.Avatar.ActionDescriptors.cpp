#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Avatar.Dock.h>
#include <format>
#include <functional>
#include "Game.Avatar.Action.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.State.h"
#include "Game.Avatar.StateTransition.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Islands.h"
#include <map>
namespace game::avatar
{
	const std::string FORMAT_UNDOCK = "You undock from {}.";

	static StateTransition OnUndock()
	{
		auto location = game::avatar::Docked::GetDockedLocation();
		auto island = game::Islands::Read(location.value()).value();
		data::game::avatar::Dock::Clear();
		return {
			game::Colors::GREEN,
			std::format(FORMAT_UNDOCK, island.name),
			avatar::State::AT_SEA
		};
	}

	void OnEnterDarkAlleyFailsInfamyRequirement();

	static StateTransition OnEnterDarkAlley()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto data = data::game::island::DarkAlley::Read(location).value();
		auto infamy = game::avatar::Statistics::GetInfamy();
		if (infamy < data.infamyRequirement)
		{
			OnEnterDarkAlleyFailsInfamyRequirement();
			return
			{
				game::Colors::GREEN,
				"You enter dark alley.",
				avatar::State::DARK_ALLEY_ENTRANCE
			};
		}
		return
		{
			game::Colors::GREEN,
			"You enter dark alley.",
			avatar::State::DARK_ALLEY
		};
	}

	static StateTransition OnDefeatRuffian()
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You enter the dark alley.",
			avatar::State::DARK_ALLEY
		};
	}

	static StateTransition OnStartGambling()
	{
		//TODO: add a message
		return
		{
			game::Colors::GREEN,
			"You approach some shady characters playing a card game.",
			avatar::State::GAMBLE_START
		};
	}

	static StateTransition OnStartFishing()
	{
		if (game::avatar::Items::Has(Item::FISHING_POLE))
		{
			if (game::avatar::Items::Has(Item::BAIT))
			{
				Fishboard::Generate();
				return
				{
					game::Colors::GRAY,
					"Nice day for fishin', innit? Huh-ha!",
					avatar::State::FISHING
				};
			}
		}
		return
		{
			game::Colors::RED,
			"You cannot fish right now.",
			avatar::State::AT_SEA
		};
	}

	static StateTransition OnStopFishing()
	{
		return
		{
			game::Colors::GRAY,
			"You wake from yer nap.",
			avatar::State::AT_SEA
		};
	}

	static std::function<StateTransition()> DoTransition(const StateTransition& transition)
	{
		return [transition]() { return transition; };
	}

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>> actionDescriptors =
	{
		{
			avatar::Action::UNDOCK,
			{
				{
					avatar::State::DOCK,
					OnUndock
				}
			}
		},
		{
			avatar::Action::ENTER_MARKET,
			{
				{
					avatar::State::MARKET_BUY,
					DoTransition(
						{
							game::Colors::GREEN,
							"You enter the market.",
							avatar::State::MARKET
						})
				},
				{
					avatar::State::MARKET_SELL,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						avatar::State::MARKET
					})
				},
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the market.",
						avatar::State::MARKET
					})
				}
			}
		},
		{
			avatar::Action::ENTER_DOCK,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the market.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::SHIPYARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the shipyard.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::JOB_BOARD,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the job board.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::DARK_ALLEY_ENTRANCE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::DARK_ALLEY,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the dark alley.",
						avatar::State::DOCK
					})
				},
				{
					avatar::State::TEMPLE,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the temple.",
						avatar::State::DOCK
					})
				},
			}
		},
		{
			avatar::Action::MARKET_BUY,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You browse for items to buy.",
						avatar::State::MARKET_BUY
					})
				}
			}
		},
		{
			avatar::Action::MARKET_SELL,
			{
				{
					avatar::State::MARKET,
					DoTransition(
					{
						game::Colors::GREEN,
						"You look to sell yer items.",
						avatar::State::MARKET_SELL
					})
				}
			}
		},
		{
			avatar::Action::ENTER_JOB_BOARD,
			{
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You check for posted jobs.",
						avatar::State::JOB_BOARD
					})
				}
			}
		},
		{
			avatar::Action::ENTER_TEMPLE,
			{
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the temple.",
						avatar::State::TEMPLE
					})
				}
			}
		},
		{
			avatar::Action::ENTER_SHIPYARD,
			{
				{
					avatar::State::DOCK,
					DoTransition(
					{
						game::Colors::GREEN,
						"You enter the shipyard.",
						avatar::State::SHIPYARD
					})
				}
			}
		},
		{
			avatar::Action::ENTER_DARK_ALLEY,
			{
				{
					avatar::State::DOCK,
					OnEnterDarkAlley
				},
				{
					avatar::State::GAMBLE_START,
					DoTransition(
					{
						game::Colors::GREEN,
						"You leave the game.",
						avatar::State::DARK_ALLEY
					})
				}
			}
		},
		{
			avatar::Action::DEFEAT_RUFFIAN,
			{
				{
					avatar::State::DARK_ALLEY_ENTRANCE,
					OnDefeatRuffian
				}
			}
		},
		{
			avatar::Action::START_GAMBLING,
			{
				{
					avatar::State::DARK_ALLEY,
					OnStartGambling
				}
			}
		},
		{
			avatar::Action::CAREEN_TO_PORT,
			{
				{
					avatar::State::AT_SEA,
					DoTransition({
						game::Colors::GREEN,
						"You heave down to port.",
						avatar::State::CAREENED_TO_PORT
					})
				}
			}
		},
		{
			avatar::Action::CAREEN_TO_STARBOARD,
			{
				{
					avatar::State::AT_SEA,
					DoTransition({
						game::Colors::GREEN,
						"You heave down to starboard.",
						avatar::State::CAREENED_TO_STARBOARD
					})
				}
			}
		},
		{
			avatar::Action::UNCAREEN,
			{
				{
					avatar::State::CAREENED_TO_PORT,
					DoTransition({
						game::Colors::GREEN,
						"You right the vessel.",
						avatar::State::AT_SEA
					})
				},
				{
					avatar::State::CAREENED_TO_STARBOARD,
					DoTransition({
						game::Colors::GREEN,
						"You right the vessel.",
						avatar::State::AT_SEA
					})
				}
			}
		},
		{
			avatar::Action::START_FISHING,
			{
				{
					avatar::State::AT_SEA,
					OnStartFishing
				}
			}
		},
		{
			avatar::Action::STOP_FISHING,
			{
				{
					avatar::State::FISHING,
					OnStopFishing
				}
			}
		}
	};

	const std::map<avatar::Action, std::map<avatar::State, std::function<StateTransition()>>>& GetActionDescriptors()
	{
		return actionDescriptors;
	}
}