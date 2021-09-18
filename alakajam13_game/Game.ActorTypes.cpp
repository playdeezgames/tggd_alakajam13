#include "Game.ActorTypes.h"
#include <map>
namespace game
{
	static const std::map<ActorType, ActorTypeDescriptor> descriptors =
	{
		{ActorType::ROBOT_1, { "Robot1", true }},
		{ActorType::ROBOT_2, { "Robot2", true }},
		{ActorType::ROBOT_3, { "Robot3", true }},
		{ActorType::ROBOT_4, { "Robot4", true }},
		{ActorType::TURD_ROBOT_1, { "TurdRobot1", true }},
		{ActorType::TURD_ROBOT_2, { "TurdRobot2", true }},
		{ActorType::TURD_ROBOT_3, { "TurdRobot3", true }},
		{ActorType::TURD_ROBOT_4, { "TurdRobot4", true }},
		{ActorType::FENCE, { "Fence", false }},
		{ActorType::PIG, { "Pig", false }},
		{ActorType::TRASH, { "Trash", false }},
		{ActorType::TURD, { "Turd", false }},
		{ActorType::DELETED, { "", false }}
	};

	const ActorTypeDescriptor& ActorTypes::Read(const ActorType& actorType)
	{
		return descriptors.find(actorType)->second;
	}
}