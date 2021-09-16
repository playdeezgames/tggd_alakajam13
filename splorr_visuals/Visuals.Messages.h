#pragma once
#include "Visuals.Message.h"
namespace visuals
{
	struct Messages
	{
		static bool HasMessage();
		static Message Read();
		static void Write(const Message&);
		static void Reset();
	};
}
