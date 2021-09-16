#include "Application.Command.h"
#include "Application.Handlers.h"
#include <Common.Utility.h>
namespace application
{
	static std::map<int, std::map<::Command, std::function<void()>>> commandHandlers;

	void Command::Handle(const ::Command& command)
	{
		application::Handlers::WithCurrent(commandHandlers, 
			[command](const std::map<::Command, std::function<void()>>& handlers) 
			{
				common::Utility::Dispatch(handlers, command);
			});
	}

	void Command::SetHandlers(int state, const std::map<::Command, std::function<void()>>& handlers)
	{
		commandHandlers[state] = handlers;
	}

	static const std::vector<::Command> commands =
	{
		::Command::UP,
		::Command::DOWN,
		::Command::LEFT,
		::Command::RIGHT,
		::Command::GREEN,
		::Command::RED,
		::Command::YELLOW,
		::Command::BLUE,
		::Command::NEXT,
		::Command::PREVIOUS,
		::Command::BACK,
		::Command::START
	};

	void Command::SetHandler(int state, std::function<void()> handler)
	{
		std::map<::Command, std::function<void()>> commandHandlers;
		for (auto command : commands)
		{
			commandHandlers[command] = handler;
		}
		SetHandlers(state, commandHandlers);
	}
}