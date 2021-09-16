#include <Common.Heading.h>
#include <format>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Destinations.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Islands.h>
#include <Game.World.h>
#include <map>
#include <string>
#include "Sublayouts.h"
#include <Visuals.Images.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace sublayout
{
	static const std::string SUBLAYOUT_NAME = "Sublayout.AtSeaStatistics";

	static const std::string TEXT_AVATAR_TURNS = "avatar-turns";
	static const std::string TEXT_AVATAR_HEALTH = "avatar-health";
	static const std::string TEXT_AVATAR_SATIETY = "avatar-satiety";
	static const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	static const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	static const std::string TEXT_AVATAR_MONEY = "avatar-money";
	static const std::string TEXT_AVATAR_REPUTATION = "avatar-reputation";

	static void RefreshAvatarSatiety()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_SATIETY, std::format(game::avatar::Statistics::FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()));
	}

	static void RefreshAvatarMoney()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_MONEY, std::format(game::avatar::Statistics::FORMAT_MONEY, game::avatar::Statistics::GetMoney()));
	}

	static void RefreshAvatarReputation()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_REPUTATION, std::format(game::avatar::Statistics::FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()));
	}

	static void RefreshAvatarHealth()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_HEALTH, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()));
	}

	static void RefreshAvatarTurns()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_TURNS, std::format(game::avatar::Statistics::FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()));
	}

	static void RefreshAvatarHeading()
	{
		auto heading = game::avatar::AtSea::GetHeading();
		visuals::Texts::SetText(
			SUBLAYOUT_NAME,
			TEXT_AVATAR_HEADING, 
			std::format(game::avatar::Statistics::FORMAT_HEADING, common::Heading::ToCompassPoint(heading)));
	}

	static void RefreshAvatarSpeed()
	{
		auto speed = game::avatar::AtSea::GetSpeed();
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_SPEED, std::format(game::avatar::Statistics::FORMAT_SPEED, speed));
	}

	void AtSeaAvatarStatus::Refresh()
	{
		RefreshAvatarHeading();
		RefreshAvatarHealth();
		RefreshAvatarSatiety();
		RefreshAvatarSpeed();
		RefreshAvatarMoney();
		RefreshAvatarReputation();
		RefreshAvatarTurns();
	}
}