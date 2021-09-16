#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Application.Engine.h"
#include <map>
namespace application
{
	static std::map<int, std::string> renderLayouts;

	void Renderer::SetRenderLayout(int state, const std::string& layoutName)
	{
		renderLayouts[state] = layoutName;
	}
}
namespace application
{
	void Engine::Render(const std::shared_ptr<application::Engine::Renderer>& renderer)
	{
		auto layoutName = ::application::renderLayouts.find(::application::UIState::Read());
		if (layoutName != ::application::renderLayouts.end())
		{
			RenderLayout(renderer, layoutName->second);
		}
	}
}
