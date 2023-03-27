#pragma once
#include <SFML/Graphics.hpp>
#include "solver.h"
#include "Entity.h"
#include "math.h"

extern math::Vector2<float> windowSizeInMeters;

sf::VertexArray createGrid(sf::RenderTarget& win, int rows, int cols) {
	// initialize values
	int numLines = rows + cols - 2;
	sf::VertexArray grid(sf::Lines, 2 * (numLines));
	win.setView(win.getDefaultView());
	auto size = win.getView().getSize();
	float rowH = size.y / rows;
	float colW = size.x / cols;
	// row separators
	for (int i = 0; i < rows - 1; i++) {
		int r = i + 1;
		float rowY = rowH * r;
		grid[i * 2].color = { 50, 50, 50, 100 };
		grid[i * 2 + 1].color = { 50, 50, 50, 100 };
		grid[i * 2].position = { 0, rowY };
		grid[i * 2 + 1].position = { size.x, rowY };
	}
	// column separators
	for (int i = rows - 1; i < numLines; i++) {
		int c = i - rows + 2;
		float colX = colW * c;
		grid[i * 2].color = { 50, 50, 50, 100 };
		grid[i * 2 + 1].color = { 50, 50, 50, 100 };
		grid[i * 2].position = { colX, 0 };
		grid[i * 2 + 1].position = { colX, size.y };
	}
	return grid;
}

struct Renderer {
	Renderer(sf::RenderTarget& target) : target{ target } {
		font.loadFromFile("./sourceCodePro.ttf");
		text.setFont(font);
		text.setCharacterSize(24);
		text.setPosition(sf::Vector2f(3, 3));
		text.setFillColor(sf::Color::Red);

		grid = createGrid(target, windowSizeInMeters.x, windowSizeInMeters.y);
	}
	void render(Solver& solver) {
		frameCount++;

		sf::CircleShape constraint_background{ 300.0f };
		constraint_background.setOrigin(300.0f, 300.0f);
		constraint_background.setFillColor(sf::Color::Black);
		constraint_background.setPosition(300.0f, 300.0f);
		constraint_background.setPointCount(128);
		target.draw(constraint_background);

		target.draw(grid);

		for (auto const& object : solver.getObjects()) {
			object->draw(target);
		}
		
		float currentTime = clock.restart().asSeconds();
		lastTime = currentTime;
		if (frameCount % 10 == 0) {
			float fps = 1.f / currentTime;
			float KE = solver.getKE();
			text.setString(std::to_string((int)fps) + " FPS " + std::to_string((int)(KE)) + " KE");
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
	sf::VertexArray grid;
};