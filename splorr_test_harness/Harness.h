#pragma once
#include <string>
#include <functional>
void Assert(bool, const std::string&);
std::function<bool()> AddTest(const std::string& functionName, std::function<void()> testFunction);
