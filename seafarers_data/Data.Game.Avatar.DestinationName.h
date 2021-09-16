#pragma once
#include <optional>
#include <string>
namespace data::game::avatar
{
	struct DestinationName
	{
		static std::optional<std::string> Read(int);
		static void Write(int, const std::string&);
		static void Clear();
	};
}