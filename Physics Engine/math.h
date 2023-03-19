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

		float dot(Vector2<t> other) const;
	};

	template <typename t>
	struct Grid {
		Grid();
		Grid(float squareSize, Vector2<float> windowSize);

		float squareSize;
		Vector2<int> dimensions;
		void calculateDimensions(Vector2<float> windowSize);
		Vector2<int> getGridCellFromPoint(Vector2<float> point);
		void updateObject(t& object);
		void updateGrid(std::vector<t>& objects);

		std::set<t*>** gridArray;
	};
	static float PI = 3.141592653589793238;
}

#include "math.cpp"