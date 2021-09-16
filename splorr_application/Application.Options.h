#pragma once
#include <functional>
namespace application
{
	struct Options
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
		static void Initialize();
		static void Save();
	};
}