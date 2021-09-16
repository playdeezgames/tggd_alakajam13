#pragma once
#include <Common.XY.h>
#include "Game.Avatar.Destination.h"
#include <list>
#include <optional>
#include <string>
namespace game::avatar
{
	struct Destinations
	{
		static const std::list<Destination>& All();

		static std::optional<common::XY<double>> GetDestination(const Destination&);
		static void SetDestination(const Destination&, const std::optional<common::XY<double>>&);

		static std::optional<std::string> GetDestinationName(const Destination&);
		static void SetDestinationName(const Destination&, const std::string&);
	};
}
