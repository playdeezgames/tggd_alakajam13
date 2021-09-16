#include "Audio.h"
#include "Audio.Mux.h"
#include "Audio.Platform.h"
#include <Data.JSON.Stores.h>
#include <json.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
namespace audio
{
	static std::map<std::string, size_t> music;

	static void AddMusic(const std::string& name, const std::string& filename)
	{
		music[name] = Platform::LoadMusic(filename);
	}

	static void Finish()
	{
		while (music.begin() != music.end())
		{
			Platform::UnloadMusic(music.begin()->second);
			music.erase(music.begin());
		}
	}

	static bool initialized = false;
	static std::optional<int> store = std::nullopt;

	void Mux::SetStore(int s)
	{
		store = s;
	}

	void Mux::Initialize()
	{
		if (!initialized)
		{
			atexit(Finish);
			nlohmann::json& j = data::json::Stores::GetStore(store.value());
			for (auto& i : j.items())
			{
				AddMusic(i.key(), i.value());
			}
			initialized = true;
		}
	}

	static std::optional<std::string> currentSong = std::nullopt;

	void Mux::Play(const std::string& song)
	{
		Initialize();
		if (!Audio::IsMuted())
		{
			if (!currentSong.has_value() || currentSong.value() != song)
			{
				currentSong = song;
				const auto& item = music.find(song);
				Platform::StartMusic(item->second);
			}
		}
	}

	void Mux::SetVolume(int volume)
	{
		Initialize();
		Platform::SetMusicVolumne(Audio::ClampVolume(volume));
	}

	int Mux::GetVolume()
	{
		return Platform::GetMusicVolume();
	}

}

