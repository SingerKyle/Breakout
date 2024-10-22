#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include <iostream>

int main()
{

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Breakout");
    sf::View _view(sf::FloatRect(0, 0, 1000, 800));
    _view.setCenter(500, 400);
    GameManager gameManager(&window, &_view);
    gameManager.initialize();

    sf::Clock clock;
    float deltaTime;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.setView(_view);

        deltaTime = clock.restart().asSeconds();

        gameManager.update(deltaTime);

        window.clear();
        gameManager.render();
        window.display();
    }

    return 0;
}
