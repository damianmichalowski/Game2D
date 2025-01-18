#pragma once
#include "Enemy.h"
#include "Math.h"
#include <iostream>

class BombMan : public Enemy {
private:
    std::vector<Bullet*> bullets;
    sf::Vector2f shootingDirection;
    float maxFireRate;
    float fireRateTimer;
    float fireSpeed;
    float bulletMaxAliveTime;

public:
    BombMan(float x, float y);
    ~BombMan() override;

    void Load() override;
    void Update(float& deltaTime, Player& player) override;
    void Draw(sf::RenderWindow &window) override;
    void Attack(float& deltaTime, Player &player);
    void CheckBulletCollisions(float& deltaTime, Player& player ,std::vector<sf::RectangleShape>& tiles) override;
    void HandleAnimation(float &deltaTime, bool isPlayerInVision) override;
};