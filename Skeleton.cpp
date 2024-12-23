#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton() : health(100) {
}

Skeleton::~Skeleton() {
    std::cout << "Skeleton destruktorized" << std::endl;
}

void Skeleton::ChangeHealth(int hp) {
    health += hp;
    healthText.setString(std::to_string(health));
}

void Skeleton::Initialize() {
    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setFillColor(sf::Color::Transparent);

    size = sf::Vector2i(64,64);
}

void Skeleton::Load() {
    if (font.loadFromFile("../Assets/Fonts/Oswald-Regular.ttf")) {
        std::cout << "Font in Skeleton loaded" << std::endl;
        healthText.setFont(font);
        healthText.setString(std::to_string(health));
        healthText.setScale(0.6f, 0.6f);
    } else {
        std::cerr << "Failed to load Oswald font in Skeleton." << std::endl;
    }

    if (!texture.loadFromFile("../Assets/Skeleton/Textures/BODY_skeleton.png")) {
        std::cerr << "Failed to load skeleton.png" << std::endl;
    }

    sprite.setTexture(texture);

    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(1,1));
    boundingBox.setSize(sf::Vector2f(size.x * sprite.getScale().x,size.y * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * size.x, spriteYIndex * size.y, size.x, size.y));
    sprite.setPosition(sf::Vector2f(400,100));
    healthText.setPosition(sprite.getPosition());
}

void Skeleton::Update(float deltaTime) {
    if (health > 0 ) {
        boundingBox.setPosition(sprite.getPosition());
    }
}

void Skeleton::Draw(sf::RenderWindow& window) {
    if (health > 0) {
        window.draw(sprite);
        window.draw(boundingBox);
        window.draw(healthText);
    }
}
