#pragma once
#include <map>
namespace data::game::avatar
{
	struct Items
	{
		static size_t Read(const int&);
		static std::map<int, size_t> All();
		static void Write(const int&, const size_t&);
		static void Clear();
	};
}
