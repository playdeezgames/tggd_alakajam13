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
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MAKE_OFFERING;
	static const std::string LAYOUT_NAME = "State.InPlay.MakeOffering";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string AREA_LIST = "List";
	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string TEXT_GO_BACK = "GoBack";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}", "Item"), game::Colors::YELLOW);
	}

	static std::map<game::Item, size_t> items;
	static size_t hiliteRow = 0;
	static bool hoverGoBack = false;

	static void UpdateItems()
	{
		items = game::avatar::Items::All();
	}

	static void RefreshItems()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& item : items)
		{
			auto& itemDescriptor = game::Items::Read(item.first);
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:15s}",
					itemDescriptor.name),
				(row == hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			game::Colors::GRAY);
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshItems();
		RefreshStatistics();
	}

	static void RefreshButton()
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_GO_BACK, (hoverGoBack) ? (game::Colors::CYAN) : (game::Colors::GRAY));
	}

	static void Refresh()
	{
		RefreshGrid();
		RefreshButton();
	}



	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateItems();
		Refresh();
	}

	static void OnOfferingResult(game::OfferingResult result)
	{
		switch (result)
		{
		case game::OfferingResult::BLESSING:
			visuals::Messages::Write(
				{
				"==BLESSING RECEIVED==",
					{
						{
							{19,9},
							"The demigod is sufficiently please with you",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,11},
							"and decides to bless you!",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			break;
		case game::OfferingResult::CURSE:
			visuals::Messages::Write(
				{
				"==CURSE RECEIVED==",
					{
						{
							{19,9},
							"The demigod is sufficiently displeased with you",
							game::Colors::RED,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,11},
							"and decides to curse you!",
							game::Colors::RED,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			break;
		case game::OfferingResult::COOLING_DOWN:
			visuals::Messages::Write(
				{
				"",
					{
						{
							{19,9},
							"You have given enough for now...",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						},
						{
							{19,11},
							"...try again later.",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			break;
		case game::OfferingResult::SUCCESS:
			visuals::Messages::Write(
				{
				"==OFFERING ACCEPTED==",
					{
						{
							{19,10},
							"Yer offering has been accepted.",
							game::Colors::GRAY,
							visuals::HorizontalAlignment::CENTER
						}
					}
				});
			break;
		}
	}

	static void OfferItem()
	{
		auto item = common::Utility::GetNthKey(items, hiliteRow);
		if (item)
		{
			auto location = game::avatar::Docked::GetDockedLocation().value();
			auto island = game::Islands::Read(location).value();
			game::avatar::Items::Remove(item.value(), 1);
			OnOfferingResult(game::Demigods::MakeOffering(island.patronDemigod, item.value()));
			OnLeave();
		}
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_GO_BACK)
		{
			OnLeave();
		}
		else if (areaName == AREA_LIST)
		{
			OfferItem();
		}
		return true;
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, items, Refresh) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, items, Refresh) },
		{ ::Command::GREEN, OfferItem },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		hoverGoBack = false;
		if (areaName == AREA_LIST)
		{
			auto cellHeight = visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_ID);
			hiliteRow = location.GetY() / cellHeight;
		}
		else if (areaName == AREA_GO_BACK)
		{
			hoverGoBack = true;
		}
		Refresh();
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>&)
	{
		hoverGoBack = false;
		Refresh();
	}

	void MakeOffering::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
