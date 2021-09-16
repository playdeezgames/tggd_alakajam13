#include <Audio.Mux.h>
#include <Game.Audio.Mux.h>
#include <map>
namespace game::audio
{
	static const std::map<Theme, std::string> themeTable =
	{
		{Theme::MAIN, "main"}
	};
	void Mux::Play(const Theme& theme)
	{
		::audio::Mux::Play(themeTable.find(theme)->second);
	}
	std::function<void()> Mux::GoToTheme(const Theme& theme)
	{
		return [theme]() { Play(theme); };
	}

}

