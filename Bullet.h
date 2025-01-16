#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::RectangleShape bulletShape;
    sf::Vector2f direction;
    float speed;
    bool isAlive;
    float lifeTime;
    float maxAliveTime;
    float damage;

public:
    Bullet();
    ~Bullet();

    void Initialize(const sf::Vector2f &position, sf::Vector2f &direction, float speed, float lifeTime);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    bool CheckCollision(const sf::Sprite& target) const;
    float GetDamage() const;
    void SetAlive(bool isAlive);
    bool IsAlive() const;

    sf::FloatRect GetGlobalBounds() {
        return bulletShape.getGlobalBounds();
    };
};

#endif //BULLET_H
