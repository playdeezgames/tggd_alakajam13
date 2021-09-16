#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct Avatar 
	{
		common::XY<double> location;
		double heading;
		double speed;
		int state;
		static void Write(const Avatar&);
		static std::optional<Avatar> Read();
		static void WriteState(int);
		static std::optional<int> ReadState();
	};
}
