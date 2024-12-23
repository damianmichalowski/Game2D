#include "Bullet.h"
#include "Math.h"

Bullet::Bullet() : speed(0) {
}

Bullet::~Bullet() {
}

void Bullet::Initialize(const sf::Vector2f& position, const sf::Vector2f& target, float speed) {
    this->speed = speed;
    bulletShape.setSize(sf::Vector2f(10, 5));
    bulletShape.setPosition(position);
    direction = target - position;
    direction = Math::NormalizeVector(direction);
}

void Bullet::Update(float deltaTime) {
    const sf::Vector2f newPosition = bulletShape.getPosition() + direction * speed * deltaTime;
    bulletShape.setPosition(newPosition);
}

void Bullet::Draw(sf::RenderWindow &window) {
    window.draw(bulletShape);
}
