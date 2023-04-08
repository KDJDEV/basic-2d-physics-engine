#include "solver.h"
#include "math.h"
extern math::Vector2<float> windowSizeInMeters;

void spawnBarrierWalls(Solver& solver) {
	auto& rec1 = solver.addObject<shapes::Rectangle>();
	rec1.position = { windowSizeInMeters.x / 2, windowSizeInMeters.y + 4 };
	rec1.updateSize(windowSizeInMeters.x, 10);
	rec1.setAnchored(true);

	auto& rec2 = solver.addObject<shapes::Rectangle>();
	rec2.position = { windowSizeInMeters.x / 2, -4 };
	rec2.updateSize(windowSizeInMeters.x, 10);
	rec2.setAnchored(true);

	auto& rec3 = solver.addObject<shapes::Rectangle>();
	rec3.position = { windowSizeInMeters.x + 4 , windowSizeInMeters.y / 2 };
	rec3.updateSize(10, windowSizeInMeters.y - 2.1);
	rec3.setAnchored(true);

	auto& rec4 = solver.addObject<shapes::Rectangle>();
	rec4.position = { -4, windowSizeInMeters.y / 2 };
	rec4.updateSize(10, windowSizeInMeters.y - 2.1);
	rec4.setAnchored(true);
}

void miskTest1(Solver& solver) { //Misk objects and springs everywhere
	spawnBarrierWalls(solver);

	auto& rec1 = solver.addObject<shapes::Rectangle>();
	auto& rec2 = solver.addObject<shapes::Rectangle>();
	auto& circle = solver.addObject<shapes::Circle>();
	auto& circle2 = solver.addObject<shapes::Circle>();

	rec1.position = { 3, 5 };
	rec1.velocity = { 2, 0 };
	rec1.updateSize(4, 5);
	rec1.angularVelocity = 0.1;
	rec2.position = { 20, 5 };
	rec2.velocity = { -2.5, 0 };
	rec2.updateSize(4, 0.4);
	rec2.angularVelocity = 0.5;


	circle.position = { 10, 5 };
	circle.angularVelocity = 0;
	circle.velocity = { 5, 1 };
	circle.updateRadius(5);
	circle2.position = { 15, 15 };
	circle2.angularVelocity = 0;
	circle2.velocity = { 0, 5 };

	auto& spring = solver.addSpring(circle, circle2);
	auto& spring2 = solver.addSpring(rec1, rec2);
	auto& spring3 = solver.addSpring(rec1, circle);
}
void miskTest2(Solver& solver) { //Simple circle connected to anchored rectangle with a spring
	auto& rec = solver.addObject<shapes::Rectangle>();
	rec.position = windowSizeInMeters / 2;
	rec.anchored = true;
	rec.setDoesCollide(false);
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 4};
	circle.velocity = { 5,0 };

	auto& spring = solver.addSpring(rec, circle, 20);
}
void miskTest3(Solver& solver) { //Pendulum
	auto& rec = solver.addObject<shapes::Rectangle>();
	rec.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 0};
	rec.anchored = true;
	rec.setDoesCollide(false);
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{3, -4};
	circle.velocity = { 0,0 };

	auto& spring = solver.addSpring(rec, circle, 10000, 10000);
	spring.setSimulateSolid(true);
	spring.autoSetLength();
}
void miskTest4(Solver& solver) {//Double pendulum
	auto& rec = solver.addObject<shapes::Rectangle>();
	rec.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 0};
	rec.anchored = true;
	rec.setDoesCollide(false);
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{1.5, -2};
	circle.velocity = { 0,0 };
	auto& circle2 = solver.addObject<shapes::Circle>();
	circle2.updateRadius(1);
	circle2.position = windowSizeInMeters / 2 + math::Vector2<float>{4.5, -8};
	circle2.velocity = { 0,0 };
	auto& spring = solver.addSpring(rec, circle, 10000, 10000);
	spring.setSimulateSolid(true);
	spring.autoSetLength();
	auto& spring2 = solver.addSpring(circle, circle2, 10000, 10000);
	spring2.setSimulateSolid(true);
	spring2.autoSetLength();
}
void miskTest5(Solver& solver) { //Double pendulum with pretty phase space
	auto& rec = solver.addObject<shapes::Rectangle>();
	rec.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 0};
	rec.anchored = true;
	rec.setDoesCollide(false);
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{1.5, 2};
	circle.velocity = { 0,0 };
	auto& circle2 = solver.addObject<shapes::Circle>();
	circle2.updateRadius(1);
	circle2.position = windowSizeInMeters / 2 + math::Vector2<float>{3, 4};
	circle2.velocity = { 0,0 };
	auto& spring = solver.addSpring(rec, circle, 10000, 10000);
	spring.setSimulateSolid(true);
	spring.autoSetLength();
	auto& spring2 = solver.addSpring(circle, circle2, 10000, 10000);
	spring2.setSimulateSolid(true);
	spring2.autoSetLength();
}
void miskTest6(Solver& solver) { //Double pendulum where the first pendulum doesn't loop
	auto& rec = solver.addObject<shapes::Rectangle>();
	rec.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 0};
	rec.anchored = true;
	rec.setDoesCollide(false);
	rec.updateSize(0.1,0.1);
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{3, -2};
	circle.velocity = { 0,0 };
	auto& circle2 = solver.addObject<shapes::Circle>();
	circle2.updateRadius(1);
	circle2.position = windowSizeInMeters / 2 + math::Vector2<float>{6, -4};
	circle2.velocity = { 0,0 };
	auto& spring = solver.addSpring(rec, circle, 10000, 10000);
	spring.setSimulateSolid(true);
	spring.autoSetLength();
	auto& spring2 = solver.addSpring(circle, circle2, 10000, 10000);
	spring2.setSimulateSolid(true);
	spring2.autoSetLength();
}
void miskTest7(Solver& solver) {//Three body system
	spawnBarrierWalls(solver);

	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{0, 0};
	circle.updateRadius(.5);
	auto& circle2 = solver.addObject<shapes::Circle>();
	circle2.position = windowSizeInMeters / 2 + math::Vector2<float>{6, 8};
	circle2.updateRadius(.5);
	auto& circle3 = solver.addObject<shapes::Circle>();
	circle3.position = windowSizeInMeters / 2 + math::Vector2<float>{-6, 7};
	circle3.updateRadius(.5);
}
void miskTest8(Solver& solver) { //A single falling ball for testing gravity
	auto& circle = solver.addObject<shapes::Circle>();
	circle.position = windowSizeInMeters / 2 + math::Vector2<float>{0, -8};
}
void miskTest9(Solver& solver) { //two balls colliding for testing collisions
	auto& circle1 = solver.addObject<shapes::Circle>();
	circle1.position = windowSizeInMeters / 2 + math::Vector2<float>{-4, 0};
	circle1.velocity = { 1, 0 };
	auto& circle2 = solver.addObject<shapes::Circle>();
	circle2.position = windowSizeInMeters / 2 + math::Vector2<float>{4, 0.5};
	circle2.velocity = { -1, 0 };
}