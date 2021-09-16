#include <algorithm>
#include <Data.JSON.h>
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.Layouts.h"
#include "Visuals.Texts.h"
namespace visuals
{
	struct InternalText
	{
		std::string font;
		std::string text;
		std::string color;
		common::XY<int> xy;
		HorizontalAlignment alignment;
		bool dropShadow;
		std::string dropShadowColor;
		common::XY<int> dropShadowXY;
	};

	static std::vector<InternalText> internalTexts;
	static std::map<std::string, std::map<std::string, size_t>> textTable;

	static void DrawInternalText(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t textIndex)
	{
		auto& text = internalTexts[textIndex];
		if (text.dropShadow)
		{
			visuals::Fonts::WriteText(
				text.font,
				renderer,
				text.xy + text.dropShadowXY,
				text.text,
				text.dropShadowColor,
				text.alignment);
		}
		visuals::Fonts::WriteText(
			text.font,
			renderer,
			text.xy,
			text.text,
			text.color,
			text.alignment);
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Texts::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t textIndex = internalTexts.size();
		internalTexts.push_back(
			{
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::TEXT],
				model[visuals::data::Properties::COLOR],
				common::XY<int>(
					model[visuals::data::Properties::X],
					model[visuals::data::Properties::Y]),
				(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT],
				(bool)model[visuals::data::Properties::DROP_SHADOW],
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				common::XY<int>(
					model[visuals::data::Properties::DROP_SHADOW_X],
					model[visuals::data::Properties::DROP_SHADOW_Y])
			});
		if (model.count(visuals::data::Properties::TEXT_ID) > 0)
		{
			textTable[layoutName][model[visuals::data::Properties::TEXT_ID]] = textIndex;
		}
		return [textIndex](const std::shared_ptr<application::Engine::Renderer>& renderer) 
		{
			DrawInternalText(renderer, textIndex);
		};
	}

	void Texts::SetText(const std::string& layoutName, const std::string& textId, const std::string& text)
	{
		auto textIndex = textTable[layoutName][textId];
		internalTexts[textIndex].text = text;
	}

	void Texts::SetColor(const std::string& layoutName, const std::string& textId, const std::string& color)
	{
		auto textIndex = textTable[layoutName][textId];
		internalTexts[textIndex].color = color;
	}

	void Texts::SetLocation(const std::string& layoutName, const std::string& textId, const common::XY<int>& location)
	{
		auto textIndex = textTable[layoutName][textId];
		internalTexts[textIndex].xy = location;
	}
}