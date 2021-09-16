#include "Audio.h"
#include "Audio.Platform.h"
#include "Audio.Sfx.h"
#include <Data.JSON.Stores.h>
#include <memory>
#include <json.hpp>
namespace audio
{
	static std::map<std::string, size_t> sounds;
	static int sfxVolume = audio::Platform::VOLUME_MAXIMUM;

	static void AddSound(const std::string& name, const std::string& filename)
	{
		sounds[name] = Platform::LoadSound(filename);
	}

	static bool initialized = false;
	static std::optional<int> store = std::nullopt;

	void Sfx::SetStore(int s)
	{
		store = s;
	}

	static void Finish()
	{
		Platform::UnloadSounds();
	}

	void Sfx::Initialize()
	{
		if (!initialized)
		{
			nlohmann::json& j = data::json::Stores::GetStore(store.value());
			for (auto& i : j.items())
			{
				AddSound(i.key(), i.value());
			}
			initialized = true;
			atexit(Finish);
		}
	}

	static void Play2(const std::string& name)
	{
		Sfx::Initialize();
		if (!Audio::IsMuted())
		{
			const auto& item = sounds.find(name);
			if (item != sounds.end())
			{
				Platform::PlaySound(item->second);
			}
		}
	}

	void Sfx::Play(const std::optional<std::string>& name)
	{
		if (name)
		{
			Play2(name.value());
		}
	}

	void Sfx::SetVolume(int volume)
	{
		Initialize();
		sfxVolume = Audio::ClampVolume(volume);
		for (auto& entry : sounds)
		{
			Platform::SetSoundVolume(entry.second, sfxVolume);
		}
	}

	int Sfx::GetVolume()
	{
		return sfxVolume;
	}
}
