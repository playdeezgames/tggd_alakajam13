#pragma once
#include "Visuals.DrawerFunction.h"
#include <string>
#include <json.hpp>
namespace visuals
{
	struct Sublayout
	{
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}
