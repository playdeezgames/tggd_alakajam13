#pragma once
#include <functional>
#include <string>
namespace visuals
{
	struct Confirmation
	{
		std::string prompt;
		std::function<void()> onYes;
		std::function<void()> onNo;
	};
}