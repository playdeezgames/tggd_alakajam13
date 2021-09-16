#pragma once
#include "Game.Difficulty.h"
#include <list>
#include <string>
namespace game::avatar
{
	struct Log
	{
		std::string color;
		std::string text;
		static void Reset(const Difficulty&);
		static std::list<Log> Read(size_t);
		static void Write(const Log&);
	};
}
