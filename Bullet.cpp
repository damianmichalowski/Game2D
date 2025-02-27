#include "Bullet.h"

#include <iostream>

#include "Globals.h"
#include "Math.h"

Bullet::Bullet() : aliveTimer(0), isAlive(true), frameRect(0, 32, 32, 32) {
}

Bullet::~Bullet() {
    std::cout << "Bullet destructed" << std::endl;
}

void Bullet::Initialize(const sf::Vector2f &position, sf::Vector2f &direction, int damage, float speed, float maxAliveTime, Type type) {
    this->damage = damage;
    this->speed = speed;
    this->maxAliveTime = maxAliveTime;
    bulletShape.setSize(sf::Vector2f(13, 13));
    bulletShape.setPosition(position);
    bulletShape.setFillColor(sf::Color::Transparent);
    if(Globals::IsDebugMode()) {
        bulletShape.setFillColor(sf::Color::Red);
    }

    switch (type) {
        case Default:
            if(!defaultTexture.loadFromFile("../Assets/Bullet/eyeTear.png")) {
                std::cerr << "Failed to load eyeTear texture" << std::endl;
            }
            texture = &defaultTexture;
        break;
        case Player:
            if(!tearTexture.loadFromFile("../Assets/Bullet/redEyeTear.png")) {
                std::cerr << "Failed to load eyeTear texture" << std::endl;
            }
            texture = &tearTexture;
        break;
        case Skeleton:
            if(!skeletonBulletTexture.loadFromFile("../Assets/Bullet/inchTear.png")) {
                std::cerr << "Failed to load inchTear texture" << std::endl;
            }
            texture = &skeletonBulletTexture;
        break;
        case Bomb:
            if(!bombTexture.loadFromFile("../Assets/Bullet/bombTear.png")) {
                std::cerr << "Failed to load bombTear texture" << std::endl;
            }
        texture = &bombTexture;
        break;
    }


    sprite.setTexture(*texture);
    sprite.setTextureRect(frameRect);
    sprite.setOrigin(frameRect.width / 2.0f, frameRect.height / 2.0f);
    sprite.setPosition(bulletShape.getPosition() + bulletShape.getSize() / 2.0f);

    sf::Vector2f scale(
        bulletShape.getSize().x / static_cast<float>(frameRect.width),
        bulletShape.getSize().y / static_cast<float>(frameRect.height)
    );
    sprite.setScale(scale);

    this->direction = Math::NormalizeVector(direction);
    float angle = std::atan2(this->direction.y, this->direction.x) * 180.0f / 3.14159265f;
    sprite.setRotation(angle);
}

void Bullet::Update(float deltaTime) {
    const sf::Vector2f newPosition = bulletShape.getPosition() + direction * speed * deltaTime;
    bulletShape.setPosition(newPosition);
    sprite.setPosition(bulletShape.getPosition() + bulletShape.getSize() / 2.0f);

    aliveTimer += deltaTime;
    AnimateBullet(deltaTime);

    if (aliveTimer >= maxAliveTime) {
        aliveTimer = 0;
        isAlive = false;
    }
}


void Bullet::Draw(sf::RenderWindow &window) {
    if (isAlive) {
        window.draw(bulletShape);
        window.draw(sprite);
    }
}

void Bullet::AnimateBullet(float deltaTime) {
    const int framesPerColumn = 5;

    int currentFrame = static_cast<int>((aliveTimer / 300) * framesPerColumn);

    if (currentFrame >= framesPerColumn) {
        currentFrame = framesPerColumn - 1;
    }

    frameRect.left = currentFrame * 32;
    frameRect.top = 32;
    if(texture == &skeletonBulletTexture) {
        frameRect.top = 97;
    }
    sprite.setTextureRect(frameRect);
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