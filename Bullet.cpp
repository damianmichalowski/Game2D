#include "Bullet.h"
#include "Math.h"

Bullet::Bullet() : speed(0), damage(10.0f) {
}

Bullet::~Bullet() {
}

void Bullet::Initialize(const sf::Vector2f &position, sf::Vector2f &direction, float speed, float lifeTime) {
    this->speed = speed;
    this->maxAliveTime = lifeTime;
    this->lifeTime = 0;
    this->isAlive = true;

    bulletShape.setSize(sf::Vector2f(10, 10));
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

bool Bullet::CheckCollision(const sf::Sprite& target) const {
    return isAlive && bulletShape.getGlobalBounds().intersects(target.getGlobalBounds());
}

void Bullet::SetInactive() {
    isAlive = false;
}

bool Bullet::IsAlive() const {
    return isAlive;
}
