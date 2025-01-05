#include <iostream>
#include "Skeleton.h"
#include "Math.h"

Skeleton::Skeleton() : health(100), speed(0.15f) {
}

Skeleton::~Skeleton() {
    std::cout << "Skeleton destruktorized" << std::endl;
}

void Skeleton::ChangeHealth(int hp) {
    health += hp;
    healthText.setString(std::to_string(health));
}

void Skeleton::HandleAnimation(float deltaTime, bool isPlayerInVision) {
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
        // Domyślna animacja, gdy gracz nie jest w zasięgu wzroku
        sprite.setTextureRect(sf::IntRect(0, 2 * size.y, size.x, size.y)); // Ustawienie klatki "stojącej"
    }
}


void Skeleton::Initialize() {
    animationTimer = 0.0f;
    animationSpeed = 150.0f;
    currentFrame = 0;
    frameCount = 8;

    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setFillColor(sf::Color::Transparent);

    boundingVisionBox.setOutlineThickness(2);
    boundingVisionBox.setOutlineColor(sf::Color::Yellow);
    boundingVisionBox.setFillColor(sf::Color::Transparent);

    size = sf::Vector2i(64,64);
}

void Skeleton::Load() {
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

    sprite.setTexture(texture);

    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(1,1));
    boundingBox.setSize(sf::Vector2f(size.x * sprite.getScale().x,size.y * sprite.getScale().y));
    boundingVisionBox.setSize(sf::Vector2f(size.x * 6 * sprite.getScale().x,size.y * 6 * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * size.x, spriteYIndex * size.y, size.x, size.y));
    sprite.setPosition(sf::Vector2f(400,100));
    healthText.setPosition(sprite.getPosition());
}

void Skeleton::Update(float deltaTime, Player& player) {
    if (health > 0 ) {
        boundingBox.setPosition(sprite.getPosition());

        sf::Vector2f spriteCenter = sprite.getPosition() + sf::Vector2f(size.x * sprite.getScale().x / 2, size.y * sprite.getScale().y / 2);
        sf::Vector2f visionBoxOffset = sf::Vector2f(boundingVisionBox.getSize().x / 2, boundingVisionBox.getSize().y / 2);
        boundingVisionBox.setPosition(spriteCenter - visionBoxOffset);

        healthText.setPosition(sprite.getPosition());

        bool isPlayerInVision = Math::CheckCollision(boundingVisionBox.getGlobalBounds(), player.sprite.getGlobalBounds());

        if (isPlayerInVision) {
            direction = player.sprite.getPosition() - sprite.getPosition();
            direction = Math::NormalizeVector(direction);

            sprite.setPosition(sprite.getPosition() + direction * speed * deltaTime);
        }

        HandleAnimation(deltaTime, isPlayerInVision);
    }
}

void Skeleton::Draw(sf::RenderWindow& window) {
    if (health > 0) {
        window.draw(sprite);
        window.draw(boundingBox);
        window.draw(boundingVisionBox);
        window.draw(healthText);
    }
}
