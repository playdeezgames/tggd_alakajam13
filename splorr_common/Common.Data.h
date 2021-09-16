#pragma once
#include <sstream>
#include <string>
#include <optional>
namespace common
{
	struct Data
	{
		static double ToDouble(const std::string&);
		static int ToInt(const std::string&);

		static std::optional<int> ToOptionalInt(const std::string&);
		static std::optional<double> ToOptionalDouble(const std::string&);

		static std::optional<int> ToPercentage(int, int);
		static std::optional<double> ToPercentage(double, double);

		static double ClampDouble(double, double, double);
		static std::optional<double> ModuloDouble(double, double);

		static std::string QuoteString(const std::string&);
		template<typename TValue>
		static std::string OfOptional(const std::optional<TValue>& value)
		{
			std::stringstream ss;
			if (value.has_value())
			{
				ss << value.value();
			}
			else
			{
				ss << "NULL";
			}
			return ss.str();
		}
	};
}