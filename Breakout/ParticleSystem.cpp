#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(sf::RenderWindow* window, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, float life)
	: _window(window), _position(pos), _velocity(vel), _colour(col), _lifespan(life), _timeAlive(0)
{

}

ParticleSystem::~ParticleSystem()
{
	delete _window;
}

void ParticleSystem::Update(float dt)
{
	if (GetAlive())
	{
		_position += _velocity * dt;
		_timeAlive += dt;
	}
}

bool ParticleSystem::GetAlive()
{
	if (_timeAlive < _lifespan)
	{
		return true;
	}

	return false;
}
