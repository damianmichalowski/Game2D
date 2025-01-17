#include "RockObstacle.h"

RockObstacle::RockObstacle(const sf::Vector2i& position) : Obstacle(position) {
    isDanger = false;
    isDestroyable = true;
    Initialize();
}

RockObstacle::~RockObstacle() {
    std::cout << "RockObstacle zniszczony" << std::endl;
}

void RockObstacle::Initialize() {
    if (!texture.loadFromFile("../Assets/Map/rocks.png")) {
        throw std::runtime_error("Failed to load rock texture!");
    }
    sprite.setTexture(texture);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(2 * 24, 24, 24, 24));
    sprite.setPosition(position.x * 32 + 15, position.y * 32 + 4);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 14.f);
     float scaleX = 32.f / sprite.getLocalBounds().width;
    float scaleY = 32.f / sprite.getLocalBounds().height;
    sprite.setScale(scaleX, scaleY);
}