#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include "Sublayouts.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
namespace sublayout
{
	static const std::string LAYOUT_NAME = "Sublayout.UIHamburger";
	static const std::string AREA_UI_HAMBURGER = "UIHamburger";
	static const std::string IMAGE_UI_HAMBURGER = "ui-hamburger";
	static const std::string SPRITE_UI_HAMBURGER_NORMAL = "UIHamburgerNormal";
	static const std::string SPRITE_UI_HAMBURGER_HOVER = "UIHamburgerHover";

	static const std::vector<::UIState> states =
	{
		::UIState::IN_PLAY_AT_SEA,
		::UIState::IN_PLAY_CHANGE_HEADING
	};

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_HOVER);
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_HAMBURGER, SPRITE_UI_HAMBURGER_NORMAL);
	}

	static bool LeavePlay()
	{
		application::UIState::Write(::UIState::LEAVE_PLAY);
		return true;
	}

	static const std::map<std::string, std::function<bool()>> mouseUpHandlers =
	{
		{AREA_UI_HAMBURGER, LeavePlay }
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		return common::Utility::Dispatch(mouseUpHandlers, area, false);
	}

	void UIHamburger::Start()
	{
		for (auto state : states)
		{
			::application::MouseButtonUp::AddHandler(state, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
			::application::MouseMotion::AddHandler(state, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		}
	}
}