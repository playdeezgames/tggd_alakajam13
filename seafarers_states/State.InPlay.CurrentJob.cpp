#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Quest.h>
#include <Game.Islands.h>
#include <Game.Islands.Quests.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.AvatarJob";
	static const std::string MENU_ID = "CurrentJob";
	static const std::string MENU_ITEM_ABANDON_JOB = "Abandon";
	static const std::string TEXT_LINE1 = "Line1";
	static const std::string TEXT_LINE2 = "Line2";
	static const std::string TEXT_LINE3 = "Line3";
	static const std::string TEXT_LINE4 = "Line4";
	static const std::string TEXT_LINE5 = "Line5";
	static const std::string TEXT_LINE6 = "Line6";

	enum class CurrentJobMenuItem
	{
		ABANDON,
		CANCEL
	};

	static const std::map<CurrentJobMenuItem, std::function<void()>> activators =
	{
		{ CurrentJobMenuItem::ABANDON, ::application::UIState::GoTo(::UIState::IN_PLAY_CONFIRM_ABANDON_JOB) },
		{ CurrentJobMenuItem::CANCEL, ::application::UIState::Pop }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::Pop },
		{::Command::RED, ::application::UIState::Pop }
	};

	static void UpdateQuestText(const game::Quest& questModel)//TODO: duplicated!
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "Please deliver this");//TODO: hardcoded
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, questModel.itemName);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, std::format("to {}", questModel.personName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, std::format("the {}", questModel.professionName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, std::format("Reward: {:.2f}", questModel.reward));

		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = common::Heading::Distance(questModel.destination, game::avatar::AtSea::GetLocation());
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, std::format("at {} ({:.2f}).", islandModel.name, distance));
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ABANDON_JOB, true);
	}

	static void UpdateNoQuestText()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "No Jobs");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, "");
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ABANDON_JOB, false);
	}

	static void UpdateText()
	{
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			UpdateQuestText(quest.value());
			return;
		}
		UpdateNoQuestText();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateText();
	}

	void CurrentJob::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CURRENT_JOB, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CURRENT_JOB, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CURRENT_JOB, LAYOUT_NAME);
	}
}