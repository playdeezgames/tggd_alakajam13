#include <Application.Engine.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Game.Achievements.h>
#include <SDL.h>
namespace application
{
	bool Engine::ShouldStart()
	{
		if (game::Achievements::ShouldRestartApp())
		{
			return false;
		}
		return true;
	}
	const std::string Engine::CONFIG_FILE = "config/ui/application.json";
}
