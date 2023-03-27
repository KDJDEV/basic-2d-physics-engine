#include "spring.h"

extern float pixelsPerMeter;
Spring::Spring(Entity& obj1, Entity& obj2, float k) : obj1{ obj1 }, obj2{ obj2 }, springConstant{ k } {}

void Spring::applyForces(float dt) {
	math::Vector2<float> objectDisplacement = obj1.position - obj2.position;
	float mag = std::sqrt(pow(objectDisplacement.x, 2) + pow(objectDisplacement.y, 2));

	float force = springConstant * mag; //Hooke's Law
	math::Vector2<float> springNormal = objectDisplacement / mag;
	if (!obj1.anchored)
	obj1.velocity = obj1.velocity - springNormal * force / dt / obj1.mass;
	if (!obj2.anchored)
	obj2.velocity = obj2.velocity + springNormal * force / dt / obj2.mass;
}

void Spring::draw(sf::RenderTarget& target) {
    int nodes = 50;
    float width = 20;

    math::Vector2<float> start = obj1.position * pixelsPerMeter;
    math::Vector2<float> end = obj2.position * pixelsPerMeter;

    if (start.x == end.x && start.y == end.y) { //make sure the objects are not at the same location
        return;
    }

    float length = std::sqrt(pow((end - start).x, 2) + pow((end - start).y, 2));

    math::Vector2<float> u_t = (end - start) / length;
    math::Vector2<float> u_n(-u_t.y, u_t.x);

    std::vector<math::Vector2<float>> springCoords(nodes + 2);
    springCoords[0] = start;
    springCoords[nodes + 1] = end;

    float normal_dist = std::sqrt(std::max(0.0f, width * width - (length * length) / (nodes * nodes))) / 2.0f;

    for (int i = 1; i <= nodes; i++) {
        springCoords[i] = start + (u_t * (length * (2 * i - 1)) / (2 * nodes)) + (u_n * normal_dist * std::pow(-1, i));
    }

    sf::VertexArray spring(sf::LineStrip, nodes + 2);
    for (int i = 0; i < nodes + 2; i++) {
        spring[i].position = sf::Vector2f(springCoords[i].x, springCoords[i].y);
        spring[i].color = sf::Color::Black;
    }

    target.draw(spring);
}
