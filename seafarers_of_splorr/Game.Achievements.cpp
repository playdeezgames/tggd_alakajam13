#define AINT_PAID_STEAM_FEE
#define _CRT_SECURE_NO_WARNINGS
#include <Game.Achievements.h>
#include <map>
#include <string>
#ifndef AINT_PAID_STEAM_FEE
#include <steam_api.h>
#endif
#include <memory>
namespace game::Achievements
{
#ifndef AINT_PAID_STEAM_FEE
	static ISteamUser* GetSteamUser()
	{
		static bool initialized = false;
		static ISteamUser* steamUser;
		if (!initialized)
		{
			steamUser = SteamUser();
			initialized = true;
		}
		return steamUser;
	}

	static ISteamUserStats* GetSteamUserStats()
	{
		static bool initialized = false;
		static ISteamUserStats* steamUserStats;
		if (!initialized)
		{
			steamUserStats = SteamUserStats();
			initialized = true;
		}
		return steamUserStats;
	}
#endif
	const std::map<game::Achievement, std::string> achievementNames =
	{
		{game::Achievement::YER_DEAD, "YER_DEAD"}
	};

	void Add(const game::Achievement& achievement)
	{
#ifndef AINT_PAID_STEAM_FEE
		if (GetSteamUserStats())
		{
			GetSteamUserStats()->SetAchievement(achievementNames.find(achievement)->second.c_str());
			GetSteamUserStats()->StoreStats();
		}
#endif
	}

#ifndef AINT_PAID_STEAM_FEE
	const uint32 APP_ID = XXXXXXXXXXXXXXXX;
#endif

	static void DoFinish()
	{
#ifndef AINT_PAID_STEAM_FEE
		SteamAPI_Shutdown();
#endif
	}

	bool ShouldRestartApp()
	{
#ifndef AINT_PAID_STEAM_FEE
#ifndef _DEBUG
		if (SteamAPI_RestartAppIfNecessary(APP_ID))
		{
			return true;
		}
#endif
#endif
		return false;
	}

	void Start()
	{
#ifndef AINT_PAID_STEAM_FEE
		if (SteamAPI_Init())
		{
			atexit(DoFinish);

			if (SteamUser()->BLoggedOn())
			{
				GetSteamUser();
				if (GetSteamUserStats())
				{
					GetSteamUserStats()->RequestCurrentStats();
				}
			}
		}
#endif
	}
}
namespace game::Statistics
{
	const std::map<game::Statistic, std::string> statisticsName =
	{
		{ game::Statistic::GAMES_PLAYED, "GAMES_PLAYED" },
		{ game::Statistic::MOVES_MADE, "MOVES_MADE" }
	};

	void Increment(const game::Statistic& statistic)
	{
#ifndef AINT_PAID_STEAM_FEE
		int32 value;
		auto& statisticName = statisticsName.find(statistic)->second;
		if (game::Achievements::GetSteamUserStats() && 
			game::Achievements::GetSteamUserStats() -> GetStat(statisticName.c_str(), &value))
		{
			value++;
			if (game::Achievements::GetSteamUserStats()->SetStat(statisticName.c_str(), value))
			{
				game::Achievements::GetSteamUserStats()->StoreStats();
			}
		}
#endif
	}

	std::optional<int> Read(const game::Statistic& statistic)
	{
#ifndef AINT_PAID_STEAM_FEE
		int32 value;
		auto& statisticName = statisticsName.find(statistic)->second;
		if (game::Achievements::GetSteamUserStats() &&
			game::Achievements::GetSteamUserStats()->GetStat(statisticName.c_str(), &value))
		{
			return value;
		}
#endif
		return std::nullopt;
	}

}
