#pragma once
#include <string>
namespace application
{
	struct Renderer
	{
		static void SetRenderLayout(int, const std::string&);
		template<typename TState>
		static void SetRenderLayout(const TState& state, const std::string& layoutName)
		{
			SetRenderLayout((int)state, layoutName);
		}
	};
}
