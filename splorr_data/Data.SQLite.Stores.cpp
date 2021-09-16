#include "Data.SQLite.Stores.h"
#include <memory>
#include "sqlite3.h"
namespace data::sqlite
{
	static std::map<int, std::shared_ptr<sqlite3>> connections;
	static std::map<int, std::string> connectionStrings;
	void Stores::SetConnection(int index, const std::string& connectionString)
	{
		connectionStrings[index] = connectionString;
	}


	static std::shared_ptr<sqlite3> GetConnection(int store)
	{
		auto iter = connections.find(store);
		if (iter == connections.end())
		{
			sqlite3* db = nullptr;
			sqlite3_open(connectionStrings.find(store)->second.c_str(), &db);
			auto ptr = std::shared_ptr<sqlite3>(db, sqlite3_close);
			connections[store] = ptr;
			return ptr;
		}
		return iter->second;
	}

	static int ExecutionCallback(void* data, int column_count, char** column_values, char** column_names)
	{
		std::list<std::map<std::string, std::string>>* results = (std::list<std::map<std::string, std::string>>*)data;
		std::map<std::string, std::string> record;
		for (int column = 0; column < column_count; ++column)
		{
			record[column_names[column]] = column_values[column] ? column_values[column] : "";
		}
		results->push_back(record);
		return 0;
	}

	static std::list<std::map<std::string, std::string>> DoExecute(const std::shared_ptr<sqlite3>& connection, const std::string& query)
	{
		std::list<std::map<std::string, std::string>> results;
		char* errmsg = nullptr;
		sqlite3_exec(connection.get(), query.c_str(), ExecutionCallback, &results, &errmsg);
		if (errmsg)
		{
			std::string errorMessage = errmsg;
			sqlite3_free(errmsg);
			throw errorMessage;
		}
		return results;
	}

	std::list<std::map<std::string, std::string>> Stores::Execute(int store, const std::string& query)
	{
		auto connection = GetConnection(store);
		if (connection)
		{
			return DoExecute(connection, query);
		}
		return std::list<std::map<std::string, std::string>>();
	}

	void Stores::Copy(int storeFrom, int storeTo)
	{
		auto connectionFrom = GetConnection(storeFrom);
		auto connectionTo = GetConnection(storeTo);
		auto handle = sqlite3_backup_init(connectionTo.get(), "main", connectionFrom.get(), "main");
		sqlite3_backup_step(handle, -1);
		sqlite3_backup_finish(handle);
	}

	void Stores::Bounce(int store)
	{
		connections.erase(store);
	}
}