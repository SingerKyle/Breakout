#include "ParticleSystem.h"
#include <iostream>

ParticleSystem::ParticleSystem(sf::RenderWindow* window) : _window(window)
{

}

ParticleSystem::~ParticleSystem()
{
	
}

void ParticleSystem::StartSpawning(sf::Vector2f position, sf::Vector2f velocity, float lifetime)
{
	_particles.emplace_back(Particle(_window, position, velocity, sf::Color::White, lifetime));
}

void ParticleSystem::Update(float dt, sf::Vector2f ballPos)
{
	std::vector<int> particlesToRemove;

	for (int i = 0; i < _particles.size(); i++)
	{
		if (!_particles[i].GetAlive())
		{
			_particles.erase(_particles.begin() + i);
		}
		else
		{
			_particles[i].Update(dt, ballPos);
		}
	}
}

void ParticleSystem::Render()
{
	//std::cout << _particles.size() << std::endl;
	if (!_particles.empty())
	{
		for (int i = 0; i < _particles.size(); i++)
		{
			_window->draw(_particles[i].GetShape());
			std::cout << i << std::endl;
		}
	}

	// Test shape
/*	sf::CircleShape testShape(10.f); // Radius of 10
	testShape.setFillColor(sf::Color::Red); // Color it red
	testShape.setPosition(450, 450.f); // Position it at (100, 100)

	// Draw the test shape
	_window->draw(testShape);*/
}


