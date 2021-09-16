#include "Application.Handlers.h"
#include "Application.TextInput.h"
#include <map>
namespace application
{
	static std::map<int, std::vector<TextInput::Handler>> textInputHandlers;

	void TextInput::Handle(const std::string& text)
	{
		application::Handlers::WithCurrent(
			textInputHandlers,
			[text](const std::vector<TextInput::Handler>& handlers)
			{
				for (auto& handler : handlers)
				{
					handler(text);
				}
			});
	}

	void TextInput::AddHandler(int state, TextInput::Handler handler)
	{
		textInputHandlers[state].push_back(handler);
	}
}