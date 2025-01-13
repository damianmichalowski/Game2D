#include <iostream>
#include <SFML/Graphics.hpp>

#include "FrameRate.h"
#include "Player.h"
#include "Skeleton.h"
#include "Room.h"

auto main() -> int {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktopMode.width * 0.8, desktopMode.height * 0.8), "Game2D", sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(180);

    Room room = Room(13,7,32,Difficulty::Easy);
    Player player;
    FrameRate frameRate;

    int roomWidthPx = room.GetRoomWidthPX();
    int roomHeightPx = room.GetRoomHeightPX();

    sf::View view(sf::FloatRect(0, 0, roomWidthPx, roomHeightPx));
    window.setView(view);

    room.Initialize();
    frameRate.Initialize();
    player.Initialize(room);

    frameRate.Load();
    player.Load();

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
        player.Update(deltaTime);
        room.Update(deltaTime, player);

        window.clear(sf::Color::Black);

        room.Draw(window);
        player.Draw(window, view);
        frameRate.Draw(window);

        window.display();
    }

    return 0;
}
