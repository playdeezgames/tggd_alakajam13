#pragma once
#include <optional>
namespace game
{
	enum class Achievement
	{
		YER_DEAD
	};
	enum class Statistic
	{
		GAMES_PLAYED,
		MOVES_MADE
	};
}
namespace game::Statistics
{
	void Increment(const game::Statistic&);
	std::optional<int> Read(const game::Statistic&);
}
namespace game::Achievements
{
	void Add(const game::Achievement&);
	void Start();
	bool ShouldRestartApp();
}
