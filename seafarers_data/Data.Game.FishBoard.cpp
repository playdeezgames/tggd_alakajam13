#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.FishBoard.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [FishBoards]([AvatarId] INT NOT NULL, [X] INT NOT NULL, [Y] INT NOT NULL, [Revealed] INT NOT NULL, [FishType] INT NULL, UNIQUE([AvatarId],[X],[Y]));";
	static const std::string DELETE_ALL = "DELETE FROM [FishBoards] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [FishBoards] ([AvatarId],[X],[Y],[Revealed],[FishType]) VALUES ({},{},{},{},{});";
	static const std::string QUERY_ALL = "SELECT [X],[Y],[Revealed],[FishType] FROM [FishBoards] WHERE [AvatarId]={};";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_REVEALED = "Revealed";
	static const std::string FIELD_FISH_TYPE = "FishType";

	static const auto AutoCreateFishBoardTable = Common::Run(CREATE_TABLE);

	void FishBoard::Clear()
	{
		AutoCreateFishBoardTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}

	void FishBoard::Write(const FishBoard& cell)
	{
		AutoCreateFishBoardTable();
		Common::Execute(
			std::format(
				REPLACE_ITEM,
				Common::AVATAR_ID,
				cell.location.GetX(),
				cell.location.GetY(),
				(cell.revealed) ? (1) : (0),
				common::Data::OfOptional(cell.fishType)));
	}

	static FishBoard ToFishBoard(const std::map<std::string, std::string> record)
	{
		return {
			{
				common::Data::ToInt(record.find(FIELD_X)->second),
				common::Data::ToInt(record.find(FIELD_Y)->second)
			},
			common::Data::ToInt(record.find(FIELD_REVEALED)->second) > 0,
			common::Data::ToOptionalInt(record.find(FIELD_FISH_TYPE)->second)
		};
	}

	std::list<FishBoard> FishBoard::All()
	{
		AutoCreateFishBoardTable();
		auto records = Common::Execute(std::format(QUERY_ALL, Common::AVATAR_ID));
		std::list<FishBoard> results;
		for (auto& record : records)
		{
			results.push_back(ToFishBoard(record));
		}
		return results;
	}
}