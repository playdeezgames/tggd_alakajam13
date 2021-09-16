#include "Application.Handlers.h"
#include "Application.Update.h"
#include <map>
#include <vector>
namespace application
{
	static std::map<int, std::vector<Update::Handler>> handlers;

	void Update::AddHandler(int state, Update::Handler handler)
	{
		if (handlers.find(state) == handlers.end())
		{
			handlers[state] = std::vector<Update::Handler>();
		}
		handlers[state].push_back(handler);
	}

	void Update::Handle(unsigned int ticks)
	{
		application::Handlers::WithCurrent(handlers, [ticks](const std::vector<Update::Handler>& updaters)
		{
			for (auto& updater : updaters)
			{
				updater(ticks);
			}
		});
	}


}