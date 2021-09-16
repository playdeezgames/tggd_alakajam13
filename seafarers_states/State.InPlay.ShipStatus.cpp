#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Islands.h>
#include <Game.Ships.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIP_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.ShipStatus";

	static const std::string TEXT_SHIP_TYPE = "ShipType";
	static const std::string TEXT_SPEED_FACTOR = "SpeedFactor";
	static const std::string TEXT_TONNAGE = "Tonnage";
	static const std::string TEXT_FOULING = "Fouling";

	static const std::string FORMAT_SHIP_TYPE = "Ship Type: {}";
	static const std::string FORMAT_SPEED_FACTOR = "Speed Factor: {}";
	static const std::string FORMAT_TONNAGE = "Tonnage: {}";
	static const std::string FORMAT_FOULING = "Fouling: {:.0f}%";

	static const std::string MENU_ID = "ShipStatus";
	static const std::string MENU_ITEM_ID = "Careen";

	enum class ShipStatusItem
	{
		CHANGE_SPEED,
		CARGO,
		CAREEN,
		GO_BACK
	};

	static void OnGoBack()
	{
		::application::UIState::Pop();
	}

	static void OnCareen()
	{
		::application::UIState::Push(::UIState::IN_PLAY_AT_SEA_CAREEN_SELECT);
	}

	static void OnCargo()
	{
		::application::UIState::Push(::UIState::IN_PLAY_CARGO);
	}

	static void OnChangeSpeed()
	{
		::application::UIState::Push(::UIState::IN_PLAY_CHANGE_SPEED);
	}

	static void UpdateShipProperties()
	{
		auto shipType = game::avatar::Ship::Read();
		auto& descriptor = game::Ships::Read(shipType);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_SHIP_TYPE, std::format(FORMAT_SHIP_TYPE, descriptor.name));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_SPEED_FACTOR,
			std::format(FORMAT_SPEED_FACTOR, 
				descriptor.properties.find(game::ship::Property::SPEED_FACTOR)->second));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_TONNAGE,
			std::format(FORMAT_TONNAGE,
				descriptor.properties.find(game::ship::Property::TONNAGE)->second));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_FOULING,
			std::format(FORMAT_FOULING,
				game::avatar::ShipStatistics::GetFoulingPercentage()));
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_ID, game::Islands::CanDock());
		UpdateShipProperties();
	}

	static const std::map<ShipStatusItem, std::function<void()>> activators =
	{
		{ ShipStatusItem::CAREEN, OnCareen },
		{ ShipStatusItem::CARGO, OnCargo },
		{ ShipStatusItem::CHANGE_SPEED, OnChangeSpeed },
		{ ShipStatusItem::GO_BACK, OnGoBack }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, OnGoBack },
		{ ::Command::RED, OnGoBack }
	};

	void ShipStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
