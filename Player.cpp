#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Math.h"

Player::Player() :
playerSpeed(0.1f),
maxFireRate(600),
fireRateTimer(0),
immortal(false),
direction(0.0f, 0.0f),
currentFrame(0),
animationTimer(0.0f),
animationSpeed(150.0f),
frameCount(8),
maxHealth(3),
takeDamageCooldownTimer(0),
takeDamageCooldown(1000){
}

Player::~Player() {
}

void Player::Initialize(Room& room) {
    currentRoom = &room;
    currentHealth = maxHealth;

    boundingBox.setOutlineThickness(2);
    boundingBox.setOutlineColor(sf::Color::Green);
    boundingBox.setFillColor(sf::Color::Transparent);

    spriteSize = sf::Vector2i(64,64);
    sprite.setTexture(texture);
    // max x: 8, y: 3
    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.scale(sf::Vector2f(0.25f,0.25f));  //tile_size/aktualny rozmiar 64px
    boundingBox.setSize(sf::Vector2f(spriteSize.x * sprite.getScale().x,spriteSize.y * sprite.getScale().y));

    sprite.setTextureRect(sf::IntRect(spriteXIndex * spriteSize.x, spriteYIndex * spriteSize.y, spriteSize.x, spriteSize.y));
    sprite.setPosition(sf::Vector2f(40,40));

    for (int i = 0; i < maxHealth; i++) {
        sf::Sprite heart(heartTexture);
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
    boundingBox.setPosition(sprite.getPosition());

    if (immortal) {
        takeDamageCooldownTimer += deltaTime;
        if (takeDamageCooldownTimer >= takeDamageCooldown) {
            immortal = false;
            takeDamageCooldownTimer = 0;
        }
    }
}

void Player::Draw(sf::RenderWindow& window, sf::View& view) {
    window.draw(sprite);
    for (Bullet* bullet: bullets) {
        bullet->Draw(window);
    }

    //halth
    float windowHeight = view.getSize().y;
    float heartYPosition = windowHeight - 50;
    for (int i = 0; i < hearts.size(); ++i) {
        hearts[i].setPosition(sf::Vector2f(10 + i * 40, heartYPosition));
    }

    for (int i = 0; i < currentHealth; i++) {
        window.draw(hearts[i]);
    }

    window.draw(boundingBox);
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

    sf::Vector2f position = sprite.getPosition();

    position += direction * playerSpeed * deltaTime;

    if (position.x < currentRoom->GetTileSize() ||
    position.x + sprite.getGlobalBounds().width > currentRoom->GetRoomWidthPX() - currentRoom->GetTileSize() ||
    position.y < currentRoom->GetTileSize() ||
    position.y + sprite.getGlobalBounds().height > currentRoom->GetRoomHeightPX() - currentRoom->GetTileSize()) {
        return;
    }

    sprite.setPosition(position);

    HandleAnimation(deltaTime, direction);
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

void Player::HandleObstacles(sf::Vector2f& newPosition) {

    if (newPosition.x >= currentRoom->GetTileSize() &&
        newPosition.x + sprite.getPosition().x <= currentRoom->GetRoomWidthPX() - currentRoom->GetTileSize() &&
        newPosition.y >= currentRoom->GetTileSize() &&
        newPosition.y + sprite.getPosition().y <= currentRoom->GetTileSize() - currentRoom->GetTileSize()) {
            sprite.setPosition(direction);
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
            bullet->Initialize(GetCenterOfSprite(), shootingDirection, 0.2f, 600);
            bullets.push_back(bullet);
            fireRateTimer = 0;
        }
    }
}

// void Player::CheckBulletCollisions(const float& deltaTime, Enemy& skeleton) {
//     bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Bullet& bullet) {
//         bullet.Update(deltaTime);
//
//         if(!bullet.IsAlive()) {
//             return true;
//         }
//
//         if (skeleton.IsAlive() && Math::CheckCollision(bullet.GetGlobalBounds(), skeleton.GetGlobalBounds())) {
//             std::cout << "collision" << std::endl;
//             skeleton.TakeDamage(-bullet.GetDamage());
//             return true;
//         }
//
//         return false;
//     }), bullets.end());
// }

void Player::CheckBulletCollisions(const float& deltaTime, std::vector<Enemy*>& enemies) {
    for (auto& bullet : bullets) {
        if (!bullet->IsAlive()) continue;

        bullet->Update(deltaTime);

        for (auto& enemy : enemies) {
            if (bullet->CheckCollision(enemy->GetSprite()) && enemy->IsAlive()) {
                enemy->TakeDamage(bullet->GetDamage());
                bullet->SetInactive();
                break;
            }
        }
    }

    // Usuń nieaktywne pociski
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                                 [](Bullet* bullet) {
                                     bool toRemove = !bullet->IsAlive();
                                     if (toRemove) delete bullet;  // Usuń z pamięci
                                     return toRemove;
                                 }),
                  bullets.end());
}



void Player::TakeDamage(int heartsTaken) {
    if (!immortal) {
        currentHealth -= heartsTaken;
        immortal = true;
    }
}

void Player::ChangeRoom(Player& player, Room& newRoom) {
    player.Initialize(newRoom);
}

sf::Vector2f Player::GetCenterOfSprite() const {
    sf::Vector2f position = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f center(position.x + bounds.width / 2.f, position.y + bounds.height / 2.f);
    return center;
}