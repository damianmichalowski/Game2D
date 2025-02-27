#include "BombMan.h"

BombMan::BombMan(float x, float y): Enemy(x, y),
fireSpeed(0.08f),
fireRateTimer(0),
bulletMaxAliveTime(10000)
{
    damage = 2;
    maxFireRate = 3000 + std::rand() % 501;;
}

BombMan::~BombMan() {
    std::cout << "Bombam destructed" << std::endl;
}

void BombMan::Load() {
    Enemy::Load();
    if (!texture.loadFromFile("../Assets/BombMan/witchKitty.png")) {
        std::cerr << "Failed to load witchKitty.png" << std::endl;
    }

    sprite.setTexture(texture);
}

void BombMan::Update(float& deltaTime, Player& player) {
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

void BombMan::Draw(sf::RenderWindow &window) {
    Enemy::Draw(window);

    for (Bullet* bullet: bullets) {
        bullet->Draw(window);
    }
}

void BombMan::Attack(float& deltaTime, Player &player) {
    fireRateTimer+=deltaTime;
    if (fireRateTimer >= maxFireRate) {
        sf::Vector2f bulletDirection = player.GetCenterHitBox() - GetCenterSprite();
        bulletDirection = Math::NormalizeVector(bulletDirection);

        if (bulletDirection.x != 0.0f || bulletDirection.y != 0.0f) {
            Bullet* bullet = new Bullet();
            bullet->Initialize(sf::Vector2f(GetCenterHitBox().x - 6, GetCenterHitBox().y - 8), bulletDirection, damage, fireSpeed, bulletMaxAliveTime, Bullet::Bomb);
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

void BombMan::CheckBulletCollisions(float& deltaTime, Player& player ,std::vector<sf::RectangleShape>& tiles) {
    for (auto& bullet : bullets) {
        if (!bullet->IsAlive()) continue;

        if (bullet->CheckCollision(player.GetHitBox()) && player.IsAlive()) {
            player.TakeDamage(bullet->GetDamage());
            bullet->SetAlive(false);
            break;
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

void BombMan::HandleAnimation(float& deltaTime, bool isPlayerInVision) {
    animationTimer += deltaTime;
    frameCount = 4;

    if (isPlayerInVision) {
        if (direction != sf::Vector2f(0.0f, 0.0f)) {
            if (animationTimer >= animationSpeed) {
                currentFrame = (currentFrame + 1) % frameCount;
                animationTimer = 0.0f;
            }

            int row = 0;
            if (direction.x > 0) row = 3;
            else if (direction.x < 0) row = 2;
            else if (direction.y < 0) row = 1;

            sprite.setTextureRect(sf::IntRect(currentFrame * spriteSize.x, row * spriteSize.y, spriteSize.x, spriteSize.y));
        }
    } else {
        sprite.setTextureRect(sf::IntRect(0, 2 * spriteSize.y, spriteSize.x, spriteSize.y));
    }
}