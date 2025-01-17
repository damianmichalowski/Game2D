#pragma once
#include "Obstacle.h"
#include <iostream>

class RockObstacle : public Obstacle {
public:
    RockObstacle(const sf::Vector2i& position);
    ~RockObstacle() override;
    void Initialize() override;
};