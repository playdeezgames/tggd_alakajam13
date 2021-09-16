#include "Data.JSON.h"
#include <fstream>
namespace data
{
	nlohmann::json JSON::Load(const std::string& fileName)
	{
		nlohmann::json document;
		std::ifstream input(fileName);
		if (input.is_open())
		{
			input >> document;
			input.close();
		}
		return document;
	}

	void JSON::Save(const std::string& fileName, const nlohmann::json& document)
	{
		std::ofstream output(fileName);
		if (output.is_open())
		{
			output << document;
			output.close();
		}
	}
}

