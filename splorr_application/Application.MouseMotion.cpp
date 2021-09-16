#include "Application.Handlers.h"
#include "Application.MouseMotion.h"
#include <map>
namespace application
{
	static std::map<int, std::vector<MouseMotion::Handler>> mouseMotionHandlers;

	void MouseMotion::Handle(const int& x, const int& y)
	{
		application::Handlers::WithCurrent(
			mouseMotionHandlers,
			[x,y](const std::vector<MouseMotion::Handler>& handlers)
		{ 
			for (auto& handler : handlers)
			{
				handler(common::XY<int>(x, y));
			}
		});
	}

	void MouseMotion::AddHandler(int state, MouseMotion::Handler handler)
	{
		mouseMotionHandlers[state].push_back(handler);
	}
}