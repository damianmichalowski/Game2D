#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Obstacle {
protected:
    const sf::Vector2i position;
    sf::Sprite sprite;
    sf::Texture texture;
    bool isDanger;
    bool isDestroyable;

public:
    Obstacle(sf::Vector2i position);
    virtual ~Obstacle();

    virtual void Initialize(); // Pure virtual function
    virtual void Update(const float &deltaTime);
    virtual void Draw(sf::RenderWindow& window);

    bool IsDanger() const;
    bool IsDestroyable() const;

    const sf::Sprite& GetSprite() const;
    const sf::Vector2i& GetPosition() const;
    sf::RectangleShape GetObstacleRect() const;
};