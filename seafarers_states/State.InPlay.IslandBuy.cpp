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
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_BUY;
	static const std::string LAYOUT_NAME = "State.InPlay.IslandBuy";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string AREA_LIST = "List";
	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string AREA_INCREASE_QUANTITY = "IncreaseQuantity";
	static const std::string AREA_DECREASE_QUANTITY = "DecreaseQuantity";
	static const std::string TEXT_GO_BACK = "GoBack";
	static const std::string TEXT_INCREASE_QUANTITY = "IncreaseQuantity";
	static const std::string TEXT_QUANTITY = "Quantity";
	static const std::string TEXT_DECREASE_QUANTITY = "DecreaseQuantity";
	static const std::string FORMAT_QUANTITY = "Qty: {}";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format("{:14s}   {:7s}   {:4s}   {:5s}", "Item", "  Price", " Own", "Tonn."), game::Colors::YELLOW);
	}

	static std::map<game::Item, double> unitPrices;
	static size_t hiliteRow = 0;
	static const std::vector<size_t> quantities = 
	{
		1,
		2,
		5,
		10,
		20,
		50,
		100,
		200,
		500
	};
	static size_t quantityIndex = 0;
	enum class HoverButton
	{
		GO_BACK,
		INCREASE_QUANTITY,
		DECREASE_QUANTITY
	};
	static std::optional<HoverButton> hoverButton = std::nullopt;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetPurchasePrices(game::avatar::Docked::GetDockedLocation().value());
	}

	static const std::string LIST_ITEM_FORMAT = "{:14s} | {:7.3f} | {:4d} | {:5.3f}";

	static void RefreshUnitPrices()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& unitPrice : unitPrices)
		{
			auto& itemDescriptor = game::Items::Read(unitPrice.first);
			WriteTextToGrid(
				{ 0, gridRow }, 
				std::format(LIST_ITEM_FORMAT,
					itemDescriptor.name, 
					unitPrice.second,
					game::avatar::Items::Read(unitPrice.first),
					itemDescriptor.tonnage), 
				(row==hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static const std::string FORMAT_TONNAGE = "Avail. Tonn.: {:.3f}";
	static const std::string FORMAT_MONEY = "Money: {:.3f}";

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 18 },
			std::format(
				FORMAT_TONNAGE,
				game::avatar::Ship::AvailableTonnage()),
			game::Colors::WHITE);
		WriteTextToGrid(
			{ 0, 19 }, 
			std::format(
				FORMAT_MONEY,
				game::avatar::Statistics::GetMoney()),
			game::Colors::WHITE);
	}

	static void RefreshButtons()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_QUANTITY, std::format(FORMAT_QUANTITY, quantities[quantityIndex]));
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_GO_BACK, (hoverButton.has_value() && hoverButton.value()== HoverButton::GO_BACK) ? (game::Colors::CYAN) : (game::Colors::GRAY));
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_INCREASE_QUANTITY, (hoverButton.has_value() && hoverButton.value() == HoverButton::INCREASE_QUANTITY) ? (game::Colors::CYAN) : (game::Colors::GRAY));
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_DECREASE_QUANTITY, (hoverButton.has_value() && hoverButton.value() == HoverButton::DECREASE_QUANTITY) ? (game::Colors::CYAN) : (game::Colors::GRAY));
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshUnitPrices();
		RefreshStatistics();
	}

	static void Refresh()
	{
		RefreshButtons();
		RefreshGrid();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateUnitPrices();
		Refresh();
	}

	static void BuyItem()
	{
		auto item = common::Utility::GetNthKey(unitPrices, hiliteRow);
		if (item)
		{
			auto quantity = quantities[quantityIndex];
			double totalPrice = unitPrices[item.value()] * quantity;
			if (game::avatar::Statistics::GetMoney() >= totalPrice)
			{
				if (game::avatar::Ship::AvailableTonnage() >= game::Items::Read(item.value()).tonnage * quantity)
				{
					game::avatar::Statistics::ChangeMoney(-totalPrice);
					game::islands::Markets::BuyItems(game::avatar::Docked::GetDockedLocation().value(), item.value(), quantity);
					game::avatar::Items::Add(item.value(), quantity);

					UpdateUnitPrices();
					Refresh();
				}
			}
		}
	}

	static void DecreaseQuantity()
	{
		if (quantityIndex > 0)
		{
			quantityIndex--;
			Refresh();
		}
	}

	static void IncreaseQuantity()
	{
		if (quantityIndex < quantities.size() - 1)
		{
			quantityIndex++;
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> mouseButtonHandlers =
	{
		{ AREA_GO_BACK, OnLeave},
		{ AREA_LIST, BuyItem },
		{ AREA_DECREASE_QUANTITY, DecreaseQuantity },
		{ AREA_INCREASE_QUANTITY, IncreaseQuantity }
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		common::Utility::Dispatch(mouseButtonHandlers, areaName);
		return true;
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, unitPrices, Refresh) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, unitPrices, Refresh) },
		{ ::Command::GREEN, BuyItem },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	static std::function<void(const common::XY<int>&)> GoToHoverButton(HoverButton button)
	{
		return [button](const common::XY<int>&)
		{
			hoverButton = button;
		};
	}

	static void SetHiliteRow(const common::XY<int>& location)
	{
		hiliteRow = location.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_ID);
	}

	static const std::map<std::string, std::function<void(const common::XY<int>&)>> mouseMotionHandlers =
	{
		{ AREA_LIST, SetHiliteRow },
		{ AREA_GO_BACK, GoToHoverButton(HoverButton::GO_BACK) },
		{ AREA_DECREASE_QUANTITY, GoToHoverButton(HoverButton::DECREASE_QUANTITY) },
		{ AREA_INCREASE_QUANTITY, GoToHoverButton(HoverButton::INCREASE_QUANTITY) }
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		hoverButton = std::nullopt;
		common::Utility::DispatchParameter(mouseMotionHandlers, areaName, location);
		Refresh();
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>&)
	{
		hoverButton = std::nullopt;
		Refresh();
	}

	void IslandBuy::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
