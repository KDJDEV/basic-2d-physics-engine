#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "math.h"
#include "entity.h"
#include "spring.h"
#include <set>

extern math::Vector2<float> gravity;
extern math::Vector2<float> windowSizeInMeters;

class Solver {
public:
	Solver(math::Vector2<float> windowSize) {
		grid = math::Grid<Entity>{ 20.0f, windowSize }; //must be no less than one third of the windowSize and twice the size of the ball
		grid.calculateDimensions(windowSize);
	}
	void update() {
		float substep_dt = get_substep_dt();
		for (uint32_t i{ substeps }; i--;) {
			applyGravity();
			updateObjects(substep_dt);
			applySpringForces(substep_dt);
			checkCollisions();
			applyConstraint();
		}
	}

	template <typename t>
	t& addObject() {
		std::unique_ptr<t> object = std::make_unique<t>();
		t* ptr = object.get();
		objects.push_back(std::move(object));
		return *ptr;
	}
	Spring& addSpring(Entity& obj1, Entity& obj2, float k = 1) {
		std::unique_ptr<Spring> spring = std::make_unique<Spring>(obj1, obj2, k);
		Spring* ptr = spring.get();
		springs.push_back(std::move(spring));
		return *ptr;
	}
	void updateObjects(float substep_dt) {
		for (auto& object : objects) {
			object->update(substep_dt);
		}
	}
	void applyGravity() {
		for (auto& object : objects) {
			object->acceleration = object->acceleration + gravity / substeps;
		}
	}

	void checkCollisions()
	{
		for (int i = 0;i < objects.size();i++) {
			for (int i2 = i + 1;i2 < objects.size();i2++) {
				objects[i]->collide(*objects[i2]);
			}
		}
	}
	void applySpringForces(float substep_dt) {
		for (auto& spring : springs) {
			spring->applyForces(substep_dt);
		}
	}
	void applyConstraint()
	{
		for (auto& obj : objects) {
			//obj->applyWithinCircleConstraint();
		}
	}
	float getKE() {
		float KE = 0;
		for (auto& obj : objects) {
			if (!obj->anchored)
			KE += obj->mass * (pow(obj->velocity.x, 2) + pow(obj->velocity.y, 2)) / 2 + obj->momentOfInteria * pow(obj->angularVelocity, 2) / 2;
		}
		return KE;
	}
	std::vector<std::unique_ptr<Entity>>& const getObjects() {
		return objects;
	}
	std::vector<std::unique_ptr<Spring>>& const getSprings() {
		return springs;
	}
	void set_frame_dt(uint32_t rate) {
		frame_dt = 1.0f / static_cast<float>(rate);
	}

private:
	std::chrono::milliseconds getCurrentTimestamp() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());
	}
	float get_substep_dt() {
		return frame_dt / (float)substeps;
	}

	uint32_t substeps = 1000;
	std::vector<std::unique_ptr<Entity>> objects;
	std::vector<std::unique_ptr<Spring>> springs;
	math::Grid<Entity> grid;
	float frame_dt;
};