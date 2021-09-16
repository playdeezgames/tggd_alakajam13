#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Plights.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Menus.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AVATAR_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.AvatarStatus";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string MENU_ID = "Status";
	static const std::string MENU_ITEM_JOB = "Job";

	enum class StatusMenuItem
	{
		JOB,
		GO_BACK
	};

	static const std::map<StatusMenuItem, std::function<void()>> activators =
	{
		{ StatusMenuItem::JOB, ::application::UIState::PushTo(::UIState::IN_PLAY_CURRENT_JOB) },
		{ StatusMenuItem::GO_BACK, ::application::UIState::Pop }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);


	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);
	static const auto WriteTextToGridRight = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::RIGHT);

	static void OnLeave()
	{
		::application::UIState::Pop();
	}

	static void UpdateStatistics()
	{
		WriteTextToGridRight({ 39,0 }, std::format(game::avatar::Statistics::FORMAT_MONEY, game::avatar::Statistics::GetMoney()), game::Colors::YELLOW);
		WriteTextToGridRight({ 39,1 }, std::format(game::avatar::Statistics::FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()), game::Colors::BLUE);
		WriteTextToGridRight({ 39,2 }, std::format(game::avatar::Statistics::FORMAT_BRAWLING, game::avatar::Statistics::GetBrawling()), game::Colors::BROWN);
		WriteTextToGridRight({ 39,3 }, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()), game::Colors::RED);
		WriteTextToGridRight({ 39,4 }, std::format(game::avatar::Statistics::FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()), game::Colors::MAGENTA);
		WriteTextToGridRight({ 39,5 }, std::format(game::avatar::Statistics::FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()), game::Colors::DARK_GRAY);
	}

	static void UpdatePlights()
	{
		WriteTextToGrid({ 0,0 }, "Plights:", game::Colors::GRAY);
		auto inflicted = game::avatar::Plights::InflictedWith();
		int row = 1;
		if (inflicted.empty())
		{
			WriteTextToGrid({ 0, row }, "(none)", game::Colors::GRAY);
			return;
		}
		for (auto& plight : inflicted)
		{
			auto descriptor = game::avatar::Plights::Read(plight);
			WriteTextToGrid({ 0,row++ }, descriptor.name, (descriptor.type == game::avatar::PlightType::CURSE) ? (game::Colors::RED) : (game::Colors::GREEN));
		}
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		UpdateStatistics();
		UpdatePlights();
	}

	static void RefreshMenu()
	{
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_JOB, game::avatar::Quest::Read().has_value());
	}

	static void Refresh()
	{
		RefreshGrid();
		RefreshMenu();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	void AvatarStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
