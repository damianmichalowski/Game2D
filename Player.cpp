#include <iostream>
#include "Player.h"
#include "Math.h"

Player::Player() : bulletSpeed(0.5f), playerSpeed(1.0f) {
}

Player::~Player() {
}

void Player::Initialize() {
    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Green);
    boundingBox.setFillColor(sf::Color::Transparent);

    size = sf::Vector2i(64,64);
}

void Player::Load(){
    if (!texture.loadFromFile("../Assets/Player/Textures/BODY_male.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
    }

    sprite.setTexture(texture);

    // max x: 8, y: 3
    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(1,1));
    boundingBox.setSize(sf::Vector2f(size.x * sprite.getScale().x,size.y * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * size.x, spriteYIndex * size.y, size.x, size.y));
    sprite.setPosition(sf::Vector2f(1000,600));
}


void Player::Update(double deltaTime ,Skeleton& skeleton) {
    sf::Vector2f position = sprite.getPosition();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        sprite.setPosition(position + sf::Vector2(1.0f,0.0f) * playerSpeed * (float)deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        sprite.setPosition(position + sf::Vector2(-1.0f,0.f) * playerSpeed * (float)deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        sprite.setPosition(position + sf::Vector2(0.f,-1.f) * playerSpeed * (float)deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        sprite.setPosition(position + sf::Vector2(0.f,1.f) * playerSpeed * (float)deltaTime);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        bullets.push_back(sf::RectangleShape(sf::Vector2f(10,10)));

        int i = bullets.size() - 1;
        bullets[i].setPosition(sprite.getPosition());
    }

    for (sf::RectangleShape& bullet: bullets) {
        sf::Vector2f bulletDirection = skeleton.sprite.getPosition() - bullet.getPosition();
        bulletDirection = Math::NormalizeVector(bulletDirection);
        bullet.setPosition(bullet.getPosition() + bulletDirection * bulletSpeed * (float)deltaTime);
    }

    boundingBox.setPosition(sprite.getPosition());

    if(Math::CheckCollision(sprite, skeleton.sprite)) {
        std::cout << "Collision" << std::endl;
    }

}


void Player::Draw(sf::RenderWindow& window){
    window.draw(sprite);
    for (const sf::RectangleShape& bullet: bullets) {
        window.draw(bullet);
    }
    window.draw(boundingBox);
}
