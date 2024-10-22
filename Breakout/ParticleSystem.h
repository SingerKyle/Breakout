#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Particle.h"

class ParticleSystem
{
public:
	ParticleSystem(sf::RenderWindow* window);
	~ParticleSystem();

	void StartSpawning(sf::Vector2f position, sf::Vector2f velocity, float lifetime);

	void Update(float dt, sf::Vector2f);

	void Render();

private:
	std::vector<Particle> _particles;
	sf::RenderWindow* _window;

protected:


};

