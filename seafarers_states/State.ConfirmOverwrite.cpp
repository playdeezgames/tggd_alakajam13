#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state
{
	static const std::string LAYOUT_NAME = "State.ConfirmOverwrite";
	static const std::string MENU_ID = "ConfirmOverwrite";

	enum class ConfirmOverwriteItem
	{
		NO,
		YES
	};

	static const std::map<::UIState, int> slotTable =
	{
		{::UIState::CONFIRM_OVERWRITE_SLOT1, 1},
		{::UIState::CONFIRM_OVERWRITE_SLOT2, 2},
		{::UIState::CONFIRM_OVERWRITE_SLOT3, 3},
		{::UIState::CONFIRM_OVERWRITE_SLOT4, 4},
		{::UIState::CONFIRM_OVERWRITE_SLOT5, 5},
	};

	static void ConfirmOverwrite()
	{
		int slot = slotTable.find((::UIState)application::UIState::Read())->second;
		game::SaveToSlot(slot);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmOverwriteItem, std::function<void()>> activators =
	{
		{ ConfirmOverwriteItem::NO, application::UIState::GoTo(::UIState::SAVE_GAME) },
		{ ConfirmOverwriteItem::YES, ConfirmOverwrite }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void SetCurrentMenuItem(ConfirmOverwriteItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	static const std::vector<::UIState> states =
	{
		::UIState::CONFIRM_OVERWRITE_SLOT1,
		::UIState::CONFIRM_OVERWRITE_SLOT2,
		::UIState::CONFIRM_OVERWRITE_SLOT3,
		::UIState::CONFIRM_OVERWRITE_SLOT4,
		::UIState::CONFIRM_OVERWRITE_SLOT5,
	};

	static void ConfigureState(const ::UIState& state)
	{
		::application::OnEnter::AddHandler(state, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));

		::application::MouseButtonUp::AddHandler(state, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(state, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(state, commandHandlers);
		::application::Renderer::SetRenderLayout(state, LAYOUT_NAME);
	}

	void ConfirmOverwrite::Start()
	{
		for (auto state : states)
		{
			ConfigureState(state);
		}
	}
}