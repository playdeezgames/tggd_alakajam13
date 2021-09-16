#include "Data.JSON.h"
#include <filesystem>
#include <fstream>
#include "Harness.h"
namespace common
{
	static void RemoveExistingFile(const std::string& fileName)
	{
		std::filesystem::remove(fileName);
	}

	static void SetupExistingFile(const std::string& fileName, const std::string& fileContents)
	{
		RemoveExistingFile(fileName);
		std::ofstream ofs(fileName, std::ofstream::out);
		ofs << fileContents;
		ofs.close();
	}

	static auto tests =
	{
		AddTest(
			"data::JSON::Load should load a json file",
			[]()
			{
				const std::string fileName = ".\\loadTest.json";
				const std::string fileContents = "{\"x\":1}";
				SetupExistingFile(fileName, fileContents);

				auto json = data::JSON::Load(fileName);
				Assert(json.is_object(), "loaded json should be an object");
			}),
		AddTest(
			"data::JSON::Save should save a json file",
			[]() {
				const std::string fileName = ".\\saveTest.json";
				RemoveExistingFile(fileName);

				nlohmann::json json;
				json["x"] = 1;
				data::JSON::Save(fileName, json);
				Assert(std::filesystem::exists(fileName),"saved json file should exist");
			}),
	};
}
