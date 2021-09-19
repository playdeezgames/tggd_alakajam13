#include <Audio.Sfx.h>
#include <Game.Audio.Sfx.h>
#include <map>
namespace game::audio
{
	static const std::map<game::audio::GameSfx, std::string> table =
	{
		{ GameSfx::PIG_DIE , "deadmonster"},
		{ GameSfx::PIG_EAT , "eat"},
		{ GameSfx::GET_TURD , "eww"},
		{ GameSfx::THROW_FEED , "meh"},
		{ GameSfx::NO_CHARGE , "shucks"},
		{ GameSfx::GET_FEED , "tasty"},
		{ GameSfx::THROW_TURD , "woohoo"},
		{ GameSfx::GET_CHARGE , "yoink"}
	};

	void Sfx::Play(const game::audio::GameSfx& gameSfx)
	{
		auto iter = table.find(gameSfx);
		if (iter != table.end())
		{
			::audio::Sfx::Play(iter->second);
		}
	}
}