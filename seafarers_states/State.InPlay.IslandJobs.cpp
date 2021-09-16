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
#include <Game.Avatar.h>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Docked.h>
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
	static const std::string LAYOUT_NAME = "State.InPlay.IslandJobs";
	static const std::string MENU_ID = "AcceptJob";
	static const std::string MENU_ITEM_ACCEPT_JOB = "Accept";
	static const std::string TEXT_LINE1 = "Line1";
	static const std::string TEXT_LINE2 = "Line2";
	static const std::string TEXT_LINE3 = "Line3";
	static const std::string TEXT_LINE4 = "Line4";
	static const std::string TEXT_LINE5 = "Line5";
	static const std::string TEXT_LINE6 = "Line6";

	enum class AcceptJobMenuItem
	{
		ACCEPT,
		CANCEL
	};

	static void OnAccept()//TODO: make this more declarative
	{
		switch (game::avatar::Quest::AcceptQuest(game::avatar::Docked::GetDockedLocation().value()))
		{
		case game::avatar::Quest::AcceptQuestResult::ACCEPTED_QUEST:
			game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			break;
		case game::avatar::Quest::AcceptQuestResult::ALREADY_HAS_QUEST:
			::application::UIState::Write(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB);
			break;
		}
	}

	static void OnCancel()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<AcceptJobMenuItem, std::function<void()>> activators =
	{
		{ AcceptJobMenuItem::ACCEPT, OnAccept },
		{ AcceptJobMenuItem::CANCEL, OnCancel }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnCancel },
		{::Command::RED, OnCancel }
	};

	static void UpdateQuestText(const game::Quest& questModel)
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "Please deliver this");//TODO: hardcoded
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, questModel.itemName);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, std::format("to {}", questModel.personName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, std::format("the {}", questModel.professionName));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, std::format("Reward: {:.2f}", questModel.reward));

		auto islandModel = game::Islands::Read(questModel.destination).value();
		double distance = common::Heading::Distance(questModel.destination, game::avatar::AtSea::GetLocation());
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, std::format("at {} ({:.2f}).", islandModel.name, distance));
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ACCEPT_JOB, true);
	}

	static void UpdateNoQuestText()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE1, "No Jobs");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE2, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE3, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE4, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE5, "");
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_LINE6, "");
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ACCEPT_JOB, false);
	}

	static void UpdateText()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto quest = game::islands::Quests::Read(location);
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

	void IslandJobs::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_JOBS, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_JOBS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_JOBS, LAYOUT_NAME);
	}
}