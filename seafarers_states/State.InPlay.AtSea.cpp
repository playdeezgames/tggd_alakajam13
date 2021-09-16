#include <Application.Command.h>
#include <Application.OnEnter.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Application.Update.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Islands.h>
#include <Game.Colors.h>
#include "States.h"
#include "Sublayouts.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.AtSea";

	static const std::string MENU_ID = "Order";

	static const std::string AREA_CHANGE_HEADING = "ChangeHeading";
	static const std::string AREA_CHANGE_SPEED = "ChangeSpeed";
	static const std::string AREA_MOVE = "Move";

	static const std::string IMAGE_NEW_HEADING = "NewHeading";

	static const std::string MENU_ITEM_MOVE = "Move";
	static const std::string MENU_ITEM_DOCK = "Dock";

	static const std::string SPRITE_GRID_LOG = "Log";
	static const std::string FONT_LOG = "font5x7";
	static const size_t LOG_ENTRIES = 20;

	static const common::XY<int> CENTER = { 160, 160 };//TODO: hardcoded


	static double newHeading = 0.0;

	enum class OrderMenuItem
	{
		WORLD_MAP,
		MOVE,
		MOVE_ONCE,
		STATUS,
		SHIP,
		DOCK
	};

	static const std::string STOP_MOVE = "Stop Move";
	static const std::string START_MOVE = "Start Move";

	static void RefreshMoveMenuItem()
	{
		visuals::MenuItems::SetText(
			LAYOUT_NAME,
			MENU_ITEM_MOVE,
			(AtSea::IsAutoMoveEngaged()) ?
			(STOP_MOVE) :
			(START_MOVE));
	}

	static void OnUpdate(const unsigned int& ticks)
	{
		AtSea::DoAutomoveTimer(ticks);
		RefreshMoveMenuItem();
	}

	static void OnDock()
	{
		auto quest = game::avatar::Quest::Read();
		if (game::avatar::Docked::Dock() == game::avatar::Docked::DockResult::COMPLETED_QUEST)
		{
			visuals::Messages::Write(
				{
					"==DELIVERY COMPLETE==",
					{
						{
							{19,5},
							std::format("{} the {}",quest.value().personName, quest.value().professionName),
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,7},
							std::format("is {}",quest.value().receiptEmotion),
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,9},
							"when given the",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,11},
							std::format("{}.",quest.value().itemName),
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,14},
							"Yer reputation increases!",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnMoveOnce()
	{
		if (!AtSea::IsAutoMoveEngaged())
		{
			game::avatar::AtSea::Move();
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
	}

	static const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{OrderMenuItem::MOVE, AtSea::ToggleAutoMove},
		{OrderMenuItem::MOVE_ONCE, OnMoveOnce},
		{OrderMenuItem::DOCK, OnDock},
		{OrderMenuItem::WORLD_MAP, application::UIState::GoTo(::UIState::IN_PLAY_WORLD_MAP)},
		{OrderMenuItem::STATUS, application::UIState::PushTo(::UIState::IN_PLAY_AVATAR_STATUS)},
		{OrderMenuItem::SHIP, application::UIState::PushTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	static void RefreshLog()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_LOG);
		auto entries = game::avatar::Log::Read(LOG_ENTRIES);
		int row = (int)entries.size() - 1;
		for (auto entry : entries)
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_LOG, { 0,row }, FONT_LOG, entry.text, entry.color, visuals::HorizontalAlignment::LEFT);
			--row;
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		sublayout::AtSeaAvatarStatus::Refresh();
		RefreshLog();
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_DOCK, game::Islands::CanDock());
		AtSea::UpdateAutoMoveState();
	}

	static void OnMouseMotionInHelm(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, true);
		auto delta = location - CENTER;
		newHeading = common::Heading::XYToDegrees({(double)delta.GetX(), (double)delta.GetY()});
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_NEW_HEADING, newHeading);
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			OnMouseMotionInHelm(location);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static bool OnMouseButtonUp(const std::string& areaName)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			game::avatar::AtSea::SetHeading(newHeading);
			sublayout::AtSeaAvatarStatus::Refresh();
			return true;
		}
		return false;
	}

	void AtSea::Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_AT_SEA, OnUpdate);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUp));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
	}
}