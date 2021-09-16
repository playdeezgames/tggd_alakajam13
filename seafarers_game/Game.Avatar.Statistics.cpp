#include <Data.Game.Avatar.Statistic.h>
#include "Game.Avatar.Statistic.h"
#include "Game.Avatar.Statistics.h"
#include <map>
#include <list>
namespace game::avatar
{
	const std::string Statistics::FORMAT_HEALTH = "\x83 {:.0f}";
	const std::string Statistics::FORMAT_BRAWLING = "\x88 {:.1f}";
	const std::string Statistics::FORMAT_TURNS = "\x81 {:d}";
	const std::string Statistics::FORMAT_MONEY = "\x80 {:.3f}";
	const std::string Statistics::FORMAT_REPUTATION = "\x86 {:.0f}";
	const std::string Statistics::FORMAT_SATIETY = "\x82 {:.0f}";
	const std::string Statistics::FORMAT_HEADING = "\x84 {}";
	const std::string Statistics::FORMAT_SPEED = "\x85 {:.2f}";

	struct StatisticDescriptor
	{
		game::avatar::Statistic statistic;
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};

	static const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 100.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 50.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 25.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}}
	};

	void Statistics::Reset(const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::avatar::Statistic::Write((int)value.statistic,
				{
					value.minimum,
					value.maximum,
					value.initial
				});
		}
	}

	static std::optional<double> GetMaximum(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read((int)statistic).value().maximum;
	}

	static std::optional<double> GetMinimum(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read((int)statistic).value().minimum;
	}

	double GetCurrent(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read((int)statistic).value().current;
	}

	static void SetCurrent(const game::avatar::Statistic& statistic, double value)
	{
		auto data = data::game::avatar::Statistic::Read((int)statistic).value();
		data.current =
			(data.maximum.has_value() && value > data.maximum.value()) ? (data.maximum.value()) :
			(data.minimum.has_value() && value < data.minimum.value()) ? (data.minimum.value()) :
			(value);
		data::game::avatar::Statistic::Write((int)statistic, data);
	}

	static double ChangeCurrent(const game::avatar::Statistic& statistic, double delta)
	{
		SetCurrent(statistic, GetCurrent(statistic) + delta);
		return GetCurrent(statistic);
	}

	double Statistics::GetMoney()
	{
		return GetCurrent(game::avatar::Statistic::MONEY);
	}

	double Statistics::GetHealth()
	{
		return GetCurrent(game::avatar::Statistic::HEALTH);
	}

	double Statistics::GetSatiety()
	{
		return GetCurrent(game::avatar::Statistic::SATIETY);
	}

	double Statistics::GetInfamy()
	{
		return GetCurrent(game::avatar::Statistic::INFAMY);
	}

	double Statistics::GetBrawling()
	{
		return GetCurrent(game::avatar::Statistic::BRAWLING);
	}

	void Statistics::Eat(double amount)
	{
		double healthDown = GetMaximum(game::avatar::Statistic::HEALTH).value() - GetCurrent(game::avatar::Statistic::HEALTH);
		if (healthDown > 0)
		{
			ChangeCurrent(game::avatar::Statistic::HEALTH, amount);
			amount -= healthDown;
		}
		if (amount > 0)
		{
			ChangeCurrent(game::avatar::Statistic::SATIETY, amount);
		}
	}

	bool Statistics::NeedToEat(double amount)
	{
		double totalDown =
			GetMaximum(game::avatar::Statistic::HEALTH).value() - GetCurrent(game::avatar::Statistic::HEALTH) +
			GetMaximum(game::avatar::Statistic::SATIETY).value() - GetCurrent(game::avatar::Statistic::SATIETY);
		return totalDown >= amount;
	}

	double Statistics::GetReputation()
	{
		return GetCurrent(game::avatar::Statistic::REPUTATION);
	}

	void Statistics::ChangeReputation(double delta)
	{
		ChangeCurrent(game::avatar::Statistic::REPUTATION, delta);
	}

	int Statistics::GetTurnsRemaining()
	{
		return (int)GetCurrent(game::avatar::Statistic::TURNS_REMAINING);
	}

	void Statistics::SpendTurn()
	{
		const double TURN_DELTA = -1.0;
		ChangeCurrent(game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
	}

	bool Statistics::IsOutOfTurns()
	{
		return GetCurrent(game::avatar::Statistic::TURNS_REMAINING) <= GetMinimum(game::avatar::Statistic::TURNS_REMAINING);
	}

	static bool IsMinimal(const Statistic& statistic)
	{
		return GetCurrent(statistic) <= GetMinimum(statistic);
	}

	bool Statistics::IsDead()
	{
		return IsMinimal(Statistic::HEALTH);
	}

	bool Statistics::IsStarving()
	{
		return IsMinimal(Statistic::SATIETY);
	}

	void Statistics::ChangeMoney(double delta)
	{
		ChangeCurrent(Statistic::MONEY, delta);
	}

	void Statistics::ChangeHealth(double delta)
	{
		ChangeCurrent(Statistic::HEALTH, delta);
	}

	void Statistics::ChangeSatiety(double delta)
	{
		ChangeCurrent(Statistic::SATIETY, delta);
	}

	void Statistics::ChangeInfamy(double delta)
	{
		ChangeCurrent(Statistic::INFAMY, delta);
	}

	void Statistics::ChangeBrawling(double delta)
	{
		ChangeCurrent(Statistic::BRAWLING, delta);
	}
}