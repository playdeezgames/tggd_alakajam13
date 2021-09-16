#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.DarkAlley.h"
#include <format>
namespace data::game::island
{
	static const std::string FIELD_INFAMY_REQUIREMENT = "InfamyRequirement";
	static const std::string FIELD_RUFFIAN_BRAWLING_STRENGTH = "RuffianBrawlingStrength";
	static const std::string FIELD_MINIMUM_WAGER = "MinimumWager";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [DarkAlleys]([X] REAL NOT NULL,[Y] REAL NOT NULL,[InfamyRequirement] REAL NOT NULL,[RuffianBrawlingStrength] REAL NOT NULL, [MinimumWager] REAL NOT NULL, UNIQUE([X],[Y]));";
	static const std::string DELETE_ALL = "DELETE FROM [DarkAlleys];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [DarkAlleys]([X],[Y],[InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager]) VALUES({:.4f},{:.4f},{},{},{});";
	static const std::string QUERY_ITEM = "SELECT [InfamyRequirement],[RuffianBrawlingStrength],[MinimumWager] FROM [DarkAlleys] WHERE [X]={:.4f} AND [Y]={:.4f};";

	static const auto AutoCreateDarkAlleysTable = data::game::Common::Run(CREATE_TABLE);

	void DarkAlley::Clear()
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void DarkAlley::Write(const common::XY<double>& location, const DarkAlley& data)
	{
		AutoCreateDarkAlleysTable();
		data::game::Common::Execute(
			std::format(
				REPLACE_ITEM, 
				location.GetX(), 
				location.GetY(), 
				data.infamyRequirement, 
				data.ruffianBrawlingStrength,
				data.minimumWager));
	}

	std::optional<DarkAlley> DarkAlley::Read(const common::XY<double>& location)
	{
		AutoCreateDarkAlleysTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, location.GetX(), location.GetY()));
		if (!records.empty())
		{
			auto record = records.front();
			return std::optional<DarkAlley>({
				common::Data::ToDouble(record[FIELD_INFAMY_REQUIREMENT]),
				common::Data::ToDouble(record[FIELD_RUFFIAN_BRAWLING_STRENGTH]),
				common::Data::ToDouble(record[FIELD_MINIMUM_WAGER])
				});
		}
		return std::nullopt;
	}
}