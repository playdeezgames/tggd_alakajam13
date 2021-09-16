#include "Application.Handlers.h"
#include "Application.OnEnter.h"
#include <map>
#include <vector>
namespace application
{
	static std::map<int, std::vector<OnEnter::Handler>> handlers;

	void OnEnter::AddHandler(int state, OnEnter::Handler handler)
	{
		if (handlers.find(state) == handlers.end())
		{
			handlers[state] = std::vector<OnEnter::Handler>();
		}
		handlers[state].push_back(handler);
	}

	void OnEnter::Handle()
	{
		application::Handlers::WithCurrent(handlers, [](const std::vector<OnEnter::Handler>& enterers)
		{
			for (auto& enterer : enterers)
			{
				enterer();
			}
		});
	}
}