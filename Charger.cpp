//
// Created by Damian Michałowski on 18/01/2025.
//

#include "Charger.h"

#include "Math.h"

Charger::Charger(float x, float y) : Enemy(x, y),directionChangeTimer(0), directionChangeInterval(2000), attackTimer(0), attackInterval(0){
    speed = 0.07f;
    inactiveTimer = 0.f;
    int randDirection = rand() % 3;

    switch (randDirection) {
        case 0:
            direction = sf::Vector2f(1.0f, 0.0f);
        break;
        case 1:
            direction = sf::Vector2f(-1.0f, 0.0f);
        break;
        case 2:
            direction = sf::Vector2f(0.0f, -1.0f);
        break;
    }
}

Charger::~Charger() {
    std::cout << "Charger destructed" << std::endl;
}

void Charger::Load() {
    Enemy::Load();
    if (!texture.loadFromFile("../Assets/Charger/charger.png")) {
        std::cerr << "Failed to load charger.png" << std::endl;
    }

    sprite.setTexture(texture);
}

void Charger::Initialize(Room &room) {
    Enemy::Initialize(room);

    hitBox.setSize(sf::Vector2f(16.f, 16.f));
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setOrigin((sprite.getLocalBounds().width / 2.f), (sprite.getLocalBounds().height / 2.f));
    sprite.setScale(hitBox.getSize().x * 1.5 / sprite.getLocalBounds().width,hitBox.getSize().y * 1.5 / sprite.getLocalBounds().height);
}

void Charger::Update(float &deltaTime, Player &player) {
    if(!isAlive) return;
    if(!player.IsAlive()) return;

    visionBox.setPosition(hitBox.getPosition());
    visionBox.setSize(sf::Vector2f(hitBox.getSize().x * 50, hitBox.getSize().y * 20));

    visionBox.setOrigin(visionBox.getSize().x / 2.f, visionBox.getSize().y / 2.f);
    sf::Vector2f hitBoxCenter = hitBox.getPosition() + sf::Vector2f(hitBox.getSize().x / 2.f, hitBox.getSize().y / 2.f);
    sprite.setPosition(hitBoxCenter);
    visionBox.setPosition(hitBoxCenter);
    healthText.setPosition(sf::Vector2f(hitBox.getPosition().x + 3, hitBox.getPosition().y - 16.f));

    // sf::Vector2f hitBoxCenter = hitBox.getPosition() + sf::Vector2f(hitBox.getSize().x / 2.f, hitBox.getSize().y / 2.f);
    // sprite.setPosition(hitBoxCenter);
    // border.setPosition(hitBoxCenter);
    // visionBox.setPosition(hitBoxCenter);
    // visionBox.setSize(sf::Vector2f(hitBox.getSize().x * 20, hitBox.getSize().y * 20));

    inactiveTimer+=deltaTime;
    if(inactiveTimer >= maxInactiveCooldown) {
        bool isPlayerInVision = visionBox.getGlobalBounds().intersects(player.GetHitBox().getGlobalBounds());

        if (isPlayerInVision) {
            HandleMovement(deltaTime, player);
        }
       HandleAnimation(deltaTime, isPlayerInVision);
        Attack(deltaTime, player);
    }
}

void Charger::Draw(sf::RenderWindow &window) {
    Enemy::Draw(window);
}

void Charger::HandleMovement(float &deltaTime, Player &player) {
    directionChangeTimer += deltaTime;

    if (directionChangeTimer >= directionChangeInterval) {
        int randDirection = rand() % 4;

        switch (randDirection) {
            case 0:
                direction = sf::Vector2f(1.0f, 0.0f);
            break;
            case 1:
                direction = sf::Vector2f(-1.0f, 0.0f);
            break;
            case 2:
                direction = sf::Vector2f(0.0f, -1.0f);
            break;
            case 3:
                direction = sf::Vector2f(0.0f, 1.0f);
            break;
        }
        directionChangeTimer = 0.0f;
    }

    sf::Vector2f position = hitBox.getPosition();
    sf::Vector2f newPosition = position + direction * speed * deltaTime;

    if (CheckCollision(newPosition)) {
        int randDirection = rand() % 4;
        switch (randDirection) {
            case 0: // W prawo
                direction = sf::Vector2f(1.0f, 0.0f);
            break;
            case 1: // W lewo
                direction = sf::Vector2f(-1.0f, 0.0f);
            break;
            case 2: // W górę
                direction = sf::Vector2f(0.0f, -1.0f);
            break;
            case 3: // W dół
                direction = sf::Vector2f(0.0f, 1.0f);
            break;
        }
    } else {
        hitBox.setPosition(newPosition);
    }
}

void Charger::HandleAnimation(float &deltaTime, bool isPlayerInVision) {
    animationTimer += deltaTime;
    int frameCount = 3;

    if (isPlayerInVision) {
        if (direction != sf::Vector2f(0.0f, 0.0f)) {
            if (animationTimer >= animationSpeed) {
                currentFrame = (currentFrame + 1) % frameCount;
                animationTimer = 0.0f;
            }

            int row = 0;
            if (direction.x > 0) row = 0;
            else if (direction.x < 0) row = 0;
            else if (direction.y < 0) row = 1;
            else if (direction.y > 0) row = 2;

            sprite.setTextureRect(sf::IntRect(currentFrame * 32, row * 32, 32, 32));
        }
    } else {
        sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
}

void Charger::Attack(float &deltaTime, Player &player) {
    attackTimer += deltaTime;

    if (attackTimer >= attackInterval) {
        if (hitBox.getGlobalBounds().intersects(player.GetHitBox().getGlobalBounds())) {
            player.TakeDamage(1);
            sprite.setTextureRect(sf::IntRect(0, 32*3, 32, 32));
            std::cout << "Player hit by Charger" << std::endl;
        }

        attackTimer = 0.0f;
    }
}