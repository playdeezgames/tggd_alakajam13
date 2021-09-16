#include <Common.Data.h>
#include "Data.Game.Common.h"
#include "Data.Game.Island.Market.h"
#include <format>
namespace data::game::island
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_COMMODITY_ID = "CommodityId";
	static const std::string FIELD_SUPPLY = "Supply";
	static const std::string FIELD_DEMAND = "Demand";
	static const std::string FIELD_PURCHASES = "Purchases";
	static const std::string FIELD_SALES = "Sales";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandMarkets]([X] REAL NOT NULL, [Y] REAL NOT NULL, [CommodityId] INT NOT NULL,[Supply] REAL NOT NULL, [Demand] REAL NOT NULL, [Purchases] REAL NOT NULL, [Sales] REAL NOT NULL, UNIQUE([X],[Y],[CommodityId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandMarkets]([X],[Y],[CommodityId],[Supply],[Demand],[Purchases],[Sales]) VALUES ({:.4f},{:.4f},{},{},{},{},{})";
	static const std::string QUERY_ITEM = "SELECT [Supply], [Demand], [Purchases], [Sales] FROM [IslandMarkets] WHERE [X]={:.4f} AND [Y]={:.4f} AND [CommodityId]={};";
	static const std::string QUERY_ALL = "SELECT [CommodityId], [Supply], [Demand], [Purchases], [Sales] FROM [IslandMarkets] WHERE [X]={:.4f} AND [Y]={:.4f};";
	static const std::string DELETE_ALL = "DELETE FROM [IslandMarkets];";

	static const auto AutoCreateIslandMarketsTable = data::game::Common::Run(CREATE_TABLE);

	void Market::Write(const common::XY<double>& location, int commodityId, const Market& data)
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(
			std::format(REPLACE_ITEM, location.GetX(), location.GetY(), commodityId, data.supply, data.demand, data.purchases, data.sales)
		);
	}

	std::optional<Market> Market::Read(const common::XY<double>& location, int commodityId)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				std::format(QUERY_ITEM, location.GetX(), location.GetY(), commodityId)
			);
		if (!records.empty())
		{
			auto& record = records.front();
			return std::optional<Market>(
				{
					common::Data::ToDouble(record[FIELD_SUPPLY]),
					common::Data::ToDouble(record[FIELD_DEMAND]),
					common::Data::ToDouble(record[FIELD_PURCHASES]),
					common::Data::ToDouble(record[FIELD_SALES])
				});
		}
		return std::nullopt;
	}

	std::map<int, Market> Market::All(const common::XY<double>& location)
	{
		AutoCreateIslandMarketsTable();
		auto records =
			data::game::Common::Execute(
				std::format(QUERY_ALL, location.GetX(), location.GetY())
			);
		std::map<int, Market> result;
		for (auto& record : records)
		{
			result[common::Data::ToInt(record[FIELD_COMMODITY_ID])] =
				{
					common::Data::ToDouble(record[FIELD_SUPPLY]),
					common::Data::ToDouble(record[FIELD_DEMAND]),
					common::Data::ToDouble(record[FIELD_PURCHASES]),
					common::Data::ToDouble(record[FIELD_SALES])
				};
		}
		return result;
	}

	void Market::Clear()
	{
		AutoCreateIslandMarketsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}