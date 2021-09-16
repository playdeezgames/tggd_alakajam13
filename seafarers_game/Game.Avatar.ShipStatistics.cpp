#include <Common.Data.h>
#include <Data.Game.Avatar.ShipStatistic.h>
#include "Game.Avatar.Log.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.Colors.h"
#include "Game.ShipStatistic.h"
#include <map>
namespace game::avatar
{
	static double GetMaximumFouling()
	{
		double portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value().maximum.value();
		double starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value().maximum.value();
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFouling()
	{
		double portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value().current;
		double starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value().current;
		return portFouling + starboardFouling;
	}

	double ShipStatistics::GetFoulingPercentage(const game::Side& side)
	{
		double maximum = data::game::avatar::ShipStatistic::Read((int)side).value().maximum.value();
		double current = data::game::avatar::ShipStatistic::Read((int)side).value().current;
		return common::Data::ToPercentage(current, maximum).value();
	}


	double ShipStatistics::GetFoulingPercentage()
	{
		return common::Data::ToPercentage(GetFouling(), GetMaximumFouling()).value();
	}


	void ShipStatistics::IncreaseFouling(double multiplier)
	{
		auto portFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::PORT_FOULING).value();
		auto starboardFouling = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::STARBOARD_FOULING).value();
		auto foulingRate = data::game::avatar::ShipStatistic::Read((int)ShipStatistic::FOULING_RATE).value().current;

		portFouling.current += (foulingRate * multiplier);
		if (portFouling.maximum.has_value() && portFouling.current > portFouling.maximum.value())
		{
			portFouling.current = portFouling.maximum.value();
		}
		starboardFouling.current += (foulingRate * multiplier);
		if (starboardFouling.maximum.has_value() && starboardFouling.current > starboardFouling.maximum.value())
		{
			starboardFouling.current = starboardFouling.maximum.value();
		}
		data::game::avatar::ShipStatistic::Write((int)ShipStatistic::PORT_FOULING, portFouling);
		data::game::avatar::ShipStatistic::Write((int)ShipStatistic::STARBOARD_FOULING, starboardFouling);
	}

	struct SideDescriptor
	{
		ShipStatistic statistic;
		std::string cleaningMessage;
	};

	static const std::map<game::Side, SideDescriptor> foulingTable =
	{
		{ Side::PORT, {ShipStatistic::PORT_FOULING, "You clean the port hull."}},
		{ Side::STARBOARD, {ShipStatistic::STARBOARD_FOULING, "You clean the starboard hull."}},
	};

	void ShipStatistics::CleanHull(const Side& side)
	{
		auto descriptor = foulingTable.find(side)->second;
		auto fouling = data::game::avatar::ShipStatistic::Read((int)descriptor.statistic).value();
		fouling.current = fouling.minimum.value_or(0.0);
		data::game::avatar::ShipStatistic::Write((int)descriptor.statistic, fouling);
		Log::Write({Colors::GRAY, descriptor.cleaningMessage});
	}
}