#include "Game.ActorTypes.h"
#include <map>
namespace game
{
	static const std::map<ActorType, ActorTypeDescriptor> descriptors =
	{
		{ActorType::ROBOT_1, { "Robot1", true, "MiniRobot", "Battery1"}},
		{ActorType::ROBOT_2, { "Robot2", true, "MiniRobot", "Battery2" }},
		{ActorType::ROBOT_3, { "Robot3", true, "MiniRobot", "Battery3" }},
		{ActorType::ROBOT_4, { "Robot4", true, "MiniRobot", "Battery4" }},
		{ActorType::TURD_ROBOT_1, { "TurdRobot1", true, "MiniRobot", "Battery1" }},
		{ActorType::TURD_ROBOT_2, { "TurdRobot2", true, "MiniRobot", "Battery2" }},
		{ActorType::TURD_ROBOT_3, { "TurdRobot3", true, "MiniRobot", "Battery3" }},
		{ActorType::TURD_ROBOT_4, { "TurdRobot4", true, "MiniRobot", "Battery4" }},
		{ActorType::FENCE, { "Fence", false, "MiniFence", std::nullopt }},
		{ActorType::PIG, { "Pig", false, "MiniPig", std::nullopt }},
		{ActorType::PECKISH_PIG, { "PeckishPig", false, "MiniPig", std::nullopt }},
		{ActorType::HUNGRY_PIG, { "HungryPig", false, "MiniPig", std::nullopt }},
		{ActorType::STARVING_PIG, { "StarvingPig", false, "MiniPig", std::nullopt }},
		{ActorType::DEAD_PIG, { "DeadPig", false, "MiniEmpty", std::nullopt }},
		{ActorType::TRASH, { "Trash", false, "MiniTrash", std::nullopt }},
		{ActorType::TURD, { "Turd", false, "MiniTurd", std::nullopt }},
		{ActorType::DELETED, { "", false, "MiniEmpty", std::nullopt }},
		{ActorType::BATTERY_0, { "Battery0", false, "MiniCharge", std::nullopt }},
		{ActorType::BATTERY_25, { "Battery25", false, "MiniCharge", std::nullopt }},
		{ActorType::BATTERY_50, { "Battery50", false, "MiniCharge", std::nullopt }},
		{ActorType::BATTERY_75, { "Battery75", false, "MiniCharge", std::nullopt }},
		{ActorType::BATTERY_100, { "Battery100", false, "MiniCharge", std::nullopt }}
	};

	const ActorTypeDescriptor& ActorTypes::Read(const ActorType& actorType)
	{
		return descriptors.find(actorType)->second;
	}
}