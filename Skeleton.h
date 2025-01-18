#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"
#include "Math.h"

class Skeleton : public Enemy {
private:
    std::vector<Bullet*> bullets;
    sf::Vector2f shootingDirection;
    float maxFireRate;
    float fireRateTimer;
    float fireSpeed;
    float bulletMaxAliveTime;

public:
    Skeleton(float x, float y);
    ~Skeleton() override;

    void Load() override;
    void Update(float& deltaTime, Player& player) override;
    void Draw(sf::RenderWindow &window) override;
    void Attack(float& deltaTime, Player &player);
    void CheckBulletCollisions(float& deltaTime, Player& player ,std::vector<sf::RectangleShape>& tiles) override;
};

#endif // SKELETON_H
