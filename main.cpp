#include <iostream>
#include <SFML/Graphics.hpp>

#include "FrameRate.h"
#include "Player.h"
#include "Skeleton.h"

auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode(1200, 680), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,8)
    );
    window.setFramerateLimit(60);

    Player player;
    Skeleton skeleton;
    FrameRate frameRate;

    frameRate.Initialize();
    player.Initialize();
    skeleton.Initialize();

    frameRate.Load();
    player.Load();
    skeleton.Load();


    sf::Clock clock;
    while (window.isOpen()){
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        sf::Time deltaTimeTimer = clock.restart();
        double deltaTime = deltaTimeTimer.asMicroseconds() / 1000.0; // how long to render single frame
        frameRate.Update(deltaTime);

        skeleton.Update(deltaTime);
        player.Update(deltaTime , skeleton);

        window.clear(sf::Color::Black);
        player.Draw(window);
        skeleton.Draw(window);
        frameRate.Draw(window);
        window.display();
    }
    return 0;
}