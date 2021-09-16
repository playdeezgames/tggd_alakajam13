#include <queue>
#include "Visuals.Messages.h"
namespace visuals
{
	static std::queue<Message> messages;

	bool Messages::HasMessage()
	{
		return !messages.empty();
	}

	Message Messages::Read()
	{
		Message result = messages.front();
		messages.pop();
		return result;
	}

	void Messages::Write(const Message& message)
	{
		messages.push(message);
	}

	void Messages::Reset()
	{
		while (HasMessage())
		{
			Read();
		}
	}
}