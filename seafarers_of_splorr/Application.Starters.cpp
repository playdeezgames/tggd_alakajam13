#include <Application.Keyboard.h>
#include <Application.Options.h>
#include <Application.UIState.h>
#include <Audio.Mux.h>
#include <Audio.Sfx.h>
#include <Data.JSON.Store.h>
#include <Data.JSON.Stores.h>
#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include <functional>
#include <Game.h>
#include <Game.Achievements.h>
#include <States.h>
#include <Sublayouts.h>
#include <UIState.h>
#include <vector>
#include <Visuals.Colors.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Fishboard.h>
#include <Visuals.Fonts.h>
#include <Visuals.Layouts.h>
#include <Visuals.Messages.h>
#include <Visuals.Navigator.h>
#include <Visuals.Sprites.h>
#include <Visuals.Textures.h>
#include <Visuals.WorldMap.h>
namespace application
{
	static const std::string FILE_CONFIG_COLORS = "config/graphics/colors.json";
	static const std::string FILE_CONFIG_SFX = "config/audio/sfx.json";
	static const std::string FILE_CONFIG_MUX = "config/audio/mux.json";
	static const std::string FILE_CONFIG_TEXTURES = "config/graphics/textures.json";
	static const std::string FILE_CONFIG_SPRITES = "config/graphics/sprites.json";
	static const std::string FILE_CONFIG_FONTS = "config/graphics/fonts.json";
	static const std::string FILE_CONFIG_LAYOUTS = "config/ui/layouts.json";
	static const std::string FILE_CONFIG_KEYBOARD = "config/keyboard.json";
	static const std::string FILE_CONFIG_OPTIONS = "config/options.json";
	static const std::string FILE_CONFIG_TIPS = "config/tips.json";
	static const std::string VISUAL_TYPE_WORLD_MAP = "WorldMap";
	static const std::string VISUAL_TYPE_NAVIGATOR = "Navigator";
	static const std::string VISUAL_TYPE_FISHBOARD = "Fishboard";
	static const std::string CONNECTION_MEMORY = ":memory:";
	static const std::string CONNECTION_AUTOSAVE = "autosave.db";
	static const std::string CONNECTION_SLOT_1 = "slot1.db";
	static const std::string CONNECTION_SLOT_2 = "slot2.db";
	static const std::string CONNECTION_SLOT_3 = "slot3.db";
	static const std::string CONNECTION_SLOT_4 = "slot4.db";
	static const std::string CONNECTION_SLOT_5 = "slot5.db";

	std::vector<std::function<void()>> starters =
	{
		application::UIState::DoSetFinalState(::UIState::QUIT),
		game::Achievements::Start,
		data::json::Stores::DoSetStoreFile(data::json::Store::COLORS, FILE_CONFIG_COLORS,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SOUND_EFFECTS, FILE_CONFIG_SFX,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::MUSIC_THEMES, FILE_CONFIG_MUX,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TEXTURES, FILE_CONFIG_TEXTURES,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::SPRITES, FILE_CONFIG_SPRITES,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::FONTS, FILE_CONFIG_FONTS,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::LAYOUTS, FILE_CONFIG_LAYOUTS,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::KEYS, FILE_CONFIG_KEYBOARD,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::OPTIONS, FILE_CONFIG_OPTIONS,std::nullopt),
		data::json::Stores::DoSetStoreFile(data::json::Store::TIPS, FILE_CONFIG_TIPS,std::nullopt),
		visuals::Colors::DoSetStore(data::json::Store::COLORS),
		visuals::Textures::DoSetStore(data::json::Store::TEXTURES),
		visuals::Sprites::DoSetStore(data::json::Store::SPRITES),
		visuals::Fonts::DoSetStore(data::json::Store::FONTS),
		visuals::Layouts::DoSetStore(data::json::Store::LAYOUTS),
		visuals::Layouts::DoRegisterType(VISUAL_TYPE_WORLD_MAP, visuals::WorldMap::Internalize),
		visuals::Layouts::DoRegisterType(VISUAL_TYPE_NAVIGATOR, visuals::Navigator::Internalize),
		visuals::Layouts::DoRegisterType(VISUAL_TYPE_FISHBOARD, visuals::Fishboard::Internalize),
		visuals::Messages::Reset,
		application::Keyboard::DoSetStore(data::json::Store::KEYS),
		Options::DoSetStore(data::json::Store::OPTIONS),
		visuals::Confirmations::Reset,
		data::json::Stores::Start,
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::IN_MEMORY, CONNECTION_MEMORY),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::AUTOSAVE, CONNECTION_AUTOSAVE),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_1, CONNECTION_SLOT_1),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_2, CONNECTION_SLOT_2),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_3, CONNECTION_SLOT_3),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_4, CONNECTION_SLOT_4),
		data::sqlite::Stores::DoSetConnection(data::sqlite::Store::SLOT_5, CONNECTION_SLOT_5),
		audio::Mux::DoSetStore(data::json::Store::MUSIC_THEMES),
		audio::Sfx::DoSetStore(data::json::Store::SOUND_EFFECTS),
		state::Splash::Start,
		state::Tip::Start,
		state::MainMenu::Start,
		state::About::Start,
		state::ConfirmQuit::Start,
		state::Options::Start,
		state::StartGame::Start,
		state::LeavePlay::Start,
		game::DoAddResetter(visuals::Confirmations::Reset),
		game::DoAddResetter(visuals::Messages::Reset),
		game::Start,
		state::Statistics::Start,
		state::LoadGame::Start,
		state::SaveGame::Start,
		state::ConfirmAbandon::Start,
		state::ConfirmOverwrite::Start,
		state::in_play::AtSea::Start,
		state::in_play::ChangeSpeed::Start,
		state::in_play::Docked::Start,
		state::in_play::Win::Start,
		state::in_play::Lose::Start,
		state::in_play::WorldMap::Start,
		state::in_play::IslandJobs::Start,
		state::in_play::ConfirmReplaceJob::Start,
		state::in_play::ConfirmAbandonJob::Start,
		state::in_play::CurrentJob::Start,
		state::in_play::IslandTrade::Start,
		state::in_play::IslandBuy::Start,
		state::in_play::IslandSell::Start,
		state::in_play::Cargo::Start,
		state::in_play::ShipStatus::Start,
		state::in_play::Next::Start,
		state::in_play::Shipyard::Start,
		state::in_play::DarkAlleyEntrance::Start,
		state::in_play::DarkAlley::Start,
		state::in_play::Message::Start,
		state::in_play::Confirm::Start,
		state::in_play::GambleStart::Start,
		state::in_play::GambleFinish::Start,
		state::in_play::AtSeaCareenSelect::Start,
		state::in_play::CareenedToPort::Start,
		state::in_play::CareenedToStarboard::Start,
		state::in_play::Temple::Start,
		state::in_play::MakeOffering::Start,
		state::in_play::AvatarStatus::Start,
		state::in_play::Fishing::Start,
		sublayout::UIHamburger::Start,
		sublayout::AtSeaActions::Start,
		Options::Initialize,
		visuals::Layouts::Start
	};

	const std::vector<std::function<void()>>& Engine::GetStarters()
	{
		return starters;
	}
}