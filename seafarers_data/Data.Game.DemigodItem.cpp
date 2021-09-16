#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.DemigodItem.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DemigodItems]([Name] TEXT NOT NULL,[ItemId] INT NOT NULL,[Favor] REAL NOT NULL,UNIQUE([Name],[ItemId]));";
	static const std::string DELETE_ALL = "DELETE FROM [DemigodItems];";
	static const std::string QUERY_ITEM = "SELECT [Favor] FROM [DemigodItems] WHERE [Name]={} AND [ItemId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DemigodItems]([Name],[ItemId],[Favor]) VALUES({},{},{});";
	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateDemigodItemsTable = Common::Run(CREATE_TABLE);

	std::optional<double> DemigodItem::Read(const std::string& demigod, int itemId)
	{
		AutoCreateDemigodItemsTable();
		auto records = 
			Common::Execute(
				std::format(
					QUERY_ITEM, 
					common::Data::QuoteString(demigod),
					itemId));
		if (!records.empty())
		{
			auto record = records.front();
			return common::Data::ToOptionalDouble(record[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	void DemigodItem::Write(const std::string& demigod, int itemId, double favor)
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(std::format(REPLACE_ITEM, common::Data::QuoteString(demigod), itemId, favor));
	}

	void DemigodItem::Clear()
	{
		AutoCreateDemigodItemsTable();
		Common::Execute(DELETE_ALL);
	}
}
