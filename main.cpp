#include <iostream>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Skeleton.h"

auto main() -> int {
    //-----------------INIT-------------------
    auto window = sf::RenderWindow(
        sf::VideoMode(1200, 680), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,8)
    );
    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    //-----------------INIT-------------------

    //-----------------LOAD-------------------
    Player player;
    Skeleton skeleton;

    player.Initialize();
    skeleton.Initialize();

    player.Load();
    skeleton.Load();
    //-----------------LOAD-------------------

    // Main Loop
    sf::Clock clock;

    while (window.isOpen()){
        sf::Time deltaTimeTimer = clock.restart();
        float deltaTime = deltaTimeTimer.asMilliseconds();
        //-----------------UPDATE-------------------
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        skeleton.Update(deltaTime);
        player.Update(deltaTime , skeleton);
        //-----------------UPDATE-------------------


        //-----------------DRAW-------------------
        window.clear(sf::Color::Black);
        player.Draw(window);
        skeleton.Draw(window);
        window.display();
        //-----------------DRAW-------------------
    }
    return 0;
}