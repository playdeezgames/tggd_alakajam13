#include <Common.Data.h>
#include <Common.Utility.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.h"
#include <format>
namespace data::game
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Islands]([X] REAL NOT NULL,[Y] REAL NOT NULL,[Name] TEXT NOT NULL,[MerchantSpawnRate] REAL NOT NULL,[PatronDemigod] TEXT NOT NULL,UNIQUE([X],[Y]));";
	static const std::string QUERY_ITEM = "SELECT [X],[Y],[Name],[MerchantSpawnRate],[PatronDemigod] FROM [Islands] WHERE [X] = {:.4f} AND [Y]={:.4f};";
	static const std::string REPLACE_ITEM = "REPLACE INTO [Islands]([X],[Y],[Name],[MerchantSpawnRate],[PatronDemigod]) VALUES ({:.4f},{:.4f},{},{},{});";
	static const std::string QUERY_ALL = "SELECT [X],[Y],[Name],[MerchantSpawnRate],[PatronDemigod] FROM [Islands];";
	static const std::string CLEAR_ALL = "DELETE FROM [Islands];";
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_NAME = "Name";
	static const std::string FIELD_MERCHANT_SPAWN_RATE = "MerchantSpawnRate";
	static const std::string FIELD_PATRON_DEMIGOD = "PatronDemigod";

	static const auto AutoCreateIslandTable = data::game::Common::Run(CREATE_TABLE);

	void Island::Write(const Island& data)
	{
		AutoCreateIslandTable();
		auto query =
			std::format(
				REPLACE_ITEM,
				data.location.GetX(), 
				data.location.GetY(),
				common::Data::QuoteString(data.name),
				data.merchantSpawnRate,
				common::Data::QuoteString(data.patronDemigod));
		data::game::Common::Execute(query);
	}

	static Island ToIslandData(const std::map<std::string, std::string> record)
	{
		Island data =
		{
			{
				common::Data::ToDouble(record.find(FIELD_X)->second),
				common::Data::ToDouble(record.find(FIELD_Y)->second)
			},
			record.find(FIELD_NAME)->second,
			common::Data::ToDouble(record.find(FIELD_MERCHANT_SPAWN_RATE)->second),
			record.find(FIELD_PATRON_DEMIGOD)->second
		};
		return data;
	}

	std::optional<Island> Island::Read(const common::XY<double>& location)
	{
		AutoCreateIslandTable();
		auto query =
			std::format(
				QUERY_ITEM,
				location.GetX(), location.GetY());
		auto result = data::game::Common::Execute(query);
		if (!result.empty())
		{
			return ToIslandData(result.front());
		}
		return std::nullopt;
	}

	std::list<Island> Island::All()
	{
		AutoCreateIslandTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		std::list<Island> result;
		for (auto& record : records)
		{
			result.push_back(ToIslandData(record));
		}
		return result;
	}

	void Island::Clear()
	{
		AutoCreateIslandTable();
		data::game::Common::Execute(CLEAR_ALL);
	}
}