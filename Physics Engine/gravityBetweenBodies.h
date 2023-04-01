#pragma once
#include "entity.h"
#include "math.h"

void applyGravity(std::vector<std::unique_ptr<Entity>>& objects, float dt);
void applyGravity(Entity& obj1, Entity& obj2, float dt);