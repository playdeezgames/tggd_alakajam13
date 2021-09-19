#include "Game.Actors.h"
#include "Game.ActorTypes.h"
#include <vector>
#include <map>
#include <functional>
#include <Common.RNG.h>
#include <list>
#include <set>
namespace game
{
	static std::vector<Actor> actors;
	static size_t actorIndex = 0;

	void Actors::Next()
	{
		actorIndex = (actorIndex + 1) % actors.size();
	}

	void Actors::AddActor(const Actor& actor)
	{
		//TODO: first check for a DELETED actor
		actors.push_back(actor);
	}

	const Actor& Actors::GetCurrent()
	{
		return actors[actorIndex];
	}

	static void Clear()
	{
		actors.clear();
		actorIndex = 0;
	}

	static const int BOARD_SIZE = 15;

	static void AddRobot(const ActorType& actorType, const common::XY<int>& location)
	{
		Actors::AddActor(
			{
				actorType,
				location,
				{
					{Statistic::ENERGY, 100}
				}
			});
	}

	static void AddRobots()
	{
		AddRobot( ActorType::ROBOT_1, {0,-1} );
		AddRobot( ActorType::ROBOT_2, {1,0} );
		AddRobot( ActorType::ROBOT_3, {0,1} );
		AddRobot( ActorType::ROBOT_4, {-1,0} );
	}

	static void AddFence()
	{
		for (int x = -BOARD_SIZE + 1; x < BOARD_SIZE; ++x)
		{
			Actors::AddActor({ ActorType::FENCE, {x,-BOARD_SIZE} });
			Actors::AddActor({ ActorType::FENCE, {x,BOARD_SIZE} });
			Actors::AddActor({ ActorType::FENCE, {-BOARD_SIZE,x} });
			Actors::AddActor({ ActorType::FENCE, {BOARD_SIZE,x} });
		}
		Actors::AddActor({ ActorType::FENCE, {-BOARD_SIZE,-BOARD_SIZE} });
		Actors::AddActor({ ActorType::FENCE, {-BOARD_SIZE,BOARD_SIZE} });
		Actors::AddActor({ ActorType::FENCE, {BOARD_SIZE,-BOARD_SIZE} });
		Actors::AddActor({ ActorType::FENCE, {BOARD_SIZE,BOARD_SIZE} });
	}

	static void AddPig(const common::XY<int>& location)
	{
		Actors::AddActor(
			{
				ActorType::PIG,
				location,
				{
					{Statistic::HEALTH, 100},
					{Statistic::HUNGER, 0},
					{Statistic::BOWEL, 0},
				}
			});
	}

	static void AddPigs()
	{
		AddPig({-BOARD_SIZE+1,-BOARD_SIZE+1} );
		AddPig({-BOARD_SIZE+1,BOARD_SIZE-1} );
		AddPig({BOARD_SIZE-1,-BOARD_SIZE+1} );
		AddPig({BOARD_SIZE-1,BOARD_SIZE-1} );
	}

	static void AddTrash()
	{
		Actors::AddActor({ ActorType::TRASH, {0,0} });
	}

	static void AddBatteries()
	{
		Actors::AddActor({ ActorType::BATTERY_100, {-BOARD_SIZE / 2,0}, { { Statistic::ENERGY, 100} } });
		Actors::AddActor({ ActorType::BATTERY_100, {BOARD_SIZE / 2,0}, { { Statistic::ENERGY, 100} } });
	}

	void Actors::Reset(const Difficulty&)
	{
		Clear();
		AddRobots();
		AddFence();
		AddPigs();
		AddTrash();
		AddBatteries();
	}
	
	const std::vector<Actor>& Actors::All()
	{
		return actors;
	}

	static std::optional<size_t> FindActor(const common::XY<int>& location)
	{
		for (size_t index = 0; index < actors.size(); ++index)
		{
			if (actors[index].location == location && actors[index].actorType!=ActorType::DELETED)
			{
				return index;
			}
		}
		return std::nullopt;
	}

	static const std::map<ActorType, ActorType> turdBumpers =
	{
		{ActorType::ROBOT_1, ActorType::TURD_ROBOT_1},
		{ActorType::ROBOT_2, ActorType::TURD_ROBOT_2},
		{ActorType::ROBOT_3, ActorType::TURD_ROBOT_3},
		{ActorType::ROBOT_4, ActorType::TURD_ROBOT_4}
	};

	static bool OnInteractTurd(Actor& bumped, Actor& bumper)
	{
		auto iter = turdBumpers.find(bumper.actorType);
		if(iter!=turdBumpers.end())
		{
			bumped.actorType = ActorType::DELETED;
			bumper.actorType = iter->second;
			return true;
		}
		return false;
	}

	static const std::map<ActorType, ActorType> trashBumper =
	{
		{ActorType::TURD_ROBOT_1, ActorType::ROBOT_1},
		{ActorType::TURD_ROBOT_2, ActorType::ROBOT_2},
		{ActorType::TURD_ROBOT_3, ActorType::ROBOT_3},
		{ActorType::TURD_ROBOT_4, ActorType::ROBOT_4}
	};

	static bool OnInteractTrash(Actor& bumped, Actor& bumper)
	{
		auto iter = trashBumper.find(bumper.actorType);
		if (iter != trashBumper.end())
		{
			bumper.actorType = iter->second;
			return true;
		}
		return false;
	}

	static const std::map<ActorType, std::function<bool(Actor&, Actor&)>> interactors =
	{
		{ActorType::TURD, OnInteractTurd},
		{ActorType::TRASH, OnInteractTrash}
	};

	static void UseEnergy(Actor& actor)
	{
		if (actor.statistics.contains(Statistic::ENERGY))
		{
			actor.statistics[Statistic::ENERGY] = actor.statistics[Statistic::ENERGY] - 1;
		}
	}

	static bool DoMoveActor(Actor& actor, const common::XY<int>& location)
	{
		UseEnergy(actor);
		auto otherIndex = FindActor(location);
		if (otherIndex)
		{
			auto& other = actors[otherIndex.value()];
			auto interactor = interactors.find(other.actorType);
			if (interactor != interactors.end())
			{
				return interactor->second(other, actor);
			}
			return false;
		}
		actor.location = location;
		return true;
	}

	static bool CanMoveActor(Actor& actor)
	{
		if (actor.statistics.contains(Statistic::ENERGY))
		{
			return actor.statistics[Statistic::ENERGY] > 0;
		}
		return true;
	}

	bool Actors::MoveActor(const common::XY<int>& delta)
	{
		Actor& actor = actors[actorIndex];
		if (CanMoveActor(actor))
		{
			auto newLocation = actor.location + delta;
			return DoMoveActor(actor, newLocation);
		}
		return false;
	}

	static void DoNothing(Actor&)
	{
		//as advertised!
	}

	static const std::list<common::XY<int>> moveDeltas = 
	{
		{-1,0},
		{1,0},
		{0,1},
		{0,-1}
	};

	static const std::map<bool, size_t> turdGenerator =
	{
		{true, 1},
		{false, 2}
	};

	static const std::map<bool, size_t> moveGenerator =
	{
		{true, 1},
		{false, 1}
	};

	static void DoPigStarvation(Actor& actor)
	{
		auto health = actor.statistics[Statistic::HEALTH];
		if (health > 0)
		{
			health--;
			actor.actorType = ActorType::STARVING_PIG;
			return;
		}
		actor.actorType = ActorType::DEAD_PIG;
	}

	static const int MAXIMUM_PIG_HUNGER = 100;

	static void DoPigHunger(Actor& actor)
	{
		auto hunger = actor.statistics[Statistic::HUNGER];
		if (hunger < MAXIMUM_PIG_HUNGER)
		{
			hunger++;
			actor.statistics[Statistic::HUNGER] = hunger;
			actor.actorType =
				(hunger < 25) ? (ActorType::PIG) :
				(hunger < 50) ? (ActorType::PECKISH_PIG) :
				(hunger < 75) ? (ActorType::HUNGRY_PIG) :
				(ActorType::STARVING_PIG);
			return;
		}
		DoPigStarvation(actor);
	}

	static void BeAPig(Actor& actor)
	{
		
		if (common::RNG::FromGenerator(moveGenerator, false))
		{
			DoPigHunger(actor);
			auto delta = common::RNG::FromList(moveDeltas).value();
			auto original = actor.location;
			if (Actors::MoveActor(delta))
			{
				if (common::RNG::FromGenerator(turdGenerator, false))
				{
					Actors::AddActor({ActorType::TURD, original});
				}
			}
		}
	}

	static const std::map<ActorType, std::function<void(Actor&)>> actions =
	{
		{ActorType::FENCE, DoNothing},
		{ActorType::PIG, BeAPig},
		{ActorType::PECKISH_PIG, BeAPig},
		{ActorType::HUNGRY_PIG, BeAPig},
		{ActorType::STARVING_PIG, BeAPig},
		{ActorType::DEAD_PIG, DoNothing},
		{ActorType::TRASH, DoNothing},
		{ActorType::TURD, DoNothing},
		{ActorType::DELETED, DoNothing},
		{ActorType::BATTERY_0, DoNothing},
		{ActorType::BATTERY_25, DoNothing},
		{ActorType::BATTERY_50, DoNothing},
		{ActorType::BATTERY_75, DoNothing},
		{ActorType::BATTERY_100, DoNothing}
	};

	void Actors::Act()
	{
		Actor& actor = actors[actorIndex];
		actions.find(actor.actorType)->second(actor);
	}

	static const int MAXIMUM_ROBOT_ENERGY = 100;
	static const int ENERGY_RECHARGE_RATE = 10;

	static void Recharge(Actor& actor)
	{
		auto energy = actor.statistics[Statistic::ENERGY];
		energy += ENERGY_RECHARGE_RATE;
		actor.statistics[Statistic::ENERGY] = (energy > MAXIMUM_ROBOT_ENERGY) ? (MAXIMUM_ROBOT_ENERGY) : (energy);
	}

	static const std::map<ActorType, std::function<void(Actor&)>> rests =
	{
		{ActorType::ROBOT_1, Recharge},
		{ActorType::ROBOT_2, Recharge},
		{ActorType::ROBOT_3, Recharge},
		{ActorType::ROBOT_4, Recharge},
		{ActorType::TURD_ROBOT_1, Recharge},
		{ActorType::TURD_ROBOT_2, Recharge},
		{ActorType::TURD_ROBOT_3, Recharge},
		{ActorType::TURD_ROBOT_4, Recharge}
	};

	void Actors::Rest()
	{
		Actor& actor = actors[actorIndex];
		rests.find(actor.actorType)->second(actor);
	}

}