#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MESSAGE;
	static const std::string LAYOUT_NAME = "State.InPlay.Message";
	static const std::string SPRITE_GRID = "Grid";
	static const std::string TEXT_CAPTION = "Caption";
	static const std::string FONT_DEFAULT = "default";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		if (!visuals::Messages::HasMessage())
		{
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		auto message = visuals::Messages::Read();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CAPTION, message.caption);
		for (auto& detail : message.details)
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID, detail.position, FONT_DEFAULT, detail.text, detail.color, detail.alignment);
		}
	}

	void Message::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, OnMouseButtonUp);
		::application::Command::SetHandler(CURRENT_STATE, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT));
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
