#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.Plight.h"
#include <format>
namespace data::game::avatar
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Plights]([PlightId] INT NOT NULL UNIQUE, [Duration] INT NULL);";
	static const std::string DELETE_ALL = "DELETE FROM [Plights];";
	static const std::string DELETE_ITEM = "DELETE FROM [Plights] WHERE [PlightId]={};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Plights]([PlightId], [Duration]) VALUES({}, {});";
	static const std::string QUERY_ITEM = "SELECT [Duration] FROM [Plights] WHERE [PlightId]={};";
	static const std::string QUERY_ALL = "SELECT [PlightId], [Duration] FROM [Plights];";
	static const std::string FIELD_PLIGHT_ID = "PlightId";
	static const std::string FIELD_DURATION = "Duration";

	static const auto AutoCreatePlightTable = Common::Run(CREATE_TABLE);

	std::optional<Plight> Plight::Read(int plightId)
	{
		AutoCreatePlightTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, plightId));
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<Plight>({
				plightId,
				common::Data::ToOptionalInt(record[FIELD_DURATION])
			});
		}
		return std::nullopt;
	}

	void Plight::Write(const Plight& plight)
	{
		AutoCreatePlightTable();
		Common::Execute(std::format(REPLACE_ITEM, plight.plightId, common::Data::OfOptional(plight.duration)));
	}

	void Plight::Clear(int plightId)
	{
		AutoCreatePlightTable();
		Common::Execute(std::format(DELETE_ITEM, plightId));

	}
	void Plight::Clear()
	{
		AutoCreatePlightTable();
		Common::Execute(DELETE_ALL);
	}

	std::list<Plight> Plight::All()
	{
		AutoCreatePlightTable();
		std::list<Plight> result;
		auto records = Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			result.push_back({
				common::Data::ToInt(record[FIELD_PLIGHT_ID]),
				common::Data::ToOptionalInt(record[FIELD_DURATION])
				});
		}
		return result;
	}

}