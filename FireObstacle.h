#pragma once
#include "Obstacle.h"
#include <iostream>


class FireObstacle : public Obstacle {
public:
    float animationTimer;
    float animationDuration;
    int currentFrame;

    FireObstacle(const sf::Vector2i& position);
    ~FireObstacle() override;

    void Initialize() override;
    void Update(const float &deltaTime) override;
    void HandleFireAnimation(const float &deltaTime);
};
