#include "Game.Actors.h"
#include <vector>
namespace game
{
	static std::vector<Actor> actors;
	static size_t actorIndex = 0;

	void Actors::Next()
	{
		actorIndex = (actorIndex + 1) % actors.size();
	}

	void  Actors::AddActor(const Actor& actor)
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

	void Actors::Reset(const Difficulty&)
	{
		Clear();
		AddActor({ ActorType::ROBOT_1, {0,0} });
		AddActor({ ActorType::ROBOT_2, {1,0} });
		AddActor({ ActorType::ROBOT_3, {0,1} });
		AddActor({ ActorType::ROBOT_4, {1,1} });
	}
	
	const std::vector<Actor>& Actors::All()
	{
		return actors;
	}
}