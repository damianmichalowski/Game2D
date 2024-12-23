#include <iostream>
#include "Player.h"
#include "Math.h"

Player::Player() :
playerSpeed(1.0f),
maxFireRate(150),
fireRateTimer(0){
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


void Player::Update(float deltaTime ,Skeleton& skeleton, sf::Vector2f& mousePosition) {
    sf::Vector2f position = sprite.getPosition();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        sprite.setPosition(position + sf::Vector2(1.0f,0.0f) * playerSpeed * deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        sprite.setPosition(position + sf::Vector2(-1.0f,0.f) * playerSpeed * deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        sprite.setPosition(position + sf::Vector2(0.f,-1.f) * playerSpeed * deltaTime);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        sprite.setPosition(position + sf::Vector2(0.f,1.f) * playerSpeed * deltaTime);

    fireRateTimer += deltaTime;

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireRateTimer >= maxFireRate) {
        bullets.push_back(Bullet());
        int i = bullets.size() - 1;
        bullets[i].Initialize(sprite.getPosition(), mousePosition, 0.5f);
        fireRateTimer = 0;
    }

    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].Update(deltaTime);

        if(skeleton.health > 0) {
            if(Math::CheckCollision(bullets[i].GetGlobalBounds(), skeleton.sprite.getGlobalBounds())) {
                std::cout << "collision" << std::endl;
                skeleton.ChangeHealth(-10);
                bullets.erase(bullets.begin() + i);
            }
        }
    }

    boundingBox.setPosition(sprite.getPosition());

    if(Math::CheckCollision(sprite, skeleton.sprite)) {
        std::cout << "Collision" << std::endl;
    }

}


void Player::Draw(sf::RenderWindow& window){
    window.draw(sprite);
    for (Bullet& bullet: bullets) {
        bullet.Draw(window);
    }
    window.draw(boundingBox);
}
