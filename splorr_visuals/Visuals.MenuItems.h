#pragma once
#include <Common.Utility.h>
#include <functional>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct MenuItems
	{
		static void SetText(const std::string&, const std::string&, const std::string&);
		static void SetEnabled(const std::string&, const std::string&, bool);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}