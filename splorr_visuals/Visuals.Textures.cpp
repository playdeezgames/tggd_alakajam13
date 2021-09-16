#include <Application.Engine.h>
#include <Common.Utility.h>
#include <Data.JSON.h>
#include <Data.JSON.Stores.h>
#include <map>
#include <memory>
#include <SDL_image.h>
#include "Visuals.Textures.h"
namespace visuals
{
	static std::optional<int> store;

	void Textures::SetStore(int s)
	{
		store = s;
	}

	static std::map<std::string, std::shared_ptr<SDL_Texture>> table;

	static void Add(const std::string& name, const std::shared_ptr<SDL_Texture>& texture)
	{
		table[name] = texture;
	}

	static bool initialized = false;

	static void Initialize(const std::shared_ptr<application::Engine::Renderer>& renderer)
	{
		if (!initialized)
		{
			auto& properties = data::json ::Stores::GetStore(store.value());
			for (auto& entry : properties.items())
			{
				std::string imageFileName = entry.value();
				Add(entry.key(), std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer.get()->renderer.get(), imageFileName.c_str()), SDL_DestroyTexture));
			}
			initialized = true;
		}
	}

	const std::shared_ptr<SDL_Texture>& Textures::Read(const std::shared_ptr<application::Engine::Renderer>& renderer, const std::string& name)
	{
		Initialize(renderer);
		return table.find(name)->second;
	}
}