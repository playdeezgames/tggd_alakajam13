#include <Common.XY.h>
#include <Visuals.Data.Properties.h>
#include "Visuals.Fishboard.h"
namespace visuals
{
	static const std::string FISHBOARD_ID = "fishboard-id";

	struct InternalFishboard
	{
		common::XY<int> xy;
	};

	static std::vector<InternalFishboard> fishboards;
	static std::map<std::string, std::map<std::string, size_t>> fishboardTable;


	static void DrawInternalFishboard(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t fishboardIndex)
	{
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Fishboard::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto fishboardIndex = fishboards.size();
		fishboards.push_back({
			common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y])
			});
		if (model.count(FISHBOARD_ID) > 0)
		{
			fishboardTable[layoutName][model[FISHBOARD_ID]] = fishboardIndex;
		}

		return [fishboardIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalFishboard(renderer, fishboardIndex);
		};
	}
}
