#include "solver.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
#include "math.h"
int main() {
	extern uint32_t frame_rate;
	const extern char* windowTitle;
	extern math::Vector2<float> windowSizeInPixels;
	extern math::Vector2<float> windowSizeInMeters;

	Solver solver{ windowSizeInPixels };
	sf::RenderWindow window(sf::VideoMode(windowSizeInPixels.x, windowSizeInPixels.y), windowTitle);

	window.setFramerateLimit(frame_rate);
	solver.set_frame_dt(frame_rate);
	Renderer renderer{ window };
	uint32_t x = 0;

	auto& rec1 = solver.addObject<shapes::Rectangle>();
	auto& rec2 = solver.addObject<shapes::Rectangle>();
	auto& floorRec = solver.addObject<shapes::Rectangle>();
	auto& circle = solver.addObject<shapes::Circle>();
	auto& circle2 = solver.addObject<shapes::Circle>();

	rec1.position = { 0, 5 };
	rec1.velocity = { 2, 0 };
	rec1.updateSize(4, 5);
	rec1.angularVelocity = 0;
	rec2.position = { 20, 5 };
	rec2.velocity = { -2.5, 0 };
	rec2.updateSize(3, 1);
	rec2.angularVelocity = 0.5;


	circle.position = { 10, 5 };
	circle.angularVelocity = 0;
	circle.velocity = { 5, 1 };
	circle2.position = { 15, 15 };
	circle2.angularVelocity = 0;
	circle2.velocity = { 0, 5 };

	floorRec.position = { windowSizeInMeters.x / 2, windowSizeInMeters.y };
	floorRec.updateSize(windowSizeInMeters.x, 10);
	floorRec.setAnchored(true);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		solver.update();
		window.clear(sf::Color::White);
		renderer.render(solver);
		window.display();

		x++;
	}
}