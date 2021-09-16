#pragma once
#include <Application.Engine.h>
#include <functional>
#include <memory>
namespace visuals
{
	typedef std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> DrawerFunction;
	typedef std::function<DrawerFunction(const std::string&, const nlohmann::json&)> InternalizerFunction;
}
