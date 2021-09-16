#pragma once
#include "Application.UIState.h"
#include <map>
namespace application
{
	struct Handlers
	{
		template<typename THandler, typename TAction>
		static void WithCurrent(const std::map<int, THandler> handlers, TAction action)
		{
			auto iter = handlers.find(application::UIState::Read());
			if (iter != handlers.end())
			{
				action(iter->second);
			}
		}
	};
}