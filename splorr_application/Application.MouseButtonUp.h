#pragma once
#include <Common.XY.h>
#include <functional>
#include "MouseButton.h"
namespace application
{
	struct MouseButtonUp
	{
		typedef std::function<bool(const common::XY<int>&, MouseButton)> Handler;
		static void Handle(const int&, const int&, const MouseButton&);
		static void AddHandler(int, Handler);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
	};
}
