#pragma once
#include <functional>
#include "json.hpp"
#include <optional>
namespace data::json
{
	struct Stores
	{
		static void SetStoreFile(int, const std::string&, std::optional<unsigned char>);
		template<typename TStore>
		static std::function<void()> DoSetStoreFile(const TStore& store, const std::string& filename, std::optional<unsigned char> checksum)
		{
			return [store, filename, checksum]() 
			{
				SetStoreFile((int)store, filename, checksum);
			};
		}
		static nlohmann::json& GetStore(int);
		template<typename TStore>
		static nlohmann::json& GetStore(const TStore& store)
		{
			return GetStore((int)store);
		}
		static void Start();
		static void Save(int);
		template<typename TStore>
		static void Save(const TStore& store)
		{
			Save((int)store);
		}
		static bool IsModded();
	};
}
