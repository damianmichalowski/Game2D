#include "Math.h"
#include <iostream>
#include <cmath>

sf::Vector2f Math::NormalizeVector(sf::Vector2f& vector) {
    //chcemy wektor podzielić na kawałki o długości 1 z zachowaniem kierunku
    const float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    if (magnitude == 0) {
        return {0, 0};
    }

    sf::Vector2f normalizedVector;

    normalizedVector.x = vector.x / magnitude;
    normalizedVector.y = vector.y / magnitude;

    return normalizedVector;
};

bool Math::CheckCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}

bool Math::CheckCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    return rect1.intersects(rect2);
}