#include "Common.XY.h"
#include "Harness.h"
namespace common
{
	static auto tests =
	{
		AddTest(
			"common::XY constructor should keep x and y values",
			[]()
			{
				const int expectedX = 1;
				const int expectedY = 2;
				XY<int> actual = {expectedX,expectedY};
				Assert(actual.GetX() == expectedX, "Actual should have expected x");
				Assert(actual.GetY() == expectedY, "Actual should have expected y");
			}),
		AddTest(
			"common::XY operator+ should add x and y values toether",
			[]()
			{
				XY<int> first = {1,2};
				XY<int> second = { 3,4 };
				auto actual = first + second;
				Assert(actual.GetX() == first.GetX() + second.GetX(), "Actual x should be the sum of the x values");
				Assert(actual.GetY() == first.GetY() + second.GetY(), "Actual y should be the sum of the y values");
			}),
		AddTest(
			"common::XY operator- should subtract x and y values",
			[]()
			{
				XY<int> first = {1,2};
				XY<int> second = { 3,5 };
				auto actual = first - second;
				Assert(actual.GetX() == first.GetX() - second.GetX(), "Actual x should be the difference of the x values");
				Assert(actual.GetY() == first.GetY() - second.GetY(), "Actual y should be the difference of the y values");
			}),
		AddTest(
			"common::XY operator== should return false when the values are not equal",
			[]()
			{
				XY<int> first = {1,2};
				XY<int> second = { 3,5 };
				auto actual = first == second;
				Assert(!actual, "Actual should be false when the values are not equal");
			}),
		AddTest(
			"common::XY operator== should return true when the values are equal",
			[]()
			{
				XY<int> first = {1,2};
				XY<int> second = { 1,2 };
				auto actual = first == second;
				Assert(actual, "Actual should be true when the values are equal");
			}),
		AddTest(
			"common::XY operator* should multiply values by a scalar",
			[]()
			{
				XY<int> original = {1,2};
				const int multiplier = 5;
				auto actual = original * multiplier;
				Assert(actual.GetX() == multiplier * original.GetX(), "X should be multiplied by scalar");
				Assert(actual.GetY() == multiplier * original.GetY(), "Y should be multiplied by scalar");
			}),
		AddTest(
			"common::XY operator/ should divide values by a scalar",
			[]()
			{
				XY<int> original = {10,20};
				const int divisor = 5;
				auto actual = original / divisor;
				Assert(actual.GetX() == original.GetX() / divisor, "X should be divided by scalar");
				Assert(actual.GetY() == original.GetY() / divisor, "Y should be divided by scalar");
			}),
		AddTest(
			"common::XY operator* (scalar first) should multiply values by a scalar",
			[]()
			{
				XY<int> original = {1,2};
				const int multiplier = 5;
				auto actual = multiplier * original;
				Assert(actual.GetX() == multiplier * original.GetX(), "X should be multiplied by scalar");
				Assert(actual.GetY() == multiplier * original.GetY(), "Y should be multiplied by scalar");
			}),
	};
}