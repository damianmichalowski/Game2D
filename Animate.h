#ifndef ANIMATE_H
#define ANIMATE_H

#include <SFML/Graphics.hpp>


class Animate {
private:
    sf::Sprite& sprite;
    sf::Vector2i frameSize;

    int frameCount;
    float animationSpeed;
    float animationTimer;
    int currentFrame;

public:
    Animate(sf::Sprite& sprite, sf::Vector2i frameSize, int frameCount, float speed);
    void Update(float deltaTime, int row);
};



#endif //ANIMATE_H
