#include <Common.RNG.h>
#include <Data.Game.Avatar.DemigodFavor.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.DemigodItem.h>
#include "Game.Avatar.Plights.h"
#include "Game.Items.h"
#include "Game.Demigods.h"
#include <map>
#include <set>
#include <string>
namespace game
{
	static const std::map<std::string, size_t> consonants =
	{
		{"CH",1},
		{"TH",1},
		{"ZH",1},
		{"SH",1},
		{"PH",1},
		{"L",1},
		{"R",1},
		{"X",1},
		{"Q",1},
		{"Z",1}
	};

	static const std::map<std::string, size_t> vowels =
	{
		{"'",10},
		{"A",10},
		{"AA",1},
		{"E",10},
		{"I",10},
		{"II",1},
		{"O",10},
		{"U",10},
		{"UU",1},
		{"Y",10},
		{"YY",1},
	};

	static const std::map<size_t, size_t> nameLengths = 
	{
		{5,1},
		{7,2},
		{9,1}
	};

	static std::string GenerateName()
	{
		size_t nameLength = common::RNG::FromGenerator(nameLengths,(size_t)0);
		bool vowel = false;
		std::string result = "";
		while (nameLength > 0)
		{
			if (vowel)
			{
				result = result + common::RNG::FromGenerator(vowels, std::string(""));
			}
			else
			{
				result = result + common::RNG::FromGenerator(consonants, std::string(""));
			}
			vowel = !vowel;
			nameLength--;
		}
		return result;
	}

	static std::set<std::string> GenerateNames(size_t nameCount)
	{
		std::set<std::string> result;
		while (result.size() < nameCount)
		{
			result.insert(GenerateName());
		}
		return result;
	}

	static const std::map<int, size_t> demigodCounts =
	{
		{4,1},
		{5,4},
		{6,10},
		{7,16},
		{8,19},
		{9,16},
		{10,10},
		{11,4},
		{12,1}
	};

	static const std::map<size_t, size_t> patronWeights =
	{
		{1,1},
		{2,1},
		{3,1},
		{4,1},
		{5,1},
		{6,1},
		{7,1},
		{8,1},
		{9,1},
		{10,1}
	};

	void Demigods::Reset(const Difficulty&)
	{
		const double BLESSING_THRESHOLD = 5.0;
		const double BLESSING_MULTIPLIER = 2.0;
		const double CURSE_THRESHOLD = -5.0;
		const double CURSE_MULTIPLIER = 0.5;
		const double OFFERING_FAVOR_MINIMUM = -1.0;
		const double OFFERING_FAVOR_MAXIMUM = 1.0;
		data::game::Demigod::Clear();
		data::game::DemigodItem::Clear();
		data::game::avatar::DemigodFavor::Clear();
		auto demigodCount = common::RNG::FromGenerator(demigodCounts, 0);
		auto names = GenerateNames(demigodCount);
		auto items = Items::All();
		for (auto name : names)
		{
			data::game::Demigod demigod ={ 
				name,
				common::RNG::FromGenerator(patronWeights, (size_t)0),
				BLESSING_THRESHOLD,
				BLESSING_MULTIPLIER,
				(int)avatar::Plights::Generate(avatar::PlightType::BLESSING),
				CURSE_THRESHOLD,
				CURSE_MULTIPLIER,
				(int)avatar::Plights::Generate(avatar::PlightType::CURSE),
				(size_t)0};
			data::game::Demigod::Write(demigod);
			for (auto item : items)
			{
				data::game::DemigodItem::Write(name, (int)item, common::RNG::FromRange(OFFERING_FAVOR_MINIMUM, OFFERING_FAVOR_MAXIMUM));
			}
		}
	}

	static bool ApplyBlessing(data::game::Demigod& demigod, const Item& item, double favor)
	{
		if (favor >= demigod.blessingThreshold)
		{
			game::avatar::Plights::Inflict((game::avatar::Plight)demigod.blessingPlightId);
			favor -= demigod.blessingThreshold;
			data::game::avatar::DemigodFavor::Write(demigod.name, favor);
			demigod.blessingThreshold *= demigod.blessingMultiplier;
			data::game::Demigod::Write(demigod);
			return true;
		}
		return false;
	}

	static bool ApplyCurse(data::game::Demigod& demigod, const Item& item, double favor)
	{
		if (favor <= demigod.curseThreshold)
		{
			game::avatar::Plights::Inflict((game::avatar::Plight)demigod.cursePlightId);
			favor -= demigod.curseThreshold;
			data::game::avatar::DemigodFavor::Write(demigod.name, favor);
			demigod.curseThreshold *= demigod.curseMultiplier;
			data::game::Demigod::Write(demigod);
			return true;
		}
		return false;
	}

	static OfferingResult ApplyFavor(data::game::Demigod& demigod, const Item& item, double delta)
	{
		auto favor = data::game::avatar::DemigodFavor::Read(demigod.name).value_or(0.0);
		favor += delta;
		data::game::avatar::DemigodFavor::Write(demigod.name, favor);
		if (ApplyBlessing(demigod, item, favor))
		{
			return OfferingResult::BLESSING;
		}
		if (ApplyCurse(demigod, item, favor))
		{
			return OfferingResult::CURSE;
		}
		return OfferingResult::SUCCESS;
	}

	static void UpdateCooldown(data::game::Demigod& demigod)
	{
		demigod.offeringCooldown++;
		data::game::Demigod::Write(demigod);
	}

	static OfferingResult DoMakeOffering(data::game::Demigod& demigod, const Item& item)
	{
		auto delta = data::game::DemigodItem::Read(demigod.name, (int)item);
		if (delta)
		{
			UpdateCooldown(demigod);
			return ApplyFavor(demigod, item, delta.value());
		}
		return OfferingResult::SUCCESS;
	}

	static OfferingResult CheckOfferingCooldown(data::game::Demigod& demigod, const Item& item)
	{
		if (demigod.offeringCooldown > 0)
		{
			return OfferingResult::COOLING_DOWN;
		}
		return DoMakeOffering(demigod, item);
	}

	OfferingResult Demigods::MakeOffering(const std::string& name, const Item& item)
	{
		auto demigod = data::game::Demigod::Read(name);
		if(demigod)
		{
			return CheckOfferingCooldown(demigod.value(), item);
		}
		return OfferingResult::FAILURE;
	}

	void Demigods::ApplyTurnEffects()
	{
		auto demigods = data::game::Demigod::All();
		for (auto& demigod : demigods)
		{
			if (demigod.offeringCooldown > 0)
			{
				demigod.offeringCooldown--;
				data::game::Demigod::Write(demigod);
			}
		}
	}

}