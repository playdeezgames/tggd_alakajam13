#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <string>

std::function<void(const std::string&)> Log;

class TestAssertion
{
private:
	std::string message;
public:
	TestAssertion(const std::string& message)
	{
		this->message = message;
	}
	const std::string& GetMessage() const
	{
		return message;
	}
};

void Assert(bool condition, const std::string& message)
{
	if (!condition)
	{
		throw TestAssertion(message);
	}
}

std::list<std::function<bool()>>* tests = nullptr;
std::function<bool()> AddTest(const std::string& functionName, std::function<void()> testFunction)
{
	if (!tests)
	{
		tests = new std::list<std::function<bool()>>();
	}
	tests->push_back(
		[functionName, testFunction]()
		{
			Log(std::format("Started Test: {}", functionName));
			try
			{
				testFunction();
				Log(std::format("Test Passed: {}", functionName));
				Log("");
				return true;
			}
			catch (const TestAssertion& assertion)
			{
				Log(std::format("!!!Assertion Failed: {}", assertion.GetMessage()));
				Log(std::format("Test Failed: {}", functionName));
				Log("");
				return false;
			}
			catch (...)
			{
				Log(std::format("!!!Exception thrown"));
				Log(std::format("Test Failed: {}", functionName));
				Log("");
				return false;
			}
		});
	return tests->back();
}

int main()
{
	std::ofstream output;
	output.open("results.txt");
	Log = [&output](const std::string& message) 
	{
		output << message << std::endl;
	};
	size_t successCount = 0;
	size_t failCount = 0;
	if (tests)
	{
		for (auto test : *tests)
		{
			if (test())
			{
				successCount++;
				continue;
			}
			failCount++;
		}
		delete tests;
		tests = nullptr;
	}
	output << std::format("Total Tests: {}", successCount + failCount) << std::endl;
	output << std::format("Total Passed: {}", successCount) << std::endl;
	output << std::format("Total Failed: {}", failCount) << std::endl;
	output.close();
}
