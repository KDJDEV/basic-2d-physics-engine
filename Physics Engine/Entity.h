#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>

namespace shapes {
	struct Circle;
	struct Rectangle;
}

struct Entity {
	math::Vector2<float> position{ 0.0f, 0.0f };
	math::Vector2<float> velocity{ 0.0f, 0.0f };
	math::Vector2<float> acceleration{ 0.0f, 0.0f };


	float angle = 0;
	float angularVelocity = 0;
	float angularAcceleration = 0;

	float mass;
	float momentOfInteria;
	sf::Color color = sf::Color(rand() % 255, rand() % 255, rand() % 255);

	void update(float dt);
	virtual void draw(sf::RenderTarget& target)=0;

	virtual void applyWithinCircleConstraint()=0;

	virtual void collide(Entity&)=0;
	virtual void collideWithCircle(shapes::Circle&)=0;
	virtual void collideWithRectangle(shapes::Rectangle&)=0;
};
namespace shapes {
	struct Circle : Entity {
		Circle();
		float radius = 20.0f;

		void applyWithinCircleConstraint() override;

		sf::CircleShape circleShape{};
		void draw(sf::RenderTarget& target) override;

		void collide(Entity& entity) override;
		void collideWithCircle(shapes::Circle& circle) override;
		void collideWithRectangle(shapes::Rectangle& rec) override;
	};
	
	struct Rectangle : Entity {
		Rectangle();
		float width = 20.0f;
		float height = 40.0f;

		void applyWithinCircleConstraint() override {};

		sf::RectangleShape rectangleShape{};
		void draw(sf::RenderTarget& target) override;

		void collide(Entity& entity) override;
		void collideWithCircle(shapes::Circle& circle) override;
		void collideWithRectangle(shapes::Rectangle& rec) override;
		math::Vector2<float>* getVertices();
	};
}