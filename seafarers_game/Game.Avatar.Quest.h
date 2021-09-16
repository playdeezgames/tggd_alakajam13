#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::avatar
{
	struct Quest
	{
		enum class AcceptQuestResult
		{
			ACCEPTED_QUEST,
			NO_QUEST_TO_ACCEPT,
			ALREADY_HAS_QUEST
		};
		static std::optional<game::Quest> Read();
		static AcceptQuestResult AcceptQuest(const common::XY<double>&);
		static bool CompleteQuest(const common::XY<double>&);
		static bool AbandonQuest();
	};
}