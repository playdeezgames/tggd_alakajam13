#pragma once
#include <Common.XY.h>
#include <functional>
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
#include "Visuals.HorizontalAlignment.h"
namespace visuals
{
	struct SpriteGrid
	{
		static void SetCell(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&);
		static void WriteText(const std::string&, const std::string&, const common::XY<int>&, const std::string&, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
		static void ClearCell(const std::string&, const std::string&, const common::XY<int>&);
		static void Clear(const std::string&, const std::string&);
		static int GetCellHeight(const std::string&, const std::string&);
		static std::function<void(const common::XY<int>&, const std::string&, const std::string&)> DoWriteToGrid(const std::string&, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}