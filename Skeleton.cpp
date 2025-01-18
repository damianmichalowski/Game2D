#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton(float x, float y): Enemy(x, y),
fireSpeed(0.08f),
fireRateTimer(0),
bulletMaxAliveTime(10000)
{
    damage = 1;
    maxFireRate = 1500 + std::rand() % 501;;
}

Skeleton::~Skeleton() {
    std::cout << "Skeleton destructed" << std::endl;
}

void Skeleton::Load() {
    Enemy::Load();
    if (!texture.loadFromFile("../Assets/Skeleton/Textures/BODY_skeleton.png")) {
        std::cerr << "Failed to load skeleton.png" << std::endl;
    }

    sprite.setTexture(texture);
}

void Skeleton::Update(float& deltaTime, Player& player) {
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
            Attack(deltaTime, player);
        }

        CheckIsPlayerCollision(player, player.IsImmortal());
        HandleAnimation(deltaTime, isPlayerInVision);
    }
}

void Skeleton::Draw(sf::RenderWindow &window) {
    Enemy::Draw(window);

    for (Bullet* bullet: bullets) {
        bullet->Draw(window);
    }
}

void Skeleton::Attack(float& deltaTime, Player &player) {
    fireRateTimer+=deltaTime;
    if (fireRateTimer >= maxFireRate) {
        sf::Vector2f bulletDirection = player.GetCenterHitBox() - GetCenterSprite();
        bulletDirection = Math::NormalizeVector(bulletDirection);

        if (bulletDirection.x != 0.0f || bulletDirection.y != 0.0f) {
            Bullet* bullet = new Bullet();
            bullet->Initialize(sf::Vector2f(GetCenterHitBox().x - 6, GetCenterHitBox().y - 8), bulletDirection, damage, fireSpeed, bulletMaxAliveTime, Bullet::Skeleton);
            std::cout << "skeleton bullet direction: " << bulletDirection.x << bulletDirection.y << std::endl;
            bullets.push_back(bullet);
            fireRateTimer = 0;
        }
    }
}

//@todo miny typ atakowania
// void Skeleton::Attack(float& deltaTime, Player &player) {
//     fireRateTimer+=deltaTime;
//     if (fireRateTimer >= maxFireRate) {
//         if (direction.x != 0.0f || direction.y != 0.0f) {
//             Bullet* bullet = new Bullet();
//             bullet->Initialize(sf::Vector2f(GetCenterSprite().x - 6, GetCenterSprite().y), direction, damage, fireSpeed, bulletMaxAliveTime);
//             bullets.push_back(bullet);
//             fireRateTimer = 0;
//         }
//     }
// }

void Skeleton::CheckBulletCollisions(float& deltaTime, Player& player ,std::vector<sf::RectangleShape>& tiles) {
    for (auto& bullet : bullets) {
        if (!bullet->IsAlive()) continue;

        bullet->Update(deltaTime);

        if (bullet->CheckCollision(player.GetHitBox()) && player.IsAlive()) {
            player.TakeDamage(bullet->GetDamage());
            bullet->SetAlive(false);
            break;
        }

        //obstacles collision
        for (const auto& obstacle : currentRoom->GetObstacles()) {
            if (bullet->CheckCollision(obstacle->GetObstacleRect()) && obstacle->IsDanger() == false) {
                std::cout << "skeleton bullet collision with obstacle" << std::endl;
                bullet->SetAlive(false);
                break;
            }
        }

        //walls collision
        for (const auto& tile : tiles) {
            if (currentRoom->IsWallTile(tile.getPosition().x, tile.getPosition().y)) {
                if(bullet->CheckCollision(tile)) {
                    std::cout << "skeleton Bullet collision with wall tile" << std::endl;
                    bullet->SetAlive(false);
                    break;
                }
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

