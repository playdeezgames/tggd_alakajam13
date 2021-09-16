#include "Application.Handlers.h"
#include "Application.MouseButtonUp.h"
#include <map>
namespace application
{
	static std::map<int, std::vector<MouseButtonUp::Handler>> mouseUpHandlers;

	void MouseButtonUp::AddHandler(int state, MouseButtonUp::Handler handler)
	{
		mouseUpHandlers[state].push_back(handler);
	}

	void MouseButtonUp::Handle(const int& x, const int& y, const MouseButton& button)
	{
		application::Handlers::WithCurrent(
			mouseUpHandlers,
			[x,y,button](const std::vector<MouseButtonUp::Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				if (handler(common::XY<int>(x, y), button))
				{
					return true;
				}
			}
			return false;
		});
	}
}
