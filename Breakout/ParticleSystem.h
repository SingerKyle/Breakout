#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

class ParticleSystem
{
public:
	ParticleSystem(sf::RenderWindow* window, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, float life);
	~ParticleSystem();

	void Update(float dt);
	bool GetAlive();

private:
	sf::Vector2f _position;
	sf::Vector2f _velocity;
	sf::Color _colour;
	float _lifespan;
	float _timeAlive;
	sf::RenderWindow* _window;

protected:


};

