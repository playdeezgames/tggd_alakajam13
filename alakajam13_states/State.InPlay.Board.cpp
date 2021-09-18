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
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_BOARD;
	static const std::string LAYOUT_NAME = "State.InPlay.Board";
	static const std::string SPRITE_GRID = "Grid";
	static const int BOARD_COLUMNS = 9;
	static const int BOARD_ROWS = 9;
	static const int BOARD_OFFSET_COLUMN = -4;
	static const int BOARD_OFFSET_ROW = -4;

	static void RefreshActors()
	{
		auto currentActor = game::Actors::GetCurrent();
		int minimumColumn = currentActor.location.GetX() + BOARD_OFFSET_COLUMN;
		int minimumRow = currentActor.location.GetY() + BOARD_OFFSET_ROW;
		int maximumColumn = minimumColumn + BOARD_COLUMNS;
		int maximumRow = minimumRow + BOARD_ROWS;
		for (auto actor : game::Actors::All())
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

	static void RefreshBoard()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		RefreshActors();
	}

	static void Refresh()
	{
		RefreshBoard();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandTable =
	{
		{::Command::BACK, application::UIState::GoTo(::UIState::LEAVE_PLAY)}
	};

	void Board::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Command::SetHandlers(CURRENT_STATE, commandTable);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}