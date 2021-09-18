#include <Data.Game.World.h>
#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.h"
#include <list>
#include <map>
namespace game
{
	static std::list<std::function<void()>> resetters;
	void AddResetter(std::function<void()> resetter)
	{
		resetters.push_back(resetter);
	}

	std::function<void()> DoAddResetter(std::function<void()> resetter)
	{
		return [resetter]()
		{
			AddResetter(resetter);
		};
	}

	void Reset(const Difficulty& difficulty)
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);
		for (auto& resetter : resetters)
		{
			resetter();
		}
	}

	void Start()
	{
		Reset(Difficulty::NORMAL);
	}

	void ApplyTurnEffects()
	{
	}
}