#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "math.h"

using namespace math;

template <typename t>
Vector2<t>::Vector2() {
	x = 0;
	y = 0;
}
template <typename t>
Vector2<t>::Vector2(t x, t y) : x{ x }, y{ y } {}

template <typename t>
void Vector2<t>::print() {
	std::cout << "[ " << x << ", " << y << " ]" << '\n';
}

template <typename t>
Vector2<t> Vector2<t>::operator-(const Vector2 other) const {
	return Vector2{ x - other.x, y - other.y };
}
template <typename t>
Vector2<t> Vector2<t>::operator+(const Vector2 other) const {
	return Vector2{ x + other.x, y + other.y };
}
template <typename t>
Vector2<t> Vector2<t>::operator*(const float scalar) const {
	return Vector2{ x * scalar, y * scalar };
}
template <typename t>
Vector2<t> Vector2<t>::operator/(const float scalar) const {
	return Vector2{ x / scalar, y / scalar };
}
template <typename t>
float Vector2<t>::dot(Vector2<t> other) const {
	return x * other.x + y * other.y;
}


template <typename t>
Grid<t>::Grid() {}
template <typename t>
Grid<t>::Grid(float squareSize, Vector2<float> windowSize) : squareSize{squareSize} {
	calculateDimensions(windowSize);

	gridArray = new std::set<t*>*[dimensions.x];
	for (int i = 0; i < dimensions.x; i++) {
		gridArray[i] = new std::set<t*>[dimensions.y];
	}
}
template <typename t>
void Grid<t>::calculateDimensions(Vector2<float> windowSize) {
	dimensions = { static_cast<int>(windowSize.x / squareSize) + 1, static_cast<int>(windowSize.y / squareSize) + 1 };
}
template <typename t>
Vector2<int> Grid<t>::getGridCellFromPoint(Vector2<float> point) { //given a point, returns the grid cell indeces where the point lives
	int x = static_cast<int>(point.x / squareSize);
	int y = static_cast<int>(point.y / squareSize);
	return Vector2<int>{x, y};
}
template <typename t>
void Grid<t>::updateObject(t& object) {
	Vector2<int> gridCellIndeces = getGridCellFromPoint(object.position);
	gridArray[gridCellIndeces.x][gridCellIndeces.y].insert(&object);
}
template <typename t>
void Grid<t>::updateGrid(std::vector<t>& objects) {
	for (int i = 0; i < dimensions.x; i++) {
		for (int i2 = 0; i2 < dimensions.y; i2++) {
			gridArray[i][i2].clear();
		}
	}
	for (t& object : objects) {
		updateObject(object);
	}
}
