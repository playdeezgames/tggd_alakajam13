#pragma once
#include "Game.Difficulty.h"
#include <functional>
namespace game
{
	void AddResetter(std::function<void()>);
	std::function<void()> DoAddResetter(std::function<void()>);
	void Start();
	void Reset(const Difficulty&);
}
