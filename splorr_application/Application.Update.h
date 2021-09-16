#pragma once
#include <functional>
namespace application
{
	struct Update
	{
		typedef std::function<void(const unsigned int&)> Handler;
		static void AddHandler(int, Handler);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
		static void Handle(unsigned int);
	};
}