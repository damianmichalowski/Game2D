#include "FireObstacle.h"

FireObstacle::FireObstacle(const sf::Vector2i& position) : Obstacle(position),animationTimer(0), animationDuration(200),currentFrame(0) {
    isDanger = true;         // Fire is dangerous
    isDestroyable = false;   // Fire is not destroyable
    Initialize();
}

FireObstacle::~FireObstacle() {
    std::cout << "FireObstacle zniszczony" << std::endl;
}

void FireObstacle::Initialize() {
    std::cout << "FireObstacle init" << std::endl;
    if (!texture.loadFromFile("../Assets/Map/fire.png")) {
        throw std::runtime_error("Failed to load fire texture!");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(currentFrame * 24, 0, 24, 32));
    sprite.setPosition(position.x * 32 + 15, position.y * 32 + 4);
    sprite.setOrigin(sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f);
    sprite.setScale(2.2f, 1.7f);
}

void FireObstacle::Update(const float &deltaTime) {
    Obstacle::Update(deltaTime);
    HandleFireAnimation(deltaTime);
};

void FireObstacle::HandleFireAnimation(const float &deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationDuration) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 8;

        sprite.setTextureRect(sf::IntRect(currentFrame * 24, 0, 24, 32));
    }
}