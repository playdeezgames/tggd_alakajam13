#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.DemigodFavor.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarDemigodFavor]([AvatarId] INT NOT NULL,[DemigodName] TEXT NOT NULL,[Favor] REAL NOT NULL, UNIQUE([AvatarId],[DemigodName]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [AvatarDemigodFavor]([AvatarId],[DemigodName],[Favor]) VALUES({},{},{});";
	static const std::string QUERY_ITEM = "SELECT [Favor] FROM [AvatarDemigodFavor] WHERE [AvatarId]={} AND [DemigodName]={};";
	static const std::string DELETE_ALL = "DELETE FROM [AvatarDemigodFavor] WHERE [AvatarId]={};";
	static const std::string FIELD_FAVOR = "Favor";

	static const auto AutoCreateAvatarDemigodFavorTable = Common::Run(CREATE_TABLE);

	void DemigodFavor::Write(const std::string& name, double favor)
	{
		AutoCreateAvatarDemigodFavorTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, common::Data::QuoteString(name), favor));
	}

	std::optional<double> DemigodFavor::Read(const std::string& name)
	{
		AutoCreateAvatarDemigodFavorTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, common::Data::QuoteString(name)));
		if (!records.empty())
		{
			return common::Data::ToOptionalDouble(records.front()[FIELD_FAVOR]);
		}
		return std::nullopt;
	}

	void DemigodFavor::Clear()
	{
		AutoCreateAvatarDemigodFavorTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}
}
