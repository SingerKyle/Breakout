#include "BrickManager.h"
#include "GameManager.h"
#include <thread>
#include <vector>
#include <atomic>


BrickManager::BrickManager(sf::RenderWindow* window, GameManager* gameManager)
    : _window(window), _gameManager(gameManager)
{
}

void BrickManager::createBricks(int rows, int cols, float brickWidth, float brickHeight, float spacing)
{
    _bricks.clear();

    float leftEdge;
    if (cols % 2 == 0) 
        leftEdge = _window->getSize().x / 2 - ((cols / 2.0f) * brickWidth + (cols / 2.0f - 0.5f) * spacing);
    else
        leftEdge = _window->getSize().x / 2 - ((cols / 2.0f - 0.5f) * brickWidth + (cols / 2.0f) * spacing);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * (brickWidth + spacing) + leftEdge;
            float y = i * (brickHeight + spacing) + TOP_PADDING;
            _bricks.emplace_back(x, y, brickWidth, brickHeight);
        }
    }
}

void BrickManager::render()
{
    for (auto& brick : _bricks) {
        brick.render(*_window);
    }
}

void BrickManager::CheckBrickCollision(int start, int end, sf::CircleShape& ball)
{
    for (int i = start; i < end; i++)
    {
        if (_collisionResponse.load() != 0)
        {
            return; // don't continue if collision has been found - probably a better way of doing this.
        }

        if (!_bricks[i].getBounds().intersects(ball.getGlobalBounds())) // if brick isn't colliding with ball skip it.
        {
            continue;
        }

        sf::Vector2f ballPosition = ball.getPosition();
        float ballY = ballPosition.y + 0.5f * ball.getGlobalBounds().height;
        sf::FloatRect brickBounds = _bricks[i].getBounds();

        // default vertical bounce (collision is top/bottom)
        int response = 2;
        if (ballY > brickBounds.top && ballY < brickBounds.top + brickBounds.height)
            // unless it's horizontal (collision from side)
            response = 1;

        // Lock mutex to safely modify shared resources
        {
            std::lock_guard<std::mutex> lock(_brickMutex);
        // Mark the brick as destroyed
        _bricks[i] = _bricks.back();
        _bricks.pop_back();
        _collisionResponse.store(response);
        }
        return; // Exit once collision is handled
    }
}

int BrickManager::checkCollision(sf::CircleShape& ball, sf::Vector2f& direction)
{
    //https://www.geeksforgeeks.org/multithreading-in-cpp/
    _collisionResponse.store(0);

    // split into parts based on threads and size of brick vector
    size_t splitSize = _bricks.size() / numThreads;

    for (int i = 0; i < numThreads; i++)
    {
        int start = i * splitSize;
        int end;
        if (i == numThreads - 1) // if current thread is the last one.
        {
            end = _bricks.size(); // set end to be the highest value
        }
        else
        {
            end = start + splitSize; // else end is the splitsize calculated above
        }

        // https://stackoverflow.com/questions/11833070/when-is-the-use-of-stdref-necessary
        _threads.emplace_back(&BrickManager::CheckBrickCollision, this, start, end, std::ref(ball));
    }

    for (int i = 0; i < _threads.size(); i++) 
    {
        _threads[i].join();
    }

    if (_bricks.size() == 0)
    {
        _gameManager->levelComplete();
    }

    _threads.clear(); // empty it for next run through
    return _collisionResponse.load();
}

std::vector<Brick> BrickManager::getBricks()
{
    return _bricks;
}
