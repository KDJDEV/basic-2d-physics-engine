#include "math.h"
#include "entity.h"
#include "phaseSpaceLogger.h"

extern math::Vector2<float> windowSizeInPixels;
extern float pixelsPerMeter;

phaseSpaceLogger::phaseSpaceLogger() : window(sf::VideoMode(windowSizeInPixels.x, windowSizeInPixels.y), "Phase Space") {}

//void phaseSpaceLogger::addPhaseSpaceState(Solver& solver) {
//	for (auto& object : solver.getObjects()) {
//		if (!object->anchored) {
//			sf::Vertex point(sf::Vector2f(object->angle * pixelsPerMeter, object->angularVelocity * pixelsPerMeter), sf::Color::Red);
//			window.draw(&point, 4, sf::Points);
//		}
//	}
//	window.display();
//}
void phaseSpaceLogger::addPhaseSpaceState(Solver& solver) {
	auto& objects = solver.getObjects();
	math::Vector2<float> difference = objects[1]->position - objects[0]->position;
	math::Vector2<float> perp = { -difference.y, difference.x };
	math::Vector2<float> perpNormal = perp / perp.mag();
	math::Vector2<float> velocityDifference = objects[1]->velocity - objects[0]->velocity;
	float tangentialVelocity = velocityDifference.dot(perpNormal);
	float angularVelocity = tangentialVelocity / difference.mag();
	float angle = std::atan2(difference.y, difference.x);
	if (angle < -math::PI / 2) angle += math::PI * 2;
	angle += math::PI / 2;
	std::cout << angle/math::PI*180 << "\n";

	math::Vector2<float> point{ -angle * 40 + windowSizeInPixels.x / 2, angularVelocity * 40 + windowSizeInPixels.y / 2 };
	if (prevPointSet) {
		math::Vector2<float> difference = point - prevPoint;
		float d = difference.mag();
		float angle = std::atan2(difference.y, difference.x) / math::PI * 180 - 90;

		float width = 2;
		sf::RectangleShape rec(sf::Vector2f{ width, d });
		rec.setPosition(sf::Vector2f{ prevPoint.x, prevPoint.y });
		rec.setRotation(angle);
		rec.setOrigin(sf::Vector2f{ width / 2, 0 });
		rec.setFillColor(sf::Color(255, 255, 255));

		//sf::Vertex point(sf::Vector2f(point.x * pixelsPerMeter + windowSizeInPixels.x / 2, point.y * pixelsPerMeter + windowSizeInPixels.y / 2), sf::Color::Red);
		window.draw(rec);
	}
	prevPoint = point;
	prevPointSet = true;
	
	window.display();
}
sf::RenderWindow phaseSpaceLoggerwindow;
