#pragma once
#include <SFML/Graphics.hpp>
#include "solver.h"

struct phaseSpaceLogger {
    phaseSpaceLogger();
    void addPhaseSpaceState(Solver& solver, float frameCount);
    void drawCoordinatePlane();
    sf::RenderWindow window;
};