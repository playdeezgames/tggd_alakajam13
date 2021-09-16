#include <Data.Game.World.h>
#include <Data.SQLite.Store.h>
#include <Data.SQLite.Stores.h>
#include "Game.h"
#include "Game.World.h"
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
		World::Reset(difficulty);
		for (auto& resetter : resetters)
		{
			resetter();
		}
	}

	void Start()
	{
		Reset(Difficulty::NORMAL);
	}

	void AutoSave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, data::sqlite::Store::AUTOSAVE);
	}

	void LoadFromAutosave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::AUTOSAVE, data::sqlite::Store::IN_MEMORY);
	}

	static bool DoesWorldExistInStore(const data::sqlite::Store& store)
	{
		return !data::sqlite::Stores::Execute(store, "SELECT name FROM sqlite_master WHERE type='table' AND name='Worlds';").empty();
	}

	bool DoesAutosaveExist()
	{
		return DoesWorldExistInStore(data::sqlite::Store::AUTOSAVE);
	}

	const std::map<int, data::sqlite::Store> slotTable =
	{
		{1, data::sqlite::Store::SLOT_1},
		{2, data::sqlite::Store::SLOT_2},
		{3, data::sqlite::Store::SLOT_3},
		{4, data::sqlite::Store::SLOT_4},
		{5, data::sqlite::Store::SLOT_5}
	};

	bool DoesSlotExist(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			return DoesWorldExistInStore(iter->second);
		}
		return false;
	}

	void LoadFromSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(iter->second, data::sqlite::Store::IN_MEMORY);
		}
	}

	void SaveToSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, iter->second);
		}
	}

	void ApplyTurnEffects()
	{
	}
}