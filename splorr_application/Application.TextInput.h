#pragma once
#include <functional>
#include <string>
namespace application
{
	struct TextInput
	{
		typedef std::function<void(const std::string&)> Handler;
		static void AddHandler(int, Handler);
		static void Handle(const std::string&);
		template<typename TState>
		static void AddHandler(const TState& state, Handler handler)
		{
			AddHandler((int)state, handler);
		}
	};
}

