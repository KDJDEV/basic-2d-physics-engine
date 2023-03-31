#pragma once
#include <SFML/Graphics.hpp>
#include "solver.h"

struct phaseSpaceLogger {
    phaseSpaceLogger();
    void addPhaseSpaceState(Solver& solver);
    sf::RenderWindow window;

    math::Vector2<float> prevPoint;
    bool prevPointSet = false;
};