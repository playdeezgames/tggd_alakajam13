#include "UIState.h"
#include "States.h"
#include <string>
#include <Application.OnEnter.h>
#include <Application.Command.h>
#include <Application.Renderer.h>
#include <Game.Audio.Mux.h>
#include <Application.UIState.h>
#include <Visuals.SpriteGrid.h>
#include <Game.Actors.h>
#include <Game.ActorTypes.h>
#include <Visuals.Images.h>
#include <Game.Audio.Sfx.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_BOARD;
	static const std::string LAYOUT_NAME = "State.InPlay.Board";
	static const std::string SPRITE_GRID = "Grid";
	static const std::string SPRITE_GRID_MINI = "Mini";
	static const int BOARD_COLUMNS = 9;
	static const int BOARD_ROWS = 9;
	static const int BOARD_OFFSET_COLUMN = -4;
	static const int BOARD_OFFSET_ROW = -4;

	static void RefreshMinimap()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_MINI);
		for (const auto& actor : game::Actors::All())
		{
			auto descriptor = game::ActorTypes::Read(actor.actorType);
			visuals::SpriteGrid::SetCell(LAYOUT_NAME, SPRITE_GRID_MINI, actor.location, descriptor.miniSprite, "White");
		}
		visuals::SpriteGrid::SetCell(LAYOUT_NAME, SPRITE_GRID_MINI, game::Actors::GetCurrent().location, "MiniSelector", "White");
	}

	static void RefreshActors()
	{
		auto currentActor = game::Actors::GetCurrent();
		int minimumColumn = currentActor.location.GetX() + BOARD_OFFSET_COLUMN;
		int minimumRow = currentActor.location.GetY() + BOARD_OFFSET_ROW;
		int maximumColumn = minimumColumn + BOARD_COLUMNS;
		int maximumRow = minimumRow + BOARD_ROWS;
		for (auto actor : game::Actors::All())
		{
			if (actor.actorType != game::ActorType::DELETED)
			{
				if (actor.location.GetX() >= minimumColumn && actor.location.GetX() < maximumColumn && actor.location.GetY() >= minimumRow && actor.location.GetY() < maximumRow)
				{
					auto descriptor = game::ActorTypes::Read(actor.actorType);
					int plotX = actor.location.GetX() - currentActor.location.GetX() - BOARD_OFFSET_COLUMN;
					int plotY = actor.location.GetY() - currentActor.location.GetY() - BOARD_OFFSET_ROW;
					visuals::SpriteGrid::SetCell(LAYOUT_NAME, SPRITE_GRID, { plotX, plotY }, descriptor.sprite, "White");
				}
			}
		}
	}

	static void RefreshBoard()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		RefreshActors();
	}

	static void RefreshBatteries()
	{
		for (const auto& actor : game::Actors::All())
		{
			auto descriptor = game::ActorTypes::Read(actor.actorType);
			if (descriptor.powerImage)
			{
				auto energy = actor.statistics.find(game::Statistic::ENERGY)->second;
				visuals::Images::SetSprite(
					LAYOUT_NAME,
					descriptor.powerImage.value(),
					(energy <= 0) ? ("Battery0") :
					(energy <= 25) ? ("Battery25") :
					(energy <= 50) ? ("Battery50") :
					(energy <= 75) ? ("Battery75") :
					("Battery100"));
			}
		}
	}

	static void Refresh()
	{
		RefreshBoard();
		RefreshMinimap();
		RefreshBatteries();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static std::function<void()> Move(const common::XY<int>& delta)
	{
		return [delta]() 
		{
			if (game::Actors::MoveActor(delta))
			{
				game::Actors::Next();
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				game::audio::Sfx::Play(game::audio::GameSfx::NO_CHARGE);
			}
		};
	}

	static void Rest()
	{
		game::Actors::Rest();
		game::Actors::Next();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<::Command, std::function<void()>> commandTable =
	{
		{::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY)},
		{::Command::UP, Move({0,-1})},
		{::Command::DOWN, Move({0,1})},
		{::Command::LEFT, Move({-1,0})},
		{::Command::RIGHT, Move({1,0})},
		{::Command::GREEN, Rest}
	};

	void Board::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Command::SetHandlers(CURRENT_STATE, commandTable);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}