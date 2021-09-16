#pragma once
#include <functional>
namespace application
{
	struct OnEnter
	{
		typedef std::function<void()> Handler;
		static void Handle();
		static void AddHandler(int, Handler);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
	};
}