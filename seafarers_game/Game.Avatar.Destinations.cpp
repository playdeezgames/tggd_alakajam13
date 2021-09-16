#include <Data.Game.Avatar.Destination.h>
#include <Data.Game.Avatar.DestinationName.h>
#include "Game.Avatar.Destinations.h"
namespace game::avatar
{
	std::optional<common::XY<double>> Destinations::GetDestination(const game::avatar::Destination& index)
	{
		return data::game::avatar::Destination::Read((int)index);
	}

	void Destinations::SetDestination(const game::avatar::Destination& index, const std::optional<common::XY<double>>& destination)
	{
		if (destination)
		{
			data::game::avatar::Destination::Write((int)index, destination.value());
			return;
		}
		data::game::avatar::Destination::Clear((int)index);
	}

	static std::list<game::avatar::Destination> allDestinations = 
	{
		game::avatar::Destination::ONE,
		game::avatar::Destination::TWO,
		game::avatar::Destination::THREE,
		game::avatar::Destination::FOUR
	};

	const std::list<game::avatar::Destination>& Destinations::All()
	{
		return allDestinations;
	}

	std::optional<std::string> Destinations::GetDestinationName(const Destination& destination)
	{
		return data::game::avatar::DestinationName::Read((int)destination);
	}

	void Destinations::SetDestinationName(const Destination& destination, const std::string& name)
	{
		data::game::avatar::DestinationName::Write((int)destination, name);
	}
}