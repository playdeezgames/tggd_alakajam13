#pragma once
#include <functional>
#include <list>
#include <map>
#include <string>
namespace data::game
{
	struct Common
	{
		static std::list<std::map<std::string, std::string>> Execute(const std::string&);
		static std::function<void()> Run(const std::string&);
		static const int AVATAR_ID = 1;
	};

}
