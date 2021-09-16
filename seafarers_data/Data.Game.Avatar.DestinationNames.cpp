#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.DestinationName.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DestinationNames]([DestinationId] INT NOT NULL UNIQUE, [DestinationName] TEXT NOT NULL);";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DestinationNames]([DestinationId],[DestinationName]) VALUES({},{});";
	static const std::string QUERY_ITEM = "SELECT [DestinationName] FROM [DestinationNames] WHERE [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [DestinationNames];";
	static const std::string FIELD_DESTINATION_NAME = "DestinationName";

	static const auto AutoCreateDestinationNamesTable = Common::Run(CREATE_TABLE);

	std::optional<std::string> DestinationName::Read(int index)
	{
		AutoCreateDestinationNamesTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, index));
		if (!records.empty())
		{
			auto& record = records.front();
			return record[FIELD_DESTINATION_NAME];
		}
		return std::nullopt;
	}

	void DestinationName::Write(int index, const std::string& name)
	{
		AutoCreateDestinationNamesTable();
		Common::Execute(
			std::format(
				REPLACE_ITEM, 
				index, 
				common::Data::QuoteString(name)));
	}

	void DestinationName::Clear()
	{
		AutoCreateDestinationNamesTable();
		Common::Execute(DELETE_ALL);
	}
}