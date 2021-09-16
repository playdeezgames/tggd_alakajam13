#include <Common.Data.h>
#include "Data.Game.Avatar.Quest.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar
{
	static const std::string FIELD_DESTINATION_X = "DestinationX";
	static const std::string FIELD_DESTINATION_Y = "DestinationY";
	static const std::string FIELD_REWARD = "Reward";
	static const std::string FIELD_PERSON_NAME = "PersonName";
	static const std::string FIELD_ITEM_NAME = "ItemName";
	static const std::string FIELD_PROFESSION_NAME = "ProfessionName";
	static const std::string FIELD_RECEIPT_EMOTION = "ReceiptEmotion";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarQuests]([AvatarId] INT NOT NULL UNIQUE,[DestinationX] REAL NOT NULL, [DestinationY] REAL NOT NULL, [ItemName] TEXT NOT NULL, [PersonName] TEXT NOT NULL, [ProfessionName] TEXT NOT NULL, [ReceiptEmotion] TEXT NOT NULL, [Reward] REAL NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion] FROM [AvatarQuests] WHERE [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarQuests]([AvatarId],[DestinationX],[DestinationY],[Reward],[ItemName],[PersonName],[ProfessionName],[ReceiptEmotion]) VALUES({},{:.4f},{:.4f},{:.4f},{},{},{},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarQuests] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarQuestTable = data::game::Common::Run(CREATE_TABLE);

	void Quest::Write(const std::optional<Quest>& data)
	{
		AutoCreateAvatarQuestTable();
		if (data)
		{
			data::game::Common::Execute(
				std::format(REPLACE_ITEM,
					data::game::Common::AVATAR_ID,
					data.value().destination.GetX(),
					data.value().destination.GetY(),
					data.value().reward,
					common::Data::QuoteString(data.value().itemName),
					common::Data::QuoteString(data.value().personName),
					common::Data::QuoteString(data.value().professionName),
					common::Data::QuoteString(data.value().receiptEmotion)));
		}
		else
		{
			data::game::Common::Execute(std::format(DELETE_ITEM, data::game::Common::AVATAR_ID));
		}
	}

	std::optional<Quest> Quest::Read()
	{
		AutoCreateAvatarQuestTable();
		auto records = data::game::Common::Execute(
			std::format(QUERY_ITEM, data::game::Common::AVATAR_ID));
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<Quest>({
					{common::Data::ToDouble(record[FIELD_DESTINATION_X]),common::Data::ToDouble(record[FIELD_DESTINATION_Y])},
					common::Data::ToDouble(record[FIELD_REWARD]),
					record[FIELD_ITEM_NAME],
					record[FIELD_PERSON_NAME],
					record[FIELD_PROFESSION_NAME],
					record[FIELD_RECEIPT_EMOTION] });
		}
		return std::nullopt;
	}
}