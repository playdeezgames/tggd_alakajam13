#include <Application.Platform.h>
#include <memory>
#include <SDL.h>
namespace application
{
	bool Platform::IsFullscreen()
	{
		return false;
	}

	void Platform::SetFullscreen(bool flag)
	{

	}

	void Platform::Initialize()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);
	}
}