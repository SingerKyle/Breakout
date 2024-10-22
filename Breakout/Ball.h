#pragma once
#include <SFML/Graphics.hpp>


class GameManager;  // forward declaration
class ParticleSystem;


class Ball {
public:
    Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager, ParticleSystem* particleSystem);
    ~Ball();
    void update(float dt);
    void render();
    void setVelocity(float coeff, float duration);
    void setFireBall(float duration);

private:
    sf::CircleShape _sprite;
    sf::Vector2f _direction;
    sf::RenderWindow* _window;
    float _velocity;
    float _currentVelocity;
    bool _isAlive;
    bool _isFireBall;
    float _timeWithPowerupEffect;
    float _particleSpawnTimer;

    GameManager* _gameManager;  // Reference to the GameManager
    ParticleSystem* _particleSystem; // Reference to particle system 


    static constexpr float RADIUS = 10.0f;      
    static constexpr float VELOCITY = 350.0f;   // for reference.
};

