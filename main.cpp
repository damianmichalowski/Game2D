#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Dungeon.h"
#include "FrameRate.h"

auto main() -> int {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width * 0.8, desktopMode.height * 0.8), "Game2D", sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(180);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Music music;
    if (!music.openFromFile("../Assets/Music/zombie.wav")) {
        std::cerr << "music faild to load" << std::endl;
    }


    FrameRate frameRate;
    Dungeon dungeon;

    sf::View view(sf::FloatRect(0, 0, 416, 224));
    window.setView(view);

    frameRate.Load();
    dungeon.Load();

    frameRate.Initialize(view);
    dungeon.Initialize();

    music.setLoop(true);
    music.setVolume(50);

    music.play();

    sf::Clock clock;
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Time deltaTimeTimer = clock.restart();
        float deltaTime = deltaTimeTimer.asMicroseconds() / 1000.0;

        frameRate.Update(deltaTime);
        dungeon.Update(deltaTime);

        window.clear(sf::Color::Black);

        dungeon.Draw(window);
        frameRate.Draw(window);

        window.display();
    }

    return 0;
}
