#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishGame.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishGames]([AvatarId] INT NOT NULL UNIQUE,[Guesses] INT NOT NULL);";
	static const std::string DELETE_ITEM = "DELETE FROM [FishGames] WHERE [AvatarId]={};";
	static const std::string QUERY_ITEM = "SELECT [Guesses] FROM [FishGames] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishGames]([AvatarId],[Guesses]) VALUES({},{});";
	static const std::string FIELD_GUESSES = "Guesses";

	static const auto AutoCreateFishGameTable = Common::Run(CREATE_TABLE);

	int FishGame::ReadGuesses()
	{
		AutoCreateFishGameTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::ToInt(records.front()[FIELD_GUESSES]);
		}
		return 0;
	}

	void FishGame::WriteGuesses(int guesses)
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, guesses));
	}

	void FishGame::Clear()
	{
		AutoCreateFishGameTable();
		Common::Execute(std::format(DELETE_ITEM, Common::AVATAR_ID));
	}
}