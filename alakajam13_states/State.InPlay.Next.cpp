#include "UIState.h"
#include "States.h"
#include <Game.Audio.Mux.h>
#include <Application.OnEnter.h>
#include <Application.Update.h>
#include <Application.UIState.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_NEXT;

	static void OnEnter()
	{
		application::UIState::Write(::UIState::IN_PLAY_BOARD);
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