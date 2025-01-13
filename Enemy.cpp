#include <iostream>
#include "Enemy.h"
#include "Animate.h"
#include "Math.h"

Enemy::Enemy(const float x, const float y) : posX(x), posY(y), health(100), damage(1), speed(0.05f), isAlive(true), animationTimer(0.0f), animationSpeed(150.0f), currentFrame(0), frameCount(8) {}

Enemy::~Enemy() {
    std::cout << "Enemy destructed" << std::endl;
}

void Enemy::Initialize() {
    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setFillColor(sf::Color::Transparent);

    visionBox.setOutlineThickness(2);
    visionBox.setOutlineColor(sf::Color::Yellow);
    visionBox.setFillColor(sf::Color::Transparent);

    size = sf::Vector2i(64, 64);
    sprite.setTexture(texture);

    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(0.25f, 0.25f));
    boundingBox.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));
    visionBox.setSize(sf::Vector2f(size.x * 12 * sprite.getScale().x, size.y * 12 * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * size.x, spriteYIndex * size.y, size.x, size.y));
    sprite.setPosition(sf::Vector2f(posX, posY));
    healthText.setPosition(sprite.getPosition());
}

void Enemy::Load() {
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
}

void Enemy::Update(float deltaTime, Player& player) {
    if (health <= 0) {
        isAlive = false;
        return;
    }

    boundingBox.setPosition(sprite.getPosition());

    sf::Vector2f spriteCenter = sprite.getPosition() + sf::Vector2f(size.x * sprite.getScale().x / 2, size.y * sprite.getScale().y / 2);
    sf::Vector2f visionBoxOffset = sf::Vector2f(visionBox.getSize().x / 2, visionBox.getSize().y / 2);
    visionBox.setPosition(spriteCenter - visionBoxOffset);

    healthText.setPosition(sprite.getPosition());

    bool isPlayerInVision = Math::CheckCollision(visionBox.getGlobalBounds(), player.GetSprite().getGlobalBounds());

    if (isPlayerInVision) {
        direction = player.GetSprite().getPosition() - sprite.getPosition();
        direction = Math::NormalizeVector(direction);

        sprite.setPosition(sprite.getPosition() + direction * speed * deltaTime);
    }

    HandleAnimation(deltaTime, isPlayerInVision);

    CheckIsPlayerCollision(sprite, player, player.IsImmortal());

}

void Enemy::Draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(sprite);
        window.draw(boundingBox);
        window.draw(visionBox);
        window.draw(healthText);
    }
}

void Enemy::CheckIsPlayerCollision(sf::Sprite& sprite,Player& player, bool isImmortal) {
    if (Math::CheckCollision(sprite.getGlobalBounds(), player.GetGlobalBounds()) && !isImmortal) {
        std::cout << "Player get hit" << std::endl;
        player.TakeDamage(1);
    }
}

void Enemy::TakeDamage(int hp) {
    health -= hp;
    healthText.setString(std::to_string(health));
}

void Enemy::HandleAnimation(float deltaTime, bool isPlayerInVision) {
    animationTimer += deltaTime;

    if (isPlayerInVision) {
        if (direction != sf::Vector2f(0.0f, 0.0f)) {
            if (animationTimer >= animationSpeed) {
                currentFrame = (currentFrame + 1) % frameCount;
                animationTimer = 0.0f;
            }

            int row = 2;
            if (direction.x > 0) row = 3;
            else if (direction.x < 0) row = 1;
            else if (direction.y < 0) row = 0;

            sprite.setTextureRect(sf::IntRect(currentFrame * size.x, row * size.y, size.x, size.y));
        }
    } else {
        sprite.setTextureRect(sf::IntRect(0, 2 * size.y, size.x, size.y));
    }
}
