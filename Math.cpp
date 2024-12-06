#include <iostream>
#include <math.h>
#include "Math.h"

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