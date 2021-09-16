#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <optional>
namespace common
{
	struct Utility
	{
		template<typename TEnum>
		static void Dispatch(const std::map<TEnum, std::function<void()>> handlers, const TEnum& key)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				iter->second();
			}
		}
		template<typename TEnum, typename TResult>
		static TResult Dispatch(const std::map<TEnum, std::function<TResult()>> handlers, const TEnum& key, const TResult& defaultResult)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				return iter->second();
			}
			return defaultResult;
		}
		template<typename TEnum, typename TParameter>
		static void DispatchParameter(const std::map<TEnum, std::function<void(const TParameter&)>> handlers, const TEnum& key, const TParameter& parameter)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				iter->second(parameter);
			}
		}
		template<typename TEnum, typename TResult, typename TParameter>
		static TResult DispatchParameter(const std::map<TEnum, std::function<TResult(const TParameter&)>> handlers, const TEnum& key, const TParameter& parameter, const TResult& defaultResult)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				return iter->second(parameter);
			}
			return defaultResult;
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoNextItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index + 1) % table.size();
				refresh();
			};
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoPreviousItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index + table.size() - 1) % table.size();
				refresh();
			};
		}

		template<typename TKey, typename TValue>
		static std::optional<TKey> GetNthKey(const std::map<TKey, TValue>& table, size_t index)
		{
			auto iter = table.begin();
			while (index > 0 && iter != table.end())
			{
				iter++;
				index--;
			}
			if (iter != table.end())
			{
				return iter->first;
			}
			return std::nullopt;
		}
		template<typename TItem>
		static std::optional<TItem> GetNth(const std::list<TItem>& items, size_t index)
		{
			auto iter = items.begin();
			while (index > 0 && iter != items.end())
			{
				iter++;
				index--;
			}
			if (iter != items.end())
			{
				return *iter;
			}
			return std::nullopt;
		}
	};
}


