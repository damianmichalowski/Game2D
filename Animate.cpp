#include "Animate.h"

Animate::Animate(sf::Sprite &sprite, sf::Vector2i frameSize, int frameCount, float speed)
: sprite(sprite), frameSize(frameSize), frameCount(frameCount), animationSpeed(speed), animationTimer(0.0f), currentFrame(0) {}


void Animate::Update(float deltaTime, int row){
    animationTimer += deltaTime;

    if (animationTimer >= animationSpeed) {
        currentFrame = (currentFrame + 1) % frameCount;
        animationTimer = 0.0f;
    }

    sprite.setTextureRect(sf::IntRect(currentFrame * frameSize.x, row * frameSize.y, frameSize.x, frameSize.y));
}