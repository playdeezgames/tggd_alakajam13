#include "Game.Actors.h"
#include "Game.ActorTypes.h"
#include <vector>
#include <map>
#include <functional>
#include <Common.RNG.h>
#include <list>
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

	static void AddRobots()
	{
		Actors::AddActor({ ActorType::ROBOT_1, {0,-1} });
		//Actors::AddActor({ ActorType::ROBOT_2, {1,0} });
		//Actors::AddActor({ ActorType::ROBOT_3, {0,1} });
		//Actors::AddActor({ ActorType::ROBOT_4, {-1,0} });
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

	static void AddPigs()
	{
		Actors::AddActor({ ActorType::PIG, {-BOARD_SIZE+1,-BOARD_SIZE+1} });
		Actors::AddActor({ ActorType::PIG, {-BOARD_SIZE+1,BOARD_SIZE-1} });
		Actors::AddActor({ ActorType::PIG, {BOARD_SIZE-1,-BOARD_SIZE+1} });
		Actors::AddActor({ ActorType::PIG, {BOARD_SIZE-1,BOARD_SIZE-1} });
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
			if (actors[index].location == location)
			{
				return index;
			}
		}
		return std::nullopt;
	}

	static bool DoMoveActor(Actor& actor, const common::XY<int>& location)
	{
		auto otherIndex = FindActor(location);
		if (otherIndex)
		{
			//TODO: interaction
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

	static void BeAPig(Actor& actor)
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

	static const std::map<ActorType, std::function<void(Actor&)>> actions =
	{
		{ActorType::FENCE, DoNothing},
		{ActorType::PIG, BeAPig},
		{ActorType::TRASH, DoNothing},
		{ActorType::TURD, DoNothing}
	};

	void Actors::Act(Actor& actor)
	{
		actions.find(actor.actorType)->second(actor);
	}
}