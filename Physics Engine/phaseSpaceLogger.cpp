#include "math.h"
#include "entity.h"
#include "phaseSpaceLogger.h"

extern math::Vector2<float> windowSizeInPixels;
extern float pixelsPerMeter;
extern float phaseSpaceZoomSpeed;
extern float phaseUnitPixelSizeX;
extern float phaseUnitPixelSizeY;
phaseSpaceLogger::phaseSpaceLogger() : window(sf::VideoMode(windowSizeInPixels.x, windowSizeInPixels.y), "Phase Space") {}

std::vector<std::unique_ptr<sf::VertexArray>> trails;

void phaseSpaceLogger::drawCoordinatePlane() {
	sf::VertexArray x_axis(sf::Lines, 2);
	x_axis[0].position = sf::Vector2f(0, windowSizeInPixels.y / 2);
	x_axis[1].position = sf::Vector2f(windowSizeInPixels.x, windowSizeInPixels.y / 2);
	window.draw(x_axis);

	sf::VertexArray y_axis(sf::Lines, 2);
	y_axis[0].position = sf::Vector2f(windowSizeInPixels.x / 2, 0);
	y_axis[1].position = sf::Vector2f(windowSizeInPixels.x / 2, windowSizeInPixels.y);
	window.draw(y_axis);
	
	for (int i = -10; i <= 10; ++i)
	{
		sf::Vertex tick[] =
		{
			sf::Vertex(sf::Vector2f(i * phaseUnitPixelSizeX + windowSizeInPixels.x / 2, windowSizeInPixels.y / 2 - 5)),
			sf::Vertex(sf::Vector2f(i * phaseUnitPixelSizeX + windowSizeInPixels.x / 2, windowSizeInPixels.y / 2 + 5))
		};
		window.draw(tick, 2, sf::Lines);
	}

	for (int i = -10; i <= 10; ++i)
	{
		sf::Vertex tick[] =
		{
			sf::Vertex(sf::Vector2f(windowSizeInPixels.x / 2 - 5, i * phaseUnitPixelSizeY + windowSizeInPixels.y / 2)),
			sf::Vertex(sf::Vector2f(windowSizeInPixels.x / 2 + 5, i * phaseUnitPixelSizeY + windowSizeInPixels.y / 2))
		};
		window.draw(tick, 2, sf::Lines);
	}
}

std::optional<math::Vector2<float>> lastPoint;
void phaseSpaceLogger::addPhaseSpaceState(Solver& solver, float frameCount) {
	window.clear();

	float scaleFactor = frameCount * phaseSpaceZoomSpeed + 1;
	sf::View view = window.getView();
	view.setSize(static_cast<sf::Vector2f>(window.getSize()) / scaleFactor);
	window.setView(view);

	auto& objects = solver.getObjects();
	math::Vector2<float> difference = objects[1]->position - objects[0]->position;
	math::Vector2<float> perp = { -difference.y, difference.x };
	math::Vector2<float> perpNormal = perp / perp.mag();
	math::Vector2<float> velocityDifference = objects[1]->velocity - objects[0]->velocity;
	float tangentialVelocity = velocityDifference.dot(perpNormal);
	float angularVelocity = tangentialVelocity / difference.mag();
	float angle = std::atan2(difference.y, difference.x);
	if (angle < -math::PI / 2) angle += math::PI * 2;
	angle -= math::PI / 2;

	math::Vector2<float> point{ -angle * phaseUnitPixelSizeX + windowSizeInPixels.x / 2, angularVelocity * phaseUnitPixelSizeY + windowSizeInPixels.y / 2 };

		float radius = 4;
		sf::CircleShape circle(radius);
		circle.setPosition(sf::Vector2f{ point.x, point.y });
		circle.setOrigin(sf::Vector2f{ radius, radius });
		circle.setFillColor(sf::Color(255, 255, 255));

        drawCoordinatePlane();
		window.draw(circle);

		if (!lastPoint || ((point - lastPoint.value()).mag() > 100)) { //makes it pick up the pen when it loops back around
			trails.emplace_back(std::make_unique<sf::VertexArray>(sf::Lines));
		}
		trails[trails.size() - 1]->append(sf::Vertex(sf::Vector2f{ point.x, point.y }, sf::Color(255, 255, 255)));

		for (const auto& trail : trails) {
			window.draw(*trail);
		}
		window.display();

		lastPoint.emplace(point);
}
sf::RenderWindow phaseSpaceLoggerwindow;
