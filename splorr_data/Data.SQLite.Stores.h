#pragma once
#include <functional>
#include <list>
#include <map>
#include <string>
namespace data::sqlite
{
	struct Stores
	{
		static void SetConnection(int, const std::string&);
		template<typename TStore>
		static std::function<void()> DoSetConnection(const TStore& store, const std::string& connectionString)
		{
			return [store, connectionString]()
			{
				SetConnection((int)store, connectionString);
			};
		}

		static std::list<std::map<std::string, std::string>> Execute(int, const std::string&);
		template<typename TStore>
		static std::list<std::map<std::string, std::string>> Execute(const TStore& store, const std::string& query)
		{
			return Execute((int)store, query);
		}

		static void Copy(int, int);
		template<typename TStore>
		static void Copy(const TStore& first, const TStore& second)
		{
			Copy((int)first, (int)second);
		}

		static void Bounce(int);
		template<typename TStore>
		static void Bounce(const TStore& store)
		{
			Bounce((int)store);
		}
	};
}
