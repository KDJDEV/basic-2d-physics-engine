#pragma once
#include <SFML/Graphics.hpp>
#include "solver.h"
#include "Entity.h"

struct Renderer {
	Renderer(sf::RenderTarget& target) : target{ target } {
		font.loadFromFile("./sourceCodePro.ttf");
		text.setFont(font);
		text.setCharacterSize(24);
		text.setPosition(sf::Vector2f(3, 3));
		text.setFillColor(sf::Color::Red);
	}
	void render(Solver& solver) {
		frameCount++;

		sf::CircleShape constraint_background{ 300.0f };
		constraint_background.setOrigin(300.0f, 300.0f);
		constraint_background.setFillColor(sf::Color::Black);
		constraint_background.setPosition(300.0f, 300.0f);
		constraint_background.setPointCount(128);
		target.draw(constraint_background);

		for (auto const& object : solver.getObjects()) {
			object->draw(target);
		}
		
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		if (frameCount % 10 == 0) {
			float fps = 1.f / currentTime;
			text.setString(std::to_string((int)fps) + " FPS");
		}
		target.draw(text);
		
	}

	//FPS counter
	sf::Clock clock;
	float lastTime = 0;
	int frameCount = 0;
	sf::Font font;
	sf::Text text;

	sf::RenderTarget& target;
};