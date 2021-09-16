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
#include <Visuals.MenuItems.h>
namespace state
{
	static const std::string LAYOUT_NAME = "State.SaveGame";
	static const std::string MENU_ID = "SaveGame";
	static const std::string MENU_ITEM_AUTOSAVE = "Autosave";
	static const std::string MENU_ITEM_SLOT1 = "Slot1";
	static const std::string MENU_ITEM_SLOT2 = "Slot2";
	static const std::string MENU_ITEM_SLOT3 = "Slot3";
	static const std::string MENU_ITEM_SLOT4 = "Slot4";
	static const std::string MENU_ITEM_SLOT5 = "Slot5";
	static const std::string AUTOSAVE_PRESENT = "(autosave)";
	static const std::string NOT_PRESENT = "-";
	static const std::string SLOT_PRESENT_1 = "Slot 1";
	static const std::string SLOT_PRESENT_2 = "Slot 2";
	static const std::string SLOT_PRESENT_3 = "Slot 3";
	static const std::string SLOT_PRESENT_4 = "Slot 4";
	static const std::string SLOT_PRESENT_5 = "Slot 5";

	enum class SaveGameItem
	{
		AUTOSAVE,
		SLOT_1,
		SLOT_2,
		SLOT_3,
		SLOT_4,
		SLOT_5,
		BACK
	};

	static void SaveToAutosave()
	{
		game::AutoSave();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static std::function<void()> SlotSaver(int slot, ::UIState confirmState)
	{
		return [slot, confirmState]()
		{
			if (game::DoesSlotExist(slot))
			{
				application::UIState::Write(confirmState);
				return;
			}
			game::SaveToSlot(slot);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		};
	}

	static const std::map<SaveGameItem, std::function<void()>> activators =
	{
		{ SaveGameItem::AUTOSAVE, SaveToAutosave },
		{ SaveGameItem::SLOT_1, SlotSaver(1, ::UIState::CONFIRM_OVERWRITE_SLOT1) },
		{ SaveGameItem::SLOT_2, SlotSaver(2, ::UIState::CONFIRM_OVERWRITE_SLOT2) },
		{ SaveGameItem::SLOT_3, SlotSaver(3, ::UIState::CONFIRM_OVERWRITE_SLOT3) },
		{ SaveGameItem::SLOT_4, SlotSaver(4, ::UIState::CONFIRM_OVERWRITE_SLOT4) },
		{ SaveGameItem::SLOT_5, SlotSaver(5, ::UIState::CONFIRM_OVERWRITE_SLOT5) },
		{ SaveGameItem::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::RED, application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{ ::Command::GREEN, ActivateItem }
	};

	static void UpdateMenuItem(bool shown, const std::string& menuItem, const std::string& textWhenShown)
	{
		visuals::MenuItems::SetText(LAYOUT_NAME, menuItem, (shown) ? (textWhenShown) : (NOT_PRESENT));
	}

	static void OnEnter()
	{
		UpdateMenuItem(game::DoesAutosaveExist(), MENU_ITEM_AUTOSAVE, AUTOSAVE_PRESENT);
		UpdateMenuItem(game::DoesSlotExist(1), MENU_ITEM_SLOT1, SLOT_PRESENT_1);
		UpdateMenuItem(game::DoesSlotExist(2), MENU_ITEM_SLOT2, SLOT_PRESENT_2);
		UpdateMenuItem(game::DoesSlotExist(3), MENU_ITEM_SLOT3, SLOT_PRESENT_3);
		UpdateMenuItem(game::DoesSlotExist(4), MENU_ITEM_SLOT4, SLOT_PRESENT_4);
		UpdateMenuItem(game::DoesSlotExist(5), MENU_ITEM_SLOT5, SLOT_PRESENT_5);
	}

	void SaveGame::Start()
	{
		::application::OnEnter::AddHandler(::UIState::SAVE_GAME, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::SAVE_GAME, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::SAVE_GAME, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::SAVE_GAME, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::SAVE_GAME, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::SAVE_GAME, OnEnter);
	}
}