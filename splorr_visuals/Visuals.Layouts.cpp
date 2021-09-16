#include <algorithm>
#include <Data.JSON.h>
#include <Data.JSON.Stores.h>
#include <functional>
#include "Visuals.DrawerFunction.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Layouts.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
#include "Visuals.Menus.h"
#include "Visuals.Areas.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Sublayout.h"
namespace visuals
{
	static std::optional<int> store;

	void Layouts::SetStore(int s)
	{
		store = s;
	}

	struct InternalLayout
	{
		std::vector<DrawerFunction> drawers;
	};

	static std::map<std::string, nlohmann::json> layouts;
	static std::map<std::string, InternalLayout> internalLayouts;

	static std::map<std::string, InternalizerFunction> internalizers =
	{
		{"Image", visuals::Images::Internalize},
		{"Text", visuals::Texts::Internalize},
		{"Menu", visuals::Menus::Internalize},
		{"Layout", visuals::Sublayout::Internalize},
		{"Area", visuals::Areas::Internalize},
		{"SpriteGrid", visuals::SpriteGrid::Internalize}
	};

	void Layouts::RegisterType(const std::string& typeName, InternalizerFunction internalizer)
	{
		internalizers[typeName] = internalizer;
	}

	std::function<void()> Layouts::DoRegisterType(const std::string& typeName, InternalizerFunction internalizer)
	{
		return [typeName, internalizer]() 
		{
			RegisterType(typeName, internalizer);
		};
	}


	static void InternalizeTypedDrawn(const std::string layoutName, const std::string& drawnType, const nlohmann::json& drawn)
	{
		internalLayouts[layoutName].drawers.push_back(internalizers.find(drawnType)->second(layoutName, drawn));
	}

	static void InternalizeDrawn(const std::string& layoutName, const nlohmann::json& drawn)
	{
		auto& types = drawn[visuals::data::Properties::TYPE];
		if (types.is_array())
		{
			for (auto& type : types)
			{
				InternalizeTypedDrawn(layoutName, type, drawn);
			}
			return;
		}
		InternalizeTypedDrawn(layoutName, types, drawn);
	}

	void Layouts::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		internalLayouts[layoutName] = {};
		for (auto& drawn : model)
		{
			InternalizeDrawn(layoutName, drawn);
		}
	}

	static void InitializeLayout(const std::string& layoutName)
	{
		if (!layouts.contains(layoutName))
		{
			layouts[layoutName] = ::data::JSON::Load(::data::json::Stores::GetStore(store.value())[layoutName]);
			Layouts::Internalize(layoutName, layouts[layoutName]);
		}
	}

	void Layouts::Start()
	{
		auto& layoutStore = ::data::json::Stores::GetStore(store.value());
		for (auto& entry : layoutStore.items())
		{
			InitializeLayout(entry.key());
		}
	}

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		return layouts.find(layoutName)->second;
	}

	void Layouts::Draw(const std::shared_ptr<application::Engine::Renderer>& renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		auto& internalLayout = internalLayouts[layoutName];
		for (auto& drawer : internalLayout.drawers)
		{
			drawer(renderer);
		}
	}
}
namespace application
{
	void Engine::RenderLayout(const std::shared_ptr<application::Engine::Renderer>& renderer, const std::string& layoutName)
	{
		visuals::Layouts::Draw(renderer, layoutName);
	}
}