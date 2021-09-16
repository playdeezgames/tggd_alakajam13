#pragma once
#include "Command.h"
#include <functional>
#include <optional>
#include <string>
namespace application
{
	struct Keyboard
	{
		static void SetStore(int);
		template<typename TStore>
		static std::function<void()> DoSetStore(const TStore& store)
		{
			return [store]()
			{
				SetStore((int)store);
			};
		}
		typedef std::function<bool(const std::string&)> Handler;
		static void AddHandler(int, Handler);
		static bool Handle(const std::string&);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
		static std::optional<::Command> ToCommand(int keycode);
	};
}
