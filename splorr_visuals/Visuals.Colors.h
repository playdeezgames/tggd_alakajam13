#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>
namespace visuals
{
	struct Colors
	{
		static void SetStore(int);
		template<typename TStore>
		static std::function<void()> DoSetStore(const TStore& store)
		{
			return [store]() 
			{
				SetStore((int)store);
			};
		}
		static std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> Read(const std::string&);
	};
}