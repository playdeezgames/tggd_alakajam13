#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Common.h"
#include "Data.Game.World.h"
#include <format>
#include <string>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Worlds]([WorldId] INT NOT NULL UNIQUE,[Version] INT NOT NULL);";
	static const std::string QUERY_ITEM = "SELECT [Version] FROM [Worlds] WHERE [WorldId] = {};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Worlds]([WorldId],[Version]) VALUES ({},{});";
	static const int WORLD_ID = 1;
	static const std::string FIELD_VERSION = "Version";

	static const auto AutoCreateWorldsTable = data::game::Common::Run(CREATE_TABLE);

	void World::Write(const World& data)
	{
		AutoCreateWorldsTable();
		data::game::Common::Execute(
			std::format(REPLACE_ITEM, 
				WORLD_ID,
				data.version));
	}

	std::optional<World> World::Read()
	{
		AutoCreateWorldsTable();
		auto result = data::game::Common::Execute(std::format(QUERY_ITEM, WORLD_ID));
		if (!result.empty())
		{
			const auto& record = result.front();
			World data =
			{
				common::Data::ToInt(record.find(FIELD_VERSION)->second)
			};
			return data;
		}
		return std::nullopt;
	}
}