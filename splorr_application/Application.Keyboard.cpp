#include "Application.Handlers.h"
#include "Application.Keyboard.h"
#include "Command.h"
#include <Common.Data.h>
#include <Data.JSON.Stores.h>
#pragma warning (disable: 26812)
namespace application
{
	static std::map<int, ::Command> keyboardCommands;

	static bool initialized = false;

	static std::optional<int> store;

	void Keyboard::SetStore(int s)
	{
		store = s;
	}

	static void Initialize()
	{
		if (!initialized)
		{
			auto& config = data::json::Stores::GetStore(store.value());
			for (auto& item : config.items())
			{
				int code = common::Data::ToInt(item.key());
				::Command command = (::Command)(int)item.value();
				keyboardCommands[code] = command;
			}
			initialized = true;
		}
	}

	std::optional<::Command> Keyboard::ToCommand(int keycode)
	{
		Initialize();
		auto iter = keyboardCommands.find(keycode);
		if (iter != keyboardCommands.end())
		{
			return iter->second;
		}
		return std::nullopt;
	}

	static std::map<int, std::vector<Keyboard::Handler>> keyDownHandlers;

	void Keyboard::AddHandler(int state, Handler handler)
	{
		keyDownHandlers[state].push_back(handler);
	}

	bool Keyboard::Handle(const std::string& keyName)
	{
		bool result = false;
		application::Handlers::WithCurrent(
			keyDownHandlers,
			[&result, keyName](const std::vector<Handler>& handlers)
			{
				for (auto& handler : handlers)
				{
					if (handler(keyName))
					{
						result = true;
						break;
					}
				}
			});
		return result;
	}
}