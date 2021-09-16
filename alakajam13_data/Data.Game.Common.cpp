#include "Data.Game.Common.h"
#include "Data.SQLite.Store.h"
#include "Data.SQLite.Stores.h"
#include <sstream>
namespace data::game
{
	std::list<std::map<std::string, std::string>> Common::Execute(const std::string& query)
	{
		return data::sqlite::Stores::Execute(data::sqlite::Store::IN_MEMORY, query);
	}

	std::function<void()> Common::Run(const std::string& query)
	{
		return [query]()
		{
			Execute(query);
		};
	}

}