#pragma once
#include <Common.XY.h>
#include <list>
namespace data::game
{
	struct Merchant
	{
		int merchantId;
		common::XY<double> location;
		common::XY<double> destination;
		int shipType;
		int cargoType;
		int cargoQuantity;

		static void Add(const Merchant&);
		static std::list<Merchant> All();
		static void Update(const Merchant&);
		static void Remove(int);
		static void Clear();
	};
}