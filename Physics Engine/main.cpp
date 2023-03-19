#include "solver.h"
#include "renderer.h"
#include <SFML/Graphics.hpp>
#include "math.h"
int main() {
    math::Vector2<float> windowSize{ 600, 600 };
    Solver solver{ windowSize };
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Physics Engine");

    const uint32_t frame_rate = 200;
    window.setFramerateLimit(frame_rate);
    solver.set_frame_dt(frame_rate);
    Renderer renderer{ window };
    uint32_t x = 0;
    /*shapes::Circle& circle = solver.addObject<shapes::Circle>();
    shapes::Rectangle& rec = solver.addObject<shapes::Rectangle>();

    circle.position = math::Vector2{ 150.0f, 150.0f };
    circle.velocity = math::Vector2{ 20.0f, 0.0f };
    rec.position = math::Vector2{ 600 - 150.0f, 150.0f };
    rec.velocity = math::Vector2{ -20.0f, 0.0f };
    rec.angularVelocity = 20;*/
    shapes::Rectangle& rec1 = solver.addObject<shapes::Rectangle>();
    shapes::Rectangle& rec2 = solver.addObject<shapes::Rectangle>();

    rec1.position = math::Vector2{ 150.0f, 150.0f };
    rec1.velocity = math::Vector2{ 40.0f, 0.0f };
    rec1.angularVelocity = 3;
    rec2.position = math::Vector2{ 600 - 150.0f, 150.0f };
    rec2.velocity = math::Vector2{ -40.0f, 0.0f };
    rec2.angularVelocity = 15;
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