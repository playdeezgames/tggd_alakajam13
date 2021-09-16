#pragma once
#include <string>
namespace game
{
	struct CommodityDescriptor
	{
		std::string name;
		double basePrice;
		double supplyFactor;
		double demandFactor;
		double discount;
	};
}