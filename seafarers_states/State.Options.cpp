#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Options.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Audio.h>
#include <Audio.Mux.h>
#include <Audio.Platform.h>
#include <Audio.Sfx.h>
#include <Application.Engine.h>
#include <Common.Data.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <format>
#include "States.h"
#include "UIState.h"
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Areas.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::OPTIONS;
	static const std::string LAYOUT_NAME = "State.Options";
	static const std::string MENU_ID = "Options";
	static const std::string MENU_ITEM_MUTE = "Mute";
	static const std::string MENU_ITEM_SFX_VOLUME = "SfxVolume";
	static const std::string MENU_ITEM_MUX_VOLUME = "MuxVolume";
	static const std::string MUTE = "Mute";
	static const std::string UNMUTE = "Unmute";
	static const std::string AREA_SFX_DECREASE = "DecreaseSfxVolume";
	static const std::string AREA_SFX_INCREASE = "IncreaseSfxVolume";
	static const std::string AREA_MUX_DECREASE = "DecreaseMuxVolume";
	static const std::string AREA_MUX_INCREASE = "IncreaseMuxVolume";
	static const int VOLUME_DELTA = 8;
	static const std::string FORMAT_SFX_VOLUMNE = "< SFX Volume ({}%) >";
	static const std::string FORMAT_MUX_VOLUMNE = "< MUX Volume ({}%) >";

	enum class OptionsItem
	{
		TOGGLE_MUTE,
		SFX_VOLUME,
		MUX_VOLUME,
		TOGGLE_FULLSCREEN,
		BACK
	};

	static const std::string SFX_SAMPLE = "woohoo";

	static void AdjustSfxVolume(int delta)
	{
		::audio::Sfx::SetVolume(::audio::Sfx::GetVolume() + delta);
		::audio::Sfx::Play(SFX_SAMPLE);
		application::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		::audio::Mux::SetVolume(::audio::Mux::GetVolume() + delta);
		application::Options::Save();
	}

	static OptionsItem GetCurrentItem()
	{
		return (OptionsItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value();
	}

	static const std::map<OptionsItem, std::function<void(int)>> itemChangers =
	{
		{ OptionsItem::SFX_VOLUME, AdjustSfxVolume },
		{ OptionsItem::MUX_VOLUME, AdjustMuxVolume }
	};

	static void ChangeItem(int delta)
	{
		auto iter = itemChangers.find(GetCurrentItem());
		if (iter != itemChangers.end())
		{
			iter->second(delta);
		}
	}

	static void RefreshMuteMenuItem()
	{
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUTE,
			Audio::IsMuted() ? UNMUTE : MUTE);
	}

	static void RefreshSfxMenuItem()
	{
		std::string ss = std::format(FORMAT_SFX_VOLUMNE, common::Data::ToPercentage(::audio::Sfx::GetVolume(), audio::Platform::VOLUME_MAXIMUM).value());
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_SFX_VOLUME, ss);
	}

	static void RefreshMuxMenuItem()
	{
		std::string ss = std::format(FORMAT_MUX_VOLUMNE, common::Data::ToPercentage(::audio::Mux::GetVolume(), audio::Platform::VOLUME_MAXIMUM).value());
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUX_VOLUME, ss);
	}

	static void Refresh()
	{
		RefreshMuteMenuItem();
		RefreshSfxMenuItem();
		RefreshMuxMenuItem();
	}

	static void IncreaseItem()
	{
		ChangeItem(VOLUME_DELTA);
		Refresh();
	}

	static void DecreaseItem()
	{
		ChangeItem(-VOLUME_DELTA);
		Refresh();
	}

	static void ToggleMute()
	{
		Audio::SetMuted(!Audio::IsMuted()); 
		application::Options::Save();
		Refresh();
	}

	static void ToggleFullscreen()
	{
		application::Engine::SetFullscreen(!application::Engine::IsFullscreen());
		application::Options::Save();
	}

	static const std::map<OptionsItem, std::function<void()>> activators =
	{
		{ OptionsItem::TOGGLE_MUTE, ToggleMute },
		{ OptionsItem::BACK, ::application::UIState::Pop },
		{ OptionsItem::TOGGLE_FULLSCREEN, ToggleFullscreen }

	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::LEFT, DecreaseItem },
		{::Command::RIGHT, IncreaseItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	static const std::map<std::string, std::function<void()>> areaClickActions =
	{
		{AREA_MUX_DECREASE, DecreaseItem},
		{AREA_SFX_DECREASE, DecreaseItem},
		{AREA_MUX_INCREASE, IncreaseItem},
		{AREA_SFX_INCREASE, IncreaseItem}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto iter = areaClickActions.find(area);
		if (iter != areaClickActions.end())
		{
			iter->second();
			return true;
		}
		ActivateItem();
		return true;
	}

	void Options::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, game::audio::Mux::GoToTheme(game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::OPTIONS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::OPTIONS, Refresh);
	}
}