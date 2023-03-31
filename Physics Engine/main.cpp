#include "solver.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
#include "math.h"
#include "phaseSpaceLogger.h"

void miskTest3(Solver& solver);

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

	miskTest3(solver);

	phaseSpaceLogger phaseSpace;
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
		phaseSpace.addPhaseSpaceState(solver);
		x++;
	}
}