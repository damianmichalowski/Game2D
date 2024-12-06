#include "Skeleton.h"
#include <iostream>

void Skeleton::Initialize() {
    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setFillColor(sf::Color::Transparent);

    size = sf::Vector2i(64,64);
}

void Skeleton::Load() {
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
}

void Skeleton::Update() {
    boundingBox.setPosition(sprite.getPosition());
}

void Skeleton::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    window.draw(boundingBox);
}
