#include <iostream>
#include "Player.h"
#include "Math.h"

Player::Player() :
playerSpeed(0.3f),
maxFireRate(150),
fireRateTimer(0){
}

Player::~Player() {
}

void Player::Initialize() {
    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Green);
    boundingBox.setFillColor(sf::Color::Transparent);

    spriteSize = sf::Vector2i(64,64);
    currentFrame = 0;
    animationTimer = 0.0f;
    animationSpeed = 150.0f;
    frameCount = 8;

    //heart
    maxHealth = 3;
    currentHealth = maxHealth;
    heartAttackTimer = 0;
    maxheartAttackRate = 1000;


}

void Player::Load(){
    if (!texture.loadFromFile("../Assets/Player/Textures/BODY_male.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
    }

    if (!heartTexture.loadFromFile("../Assets/UI/heart32x32.png")) {
        std::cerr << "Failed to load heart" << std::endl;
    }

    sprite.setTexture(texture);

    for (int i = 0; i < maxHealth; i++) {
        sf::Sprite heart(heartTexture);
        hearts.push_back(heart);
    }

    // max x: 8, y: 3
    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(1,1));
    boundingBox.setSize(sf::Vector2f(spriteSize.x * sprite.getScale().x,spriteSize.y * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * spriteSize.x, spriteYIndex * spriteSize.y, spriteSize.x, spriteSize.y));
    sprite.setPosition(sf::Vector2f(1000,600));
}


void Player::Update(float deltaTime, Skeleton& skeleton, sf::Vector2f& mousePosition) {
    HandleMovement(deltaTime);
    HandleAnimation(deltaTime);
    HandleShooting(deltaTime, mousePosition);
    CheckBulletCollisions(skeleton, deltaTime);
    boundingBox.setPosition(sprite.getPosition());

    if (Math::CheckCollision(sprite, skeleton.sprite)) {
        std::cout << "Player get hit" << std::endl;
        HandleHeartAttack(deltaTime, 1);
    }
}

void Player::HandleMovement(float deltaTime) {
    sf::Vector2f direction(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        direction.x += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        direction.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        direction.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        direction.y += 1.0f;

    if (direction.x != 0.0f && direction.y != 0.0f)
        direction /= std::sqrt(2.0f);

    sf::Vector2f position = sprite.getPosition();
    position += direction * playerSpeed * deltaTime;
    sprite.setPosition(position);
}

void Player::HandleAnimation(float deltaTime) {
    animationTimer += deltaTime;
    sf::Vector2f direction(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) direction.x += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) direction.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) direction.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) direction.y += 1.0f;

    if (direction != sf::Vector2f(0.0f, 0.0f)) {
        if (animationTimer >= animationSpeed) {
            currentFrame = (currentFrame + 1) % frameCount; // Przejście do następnej klatki
            animationTimer = 0.0f;
        }

        int row = 2;
        if (direction.x > 0) row = 3;
        else if (direction.x < 0) row = 1;
        else if (direction.y < 0) row = 0;

        sprite.setTextureRect(sf::IntRect(currentFrame * spriteSize.x, row * spriteSize.y, spriteSize.x, spriteSize.y));
    } else {
        sprite.setTextureRect(sf::IntRect(0, 2 * spriteSize.y, spriteSize.x, spriteSize.y));
    }
}

void Player::HandleShooting(float deltaTime, sf::Vector2f& mousePosition) {
    fireRateTimer += deltaTime;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && fireRateTimer >= maxFireRate) {
        bullets.push_back(Bullet());
        bullets.back().Initialize(sprite.getPosition(), mousePosition, 0.5f);
        fireRateTimer = 0;
    }
}

void Player::CheckBulletCollisions(Skeleton& skeleton, float deltaTime) {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Bullet& bullet) {
        bullet.Update(deltaTime);
        if (skeleton.health > 0 && Math::CheckCollision(bullet.GetGlobalBounds(), skeleton.sprite.getGlobalBounds())) {
            std::cout << "collision" << std::endl;
            skeleton.ChangeHealth(-10);
            return true;
        }
        return false;
    }), bullets.end());
}

void Player::HandleHeartAttack(float deltaTime, int hearts) {
    heartAttackTimer += deltaTime;
    if(heartAttackTimer >= maxheartAttackRate) {
        currentHealth-= hearts;
        heartAttackTimer = 0;
    }
}

void Player::Draw(sf::RenderWindow& window){
    window.draw(sprite);
    for (Bullet& bullet: bullets) {
        bullet.Draw(window);
    }

    //halth
    float windowHeight = window.getSize().y;
    float heartYPosition = windowHeight - 50;
    for (int i = 0; i < hearts.size(); ++i) {
        hearts[i].setPosition(sf::Vector2f(10 + i * 40, heartYPosition));
    }

    for (int i = 0; i < currentHealth; i++) {
        window.draw(hearts[i]);
    }

    window.draw(boundingBox);
}
