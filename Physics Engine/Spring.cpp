#include "spring.h"

extern float pixelsPerMeter;
Spring::Spring(Entity& obj1, Entity& obj2, float k , float c, float length) : obj1{ obj1 }, obj2{ obj2 }, springConstant{ k }, dampingCoefficient{ c }, length{ length } {}

void Spring::applyForces(float dt) {
	math::Vector2<float> objectDisplacement = obj1.position - obj2.position;
    math::Vector2<float> objectVelocity = obj1.velocity - obj2.velocity;
	float mag = objectDisplacement.mag();
	math::Vector2<float> springNormal = objectDisplacement / mag;
    objectDisplacement = objectDisplacement - springNormal * length;
    mag = objectDisplacement.mag();
    if (mag > 0) {
        springNormal = objectDisplacement / mag;

        float force = springConstant * mag + dampingCoefficient * objectVelocity.dot(springNormal); //Hooke's Law
        obj1.velocity = obj1.velocity - springNormal * force * dt / obj1.mass;
        obj2.velocity = obj2.velocity + springNormal * force * dt / obj2.mass;
    }
}
void drawSolidConnector(sf::RenderTarget& target, Entity& obj1, Entity& obj2) {
    math::Vector2<float> start = obj1.position * pixelsPerMeter;
    math::Vector2<float> end = obj2.position * pixelsPerMeter;
    math::Vector2<float> difference = end - start;
    float d = difference.mag();
    float angle = std::atan2(difference.y, difference.x)/math::PI*180 - 90;

    float width = 10;
    sf::CircleShape circle1(width/2);
    circle1.setPosition(sf::Vector2f{start.x, start.y});
    circle1.setOrigin(sf::Vector2f{ width / 2, width / 2 });
    circle1.setFillColor(sf::Color(0, 0, 0));

    sf::CircleShape circle2(width/2);
    circle2.setPosition(sf::Vector2f{ end.x, end.y });
    circle2.setOrigin(sf::Vector2f{ width / 2, width / 2 });
    circle2.setFillColor(sf::Color(0, 0, 0));

    sf::RectangleShape rec(sf::Vector2f{ width, d });
    rec.setPosition(sf::Vector2f{ start.x, start.y });
    rec.setRotation(angle);
    rec.setOrigin(sf::Vector2f{width / 2, 0});
    rec.setFillColor(sf::Color(0, 0, 0));

    target.draw(circle1);
    target.draw(circle2);
    target.draw(rec);
}
void Spring::draw(sf::RenderTarget& target) {
    if (simulateSolid) {
        drawSolidConnector(target, obj1, obj2);
        return;
    }
    int nodes = 50;
    float width = 20;

    math::Vector2<float> start = obj1.position * pixelsPerMeter;
    math::Vector2<float> end = obj2.position * pixelsPerMeter;

    if (start.x == end.x && start.y == end.y) { //make sure the objects are not at the same location
        return;
    }
    math::Vector2<float> difference = end - start;
    float length = difference.mag();

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

void Spring::setSimulateSolid(bool simulateSolid) {
    this->simulateSolid = simulateSolid;
}
void Spring::autoSetLength() {
    math::Vector2<float> difference = obj2.position - obj1.position;
    this->length = difference.mag();
}
