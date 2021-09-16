#include <Audio.Sfx.h>
#include <Game.Audio.Sfx.h>
#include <map>
namespace game::audio
{
	static const std::map<game::audio::GameSfx, std::string> table =
	{
		{ GameSfx::DEATH, "death" },
		{ GameSfx::ENEMY_HIT, "enemyhit" },
		{ GameSfx::HIT, "hit" }
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