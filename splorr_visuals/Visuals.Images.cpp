#include <functional>
#include <json.hpp>
#include <map>
#include <tuple>
#include "Visuals.Colors.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Images.h"
#include "Visuals.Layouts.h"
#include "Visuals.Sprites.h"
namespace visuals
{
	struct InternalImage
	{
		std::string sprite;
		std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> color;
		common::XY<int> xy;
		bool visible;
		double angle;
	};

	static std::vector<InternalImage> images;
	static std::map<std::string, std::map<std::string, size_t>> imageTable;

	static const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> defaultColor = { 255, 255, 255, 255 };

	static void DrawInternalImage(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t imageIndex)
	{
		auto& image = images[imageIndex];
		if (image.visible)
		{
			Sprites::Draw(
				image.sprite,
				renderer,
				image.xy,
				image.color,
				image.angle);
		}
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Images::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto imageIndex = images.size();
		images.push_back(
			{
				model[visuals::data::Properties::SPRITE],
				(model.count(data::Properties::COLOR) > 0) ? (::visuals::Colors::Read(model[data::Properties::COLOR])) : (defaultColor),
				common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y]),
				model.count(data::Properties::VISIBLE) == 0 || model[data::Properties::VISIBLE] == true,
				(model.count(data::Properties::ANGLE) > 0) ? ((double)model[data::Properties::ANGLE]) : (0.0)
			});
		if (model.count(visuals::data::Properties::IMAGE_ID) > 0)
		{
			std::string imageId = model[visuals::data::Properties::IMAGE_ID];
			imageTable[layoutName][imageId] = imageIndex;
		}
		return [imageIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalImage(renderer, imageIndex);
		};
	}

	void Images::SetSprite(const std::string& layoutName, const std::string& imageId, const std::string& spriteName)
	{
		auto imageIndex = imageTable[layoutName][imageId];
		images[imageIndex].sprite = spriteName;
	}

	void Images::SetVisible(const std::string& layoutName, const std::string& imageId, bool visible)
	{
		auto imageIndex = imageTable[layoutName][imageId];
		images[imageIndex].visible = visible;
	}

	void Images::SetAngle(const std::string& layoutName, const std::string& imageId, double angle)
	{
		auto imageIndex = imageTable[layoutName][imageId];
		images[imageIndex].angle = angle;
	}

	void Images::SetLocation(const std::string& layoutName, const std::string& imageId, const common::XY<int>& location)
	{
		auto imageIndex = imageTable[layoutName][imageId];
		images[imageIndex].xy = location;
	}

}