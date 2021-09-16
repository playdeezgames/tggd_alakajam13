#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
namespace data::game
{
	struct FishBoard
	{
		common::XY<int> location;
		bool revealed;
		std::optional<int> fishType;

		static void Clear();
		static void Write(const FishBoard&);
		static std::list<FishBoard> All();
	};
}
