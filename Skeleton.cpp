#include "Skeleton.h"
#include <iostream>

void Skeleton::Initialize() {

}

void Skeleton::Load() {
    if (!texture.loadFromFile("../Assets/Skeleton/Textures/BODY_skeleton.png")) {
        std::cerr << "Failed to load skeleton.png" << std::endl;
    }

    sprite.setTexture(texture);

    int spriteXIndex = 0;
    int spriteYIndex = 2;
    sprite.setTextureRect(sf::IntRect(spriteXIndex * 64, spriteYIndex * 64, 64, 64));
    sprite.setScale(sf::Vector2f(3,3));
    sprite.setPosition(sf::Vector2f(400,100));
}

void Skeleton::Update() {

}

void Skeleton::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
