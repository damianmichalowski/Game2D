#include <iostream>
#include <SFML/Graphics.hpp>

auto main() -> int {
    //-----------------INIT-------------------
    auto window = sf::RenderWindow(
        sf::VideoMode(800, 600), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,0)
    );
    window.setFramerateLimit(240);
    auto playerTexture = sf::Texture();
    if (!playerTexture.loadFromFile("../Assets/Player/Textures/BODY_male.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
        return 1;
    }
    auto playerSprite = sf::Sprite(playerTexture);
    // max x: 8, y: 3
    int playerSpriteXIndex = 0;
    int playerSpriteYIndex = 2;
    playerSprite.setTextureRect(sf::IntRect(playerSpriteXIndex * 64, playerSpriteYIndex * 64, 64, 64));
    playerSprite.setScale(sf::Vector2f(1,1));
    //-----------------INIT-------------------

    // Main Loop
    while (window.isOpen()){
        //-----------------UPDATE-------------------
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        auto position = playerSprite.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            playerSprite.setPosition(position + sf::Vector2(1.0f,0.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            playerSprite.setPosition(position + sf::Vector2(-1.0f,0.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            playerSprite.setPosition(position + sf::Vector2(0.f,-1.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            playerSprite.setPosition(position + sf::Vector2(0.f,1.f));
        //-----------------UPDATE-------------------


        //-----------------DRAW-------------------
        window.clear(sf::Color::Black);
        window.draw(playerSprite);
        window.display();
        //-----------------DRAW-------------------
    }
}