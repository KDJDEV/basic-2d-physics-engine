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
float Vector2<t>::mag() const {
	return  std::sqrt(pow(x, 2) + pow(y, 2));
}