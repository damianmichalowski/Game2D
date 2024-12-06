#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Skeleton.h"

auto main() -> int {
    //-----------------INIT-------------------
    auto window = sf::RenderWindow(
        sf::VideoMode(1920, 1080), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,8)
    );
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
    while (window.isOpen()){
        //-----------------UPDATE-------------------
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        skeleton.Update();
        player.Update(skeleton);
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