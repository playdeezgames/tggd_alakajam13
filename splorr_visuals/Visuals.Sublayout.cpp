#include <Application.Engine.h>
#include <json.hpp>
#include <SDL.h>
#include "Visuals.Layouts.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Sublayout.h"
namespace visuals
{
	struct InternalSublayout
	{
		std::string name;
	};

	static std::vector<InternalSublayout> internalSublayouts;

	static void DrawInternalSublayout(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t index)
	{
		auto& sublayout = internalSublayouts[index];
		visuals::Layouts::Draw(renderer, sublayout.name);
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Sublayout::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalSublayouts.size();
		internalSublayouts.push_back(
			{ 
				model[visuals::data::Properties::NAME]
			});
		return [index](const std::shared_ptr<application::Engine::Renderer>& renderer) 
		{
			DrawInternalSublayout(renderer, index);
		};
	}
}