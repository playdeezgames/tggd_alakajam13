#include "Visuals.Area.h"
namespace visuals
{
	bool Area::Contains(const common::XY<int>& location) const
	{
		auto difference = location - xy;
		return(difference.GetX() >= 0 && difference.GetY() >= 0 && difference.GetX() < size.GetX() && difference.GetY() < size.GetY());
	}
}