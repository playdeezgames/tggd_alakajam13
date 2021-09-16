#include <Common.Data.h>
#include "Data.Game.Island.Item.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::island
{
	static const std::string FIELD_X = "X";
	static const std::string FIELD_Y = "Y";
	static const std::string FIELD_ITEM_ID = "ItemId";
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandItems]([X],[Y],[ItemId], UNIQUE([X],[Y],[ItemId]));";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandItems]([X],[Y],[ItemId]) VALUES({:.4f},{:.4f},{});";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f} AND [ItemId]={};";
	static const std::string DELETE_ALL = "DELETE FROM [IslandItems];";
	static const std::string QUERY_ITEM = "SELECT [ItemId] FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f} AND [ItemId]={};";
	static const std::string QUERY_ALL = "SELECT [ItemId] FROM [IslandItems] WHERE [X]={:.4f} AND [Y]={:.4f}";

	static const auto AutoCreateIslandItemTable = data::game::Common::Run(CREATE_TABLE);

	void Item::Set(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, location.GetX(), location.GetY(), item));
	}

	void Item::Clear(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, location.GetX(), location.GetY(), item));
	}

	bool Item::Get(const common::XY<double>& location, int item)
	{
		AutoCreateIslandItemTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, location.GetX(), location.GetY(), item));
		return !records.empty();
	}

	void Item::ClearAll()
	{
		AutoCreateIslandItemTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	std::set<int> Item::GetAll(const common::XY<double>& location)
	{
		std::set<int> result;
		AutoCreateIslandItemTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ALL, location.GetX(), location.GetY()));
		for (auto& record : records)
		{
			result.insert(common::Data::ToInt(record[FIELD_ITEM_ID]));
		}
		return result;
	}
}
