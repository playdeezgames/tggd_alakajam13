#pragma once
#include <functional>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
namespace application
{
	struct Engine
	{
		struct Renderer
		{
			std::shared_ptr<SDL_Renderer> renderer;
			Renderer(SDL_Renderer* r);
			void Copy(std::shared_ptr<SDL_Texture> texture, const SDL_Rect* source, const SDL_Rect* destination, double angle = 0.0) const;
		};
		static void Render(const std::shared_ptr<Renderer>&);
		static void SetFullscreen(bool);
		static bool IsFullscreen();
		static void RenderLayout(const std::shared_ptr<Renderer>&, const std::string&);
		static const std::vector<std::function<void()>>& GetStarters();
		static int Run(const std::vector<std::string>&);
		static bool ShouldStart();
		static const std::string CONFIG_FILE;
	};
}
