#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Quest.h"
#include <format>
namespace data::game::island
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_DESTINATION_X = "DestinationX";
	static const std::string FIELD_DESTINATION_Y = "DestinationY";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandQuests]([X] REAL NOT NULL, [Y] REAL NOT NULL, [DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL, [ReceiptEmotion] TEXT NOT NULL , [Reward] REAL NOT NULL, UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion] FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandQuests]([X],[Y],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion]) VALUES({:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{},{},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandQuests] WHERE [X]={:.4f} AND  [Y]={:.4f};";

	static const auto AutoCreateIslandQuestsTable = data::game::Common::Run(CREATE_TABLE);

	void Quest::Write(const Quest& data)
	{
		AutoCreateIslandQuestsTable();
		std::string query =
			std::format(
				REPLACE_ITEM,
				data.location.GetX(),
				data.location.GetY(),
				data.destination.GetX(),
				data.destination.GetY(),
				data.reward,
				common::Data::QuoteString(data.itemName),
				common::Data::QuoteString(data.personName),
				common::Data::QuoteString(data.professionName),
				common::Data::QuoteString(data.receiptEmotion));
		data::game::Common::Execute(query);
	}

	std::optional<Quest> Quest::Read(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		std::string query = std::format(QUERY_ITEM, location.GetX(), location.GetY());
		auto records = data::game::Common::Execute(query);
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<Quest>(
				{
					{
						common::Data::ToDouble(record[FIELD_X]),
						common::Data::ToDouble(record[FIELD_Y])
					},
					{
						common::Data::ToDouble(record[FIELD_DESTINATION_X]),
						common::Data::ToDouble(record[FIELD_DESTINATION_Y])
					},
					common::Data::ToDouble(record[FIELD_REWARD]),
					record[FIELD_ITEM_NAME],
					record[FIELD_PERSON_NAME],
					record[FIELD_PROFESSION_NAME],
					record[FIELD_RECEIPT_EMOTION]
				});
		}
		return std::nullopt;
	}

	void Quest::Clear(const common::XY<double>& location)
	{
		AutoCreateIslandQuestsTable();
		std::string query = std::format(DELETE_ITEM, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}
}