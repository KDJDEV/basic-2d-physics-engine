#pragma once
#include "Collisions.h"
#include "math.h"
#include <algorithm>

/*
I am using double dispatch.

Each specific kind of Entity has a collide function. If the Entity is a circle, then it calls the collideWithCircle function on the Entity it is colliding with.
*/

const float response_coef = 0.75f;
//These functions prevent me from having to fully implement a collideWithRectangle function on a Circle AND a collideWithCircle function on a Rectangle. Instead, I can just define a CIRCLE_RECTANGLE and use it for both.
void CIRCLE_CIRCLE(shapes::Circle& circle1, shapes::Circle& circle2) {
	const math::Vector2<float> d = circle1.position - circle2.position;
	const float dist2 = pow(d.x, 2) + pow(d.y, 2);
	const float min_dist = circle1.radius + circle2.radius;
	// Check overlapping
	if (dist2 < min_dist * min_dist) {
		const float dist = sqrt(dist2);
		const math::Vector2<float> n = d / dist;
		const float mass_ratio_1 = circle1.radius / (circle1.radius + circle2.radius);
		const float mass_ratio_2 = circle2.radius / (circle2.radius + circle1.radius);
		const float delta = 0.5f * response_coef * (dist - min_dist);


		// Update positions
		circle1.position = circle1.position - (n * (mass_ratio_2 * delta));
		circle2.position = circle2.position + n * (mass_ratio_1 * delta);
	}
}
void RECTANGLE_CIRCLE(shapes::Rectangle& rec, shapes::Circle& circle) {
	//We will rotate the rectangle and the circle, so that the rectangle is parallel to the world coordinates
	float rotationAngle = -rec.angle;
	float newCirclePosX = std::cos(rotationAngle) * (circle.position.x - rec.position.x) - std::sin(rotationAngle) * (circle.position.y - rec.position.y) + rec.position.x;
	float newCirclePosY = std::sin(rotationAngle) * (circle.position.x - rec.position.x) + std::cos(rotationAngle) * (circle.position.y - rec.position.y) + rec.position.y;
	
	//now just imagine that the rectangle's rotation is 0

	//get closest point to circle on rectangle
	float positionDifferenceX = newCirclePosX - rec.position.x;
	float positionDifferenceY = newCirclePosY - rec.position.y;
	math::Vector2<float> closestPointOnRec{ std::clamp(positionDifferenceX, -rec.width / 2, rec.width / 2) + rec.position.x, std::clamp(positionDifferenceY, -rec.height / 2, rec.height / 2) + rec.position.y };
	
	float distanceToCircle = std::sqrt(pow((newCirclePosX - closestPointOnRec.x), 2) + pow((newCirclePosY - closestPointOnRec.y), 2));
	if (distanceToCircle < circle.radius) {
		//there is a collision
		std::cout << "COLLISION";
	}
}
void RECTANGLE_RECTANGLE(shapes::Rectangle& rec1, shapes::Rectangle& rec2) {
	// Calculate the four vertices of each rectangle
	math::Vector2<float>* vertices1 = rec1.getVertices();
	math::Vector2<float>* vertices2 = rec2.getVertices();
	
	math::Vector2<float> edgeNormals[4];
	for (int index = 0; index < 2; index++) {
		int otherIndex = index + 1;

		auto& vertex = vertices1[index];
		auto& otherVertex = vertices1[otherIndex];
		math::Vector2 parallelVector{ otherVertex.x - vertex.x, otherVertex.y - vertex.y }; //because it's a rectangle we don't need to take the perpendicular
		float magnitude = std::sqrt(std::pow(parallelVector.x, 2) + std::pow(parallelVector.y, 2));
		math::Vector2 normalVector{ parallelVector.x / magnitude, parallelVector.y / magnitude };
		edgeNormals[index] = normalVector;
	}
	for (int index = 0; index < 2; index++) {
		int otherIndex = index + 1;

		auto& vertex = vertices2[index];
		auto& otherVertex = vertices2[otherIndex];
		math::Vector2 parallelVector{ otherVertex.x - vertex.x, otherVertex.y - vertex.y };
		float magnitude = std::sqrt(std::pow(parallelVector.x, 2) + std::pow(parallelVector.y, 2));
		math::Vector2 normalVector{ parallelVector.x / magnitude, parallelVector.y / magnitude };
		edgeNormals[index + 2] = normalVector;
	}

	// Check for overlap on each axis
	float smallestOverlap = INFINITY;
	math::Vector2<float> collisionNormal;
	for (auto& axis : edgeNormals) {
		// Project the vertices of both rectangles onto the axis
		float min1 = INFINITY, max1 = -INFINITY, min2 = INFINITY, max2 = -INFINITY;
		for (int index = 0; index < 4; index++){
			auto& vertex = vertices1[index];
			float projection = vertex.dot(axis);
			
			min1 = std::min(min1, projection);
			max1 = std::max(max1, projection);
		}
		for (int index = 0; index < 4; index++) {
			auto& vertex = vertices2[index];
			float projection = vertex.dot(axis);
			min2 = std::min(min2, projection);
			max2 = std::max(max2, projection);
		}

		// Check for overlap
		if (max1 < min2 || max2 < min1) {
			// No overlap, so there is no collision
			delete[] vertices1;
			delete[] vertices2;
			return;
		}

		float overlap = max1 > max2 ? max2 - min1 : -(max1 - min2);
		if (abs(overlap) < smallestOverlap) {
			smallestOverlap = abs(overlap);
			collisionNormal = axis * overlap;
		}
	}

	// If we get here, there is a collision
	std::cout << "collision";

	math::Vector2<float> pointOfCollision;
	for (int index = 0; index < 2; index++) {
		math::Vector2<float> point = vertices1[index];

		float rotationAngle = -rec1.angle;
		float newPointPosX = std::cos(rotationAngle) * (point.x - rec1.position.x) - std::sin(rotationAngle) * (point.y - rec1.position.y) + rec1.position.x;
		float newPointPosY = std::sin(rotationAngle) * (point.x - rec1.position.x) + std::cos(rotationAngle) * (point.y - rec1.position.y) + rec1.position.y;

		if (newPointPosX > rec1.position.x - rec1.width / 2 && newPointPosX < rec1.position.x + rec1.width / 2 && newPointPosY > rec1.position.y - rec1.height / 2 && newPointPosY < rec1.position.y + rec1.height / 2) {
			pointOfCollision = point;
			return;
		}
	}

	math::Vector2<float> r1 = pointOfCollision - rec1.position;
	math::Vector2<float> r2 = pointOfCollision - rec2.position;
	float restitutionCoefficient = 0;
	float j = (-1 - restitutionCoefficient) * (rec1.velocity.dot(collisionNormal) - rec2.velocity.dot(collisionNormal) + rec1.angularVelocity * r1.dot(collisionNormal) - rec2.angularVelocity * r2.dot(collisionNormal)) / (1 / rec1.mass + 1 / rec2.mass + pow(r1.dot(collisionNormal), 2) / rec1.momentOfInteria + pow(r2.dot(collisionNormal), 2) / rec2.momentOfInteria);

	rec1.velocity = rec1.velocity + collisionNormal * j / rec1.mass;
	rec2.velocity = rec2.velocity - collisionNormal * j / rec2.mass;
	rec1.angularVelocity = rec1.angularVelocity + (r1 * j / rec1.momentOfInteria).dot(collisionNormal);
	rec1.angularVelocity = rec2.angularVelocity - (r2 * j / rec2.momentOfInteria).dot(collisionNormal);

	delete[] vertices1;
	delete[] vertices2;
}