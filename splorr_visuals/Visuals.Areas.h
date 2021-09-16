#pragma once
#include <Common.XY.h>
#include <functional>
#include <json.hpp>
#include <MouseButton.h>
#include <optional>
#include <set>
#include <string>
#include "Visuals.Area.h"
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Areas
	{
		static std::set<std::string> Get(const std::string&, const common::XY<int>&);
		static Area Get(const std::string&, const std::string&);
		static std::function<void(const common::XY<int>&)> HandleMenuMouseMotion(const std::string&);
		static std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>, std::function<void(const common::XY<int>&)>);
		static std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>);
		static std::function<bool(const common::XY<int>&, MouseButton)> HandleMenuMouseButtonUp(const std::string&, std::function<void()>);
		static std::function<bool(const common::XY<int>&, MouseButton)> HandleMouseButtonUp(const std::string&, std::function<bool(const std::string&)>);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}
