#pragma once
struct Audio
{
	static void SetMuted(bool);
	static bool IsMuted();
	static int ClampVolume(int);
};
