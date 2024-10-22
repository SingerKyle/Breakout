#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>


class Particle
{
public:
	Particle(sf::RenderWindow* window, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, float life);
	~Particle();

	void Update(float dt, sf::Vector2f ballPos);
	bool GetAlive();

	sf::CircleShape& GetShape();
	sf::Vector2f GetPosition();
	sf::Color GetColour();
private:
	sf::CircleShape _shape;

	sf::Vector2f _position;
	sf::Vector2f _velocity;
	sf::Color _colour;
	float _lifespan;
	float _timeAlive;
};

