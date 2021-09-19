#include "UIState.h"
#include "States.h"
#include <Game.Audio.Mux.h>
#include <Application.OnEnter.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Game.Actors.h>
#include <Game.ActorTypes.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_NEXT;

	static void OnEnter()
	{
		if (game::Actors::IsAnythingAlive())
		{
			auto actor = game::Actors::GetCurrent();
			auto descriptor = game::ActorTypes::Read(actor.actorType);
			while (!descriptor.playControlled)
			{
				game::Actors::Act();
				game::Actors::Next();
				actor = game::Actors::GetCurrent();
				descriptor = game::ActorTypes::Read(actor.actorType);
			}
			application::UIState::Write(::UIState::IN_PLAY_BOARD);
			return;
		}
		application::UIState::Write(::UIState::GAME_OVER);
	}

	static void OnUpdate(const unsigned int&)
	{
		OnEnter();
	}

	void Next::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Update::AddHandler(CURRENT_STATE, OnUpdate);
	}

}