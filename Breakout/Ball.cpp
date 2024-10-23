#include "Ball.h"
#include <stdlib.h>
#include "GameManager.h" // avoid cicular dependencies
#include "ParticleSystem.h"
#include <iomanip>

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager, ParticleSystem* particleSystem)
    : _window(window), _velocity(velocity), _gameManager(gameManager), _particleSystem(particleSystem),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1}), _currentVelocity(0)
{
    srand(time(NULL));

    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);


    _sprite.setPosition((rand() % WINDOW_WIDTH), (rand() % (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / 2)));

    _particleSpawnTimer = 0.f;

}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        _direction.y *= -1;
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition((rand() % WINDOW_WIDTH), (rand() % (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / 2)));
        _currentVelocity = 0;
        _direction = { 1, 1 };
        _gameManager->loseLife();
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically
    }


    // slowly accelerate ball to speed
    if (_currentVelocity < _velocity)
    {
        // apply accelleration so ball speeds up over time
        _currentVelocity += 500.f * dt;
        _currentVelocity = std::clamp(_currentVelocity, 0.0f, _velocity);
    }

    _sprite.move(_direction * _currentVelocity * dt);

    _particleSpawnTimer += dt;

    // Particle system update to follow the player.
    if (_particleSpawnTimer >= 0.1f)
    {
        _particleSystem->StartSpawning(_sprite.getPosition(), _direction * _currentVelocity * dt, 0.4f);
        _particleSpawnTimer = 0.f;
    }

    _particleSystem->Update(dt, _direction * _currentVelocity * dt);
    
}

void Ball::reset()
{
    _sprite.setPosition((rand() % WINDOW_WIDTH), (rand() % (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / 2)));
    _currentVelocity = 0;
}

void Ball::render()
{
    _window->draw(_sprite);
    _particleSystem->Render();
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}
