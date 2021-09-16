#include <Application.Engine.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <SDL.h>
namespace application
{
	bool Engine::ShouldStart()
	{
		return true;
	}
	const std::string Engine::CONFIG_FILE = "config/ui/application.json";
}
