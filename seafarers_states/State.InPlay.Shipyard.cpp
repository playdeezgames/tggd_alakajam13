#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Markets.h>
#include <Game.Islands.Ships.h>
#include <Game.Ships.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.Shipyard";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";

	static std::map<game::Ship, double> shipPrices;
	static size_t hiliteRow = 0;

	static void UpdateShipPrices()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto prices = game::islands::Ships::GetPurchasePrices(location);
		auto tradeIn = game::islands::Ships::GetSalePrice(location, game::avatar::Ship::Read());
		shipPrices.clear();
		for (auto price : prices)
		{
			shipPrices[price.first] = price.second - tradeIn;
		}
	}

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			game::Colors::WHITE);
	}

	static void RefreshShipPrices()
	{
		int row = 0;
		int gridRow = 2;
		auto ship = game::avatar::Ship::Read();
		for (auto& unitPrice : shipPrices)
		{
			auto& shipDescriptor = game::Ships::Read(unitPrice.first);
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:10s}{}| {:7.3f}",
					shipDescriptor.name,
					(ship == unitPrice.first) ? ("*") : (" "),
					unitPrice.second),
				(row == hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshShipPrices();
		RefreshStatistics();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateShipPrices();
		RefreshGrid();
	}

	static void BuyShip()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto currentShip = game::avatar::Ship::Read();
		auto ship = common::Utility::GetNthKey(shipPrices, hiliteRow);
		if (ship)
		{
			//TODO: if current cargo is too much for the new ship type, deny buying the ship type
			double price = shipPrices[ship.value()];
			if (game::avatar::Statistics::GetMoney() >= price)
			{
				game::avatar::Statistics::ChangeMoney(-price);
				game::avatar::Ship::Write(ship.value());
				game::islands::Markets::BuyShip(location, ship.value());
				game::islands::Markets::SellShip(location, currentShip);
				UpdateShipPrices();
				RefreshGrid();
			}
		}
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::GREEN, BuyShip },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	void Shipyard::Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_SHIPYARD, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_SHIPYARD, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_SHIPYARD, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_SHIPYARD, LAYOUT_NAME);
	}
}
