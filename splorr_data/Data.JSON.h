#pragma once
#include <string>
#include "json.hpp"
namespace data
{
	struct JSON
	{
		static nlohmann::json Load(const std::string&);
		static void Save(const std::string&, const nlohmann::json&);
	};
}