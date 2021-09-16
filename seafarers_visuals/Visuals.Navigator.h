#pragma once
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Navigator
	{
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}
