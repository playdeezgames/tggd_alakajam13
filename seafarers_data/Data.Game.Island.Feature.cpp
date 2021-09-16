#include <Common.Data.h>
#include "Data.Game.Island.Feature.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::island
{
	static const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [IslandFeatures]([X] REAL NOT NULL,[Y] REAL NOT NULL,[FeatureId] INT NOT NULL,UNIQUE([X],[Y],[FeatureId]));";
	static const std::string DELETE_ALL = "DELETE FROM [IslandFeatures];";
	static const std::string REPLACE_ITEM = "REPLACE INTO [IslandFeatures]([X],[Y],[FeatureId]) VALUES({:.4f},{:.4f},{});";
	static const std::string QUERY_ITEM = "SELECT [FeatureId] FROM [IslandFeatures] WHERE [X]={:.4f} AND [Y]={:.4f} AND [FeatureId]={};";
	static const std::string DELETE_ITEM = "DELETE FROM [IslandFeatures] WHERE [X]={:.4f} AND [Y]={:.4f} AND [FeatureId]={};";

	static const auto AutoCreateIslandFeaturesTable = data::game::Common::Run(CREATE_TABLE);

	void Feature::Clear()
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(DELETE_ALL);
	}

	void Feature::Write(const common::XY<double>& xy, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, xy.GetX(), xy.GetY(), featureId));
	}

	void Feature::Clear(const common::XY<double>& xy, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, xy.GetX(), xy.GetY(), featureId));
	}

	bool Feature::Read(const common::XY<double>& xy, int featureId)
	{
		AutoCreateIslandFeaturesTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, xy.GetX(), xy.GetY(), featureId));
		return !records.empty();
	}
}
