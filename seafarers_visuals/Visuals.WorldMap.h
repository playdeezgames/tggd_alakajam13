#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct WorldMap
	{
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
		static void SetDestination(const std::string&, const std::string&, const std::optional<common::XY<int>>&);
		static std::optional<common::XY<double>> GetDestination(const std::string&, const std::string&);
		static std::optional<common::XY<double>> GetHoverIsland(const std::string&, const std::string&);
	};
}