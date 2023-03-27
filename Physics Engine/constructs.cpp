#include "solver.h"
#include "math.h"
extern math::Vector2<float> windowSizeInMeters;

void spawnBarrierWalls(Solver& solver) {
	auto& floorRec = solver.addObject<shapes::Rectangle>();
	floorRec.position = { windowSizeInMeters.x / 2, windowSizeInMeters.y };
	floorRec.updateSize(windowSizeInMeters.x, 10);
	floorRec.setAnchored(true);
}