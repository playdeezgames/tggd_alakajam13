#pragma once
#include <Common.Utility.h>
#include <functional>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Menus 
	{
		static std::optional<int> ReadIndex(const std::string&, const std::string&);
		static void WriteIndex(const std::string&, const std::string&, int);
		static void WriteMenuItemId(const std::string&, const std::string&, const std::string&);
		static size_t GetCount(const std::string&, const std::string&);
		static std::function<void()> NavigateNext(const std::string&, const std::string&);
		static std::function<void()> NavigatePrevious(const std::string&, const std::string&);

		template<typename TEnum>
		static std::function<void()> DoActivateItem(const std::string& layoutName, const std::string& menuId, const std::map<TEnum, std::function<void()>>& actions)
		{
			return [actions, layoutName, menuId]()
			{
				common::Utility::Dispatch(actions, (TEnum)visuals::Menus::ReadIndex(layoutName, menuId).value());
			};
		}
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}