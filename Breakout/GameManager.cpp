#include "GameManager.h"
#include "Ball.h"
#include "PowerupManager.h"
#include <cmath>
#include <iostream>

class ParticleSystem;

GameManager::GameManager(sf::RenderWindow* window, sf::View* view)
    : _window(window), _view(view), _paddle(nullptr), _ball(nullptr), _brickManager(nullptr), _powerupManager(nullptr),
    _messagingSystem(nullptr), _ui(nullptr), _particleSystem(nullptr), _pause(false), _time(0.f), _lives(3), _pauseHold(0.f), _levelComplete(false),
    _powerupInEffect({ none,0.f }), _timeLastPowerupSpawned(0.f)
{
    _font.loadFromFile("font/montS.ttf");
    _masterText.setFont(_font);
    _masterText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    _masterText.setCharacterSize(48);
    _masterText.setFillColor(sf::Color::Yellow);
    _isUsingMouse = false;

    _shakeIntensity = 100.f;
    _shakeDuration = 1.5f;
    _shakeTimer = 0.0f;
    _decayTimescale = 0.1f;
}

void GameManager::initialize()
{
    //_paddle = new Paddle(_window);
    //_brickManager = new BrickManager(_window, this);
    //_messagingSystem = new MessagingSystem(_window);
    //_ball = new Ball(_window, 400.0f, this); 
    //_powerupManager = new PowerupManager(_window, _paddle, _ball);
    //_ui = new UI(_window, _lives, this);

    _paddle = std::make_unique<Paddle>(_window);
    _brickManager = std::make_unique<BrickManager>(_window, this);
    _messagingSystem = std::make_unique<MessagingSystem>(_window);
    _particleSystem = std::make_unique<ParticleSystem>(_window);
    _ball = std::make_unique<Ball>(_window, 400.0f, this, _particleSystem.get());
    _powerupManager = std::make_unique<PowerupManager>(_window, _paddle.get(), _ball.get());
    _ui = std::make_unique<UI>(_window, _lives, this);

    // Create bricks
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
}

void GameManager::Restart()
{
    _lives = 3;
    _time = 0.f;
    _levelComplete = false;
    _powerupInEffect = { none, 0.f };
    _timeLastPowerupSpawned = 0.f;

    _masterText.setString("");

    // Re-initialize game components
    _brickManager->createBricks(5, 10, 80.0f, 30.0f, 5.0f);
    _paddle->reset();
    _ball->reset();
    _ui->reset(3);
}

void GameManager::update(float dt)
{
    _powerupInEffect = _powerupManager->getPowerupInEffect();
    _ui->updatePowerupText(_powerupInEffect);
    _powerupInEffect.second -= dt;
    
    // https://www.gamedev.net/forums/topic/651410-camera-shake/
    if (_shaking)
    {
        // start timer
        _shakeTimer += dt;
        
        if (_shakeTimer < _shakeDuration)
        {
            float decayFactor = exp(-_shakeTimer / _decayTimescale);
            
            // set center to oscillating offset
            _view->setCenter(VIEW_WIDTH + _shakeIntensity * sin(2 * _shakeTimer) * decayFactor, VIEW_HEIGHT + _shakeIntensity * sin(2 * _shakeTimer + 3.14f) * decayFactor);
        }
        else
        {
            _view->setCenter(VIEW_WIDTH, VIEW_HEIGHT);
            _shaking = false;
        }
    }

    if (_lives <= 0)
    {
        _masterText.setString("Game over.");
    }
    if (_levelComplete)
    {
        _masterText.setString("Level completed.");
    }

    // restart game on enter
    if (_levelComplete && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || _lives <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        Restart();
    }

    if (_levelComplete || _lives <= 0)
    {
        return;
    }

    // pause and pause handling
    if (_pauseHold > 0.f) _pauseHold -= dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        if (!_pause && _pauseHold <= 0.f)
        {
            _pause = true;
            _masterText.setString("paused.");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
        if (_pause && _pauseHold <= 0.f)
        {
            _pause = false;
            _masterText.setString("");
            _pauseHold = PAUSE_TIME_BUFFER;
        }
    }
    if (_pause)
    {
        return;
    }

    // timer.
    _time += dt;


    if (_time > _timeLastPowerupSpawned + POWERUP_FREQUENCY && rand()% POWERUP_SPAWN_CHANCE == 0)      // TODO parameterise
    {
        _powerupManager->spawnPowerup();
        _timeLastPowerupSpawned = _time;
    }

    // move paddle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) _paddle->moveRight(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) _paddle->moveLeft(dt);

    // If enabled, use mouse input
    if (_isUsingMouse)
    {
        _paddle->moveWithMouse(dt);
    }

    // update everything 
    _paddle->update(dt);
    _ball->update(dt);
    _powerupManager->update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
    {
        _isUsingMouse = !_isUsingMouse;
    }
}

void GameManager::loseLife()
{
    _lives--;
    _ui->lifeLost(_lives);

    // TODO screen shake.
    _shaking = true;
    _shakeTimer = 0.0f;
}

void GameManager::render()
{
    _paddle->render();
    _ball->render();
    _brickManager->render();
    _powerupManager->render();
    _window->draw(_masterText);
    _ui->render();
}

void GameManager::levelComplete()
{
    _levelComplete = true;
}

sf::RenderWindow* GameManager::getWindow() const { return _window; }
UI* GameManager::getUI() const { return _ui.get(); }
Paddle* GameManager::getPaddle() const { return _paddle.get(); }
BrickManager* GameManager::getBrickManager() const { return _brickManager.get(); }
PowerupManager* GameManager::getPowerupManager() const { return _powerupManager.get(); }
