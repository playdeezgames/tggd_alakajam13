#pragma once
#include <Application.Engine.h>
#include <Common.XY.h>
#include <memory>
#include <optional>
#include <SDL.h>
#include <string>
#include "Visuals.HorizontalAlignment.h"
namespace visuals
{
	struct Fonts
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
		static void WriteText(const std::string&, const std::shared_ptr<application::Engine::Renderer>&, const common::XY<int>&, const std::string& text, const std::string& color, const HorizontalAlignment& alignment);
		static std::optional<std::string> GetGlyphSpriteName(const std::string&, char);
	};
}
