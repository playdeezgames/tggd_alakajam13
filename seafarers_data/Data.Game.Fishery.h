#pragma once
#include <Common.XY.h>
#include <list>
namespace data::game
{
	struct Fishery
	{
		int fisheryId;
		int fishType;
		common::XY<double> location;
		common::XY<double> movement;
		double radius;

		static void Clear();
		static void Add(const Fishery&);
		static std::list<Fishery> All();
		static void Write(const Fishery&);
	};
}
