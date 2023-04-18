#include "solver.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
#include "math.h"
#include "phaseSpaceLogger.h"
#include "miskTests.h"

extern sf::Color secondaryColor;
int main() {
	extern uint32_t frame_rate;
	const extern char* windowTitle;
	extern math::Vector2<float> windowSizeInPixels;
	extern math::Vector2<float> windowSizeInMeters;
	extern bool includePhaseSpace;

	Solver solver;
	sf::RenderWindow window(sf::VideoMode(windowSizeInPixels.x, windowSizeInPixels.y), windowTitle);

	window.setFramerateLimit(frame_rate);
	solver.set_frame_dt(frame_rate);
	Renderer renderer{ window };
	uint32_t frame = 1;

	/*"switch out the miskTest here"*/
	miskTest13(solver);

	std::optional<phaseSpaceLogger> phaseSpace;
	if (includePhaseSpace)
		phaseSpace.emplace();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		solver.update();
		window.clear(secondaryColor);
		renderer.render(solver);
		window.display();
		if (includePhaseSpace)
		phaseSpace->addPhaseSpaceState(solver, frame);
		frame++;
	}
}