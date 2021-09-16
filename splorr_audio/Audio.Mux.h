#pragma once
#include <functional>
#include <string>
namespace audio
{
	struct Mux
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
		static void Play(const std::string&);
		static void SetVolume(int);
		static int GetVolume();
		static void Initialize();
	};
}

