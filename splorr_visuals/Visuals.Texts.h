#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Texts
	{
		static void SetText(const std::string&, const std::string&, const std::string&);
		static void SetColor(const std::string&, const std::string&, const std::string&);
		static void SetLocation(const std::string&, const std::string&, const common::XY<int>&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}