#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>

auto NormalizeVector(sf::Vector2f vector) -> sf::Vector2f {
    //chcemy wektor podzielić na kawałki o długości 1 z zachowaniem kierunku
    const float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    if (magnitude == 0) {
        return {0, 0};
    }

    sf::Vector2f normalizedVector;

    normalizedVector.x = vector.x / magnitude;
    normalizedVector.y = vector.y / magnitude;

    return normalizedVector;
};
auto main() -> int {
    //-----------------INIT-------------------
    auto window = sf::RenderWindow(
        sf::VideoMode(1920, 1080), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,8)
    );
    // window.setFramerateLimit(500);
    //-----------------INIT-------------------
    auto bullets = std::vector<sf::RectangleShape>();
    // auto bullet = sf::RectangleShape(sf::Vector2f(10, 10));
    // bullet.setFillColor(sf::Color::Red);
    float bulletSpeed = 3.0f;
    //-----------------LOAD-------------------
    //-----------------PLAYER-------------------
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
    playerSprite.setScale(sf::Vector2f(2,2));
    playerSprite.setPosition(sf::Vector2f(1000,600));
    //-----------------PLAYER-------------------
    // bullet.setPosition(playerSprite.getPosition());

    //-----------------SKELETON-------------------
    auto skeletonTexture = sf::Texture();

    if (!skeletonTexture.loadFromFile("../Assets/Skeleton/Textures/BODY_skeleton.png")) {
        std::cerr << "Failed to load skeleton.png" << std::endl;
        return 1;
    }

    auto skeletonSprite = sf::Sprite(skeletonTexture);
    skeletonSprite.setPosition(sf::Vector2f(400,100));

    int skeletonSpriteXIndex = 0;
    int skeletonSpriteYIndex = 2;
    skeletonSprite.setTextureRect(sf::IntRect(skeletonSpriteXIndex * 64, skeletonSpriteYIndex * 64, 64, 64));
    skeletonSprite.setScale(sf::Vector2f(3,3));
    //-----------------SKELETON-------------------
    //-----------------LOAD-------------------

    //-----------------Calculate direction of the bullet-------------------
    // sf::Vector2f bulletDirection = skeletonSprite.getPosition() - bullet.getPosition();
    // bulletDirection = NormalizeVector(bulletDirection);
    //-----------------Calculate direction of the bullet-------------------

    // Main Loop
    while (window.isOpen()){
        //-----------------UPDATE-------------------
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // auto bulletPosition = bullet.getPosition();
        // bullet.setPosition(bulletPosition + bulletDirection * bulletSpeed);

        auto position = playerSprite.getPosition();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            playerSprite.setPosition(position + sf::Vector2(1.0f,0.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            playerSprite.setPosition(position + sf::Vector2(-1.0f,0.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            playerSprite.setPosition(position + sf::Vector2(0.f,-1.f));

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            playerSprite.setPosition(position + sf::Vector2(0.f,1.f));

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            bullets.push_back(sf::RectangleShape(sf::Vector2f(10,10)));

            int i = bullets.size() - 1;
            bullets[i].setPosition(playerSprite.getPosition());
        }

        for (sf::RectangleShape& bullet: bullets) {
            sf::Vector2f bulletDirection = skeletonSprite.getPosition() - bullet.getPosition();
            bulletDirection = NormalizeVector(bulletDirection);
            bullet.setPosition(bullet.getPosition() + bulletDirection * bulletSpeed);
        }
        //-----------------UPDATE-------------------


        //-----------------DRAW-------------------
        window.clear(sf::Color::Black);
        window.draw(playerSprite);
        window.draw(skeletonSprite);
        for (const sf::RectangleShape& bullet: bullets) {
            window.draw(bullet);
        }
        window.display();
        //-----------------DRAW-------------------
    }
    return 0;
}