#pragma once
namespace application
{
	struct Platform
	{
		static void Initialize();
		static bool IsFullscreen();
		static void SetFullscreen(bool);
	};
}
