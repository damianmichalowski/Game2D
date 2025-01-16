#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Math.h"

Player::Player() :
playerSpeed(0.1f),
maxFireRate(100),
fireRateTimer(0),
immortal(false),
direction(0.0f, 0.0f),
currentFrame(0),
animationTimer(0.0f),
animationSpeed(150.0f),
frameCount(8),
maxHealth(3),
takeDamageCooldownTimer(0),
takeDamageCooldown(1000) {
}

Player::~Player() {
}

void Player::Initialize(Room& room) {
    currentRoom = &room;
    currentHealth = maxHealth;

    hitBox.setSize(sf::Vector2f(15,15));
    hitBox.setPosition(sf::Vector2f(40,40));
    hitBox.setFillColor(sf::Color::Transparent);

    if(Globals::IsDebugMode()) {
        hitBox.setOutlineThickness(-1);
        hitBox.setOutlineColor(sf::Color::Green);
    }

    spriteSize = sf::Vector2i(64,64);
    sprite.setTexture(texture);

    // max x: 8, y: 3
    int spriteXIndex = 0;
    int spriteYIndex = 2;
    sprite.setTextureRect(sf::IntRect(spriteXIndex * spriteSize.x, spriteYIndex * spriteSize.y, spriteSize.x, spriteSize.y));

    sprite.setScale(32.f / spriteSize.x, 32.f / spriteSize.y);
    sprite.setOrigin(spriteSize.x / 4.f, spriteSize.y / 2.f);
    sf::Vector2f hitBoxCenter = sf::Vector2f(hitBox.getPosition().x + hitBox.getSize().x / 2.f, hitBox.getPosition().y + hitBox.getSize().y / 2.f);
    sprite.setPosition(hitBoxCenter);

    for (int i = 0; i < maxHealth; i++) {
        sf::Sprite heart(heartTexture);
        heart.scale(sf::Vector2f(0.3f,0.3f));
        hearts.push_back(heart);
    }
}

void Player::Load(){
    if (!texture.loadFromFile("../Assets/Player/Textures/BODY_male.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
    }

    if (!heartTexture.loadFromFile("../Assets/UI/heart32x32.png")) {
        std::cerr << "Failed to load heart" << std::endl;
    }
}


void Player::Update(float& deltaTime) {
    HandleMovement(deltaTime);
    HandleShooting(deltaTime);
    sprite.setPosition(hitBox.getPosition());

    if (immortal) {
        takeDamageCooldownTimer += deltaTime;
        if(takeDamageCooldownTimer >= takeDamageCooldown/3) {
            sprite.setColor(originalColor);
        }

        if(takeDamageCooldownTimer >= takeDamageCooldown/2) {
            sprite.setColor(sf::Color::Red);
        }

        if (takeDamageCooldownTimer >= takeDamageCooldown) {
            immortal = false;
            sprite.setColor(originalColor);
            takeDamageCooldownTimer = 0;
        }
    }
}

void Player::Draw(sf::RenderWindow& window) {
    window.draw(hitBox);
    window.draw(sprite);
    
    for (Bullet* bullet: bullets) {
        bullet->Draw(window);
    }

    for (int i = 0; i < hearts.size(); ++i) {
        hearts[i].setPosition(sf::Vector2f(32 + i * 12, 5));
    }

    for (int i = 0; i < currentHealth; i++) {
        window.draw(hearts[i]);
    }

}

void Player::HandleMovement(float& deltaTime) {
    direction.x = 0.0f;
    direction.y = 0.0f;

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

    sf::Vector2f position = hitBox.getPosition();

    sf::Vector2f newPositionX = position;
    newPositionX.x += direction.x * playerSpeed * deltaTime;
    if (!CheckCollision(newPositionX)) {
        position.x = newPositionX.x;
    }

    sf::Vector2f newPositionY = position;
    newPositionY.y += direction.y * playerSpeed * deltaTime;
    if (!CheckCollision(newPositionY)) {
        position.y = newPositionY.y;
    }

    hitBox.setPosition(position);
    HandleAnimation(deltaTime, direction);
}

bool Player::CheckCollision(const sf::Vector2f& newPosition) {
    // Walls Collision
    if (newPosition.x < 32 ||
        newPosition.x + hitBox.getGlobalBounds().width > currentRoom->GetRoomWidthPX() - 32 ||
        newPosition.y < 32 ||
        newPosition.y + hitBox.getGlobalBounds().height > currentRoom->GetRoomHeightPX() - 32) {
        return true;
    }

    // Obstacle Collision
    for (const auto& obstacle : currentRoom->GetObstacles()) {
        sf::FloatRect obstacleRect(obstacle.x * 32, obstacle.y * 32, 32, 32);
        sf::FloatRect playerRect(newPosition.x, newPosition.y, hitBox.getGlobalBounds().width, hitBox.getGlobalBounds().height);

        if (playerRect.intersects(obstacleRect)) {
            TakeDamage(1);
        }
    }
    return false;
}

void Player::HandleAnimation(float& deltaTime, sf::Vector2f& direction) {
    animationTimer += deltaTime;

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
    } else {
        sprite.setTextureRect(sf::IntRect(0, 2 * spriteSize.y, spriteSize.x, spriteSize.y));
    }
}

void Player::HandleShooting(const float& deltaTime) {
    fireRateTimer += deltaTime;

    if (fireRateTimer >= maxFireRate) {
        sf::Vector2f shootingDirection(0.0f,0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            shootingDirection.x += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            shootingDirection.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            shootingDirection.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            shootingDirection.y += 1.0f;

        if (shootingDirection.x != 0.0f && shootingDirection.y != 0.0f) {
            shootingDirection /= std::sqrt(2.0f);
        }

        if (shootingDirection.x != 0.0f || shootingDirection.y != 0.0f) {
            Bullet* bullet = new Bullet();
            bullet->Initialize(GetCenterHitBox(), shootingDirection, 0.2f, 600);
            bullets.push_back(bullet);
            fireRateTimer = 0;
        }
    }
}

void Player::CheckBulletCollisions(const float& deltaTime, std::vector<Enemy*>& enemies) {
    for (auto& bullet : bullets) {
        if (!bullet->IsAlive()) continue;

        bullet->Update(deltaTime);

        for (auto& enemy : enemies) {
            if (bullet->CheckCollision(enemy->GetHitbox()) && enemy->IsAlive()) {
                enemy->TakeDamage(bullet->GetDamage());
                bullet->SetAlive(false);
                break;
            }
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
     [](Bullet* bullet) {
         bool toRemove = !bullet->IsAlive();
         if (toRemove) delete bullet;
         return toRemove;
     }),
    bullets.end());
}



void Player::TakeDamage(int damage) {
    if (!immortal) {
        currentHealth -= damage;
        immortal = true;
        damageTimer = 0.0f;
        originalColor = sprite.getColor();
        sprite.setColor(sf::Color::Red);
    }
}

void Player::SetCurrentRoom(Room& room) {
    currentRoom = &room;
}

sf::Vector2f Player::GetCenterHitBox() const {
    sf::Vector2f position = hitBox.getPosition();
    sf::FloatRect bounds = hitBox.getGlobalBounds();
    sf::Vector2f center(position.x + bounds.width / 2.f, position.y + bounds.height / 2.f);
    return center;
}

sf::Vector2f Player::GetCenterSprite() const {
    sf::Vector2f position = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f center(position.x + bounds.width / 2.f, position.y + bounds.height / 2.f);
    return center;
}

void Player::SetPosition(sf::Vector2f newPosition) {
    hitBox.setPosition(newPosition);
}
