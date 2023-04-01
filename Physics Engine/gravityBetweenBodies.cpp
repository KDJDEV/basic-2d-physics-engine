#include "gravityBetweenBodies.h"

extern float gravity;
extern float minGravityDistance;
void applyGravity(std::vector<std::unique_ptr<Entity>>& objects, float dt) {
	for (int i = 0;i < objects.size();i++) {
		for (int i2 = i + 1;i2 < objects.size();i2++) {
			applyGravity(*objects[i], *objects[i2], dt);
		}
	}
}

void applyGravity(Entity& obj1, Entity& obj2, float dt) { //we ignore angular effects(will cause inaccuracies rectangles)
	if (obj1.anchored || obj2.anchored) return;

	math::Vector2<float> difference = obj2.position - obj1.position;
	float dist = difference.mag();
	float force = (gravity * obj1.mass * obj2.mass)/pow(dist, 2);

	math::Vector2<float> forceNormal = difference / dist;
	if (dist > minGravityDistance) {
		obj1.velocity = obj1.velocity + forceNormal * force * dt / obj1.mass;
		obj2.velocity = obj2.velocity - forceNormal * force * dt / obj2.mass;
	}
}