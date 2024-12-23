#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::RectangleShape bulletShape;
    sf::Vector2f direction;
    float speed;

public:
    Bullet();
    ~Bullet();

    void Initialize(const sf::Vector2f& position, const sf::Vector2f& target, float speed);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    sf::FloatRect GetGlobalBounds() {
        return bulletShape.getGlobalBounds();
    };
};



#endif //BULLET_H
