#include "BombMan.h"

BombMan::BombMan(float x, float y): Enemy(x, y),
fireSpeed(0.08f),
fireRateTimer(0),
bulletMaxAliveTime(10000)
{
    damage = 2;
    maxFireRate = 1000 + std::rand() % 501;
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

        changeDirectionTimer+=deltaTime;
        if(changeDirectionTimer >= 1000) {
            isPlayerInVision = visionBox.getGlobalBounds().intersects(player.GetHitBox().getGlobalBounds());
            changeDirectionTimer = 0;
        }

        Attack(deltaTime, player);
        CheckIsPlayerCollision(player, player.IsImmortal());
        HandleMovement(deltaTime, player);
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

void BombMan::HandleMovement(float& deltaTime, Player& player) {
    visionBox.setPosition(hitBox.getPosition());
    visionBox.setSize(sf::Vector2f(hitBox.getSize().x * 14, hitBox.getSize().y * 8));

    visionBox.setOrigin(visionBox.getSize().x / 2.f, visionBox.getSize().y / 2.f);
    sf::Vector2f hitBoxCenter = hitBox.getPosition() + sf::Vector2f(hitBox.getSize().x / 2.f, hitBox.getSize().y / 2.f);
    sprite.setPosition(hitBoxCenter);
    visionBox.setPosition(hitBoxCenter);


    direction = player.GetHitBox().getPosition() - hitBox.getPosition();
    direction = Math::NormalizeVector(direction);

    sf::Vector2f position = hitBox.getPosition();

    // X
    sf::Vector2f newPositionX = position;
    if (isPlayerInVision) {
        newPositionX.x -= direction.x * speed * deltaTime;
    } else {
        newPositionX.x += direction.x * speed * deltaTime;
    }
    if (!CheckCollision(newPositionX)) {
        position.x = newPositionX.x;
    }

    //Y
    sf::Vector2f newPositionY = position;
    if (isPlayerInVision) {
        newPositionY.y -= direction.y * speed * deltaTime;
    } else {
        newPositionY.y += direction.y * speed * deltaTime;
    }
    if (!CheckCollision(newPositionY)) {
        position.y = newPositionY.y;
    }

    hitBox.setPosition(position);
    sprite.setPosition(hitBox.getPosition());
}

void BombMan::HandleAnimation(float& deltaTime, bool isPlayerInVision) {
    animationTimer += deltaTime;
    frameCount = 4;

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
}