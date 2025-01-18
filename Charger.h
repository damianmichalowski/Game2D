#pragma once
#include "Enemy.h"

class Charger : public Enemy {
private:
    float directionChangeTimer;
    float directionChangeInterval;
    float attackTimer;
    float attackInterval;
    sf::RectangleShape border;

    public:
    Charger(float x, float y);
    ~Charger() override;

    void Load() override;
    void Initialize(Room &room) override;
    void Update(float& deltaTime, Player& player) override;
    void Draw(sf::RenderWindow &window) override;
    void HandleAnimation(float& deltaTime, bool isPlayerInVision) override;
    void HandleMovement(float& deltaTime, Player& player) override;
    void Attack(float& deltaTime, Player &player);
};
