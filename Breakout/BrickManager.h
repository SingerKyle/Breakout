#pragma once
#include <vector>
#include "Brick.h"
#include <SFML/Graphics.hpp>
#include "CONSTANTS.h"
#include <mutex>

class GameManager;

class BrickManager 
{
public:
    BrickManager(sf::RenderWindow* window, GameManager* gameManager);
    void createBricks(int rows, int cols, float brickWidth, float brickHeight, float spacing);
    void render();
    void CheckBrickCollision(int start, int end, sf::CircleShape& ball);
    int checkCollision(sf::CircleShape& ball, sf::Vector2f& direction);

    std::vector<Brick> getBricks();

private:
    std::vector<Brick> _bricks;
    sf::RenderWindow* _window;

    GameManager* _gameManager;
    static constexpr float TOP_PADDING = 100.0f;

    // used to protect shared bricks vector
    std::mutex _brickMutex;

    std::atomic<int> _collisionResponse;

    std::vector<std::thread> _threads;
};
