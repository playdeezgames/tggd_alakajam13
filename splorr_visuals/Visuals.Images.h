#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Images
	{
		static void SetSprite(const std::string&, const std::string&, const std::string&);
		static void SetVisible(const std::string&, const std::string&, bool);
		static void SetAngle(const std::string&, const std::string&, double);
		static void SetLocation(const std::string&, const std::string&, const common::XY<int>&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};

}
