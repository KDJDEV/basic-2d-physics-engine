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