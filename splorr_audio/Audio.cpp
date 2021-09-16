#include "Audio.Platform.h"
#include "Audio.Mux.h"
#include "Audio.Sfx.h"
#include "Audio.h"
int Audio::ClampVolume(int volume)
{
	return
		(volume < audio::Platform::VOLUME_MINIMUM) ? (audio::Platform::VOLUME_MINIMUM) :
		(volume > audio::Platform::VOLUME_MAXIMUM) ? (audio::Platform::VOLUME_MAXIMUM) :
		(volume);
}

static bool muted = false;

static void Initialize()
{
	::audio::Sfx::Initialize();
	::audio::Mux::Initialize();
}

void Audio::SetMuted(bool newValue)
{
	Initialize();
	muted = newValue;
	if (muted)
	{
		audio::Platform::PauseMusic();
	}
	else
	{
		audio::Platform::ResumeMusic();
	}
}

bool Audio::IsMuted()
{
	return muted;
}
