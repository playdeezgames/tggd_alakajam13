#include "Game.ActorTypes.h"
#include <map>
namespace game
{
	static const std::map<ActorType, ActorTypeDescriptor> descriptors =
	{
		{ActorType::ROBOT_1, { "Robot1" }},
		{ActorType::ROBOT_2, { "Robot2" }},
		{ActorType::ROBOT_3, { "Robot3" }},
		{ActorType::ROBOT_4, { "Robot4" }}
	};

	const ActorTypeDescriptor& ActorTypes::Read(const ActorType& actorType)
	{
		return descriptors.find(actorType)->second;
	}
}