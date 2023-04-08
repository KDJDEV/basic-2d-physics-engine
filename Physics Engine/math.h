#pragma once
#include <set>
#include <vector>

namespace math {
	template <typename t> struct Vector2 {
		Vector2();
		Vector2(t x, t y);

		t x;
		t y;
		void print();

		Vector2 operator-(const Vector2 other) const;
		Vector2 operator+(const Vector2 other) const;
		Vector2 operator*(const float scalar) const;
		Vector2 operator/(const float scalar) const;

		float mag() const;
		float dot(Vector2<t> other) const;
	};

	static float PI = 3.141592653589793238;
}

#include "math.cpp"