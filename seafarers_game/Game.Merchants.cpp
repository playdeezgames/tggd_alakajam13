#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.XY.h>
#include <Data.Game.Island.h>
#include <Data.Game.Merchant.h>
#include "Game.Merchants.h"
#include "Game.Ships.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
namespace game
{
	static const std::map<Ship, size_t> shipTypeGenerator =
	{
		{ Ship::RAFT, 10},
		{ Ship::JUNK, 1}
	};

	static const std::map<Item, size_t> cargoTypeGenerator =
	{
		{ Item::SACK_OF_WHEAT, 3},
		{ Item::CLOTH, 1}
	};

	void Merchants::Spawn(const common::XY<double>& location)
	{
		auto islands = data::game::Island::All();
		size_t index = common::RNG::FromRange(0u, islands.size());
		auto island = common::Utility::GetNth(islands, index);
		auto item = common::RNG::FromGenerator(cargoTypeGenerator, Item::SACK_OF_WHEAT);
		auto itemDescriptor = Items::Read(item);
		auto ship = common::RNG::FromGenerator(shipTypeGenerator, Ship::RAFT);
		auto shipDescriptor = Ships::Read(ship);
		auto cargoQuantity = (size_t)common::RNG::FromRange(0.0, shipDescriptor.properties[game::ship::Property::TONNAGE] / itemDescriptor.tonnage);
		game::islands::Markets::BuyItems(location, item, cargoQuantity);
		data::game::Merchant::Add({ 
				0, //merchant id is auto assigned
				location, //start
				island.value().location, //destination
				(int)ship,
				(int)item,
				(int)cargoQuantity
			});
	}

	void Merchants::ApplyTurnEffects()
	{
		const double MAXIMUM_STEP = 1.0;
		auto merchants = data::game::Merchant::All();
		for (auto& merchant : merchants)
		{
			auto difference = merchant.destination - merchant.location;
			double magnitude = difference.GetMagnitude();
			if (magnitude > 0.0)
			{
				double stepSize = (magnitude > MAXIMUM_STEP) ? (MAXIMUM_STEP) : (magnitude);
				auto delta = difference * (stepSize / magnitude);
				merchant.location = merchant.location + delta;
				data::game::Merchant::Update(merchant);
			}
			else
			{
				game::islands::Markets::SellItems(merchant.location, (game::Item)merchant.cargoType, merchant.cargoQuantity);
				data::game::Merchant::Remove(merchant.merchantId);
			}
		}
	}

	void Merchants::Reset(const Difficulty&)
	{
		data::game::Merchant::Clear();
	}

	std::list<Merchant> Merchants::All()
	{
		std::list<Merchant> result;
		auto merchants = data::game::Merchant::All();
		for (auto& merchant : merchants)
		{
			result.push_back({
				merchant.location
				});
		}
		return result;
	}

}