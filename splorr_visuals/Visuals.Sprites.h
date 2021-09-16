#pragma once
#include <Application.Engine.h>
#include <Common.XY.h>
#include <memory>
#include <optional>
#include <SDL.h>
#include <string>
#include <tuple>
namespace visuals
{
	struct Sprites
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
		static void Draw(const std::string&, const std::shared_ptr<application::Engine::Renderer>&, const ::common::XY<int>&, const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>&, double angle=0.0);
		static std::optional<int> GetWidth(const std::string&);
	};
}