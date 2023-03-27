#pragma once
#include "Collisions.h"
#include "math.h"
#include <algorithm>

/*
I am using double dispatch.

Each specific kind of Entity has a collide function. If the Entity is a circle, then it calls the collideWithCircle function on the Entity it is colliding with.
*/
extern float restitutionCoefficient;
/*float calculate_j(Entity& obj1, Entity& obj2, math::Vector2<float> r1, math::Vector2<float> r2, math::Vector2<float> collisionNormal) { //calculates impulse scalar
	return (-1 - restitutionCoefficient) * (obj1.velocity.dot(collisionNormal) - obj2.velocity.dot(collisionNormal) + obj1.angularVelocity * r1.dot(collisionNormal) - obj2.angularVelocity * r2.dot(collisionNormal)) / (1 / obj1.mass + 1 / obj2.mass + pow(r1.dot(collisionNormal), 2) / obj1.momentOfInteria + pow(r2.dot(collisionNormal), 2) / obj2.momentOfInteria);
}*/
float calculate_j(Entity& obj1, Entity& obj2, math::Vector2<float> r1Perp, math::Vector2<float> r2Perp, math::Vector2<float> collisionNormal) { //calculates impulse scalar
	math::Vector2<float> velocityAtPointOfCollision1 = obj1.velocity + r1Perp * obj1.angularVelocity;
	math::Vector2<float> velocityAtPointOfCollision2 = obj2.velocity + r2Perp * obj2.angularVelocity;

	math::Vector2<float> relativePointVelocity = velocityAtPointOfCollision1 - velocityAtPointOfCollision2;
	return (-(1 + restitutionCoefficient) * relativePointVelocity.dot(collisionNormal)) / (collisionNormal.dot(collisionNormal * (1 / obj1.mass + 1 / obj2.mass)) + pow(r1Perp.dot(collisionNormal), 2) / obj1.momentOfInteria + pow(r2Perp.dot(collisionNormal), 2) / obj2.momentOfInteria);
}

std::set<std::pair<Entity*, Entity*>> objectsCollidingPairs;

//These functions prevent me from having to fully implement a collideWithRectangle function on a Circle AND a collideWithCircle function on a Rectangle. Instead, I can just define a CIRCLE_RECTANGLE and use it for both.

void CIRCLE_CIRCLE(shapes::Circle& circle1, shapes::Circle& circle2) {
	/*Since collisions are frictionless, we can ignore angular velocity for CIRCLE_CIRCLE*/
	const math::Vector2<float> d = circle1.position - circle2.position;
	const float dist2 = pow(d.x, 2) + pow(d.y, 2);
	const float min_dist = circle1.radius + circle2.radius;
	// Check overlapping
	if (dist2 < pow(min_dist, 2)) {
		if (objectsCollidingPairs.find(std::make_pair(&circle1, &circle2)) == objectsCollidingPairs.end()) { //check if an impulse has already been applied to these two objects for this collision
			objectsCollidingPairs.insert(std::make_pair(&circle1, &circle2));
			const float dist = sqrt(dist2);
			const math::Vector2<float> collisionNormal = d / dist;
			const math::Vector2<float> relativeVelocity = circle1.velocity - circle2.velocity;

			const float j = relativeVelocity.dot(collisionNormal); //j is velocity parallel to the normal //at this point the problem becomes a one dimensional collision

			float u1 = (j - (j * 2 * circle2.mass) / (circle1.mass + circle2.mass));
			float u2 = (j * 2 * circle1.mass) / (circle1.mass + circle2.mass);
			math::Vector2<float> perpCollisionNormal{ -collisionNormal.y, collisionNormal.x };
			circle1.velocity = collisionNormal * u1 + perpCollisionNormal * relativeVelocity.dot(perpCollisionNormal) + circle2.velocity;
			circle2.velocity = collisionNormal * u2 + circle2.velocity;
		}
	}
	else {
		objectsCollidingPairs.erase(std::make_pair(&circle1, &circle2));
	}
}
void RECTANGLE_CIRCLE(shapes::Rectangle& rec, shapes::Circle& circle) {
	/*Since collisions are frictionless, the circle will have no change in angular velocity*/

	//We will rotate the rectangle and the circle, so that the rectangle is parallel to the world coordinates
	const float rotationAngle = -rec.angle;
	const float newCirclePosX = std::cos(rotationAngle) * (circle.position.x - rec.position.x) - std::sin(rotationAngle) * (circle.position.y - rec.position.y) + rec.position.x;
	const float newCirclePosY = std::sin(rotationAngle) * (circle.position.x - rec.position.x) + std::cos(rotationAngle) * (circle.position.y - rec.position.y) + rec.position.y;

	//now just imagine that the rectangle's rotation is 0

	//get closest point to circle on rectangle
	const float positionDifferenceX = newCirclePosX - rec.position.x;
	const float positionDifferenceY = newCirclePosY - rec.position.y;
	const math::Vector2<float> closestPointOnRec{ std::clamp(positionDifferenceX, -rec.width / 2, rec.width / 2) + rec.position.x, std::clamp(positionDifferenceY, -rec.height / 2, rec.height / 2) + rec.position.y };
	const float dist2 = pow((newCirclePosX - closestPointOnRec.x), 2) + pow((newCirclePosY - closestPointOnRec.y), 2);

	if (dist2 < pow(circle.radius, 2)) {
		if (objectsCollidingPairs.find(std::make_pair(&circle, &rec)) == objectsCollidingPairs.end()) { //check if an impulse has already been applied to these two objects for this collision
			objectsCollidingPairs.insert(std::make_pair(&circle, &rec));

			const math::Vector2<float> closestPointOnRecRelativeToRec = closestPointOnRec - rec.position;
			const float pointOfCollisionX = std::cos(-rotationAngle) * (closestPointOnRecRelativeToRec.x) - std::sin(-rotationAngle) * (closestPointOnRecRelativeToRec.y) + rec.position.x;
			const float pointOfCollisionY = std::sin(-rotationAngle) * (closestPointOnRecRelativeToRec.x) + std::cos(-rotationAngle) * (closestPointOnRecRelativeToRec.y) + rec.position.y;
			const math::Vector2<float> pointOfCollision{ pointOfCollisionX , pointOfCollisionY };

			const math::Vector2<float> rRec = pointOfCollision - rec.position;
			const math::Vector2<float> rCircle = pointOfCollision - circle.position;
			math::Vector2<float> rRecPerp{ -rRec.y, rRec.x };
			math::Vector2<float> rCirclePerp{ -rCircle.y, rCircle.x };
			const float mag = std::sqrt(pow(rCircle.x, 2) + pow(rCircle.y, 2));
			math::Vector2<float> collisionNormal = rCircle / mag;

			const float j = calculate_j(rec, circle, rRecPerp, rCirclePerp, collisionNormal);
			rec.velocity = rec.velocity + collisionNormal * j / rec.mass;
			circle.velocity = circle.velocity - collisionNormal * j / circle.mass;
			rec.angularVelocity = rec.angularVelocity + rRecPerp.dot(collisionNormal * j) / rec.momentOfInteria;
			circle.angularVelocity = circle.angularVelocity - rCirclePerp.dot(collisionNormal * j) / circle.momentOfInteria;
		}
	}
	else {
		objectsCollidingPairs.erase(std::make_pair(&circle, &rec));
	}
}

bool isPointInsideRectangle(math::Vector2<float>& point, shapes::Rectangle& rec) {
	float rotationAngle = -rec.angle;
	float newPointPosX = std::cos(rotationAngle) * (point.x - rec.position.x) - std::sin(rotationAngle) * (point.y - rec.position.y) + rec.position.x;
	float newPointPosY = std::sin(rotationAngle) * (point.x - rec.position.x) + std::cos(rotationAngle) * (point.y - rec.position.y) + rec.position.y;

	if (newPointPosX > rec.position.x - rec.width / 2 && newPointPosX < rec.position.x + rec.width / 2 && newPointPosY > rec.position.y - rec.height / 2 && newPointPosY < rec.position.y + rec.height / 2) return true;
	return false;
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
	math::Vector2<float> collisionPerpAxis;
	for (auto& axis : edgeNormals) {
		// Project the vertices of both rectangles onto the axis
		float min1 = INFINITY, max1 = -INFINITY, min2 = INFINITY, max2 = -INFINITY;
		for (int index = 0; index < 4; index++) {
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
			objectsCollidingPairs.erase(std::make_pair(&rec1, &rec2));
			return;
		}

		float overlap = max1 > max2 ? max2 - min1 : -(max1 - min2);
		if (abs(overlap) < smallestOverlap) {
			smallestOverlap = abs(overlap);
			collisionPerpAxis = axis;
		}
	}

	// If we get here, there is a collision

	if (objectsCollidingPairs.find(std::make_pair(&rec1, &rec2)) == objectsCollidingPairs.end()) { //check if an impulse has already been applied to these two objects for this collision
		objectsCollidingPairs.insert(std::make_pair(&rec1, &rec2));

		float mag = std::sqrt(pow(collisionPerpAxis.x, 2) + pow(collisionPerpAxis.y, 2));
		math::Vector2<float> collisionNormal = collisionPerpAxis / mag;

		math::Vector2<float> pointOfCollision;
		bool foundPointOfCollision = false;
		for (int index = 0; index < 4; index++) {
			if (isPointInsideRectangle(vertices1[index], rec2)) {
				pointOfCollision = vertices1[index];
				foundPointOfCollision = true;
				break;
			}
		}
		if (!foundPointOfCollision) {
			for (int index = 0; index < 4; index++) {
				if (isPointInsideRectangle(vertices2[index], rec1)) {
					pointOfCollision = vertices2[index];
					break;
				}
			}
		}

		math::Vector2<float> r1 = pointOfCollision - rec1.position;
		math::Vector2<float> r2 = pointOfCollision - rec2.position;
		math::Vector2<float> r1Perp{ -r1.y, r1.x };
		math::Vector2<float> r2Perp{ -r2.y, r2.x };
		const float j = calculate_j(rec1, rec2, r1Perp, r2Perp, collisionNormal);
		rec1.velocity = rec1.velocity + collisionNormal * j / rec1.mass;
		rec2.velocity = rec2.velocity - collisionNormal * j / rec2.mass;
		rec1.angularVelocity = rec1.angularVelocity + r1Perp.dot(collisionNormal * j) / rec1.momentOfInteria;
		rec2.angularVelocity = rec2.angularVelocity - r2Perp.dot(collisionNormal * j) / rec2.momentOfInteria;
	}

	delete[] vertices1;
	delete[] vertices2;
}