#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::RectangleShape bulletShape;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f direction;
    float speed;
    bool isAlive;
    float aliveTimer;
    float maxAliveTime;
    float damage;
    sf::IntRect frameRect;
    sf::Clock animationClock;

public:
    Bullet();
    ~Bullet();

    void Initialize(const sf::Vector2f &position, sf::Vector2f &direction);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void AnimateBullet();
    bool CheckCollision(const sf::RectangleShape& target) const;
    float GetDamage() const;
    void SetAlive(bool isAlive);
    bool IsAlive() const;

    sf::FloatRect GetGlobalBounds() {
        return bulletShape.getGlobalBounds();
    };
};

#endif //BULLET_H
