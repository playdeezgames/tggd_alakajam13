#include "Game.ActorTypes.h"
#include <map>
namespace game
{
	static const std::map<ActorType, ActorTypeDescriptor> descriptors =
	{
		{ActorType::ROBOT_1, { "Robot1", true, "MiniRobot"}},
		{ActorType::ROBOT_2, { "Robot2", true, "MiniRobot" }},
		{ActorType::ROBOT_3, { "Robot3", true, "MiniRobot" }},
		{ActorType::ROBOT_4, { "Robot4", true, "MiniRobot" }},
		{ActorType::TURD_ROBOT_1, { "TurdRobot1", true, "MiniRobot" }},
		{ActorType::TURD_ROBOT_2, { "TurdRobot2", true, "MiniRobot" }},
		{ActorType::TURD_ROBOT_3, { "TurdRobot3", true, "MiniRobot" }},
		{ActorType::TURD_ROBOT_4, { "TurdRobot4", true, "MiniRobot" }},
		{ActorType::FENCE, { "Fence", false, "MiniFence" }},
		{ActorType::PIG, { "Pig", false, "MiniPig" }},
		{ActorType::PECKISH_PIG, { "PeckishPig", false, "MiniPig" }},
		{ActorType::HUNGRY_PIG, { "HungryPig", false, "MiniPig" }},
		{ActorType::STARVING_PIG, { "StarvingPig", false, "MiniPig" }},
		{ActorType::DEAD_PIG, { "DeadPig", false, "MiniEmpty" }},
		{ActorType::TRASH, { "Trash", false, "MiniTrash" }},
		{ActorType::TURD, { "Turd", false, "MiniTurd" }},
		{ActorType::DELETED, { "", false, "MiniEmpty" }}
	};

	const ActorTypeDescriptor& ActorTypes::Read(const ActorType& actorType)
	{
		return descriptors.find(actorType)->second;
	}
}