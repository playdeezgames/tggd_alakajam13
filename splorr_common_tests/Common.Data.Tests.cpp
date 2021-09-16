#include "Common.Data.h"
#include "Harness.h"
namespace common
{
	static auto tests =
	{
		AddTest(
			"common::Data::ToDouble should return 0.0 when given garbage",
			[]()
			{
				auto actual = Data::ToDouble("GARBAGE");
				Assert(actual == 0.0, "Actual should be zero");
			}),
		AddTest(
			"common::Data::ToDouble should parse a valid int",
			[]()
			{
				auto actual = Data::ToDouble("1");
				Assert(actual == 1.0, "Actual should be 1");
			}),
		AddTest(
			"common::Data::ToDouble should parse a valid double",
			[]()
			{
				auto actual = Data::ToDouble("1.2");
				Assert(actual == 1.2, "Actual should be 1.2");
			}),
		AddTest(
			"common::Data::ToInt should return 0 when given garbage",
			[]()
			{
				auto actual = Data::ToInt("GARBAGE");
				Assert(actual == 0, "Actual should be zero");
			}),
		AddTest(
			"common::Data::ToInt should parse a valid int",
			[]()
			{
				auto actual = Data::ToInt("1");
				Assert(actual == 1, "Actual should be 1");
			}),
		AddTest(
			"common::Data::ToOptionalInt should return nullopt when given empty string",
			[]()
			{
				auto actual = Data::ToOptionalInt("");
				Assert(!actual.has_value(), "Actual should should not have a value");
			}),
		AddTest(
			"common::Data::ToOptionalInt should parse a valid int",
			[]()
			{
				auto actual = Data::ToOptionalInt("1");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value()==1, "Actual should have a value of 1");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should return nullopt when given empty string",
			[]()
			{
				auto actual = Data::ToOptionalDouble("");
				Assert(!actual.has_value(), "Actual should should not have a value");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should parse a valid int",
			[]()
			{
				auto actual = Data::ToOptionalDouble("1");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value() == 1.0, "Actual should have a value of 1.0");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should parse a valid double",
			[]()
			{
				auto actual = Data::ToOptionalDouble("1.2");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value() == 1.2, "Actual should have a value of 1.2");
			}),
		AddTest(
			"common::Data::ToPercentage should return nullopt when given a 0 for maximum",
			[]()
			{
				auto actual = Data::ToPercentage(0,0);
				Assert(!actual.has_value(), "Actual not have a value");
			}),
		AddTest(
			"common::Data::ToPercentage should return percentage when given a nonzero for maximum",
			[]()
			{
				auto actual = Data::ToPercentage(2,10);
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value()==20, "Actual should be the correct percentage");
			}),
		AddTest(
			"common::Data::ClampDouble should return minimum when value is less than minimum",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 5.0;
				auto actual = Data::ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == minimum, "Actual should be minimum when less than minimum is passed");
			}),
		AddTest(
			"common::Data::ClampDouble should return maximum when value is greater than minimum",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 25.0;
				auto actual = Data::ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == maximum, "Actual should be maximum when higher than maximum is passed");
			}),
		AddTest(
			"common::Data::ClampDouble should return value when value is in range",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 15.0;
				auto actual = Data::ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == value, "Actual should be value when value is in range");
			}),
		AddTest(
			"common::Data::ModuloDouble should return nullopt when divisor is 0.0",
			[]()
			{
				auto actual = Data::ModuloDouble(0.0, 0.0);
				Assert(!actual.has_value(), "Actual should not have a value when divisor is zero");
			}),
		AddTest(
			"common::Data::ModuloDouble should return value when provided value is between 0 and divisor",
			[]()
			{
				const double expected = 0.5;
				const double divisor = 1.0;
				auto actual = Data::ModuloDouble(expected, divisor);
				Assert(actual.has_value(), "Actual should have a value when divisor is nonzero");
				Assert(actual.value() >= 0.0, "Actual should have a positive value");
				Assert(actual.value() < divisor, "Actual should have a value less than divisor");
				Assert(actual.value() == expected, "Actual should have provided value when in range");
			}),
		AddTest(
			"common::Data::ModuloDouble should return positive value less than divisor when provided value is negative",
			[]()
			{
				const double expected = 0.5;
				const double divisor = 1.0;
				auto actual = Data::ModuloDouble(-0.5, divisor);
				Assert(actual.has_value(), "Actual should have a value when divisor is nonzero");
				Assert(actual.value() >= 0.0, "Actual should have a positive value");
				Assert(actual.value() < divisor, "Actual should have a value less than divisor");
				Assert(actual.value() == expected, "Actual should have provided value when in range");
			}),
		AddTest(
			"common::Data::ModuloDouble should return positive value less than divisor when provided value is greater than divisor",
			[]()
			{
				const double expected = 0.5;
				const double divisor = 1.0;
				auto actual = Data::ModuloDouble(1.5, divisor);
				Assert(actual.has_value(), "Actual should have a value when divisor is nonzero");
				Assert(actual.value() >= 0.0, "Actual should have a positive value");
				Assert(actual.value() < divisor, "Actual should have a value less than divisor");
				Assert(actual.value() == expected, "Actual should have provided value when in range");
			}),
		AddTest(
			"common::Data::QuoteString should put single quotes around a string",
			[]()
			{
				const std::string expected = "'TEST'";
				auto actual = Data::QuoteString("TEST");
				Assert(actual==expected, "Actual should have single quotes around it");
			}),
		AddTest(
			"common::Data::QuoteString should put double quotes existing within input string",
			[]()
			{
				const std::string expected = "'TE''ST'";
				auto actual = Data::QuoteString("TE'ST");
				Assert(actual == expected, "Actual should double existing quotes");
			}),
		AddTest(
			"common::Data::OfOptional should return NULL when passed nullopt",
			[]()
			{
				const std::string expected = "NULL";
				auto actual = Data::OfOptional<int>(std::nullopt);
				Assert(actual == expected, "Actual should return \"NULL\"");
			}),
		AddTest(
			"common::Data::OfOptional should return string equivalent value when passed an int",
			[]()
			{
				const std::string expected = "1";
				auto actual = Data::OfOptional<int>(1);
				Assert(actual == expected, "Actual should return \"1\"");
			})
	};
}