#include <Common.Data.h>
#include "Data.Game.Avatar.Statistic.h"
#include "Data.Game.Common.h"
#include <format>
#include <string>
namespace data::game::avatar
{
	static const std::string FIELD_MINIMUM = "Minimum";
	static const std::string FIELD_MAXIMUM = "Maximum";
	static const std::string FIELD_CURRENT = "Current";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarStatistics]([AvatarId] INT NOT NULL,[StatisticId] INT NOT NULL,[Minimum] REAL NULL,[Maximum] REAL NULL,[Current] REAL NOT NULL, UNIQUE([AvatarId],[StatisticId]));";
	static const std::string QUERY_ITEM = "SELECT [Minimum], [Maximum], [Current] FROM [AvatarStatistics] WHERE [StatisticId] = {} AND [AvatarId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarStatistics]([AvatarId],[StatisticId],[Minimum],[Maximum],[Current]) VALUES ({},{},{},{},{});";

	static const auto AutoCreateAvatarStatisticsTable = data::game::Common::Run(CREATE_TABLE);

	void Statistic::Write(int statisticId, const Statistic& data)
	{
		AutoCreateAvatarStatisticsTable();
		auto query = std::format(REPLACE_ITEM, Common::AVATAR_ID, statisticId, common::Data::OfOptional(data.minimum), common::Data::OfOptional(data.maximum), data.current);
		data::game::Common::Execute(query);
	}

	std::optional<Statistic> Statistic::Read(int statisticId)
	{
		AutoCreateAvatarStatisticsTable();
		auto query = std::format(QUERY_ITEM, statisticId, Common::AVATAR_ID);
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto record = result.front();
			return std::optional<Statistic>(
			{
				common::Data::ToOptionalDouble(record[FIELD_MINIMUM]),
				common::Data::ToOptionalDouble(record[FIELD_MAXIMUM]),
				common::Data::ToDouble(record[FIELD_CURRENT])
			});
		}
		return std::nullopt;
	}
}