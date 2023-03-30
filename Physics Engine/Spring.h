#pragma once

#include "entity.h"

struct Spring {
	Entity& obj1;
	Entity& obj2;

	float springConstant;
	float dampingCoefficient;
	float length;

	bool simulateSolid = false;

	Spring(Entity& obj1, Entity& obj2, float k, float c, float length);

	void applyForces(float dt);
	void draw(sf::RenderTarget& target);
	void setSimulateSolid(bool simulateSolid);
	void autoSetLength();
};