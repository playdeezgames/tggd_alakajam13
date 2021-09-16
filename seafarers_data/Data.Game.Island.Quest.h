#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game::island
{
	struct Quest
	{
		common::XY<double> location;
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;
		static void Write(const Quest&);
		static std::optional<Quest> Read(const common::XY<double>&);
		static void Clear(const common::XY<double>&);
	};

}
