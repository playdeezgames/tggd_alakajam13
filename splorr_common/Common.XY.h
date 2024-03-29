#pragma once
#include <cmath>
namespace common
{
	template<typename TDimension>
	class XY
	{
	private:
		TDimension x;
		TDimension y;
	public:
		XY() : x(), y() {}
		XY(const TDimension& x, const TDimension& y) : x(x), y(y) {}
		XY(const XY<TDimension>& other) : x(other.x), y(other.y) {}
		XY(XY<TDimension>&& other) noexcept : x(other.x), y(other.y) {}
		XY<TDimension>& operator=(XY<TDimension>&& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
		XY<TDimension>& operator=(const XY<TDimension>& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}
		const TDimension& GetX() const { return x; }
		const TDimension& GetY() const { return y; }
		double GetMagnitude() const
		{
			return sqrt(GetX() * GetX() + GetY() * GetY());
		}
		template<typename TOther>
		XY<TOther> Convert() const
		{
			return XY<TOther>((TOther)GetX(), (TOther)GetY());
		}
	};
	template<typename TDimension>
	XY<TDimension> operator+(const XY<TDimension>& first, const XY<TDimension>& second)
	{
		return XY<TDimension>(first.GetX() + second.GetX(), first.GetY() + second.GetY());
	}
	template<typename TDimension>
	XY<TDimension> operator-(const XY<TDimension>& first, const XY<TDimension>& second)
	{
		return XY<TDimension>(first.GetX() - second.GetX(), first.GetY() - second.GetY());
	}
	template<typename TDimension>
	bool operator==(const XY<TDimension>& first, const XY<TDimension>& second)
	{
		return first.GetX() == second.GetX() && first.GetY() == second.GetY();
	}
	template<typename TDimension>
	XY<TDimension> operator*(const XY<TDimension>& lhs, const TDimension& scalar)
	{
		return XY<TDimension>(lhs.GetX() * scalar, lhs.GetY() * scalar);
	}
	template<typename TDimension>
	XY<TDimension> operator/(const XY<TDimension>& lhs, const TDimension& scalar)
	{
		return XY<TDimension>(lhs.GetX() / scalar, lhs.GetY() / scalar);
	}
	template<typename TDimension>
	XY<TDimension> operator*(const TDimension& scalar, const XY<TDimension>& rhs)
	{
		return rhs * scalar;
	}
}
