#pragma once
#include "math.h"
#include "entity.h"
#include "collisions.h"

extern float pixelsPerMeter;
void Entity::update(float dt) {
	if (!anchored) {
		position = position + velocity * dt;
		angle = angle + angularVelocity * dt;
	}
}
void Entity::setAnchored(bool anchored) {
	this->anchored = anchored;
	mass = INFINITY;
	updateMomentOfInertia();
}

using namespace shapes;
Circle::Circle() {
	updateMass();
	updateMomentOfInertia();
	circleShape.setOrigin(radius * pixelsPerMeter, radius * pixelsPerMeter);
	circleShape.setRadius(radius * pixelsPerMeter);
	circleShape.setFillColor(color);

	circleShapeDot.setOrigin(radius / 5 * pixelsPerMeter, radius / 5 * pixelsPerMeter);
	circleShapeDot.setRadius(radius / 5 * pixelsPerMeter);
	circleShapeDot.setFillColor(sf::Color{255, 255, 255});
}
	

void Circle::applyWithinCircleConstraint() {
	const math::Vector2<float> v = math::Vector2<float>{ 300.0f,300.0f } - position;
	const float dist = sqrt(v.x * v.x + v.y * v.y);
	if (dist > (300.0f - radius)) {
		const math::Vector2<float> n = v / dist;
		position = math::Vector2<float>{ 300.0f, 300.0f } - n * (300.0f - radius);
	}
}
void Circle::draw(sf::RenderTarget& target) {
	circleShape.setPosition(position.x * pixelsPerMeter, position.y * pixelsPerMeter);
	circleShapeDot.setPosition((position.x + (-radius / 2 * std::sin(angle))) * pixelsPerMeter, (position.y + (radius / 2 * std::cos(angle))) * pixelsPerMeter);
	target.draw(circleShape);
	target.draw(circleShapeDot);
}

void Circle::collide(Entity& entity) {
	entity.collideWithCircle(*this);
}
void Circle::collideWithCircle(shapes::Circle& circle) {
	CIRCLE_CIRCLE(*this, circle);
}
void Circle::collideWithRectangle(shapes::Rectangle& rec) {
	RECTANGLE_CIRCLE(rec, *this);
}
void Circle::updateMomentOfInertia() {
	momentOfInteria = mass * pow(radius, 2) / 2;
}
void Circle::updateMass() {
	mass = math::PI * pow(radius, 2);
}
void Circle::updateRadius(float radius) {
	this->radius = radius;
	circleShape.setRadius(radius * pixelsPerMeter);
	circleShape.setOrigin(radius * pixelsPerMeter, radius * pixelsPerMeter);

	updateMass();
	updateMomentOfInertia();
}

Rectangle::Rectangle() {
	updateMass();
	updateMomentOfInertia();

	rectangleShape.setOrigin(width / 2 * pixelsPerMeter, height / 2 * pixelsPerMeter);
	rectangleShape.setSize(sf::Vector2f{ width * pixelsPerMeter, height * pixelsPerMeter });
	rectangleShape.setFillColor(color);
}
void Rectangle::draw(sf::RenderTarget& target) {
	rectangleShape.setPosition(position.x * pixelsPerMeter, position.y * pixelsPerMeter);
	rectangleShape.setRotation(angle * 180.0f / math::PI);
	target.draw(rectangleShape);
}

void Rectangle::collide(Entity& entity) {
	entity.collideWithRectangle(*this);
}
void Rectangle::collideWithCircle(shapes::Circle& circle) {
	RECTANGLE_CIRCLE(*this, circle);
}
void Rectangle::collideWithRectangle(shapes::Rectangle& rec) {
	RECTANGLE_RECTANGLE(rec, *this);
}
void Rectangle::updateMomentOfInertia() {
	momentOfInteria = mass * (pow(width, 2) + pow(height, 2)) / 12;
}
void Rectangle::updateMass() {
	mass = width * height;
}
void Rectangle::updateSize(float width, float height) {
	this->width = width;
	this->height = height;
	rectangleShape.setSize(sf::Vector2f{ width * pixelsPerMeter, height * pixelsPerMeter });
	rectangleShape.setOrigin(width / 2 * pixelsPerMeter, height / 2 * pixelsPerMeter);
	updateMass();
	updateMomentOfInertia();
}
math::Vector2<float>* Rectangle::getVertices() {
	auto rotateAndTranslate = [this](math::Vector2<float>& point, float angle){
		float pointX = std::cos(angle) * point.x - std::sin(angle) * point.y;
		float pointY = std::sin(angle) * point.x + std::cos(angle) * point.y;
		point.x = pointX + position.x;
		point.y = pointY + position.y;
	};
	math::Vector2<float>* points = new math::Vector2<float>[4];
	points[0] = math::Vector2{ width / -2, height / -2 };
	points[1] = math::Vector2{ width / 2, height / -2 };
	points[2] = math::Vector2{ width / 2, height / 2 };
	points[3] = math::Vector2{ width / -2,height / 2 };
	rotateAndTranslate(points[0], angle);
	rotateAndTranslate(points[1], angle);
	rotateAndTranslate(points[2], angle);
	rotateAndTranslate(points[3], angle);

	return points;
}
