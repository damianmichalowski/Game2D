#ifndef MATH_H
#define MATH_H
#include <SFML/Graphics.hpp>


class Math {
public:
    static sf::Vector2f NormalizeVector(sf::Vector2f& vector);
    static bool CheckCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
    static bool CheckCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2);
};



#endif //MATH_H
