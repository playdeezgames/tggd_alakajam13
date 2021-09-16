#pragma once
#include <Application.Engine.h>
#include <json.hpp>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Layouts
	{
		static void SetStore(int);
		template<typename TStore>
		static std::function<void()> DoSetStore(const TStore& store)
		{
			return [store]()
			{
				SetStore((int)store);
			};
		}
		static void RegisterType(const std::string&, InternalizerFunction);
		static std::function<void()> DoRegisterType(const std::string&, InternalizerFunction);
		static void Draw(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
		static void Start();
		static void Internalize(const std::string&, const nlohmann::json&);
	};
}