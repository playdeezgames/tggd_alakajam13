#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.h>
#include "Game.h"
#include "Game.Avatar.AtSea.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Plights.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.Avatar.State.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include "Game.Ships.h"
#include "Game.World.h"
namespace game::avatar
{
	void AtSea::Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar data =
		{
			{worldSize.GetX() / 2.0, worldSize.GetY() / 2.0},
			common::Data::ModuloDouble(common::RNG::FromRange(0.0, common::Heading::DEGREES), common::Heading::DEGREES).value(),
			1.0
		};
		data::game::Avatar::Write(data);
		data::game::Avatar::WriteState((int)game::avatar::State::AT_SEA);
	}

	double AtSea::GetHeading()
	{
		return data::game::Avatar::Read().value().heading;
	}

	void AtSea::SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
		data::game::Avatar::Write(data);
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	common::XY<double> AtSea::GetLocation()
	{
		return data::game::Avatar::Read().value().location;
	}

	double AtSea::GetSpeed()
	{
		return data::game::Avatar::Read().value().speed;
	}

	void AtSea::SetSpeed(double speed)
	{
		auto data = data::game::Avatar::Read().value();
		data.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	static void ApplyHunger()
	{
		double delta = -1.0;
		if (Plights::Has(Plight::DOUBLE_HUNGER) && !Plights::Has(Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (Plights::Has(Plight::HUNGER_IMMUNITY) && !Plights::Has(Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		if (!game::avatar::Statistics::IsStarving())
		{
			game::avatar::Statistics::ChangeSatiety(delta);
		}
		else
		{
			game::avatar::Statistics::ChangeHealth(delta);
		}
	}

	static void ApplyEating()
	{
		const double EAT_BENEFIT = 10.0;
		if (game::avatar::Statistics::NeedToEat(EAT_BENEFIT))
		{
			auto rations = game::avatar::Items::Read(game::Item::RATIONS);
			if (rations > 0)
			{
				game::avatar::Statistics::Eat(EAT_BENEFIT);
				game::avatar::Items::Remove(game::Item::RATIONS, 1);
			}
		}
	}

	static void ApplyWindChange()
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		game::World::SetWindHeading(game::World::GetWindHeading()+common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}

	static void ApplyTurn()
	{
		if (Plights::Has(Plight::DOUBLE_AGING) && !Plights::Has(Plight::AGING_IMMUNITY))
		{
			game::avatar::Statistics::SpendTurn();
			game::avatar::Statistics::SpendTurn();
			return;
		}
		if (Plights::Has(Plight::AGING_IMMUNITY) && !Plights::Has(Plight::DOUBLE_AGING))
		{
			//do nothing!
			return;
		}
		game::avatar::Statistics::SpendTurn();
	}

	void AtSea::ApplyTurnEffects()
	{
		ApplyTurn();
		ApplyHunger();
		ApplyEating();
		ApplyWindChange();
	}

	static common::XY<double> ClampAvatarLocation(const common::XY<double>& candidate, AtSea::MoveResult& result)
	{
		auto nextLocation = candidate;
		auto worldSize = game::World::GetSize();
		if (nextLocation.GetX() < 0.0)
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { 0, nextLocation.GetY() };
		}
		if (nextLocation.GetX() > worldSize.GetX())
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { worldSize.GetX(), nextLocation.GetY() };
		}
		if (nextLocation.GetY() < 0.0)
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), 0.0 };
		}
		if (nextLocation.GetY() > worldSize.GetY())
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), worldSize.GetY() };
		}
		return nextLocation;
	}

	AtSea::MoveResult AtSea::Move()
	{
		MoveResult result = MoveResult::MOVED;
		auto avatar = data::game::Avatar::Read().value();

		auto fouling = ShipStatistics::GetFouling();
		ShipStatistics::IncreaseFouling(avatar.speed);
		auto effectiveSpeed = avatar.speed * (1.0 - fouling);
		auto relativeHeading = common::Heading::Difference(game::World::GetWindHeading(), avatar.heading);
		double multiplier = 1.0 - std::abs(relativeHeading / common::Heading::DEGREES);

		auto shipDescriptor = game::Ships::Read(game::avatar::Ship::Read());
		common::XY<double> delta =
			common::Heading::DegreesToXY(avatar.heading) *
			effectiveSpeed * multiplier *
			shipDescriptor.properties.find(game::ship::Property::SPEED_FACTOR)->second;

		avatar.location = ClampAvatarLocation(avatar.location + delta, result);

		game::ApplyTurnEffects();
		data::game::Avatar::Write(avatar);
		return result;
	}
}