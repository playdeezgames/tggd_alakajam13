#include <Common.Data.h>
#include "Data.Game.Avatar.Destination.h"
#include "Data.Game.Common.h"
#include "Data.Game.Island.Known.h"
#include <format>
#include <string>
namespace data::game::avatar
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDestinations]([AvatarId] INT NOT NULL,[DestinationId] INT NOT NULL, [X] REAL NOT NULL, [Y] REAL NOT NULL, UNIQUE([AvatarId],[DestinationId]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y] FROM [AvatarDestinations] WHERE [AvatarId]={} AND [DestinationId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDestinations]([AvatarId],[DestinationId],[X],[Y]) VALUES({},{},{:.4f},{:.4f});";
	static const std::string DELETE_ITEM = "DELETE FROM [AvatarDestinations] WHERE [AvatarId]={} AND [DestinationId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarDestinations] WHERE [AvatarId]={};";

	static const auto AutoCreateAvatarDestinationsTable = data::game::Common::Run(CREATE_TABLE);

	void Destination::Write(int destinationId, const common::XY<double>& location)
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, destinationId, location.GetX(), location.GetY());
		data::game::Common::Execute(query);
	}

	std::optional<common::XY<double>> Destination::Read(int destinationId)
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(QUERY_ITEM, data::game::Common::AVATAR_ID, destinationId);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<common::XY<double>>(
				{
					common::Data::ToDouble(record[FIELD_X]),
					common::Data::ToDouble(record[FIELD_Y])
				});
		}
		return std::nullopt;
	}

	void Destination::Clear(int destinationId)
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(DELETE_ITEM, data::game::Common::AVATAR_ID, destinationId);
		data::game::Common::Execute(query);
	}

	void Destination::ClearAll()
	{
		AutoCreateAvatarDestinationsTable();
		auto query = std::format(DELETE_ALL, data::game::Common::AVATAR_ID);
		data::game::Common::Execute(query);
	}

}