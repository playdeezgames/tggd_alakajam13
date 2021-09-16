#pragma once
#include "Command.h"
#include <functional>
#include <map>
namespace application
{
	struct Command
	{
		static void Handle(const ::Command&);

		static void SetHandlers(int, const std::map<::Command, std::function<void()>>&);
		template<typename TState>
		static void SetHandlers(const TState& state, const std::map<::Command, std::function<void()>>& handlers)
		{
			SetHandlers((int)state, handlers);
		}
		static void SetHandler(int, std::function<void()>);
		template<typename TState>
		static void SetHandler(const TState& state, std::function<void()> handler)
		{
			SetHandler((int)state, handler);
		}
	};
}
