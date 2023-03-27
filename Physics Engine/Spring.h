#pragma once

#include "entity.h"

struct Spring {
	Entity& obj1;
	Entity& obj2;

	float springConstant = 1;
	Spring(Entity& obj1, Entity& obj2, float k);

	void applyForces(float dt);
	void draw(sf::RenderTarget& target);
};