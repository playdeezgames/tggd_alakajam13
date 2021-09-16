#include <Common.Data.h>
#include "Data.Game.Avatar.Items.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar
{
	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string FIELD_ITEM_COUNT = "ItemCount";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarItems]([AvatarId] INT NOT NULL,[ItemId] INT NOT NULL, [ItemCount] INT NOT NULL, UNIQUE([AvatarId],[ItemId]))";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarItems]([AvatarId],[ItemId],[ItemCount]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [ItemCount] FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";
	static const std::string QUERY_ALL = "SELECT [ItemId],[ItemCount] FROM [AvatarItems] WHERE [AvatarId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarItems];";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";

	static const auto AutoCreateAvatarItemsTable = data::game::Common::Run(CREATE_TABLE);

	size_t Items::Read(const int& itemId)
	{
		AutoCreateAvatarItemsTable();
		auto records =
			data::game::Common::Execute(std::format(QUERY_ITEM,data::game::Common::AVATAR_ID, itemId));
		if (!records.empty())
		{
			auto& record = records.front();
			return (size_t)common::Data::ToInt(record[FIELD_ITEM_COUNT]);
		}
		return 0;
	}

	void Items::Write(const int& itemId, const size_t& count)
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, data::game::Common::AVATAR_ID, itemId));
		if (count > 0)
		{
			data::game::Common::Execute(std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, itemId, count));
		}
	}

	void Items::Clear()
	{
		AutoCreateAvatarItemsTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::map<int, size_t> Items::All()
	{
		std::map<int, size_t> result;
		auto records =
			data::game::Common::Execute(std::format(QUERY_ALL, data::game::Common::AVATAR_ID));
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_ITEM_ID])] = (size_t)common::Data::ToInt(record[FIELD_ITEM_COUNT]);
		}
		return result;
	}
}