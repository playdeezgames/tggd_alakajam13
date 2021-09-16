#pragma once
#include "Game.Difficulty.h"
#include <string>
namespace game::avatar
{
	struct Statistics
	{
		static const std::string FORMAT_HEALTH;
		static const std::string FORMAT_BRAWLING;
		static const std::string FORMAT_TURNS;
		static const std::string FORMAT_MONEY;
		static const std::string FORMAT_REPUTATION;
		static const std::string FORMAT_SATIETY;
		static const std::string FORMAT_HEADING;
		static const std::string FORMAT_SPEED;
		static void Reset(const game::Difficulty&);
		static double GetMoney();
		static void ChangeMoney(double);
		static double GetHealth();
		static void ChangeHealth(double);
		static bool IsDead();
		static double GetSatiety();
		static void ChangeSatiety(double);
		static bool IsStarving();
		static void Eat(double);
		static bool NeedToEat(double);
		static double GetReputation();
		static void ChangeReputation(double);
		static int GetTurnsRemaining();
		static void SpendTurn();
		static bool IsOutOfTurns();
		static double GetInfamy();
		static void ChangeInfamy(double);
		static double GetBrawling();
		static void ChangeBrawling(double);
	};
}
