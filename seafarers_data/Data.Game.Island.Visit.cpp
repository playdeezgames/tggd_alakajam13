#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Visit.h"
#include <string>
#include <format>
namespace data::game::island
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_VISITS = "Visits";
	static const std::string FIELD_TURN = "Turn";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandVisits]([X] REAL NOT NULL, [Y] REAL NOT NULL, [Visits] INT NOT NULL, [Turn] INT NULL, UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Visits],[Turn] FROM [IslandVisits] WHERE [X]={:.4f} AND  [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandVisits]([X],[Y],[Visits],[Turn]) VALUES({:.4f},{:.4f},{},{});";
	static const std::string DELETE_ALL = "DELETE FROM [IslandVisits];";

	static const auto AutoCreateIslandVisitsTable = data::game::Common::Run(CREATE_TABLE);

	void Visit::Write(const Visit& data)
	{
		AutoCreateIslandVisitsTable();
		auto query = std::format(
			REPLACE_ITEM,
			data.location.GetX(),
			data.location.GetY(),
			data.visits,
			common::Data::OfOptional(data.lastVisit));
		data::game::Common::Execute(query);
	}

	std::optional<Visit> Visit::Read(const common::XY<double>& location)
	{
		AutoCreateIslandVisitsTable();
		auto query = std::format(
			QUERY_ITEM,
			location.GetX(),
			location.GetY());
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			auto& record = result.front();
			return std::optional<Visit>({
				{
					common::Data::ToDouble(record[FIELD_X]),
					common::Data::ToDouble(record[FIELD_Y])
				},
				common::Data::ToInt(record[FIELD_VISITS]),
				common::Data::ToOptionalInt(record[FIELD_TURN])
				});
		}
		return std::nullopt;
	}

	void Visit::Clear()
	{
		AutoCreateIslandVisitsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}