#include "Particle.h"
#include <iostream>

Particle::Particle(sf::RenderWindow* window, const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, float life)
	: _position(pos), _velocity(vel), _colour(col), _lifespan(life), _timeAlive(0)
{
	_shape.setRadius(10.f);
	_shape.setPosition(_position);
	_shape.setFillColor(_colour);

	
}

Particle::~Particle()
{

}

void Particle::Update(float dt, sf::Vector2f ballPos)
{
	_timeAlive += dt;
	_shape.move(ballPos * dt);
}

bool Particle::GetAlive()
{
	if (_timeAlive < _lifespan)
	{
		return true;
	}

	return false;
}

sf::CircleShape& Particle::GetShape()
{
	return _shape;
}

sf::Vector2f Particle::GetPosition()
{
	return _position;
}

sf::Color Particle::GetColour()
{
	return _colour;
}
