#include "Game.ActorTypes.h"
#include <map>
namespace game
{
	static const std::map<ActorType, ActorTypeDescriptor> descriptors =
	{
		{ActorType::ROBOT_1, { "Robot1", true, "MiniRobot", "Battery1", false}},
		{ActorType::ROBOT_2, { "Robot2", true, "MiniRobot", "Battery2", false }},
		{ActorType::ROBOT_3, { "Robot3", true, "MiniRobot", "Battery3", false }},
		{ActorType::ROBOT_4, { "Robot4", true, "MiniRobot", "Battery4", false }},
		{ActorType::TURD_ROBOT_1, { "TurdRobot1", true, "MiniRobot", "Battery1", false }},
		{ActorType::TURD_ROBOT_2, { "TurdRobot2", true, "MiniRobot", "Battery2", false }},
		{ActorType::TURD_ROBOT_3, { "TurdRobot3", true, "MiniRobot", "Battery3", false }},
		{ActorType::TURD_ROBOT_4, { "TurdRobot4", true, "MiniRobot", "Battery4", false }},
		{ActorType::FEED_ROBOT_1, { "WheatRobot1", true, "MiniRobot", "Battery1", false }},
		{ActorType::FEED_ROBOT_2, { "WheatRobot2", true, "MiniRobot", "Battery2", false }},
		{ActorType::FEED_ROBOT_3, { "WheatRobot3", true, "MiniRobot", "Battery3", false }},
		{ActorType::FEED_ROBOT_4, { "WheatRobot4", true, "MiniRobot", "Battery4", false }},
		{ActorType::FENCE, { "Fence", false, "MiniFence", std::nullopt, false }},
		{ActorType::PIG, { "Pig", false, "MiniPig", std::nullopt, true }},
		{ActorType::PECKISH_PIG, { "PeckishPig", false, "MiniPig", std::nullopt, true }},
		{ActorType::HUNGRY_PIG, { "HungryPig", false, "MiniPig", std::nullopt, true }},
		{ActorType::STARVING_PIG, { "StarvingPig", false, "MiniPig", std::nullopt, true }},
		{ActorType::DEAD_PIG, { "DeadPig", false, "MiniEmpty", std::nullopt, false }},
		{ActorType::TRASH, { "Trash", false, "MiniTrash", std::nullopt, false }},
		{ActorType::TURD, { "Turd", false, "MiniTurd", std::nullopt, false }},
		{ActorType::DELETED, { "", false, "MiniEmpty", std::nullopt, false }},
		{ActorType::BATTERY_0, { "Battery0", false, "MiniCharge", std::nullopt, false }},
		{ActorType::BATTERY_25, { "Battery25", false, "MiniCharge", std::nullopt, false }},
		{ActorType::BATTERY_50, { "Battery50", false, "MiniCharge", std::nullopt, false }},
		{ActorType::BATTERY_75, { "Battery75", false, "MiniCharge", std::nullopt, false }},
		{ActorType::BATTERY_100, { "Battery100", false, "MiniCharge", std::nullopt, false }},
		{ActorType::GRAIN_SOURCE, { "WheatSack", false, "MiniWheat", std::nullopt, false }}
	};

	const ActorTypeDescriptor& ActorTypes::Read(const ActorType& actorType)
	{
		return descriptors.find(actorType)->second;
	}
}