#include <Data.Game.Avatar.h>
#include <Data.Game.Avatar.Dock.h>
#include <Data.Game.Island.DarkAlley.h>
#include <format>
#include <functional>
#include "Game.Avatar.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.StateTransition.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Colors.h"
#include "Game.Islands.Quests.h"
#include "Game.Islands.h"
#include <map>
namespace game::avatar
{
	void Docked::Reset(const game::Difficulty&)
	{

	}

	static const std::string FORMAT_DOCK = "You dock at {}!";

	static std::optional<Docked::DockResult> DoDock(const common::XY<double>& location)
	{
		std::optional<Docked::DockResult> result = Docked::DockResult::DOCKED;
		game::Islands::AddVisit(
			location,
			Statistics::GetTurnsRemaining());
		game::islands::Quests::Update(location);
		if (game::avatar::Quest::CompleteQuest(location))
		{
			result = Docked::DockResult::COMPLETED_QUEST;
		}
		data::game::avatar::Dock::Write(location);
		data::game::Avatar::WriteState((int)game::avatar::State::DOCK);
		auto island = game::Islands::Read(location).value();
		Log::Write({ game::Colors::GREEN, std::format(FORMAT_DOCK, island.name) });
		return result;
	}

	std::optional<Docked::DockResult> Docked::Dock()
	{
		if (GetDockedLocation().has_value())
		{
			return DockResult::ALREADY_DOCKED;
		}
		auto dockables = game::Islands::GetDockableIslands();
		if (!dockables.empty())
		{
			return DoDock(dockables.front().absoluteLocation);
		}
		return std::nullopt;
	}

	std::optional<common::XY<double>> Docked::GetDockedLocation()
	{
		return data::game::avatar::Dock::Read();
	}
}