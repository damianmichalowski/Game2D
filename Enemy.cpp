#include <iostream>
#include "Enemy.h"
#include "Animate.h"
#include "Math.h"

Enemy::Enemy(const float x, const float y)
: posX(x), posY(y), health(100),
damage(1), speed(0.03f), isAlive(true),
animationTimer(0.0f), animationSpeed(150.0f), currentFrame(0), frameCount(8),
inactiveTimer(0.0f), maxInactiveCooldown(1500){}

Enemy::~Enemy() {
    std::cout << "Enemy destructed" << std::endl;
}

void Enemy::Initialize(Room& room) {
    hitBox.setSize(sf::Vector2f(20.f, 20.f));
    hitBox.setPosition(sf::Vector2f(posX, posY));
    hitBox.setFillColor(sf::Color::Transparent);

    visionBox.setSize(sf::Vector2f(hitBox.getSize().x * 12, hitBox.getSize().y * 12));
    visionBox.setPosition(hitBox.getPosition());
    visionBox.setFillColor(sf::Color::Transparent);
    visionBox.setOrigin(visionBox.getSize().x / 2.f, visionBox.getSize().y / 2.f);

    if(Globals::IsDebugMode()) {
        hitBox.setOutlineThickness(-1);
        hitBox.setOutlineColor(sf::Color::Red);

        visionBox.setOutlineThickness(-1);
        visionBox.setOutlineColor(sf::Color::Yellow);
    }

    sprite.setTexture(texture);
    spriteSize = sf::Vector2i(64, 64);
    int spriteXIndex = 0;
    int spriteYIndex = 2;
    sprite.setScale(32.f / spriteSize.x, 32.f / spriteSize.y);
    sprite.setTextureRect(sf::IntRect(spriteXIndex * spriteSize.x, spriteYIndex * spriteSize.y, spriteSize.x, spriteSize.y));
    sprite.setOrigin(spriteSize.x / 5.f, spriteSize.y / 3.f);
    sprite.setPosition(hitBox.getPosition());

    healthText.setFont(font);
    healthText.setString(std::to_string(health));
    healthText.setScale(0.25f, 0.25f);
    healthText.setPosition(sf::Vector2f(hitBox.getPosition().x + 3, hitBox.getPosition().y - 16.f));

    SetCurrentRoom(room);
}

void Enemy::Load() {
    if (!font.loadFromFile("../Assets/Fonts/Oswald-Regular.ttf")) {
        std::cout << "Failed to load Oswald font in Enemy." << std::endl;
    }
}

void Enemy::Update(float deltaTime, Player& player) {
    if(!isAlive) return;
    if(!player.IsAlive()) return;


    sprite.setPosition(hitBox.getPosition());
    sf::Vector2f hitBoxCenter = hitBox.getPosition() + sf::Vector2f(hitBox.getSize().x / 2.f, hitBox.getSize().y / 2.f);
    visionBox.setPosition(hitBoxCenter);
    healthText.setPosition(sf::Vector2f(hitBox.getPosition().x + 3, hitBox.getPosition().y - 16.f));

    inactiveTimer+=deltaTime;
    if(inactiveTimer >= maxInactiveCooldown) {
        bool isPlayerInVision = visionBox.getGlobalBounds().intersects(player.GetHitBox().getGlobalBounds());

        if (isPlayerInVision) {
            HandleMovement(deltaTime, player);
        }

        CheckIsPlayerCollision(player, player.IsImmortal());
        HandleAnimation(deltaTime, isPlayerInVision);
    }
}

void Enemy::Draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(hitBox);
        window.draw(sprite);
        window.draw(visionBox);
        window.draw(healthText);
    }
}

void Enemy::HandleMovement(float deltaTime, Player& player) {
    direction = player.GetHitBox().getPosition() - hitBox.getPosition();
    direction = Math::NormalizeVector(direction);

    sf::Vector2f position = hitBox.getPosition();

    // X
    sf::Vector2f newPositionX = position;
    newPositionX.x += direction.x * speed * deltaTime;
    if (!CheckCollision(newPositionX)) {
        position.x = newPositionX.x;
    }

    //Y
    sf::Vector2f newPositionY = position;
    newPositionY.y += direction.y * speed * deltaTime;
    if (!CheckCollision(newPositionY)) {
        position.y = newPositionY.y;
    }

    hitBox.setPosition(position);
    sprite.setPosition(hitBox.getPosition());
}

bool Enemy::CheckCollision(const sf::Vector2f& newPosition) {
    // Wall collision
    if (newPosition.x < 32 ||
        newPosition.x + hitBox.getGlobalBounds().width > currentRoom->GetRoomWidthPX() - 32 ||
        newPosition.y < 32 ||
        newPosition.y + hitBox.getGlobalBounds().height > currentRoom->GetRoomHeightPX() - 32) {
        return true;
    }

    // Obstacle collison
    for (const auto& obstacle : currentRoom->GetObstacles()) {
        sf::FloatRect obstacleRect(obstacle->GetPosition().x * 32, obstacle->GetPosition().y * 32, 32, 32);
        sf::FloatRect enemyRect(newPosition.x, newPosition.y, hitBox.getGlobalBounds().width, hitBox.getGlobalBounds().height);

        if (enemyRect.intersects(obstacleRect) && obstacle->IsDanger() == false) {
            return true;
        }
    }
    return false;
}

void Enemy::SetCurrentRoom(Room& room) {
    currentRoom = &room;
}

void Enemy::CheckIsPlayerCollision(Player& player, bool isImmortal) {
    if (Math::CheckCollision(hitBox.getGlobalBounds(), player.GetGlobalBounds()) && !isImmortal) {
        std::cout << "Player get hit" << std::endl;
        player.TakeDamage(1);
    }
}

void Enemy::TakeDamage(int hp) {
    health -= hp;
    if(health <= 0) isAlive = false;
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

            sprite.setTextureRect(sf::IntRect(currentFrame * spriteSize.x, row * spriteSize.y, spriteSize.x, spriteSize.y));
        }
    } else {
        sprite.setTextureRect(sf::IntRect(0, 2 * spriteSize.y, spriteSize.x, spriteSize.y));
    }
}
