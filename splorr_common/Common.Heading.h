#pragma once
#include "Common.XY.h"
#include <string>
namespace common
{
	struct Heading
	{
		static common::XY<double> DegreesToXY(double);
		static double XYToDegrees(const common::XY<double>&);
		static double XYToRadians(const common::XY<double>&);
		static double ToDegrees(double);
		static double ToRadians(double);
		static const std::string& ToCompassPoint(double);
		static const double DEGREES;
		static double Distance(const common::XY<double>&, const common::XY<double>&);
		static common::XY<double> ClampDistance(const common::XY<double>&, double);
		static double Difference(double, double);
	};
}
