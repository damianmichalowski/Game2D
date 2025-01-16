#include "Bullet.h"

#include <iostream>

#include "Math.h"

Bullet::Bullet() : speed(0), damage(10.0f) {
}

Bullet::~Bullet() {
    std::cout << "Bullet destructed" << std::endl;
}

void Bullet::Initialize(const sf::Vector2f &position, sf::Vector2f &direction, float speed, float lifeTime) {
    this->speed = speed;
    this->maxAliveTime = lifeTime;
    this->lifeTime = 0;
    this->isAlive = true;

    bulletShape.setSize(sf::Vector2f(6, 6));
    bulletShape.setPosition(position);
    this->direction = Math::NormalizeVector(direction);
}

void Bullet::Update(float deltaTime) {
    const sf::Vector2f newPosition = bulletShape.getPosition() + direction * speed * deltaTime;
    bulletShape.setPosition(newPosition);

    lifeTime += deltaTime;
    if (lifeTime >= maxAliveTime) {
        lifeTime = 0;
        isAlive = false;
    }
}


void Bullet::Draw(sf::RenderWindow &window) {
    if (isAlive) {
        window.draw(bulletShape);
    }
}

float Bullet::GetDamage() const {
    return damage;
}

bool Bullet::CheckCollision(const sf::RectangleShape& target) const {
    return isAlive && bulletShape.getGlobalBounds().intersects(target.getGlobalBounds());
}

void Bullet::SetAlive(bool isAlive) {
    this->isAlive = isAlive;
}

bool Bullet::IsAlive() const {
    return isAlive;
}