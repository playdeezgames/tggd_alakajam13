#pragma once
#include <Common.XY.h>
#include <functional>
namespace application
{
	struct MouseMotion
	{
		typedef std::function<void(const common::XY<int>&)> Handler;
		static void AddHandler(int, Handler);
		static void Handle(const int&, const int&);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
	};
}
