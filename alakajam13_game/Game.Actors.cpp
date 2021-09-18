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

	void Actors::Reset(const Difficulty&)
	{
		Clear();
		AddRobots();
		AddFence();
		AddPigs();
		AddTrash();
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

	static bool DoMoveActor(Actor& actor, const common::XY<int>& location)
	{
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

	bool Actors::MoveActor(const common::XY<int>& delta)
	{
		Actor& actor = actors[actorIndex];
		auto newLocation = actor.location + delta;
		return DoMoveActor(actor, newLocation);
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
		{false, 4}
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
		DoPigHunger(actor);
		if (common::RNG::FromGenerator(moveGenerator, false))
		{
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
		{ActorType::DELETED, DoNothing}
	};

	void Actors::Act()
	{
		Actor& actor = actors[actorIndex];
		actions.find(actor.actorType)->second(actor);
	}
}