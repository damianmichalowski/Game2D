#include "Obstacle.h"
Obstacle::Obstacle(const sf::Vector2i position): position(position) {
    sprite.setPosition(static_cast<float>(position.x), static_cast<float>(position.y));
}

Obstacle::~Obstacle() {
    std::cout << "Obstacle zniszczony" << std::endl;
}

void Obstacle::Initialize() {
}


void Obstacle::Update(const float &deltaTime) {
    //logika do naspiania
}

void Obstacle::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Obstacle::IsDanger() const { return isDanger; }
bool Obstacle::IsDestroyable() const { return isDestroyable; }
const sf::Sprite& Obstacle::GetSprite() const { return sprite; }
const sf::Vector2i& Obstacle::GetPosition() const { return position; }
sf::RectangleShape Obstacle::GetObstacleRect() const {
    sf::RectangleShape obstacleRect;
    obstacleRect.setSize(sf::Vector2f(32, 32));
    obstacleRect.setPosition(static_cast<float>(position.x * 32), static_cast<float>(position.y * 32));
    return obstacleRect;
}