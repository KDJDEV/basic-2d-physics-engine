#pragma once
#include <SFML/Graphics.hpp>
#include "solver.h"
#include "entity.h"
#include "math.h"

extern math::Vector2<float> windowSizeInMeters;
extern bool darkMode;
extern sf::Color primaryColor;
extern sf::Color secondaryColor;
extern float stopAtTime;

sf::VertexArray createGrid(sf::RenderTarget& win, int rows, int cols) {
	// initialize values
	int numLines = rows + cols - 2;
	sf::VertexArray grid(sf::Lines, 2 * (numLines));
	win.setView(win.getDefaultView());
	auto size = win.getView().getSize();
	float rowH = size.y / rows;
	float colW = size.x / cols;
	// row separators
	sf::Color gridColor = darkMode ? sf::Color{ 50,50,50 } : sf::Color{ 50, 50, 50, 100 };
	for (int i = 0; i < rows - 1; i++) {
		int r = i + 1;
		float rowY = rowH * r;
		grid[i * 2].color = gridColor;
		grid[i * 2 + 1].color = gridColor;
		grid[i * 2].position = { 0, rowY };
		grid[i * 2 + 1].position = { size.x, rowY };
	}
	// column separators
	for (int i = rows - 1; i < numLines; i++) {
		int c = i - rows + 2;
		float colX = colW * c;
		grid[i * 2].color = gridColor;
		grid[i * 2 + 1].color = gridColor;
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
		text.setFillColor(primaryColor);

		grid = createGrid(target, windowSizeInMeters.x, windowSizeInMeters.y);
	}
	void render(Solver& solver) {
		frameCount++;

		target.draw(grid);

		for (auto const& object : solver.getObjects()) {
			object->draw(target);
		}
		for (auto const& spring : solver.getSprings()) {
			spring->draw(target);
		}
		
		float currentTime = clock.restart().asSeconds();
		totalTime += currentTime;
		if (stopAtTime != -1 && totalTime >= stopAtTime) {
			std::cin.get();
		}
		lastTime = currentTime;
		if (frameCount % 10 == 0) {
			float fps = 1.f / currentTime;
			float KE = solver.getKE();
			std::string timeString = std::to_string(round(totalTime * 100) / 100);
			timeString.erase(timeString.find('.') + 3, std::string::npos);

			text.setString(std::to_string((int)fps) + " FPS " + std::to_string((int)(KE)) + " KE " + timeString + "s");
		}
		//target.draw(text);
	}

	//FPS counter
	sf::Clock clock;
	float lastTime = 0;
	float totalTime = 0;
	int frameCount = 0;
	sf::Font font;
	sf::Text text;

	sf::RenderTarget& target;
	sf::VertexArray grid;
};