#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet {
public:
    enum Type {
        Default,
        Player,
        Skeleton
    };
private:
    sf::RectangleShape bulletShape;
    sf::Sprite sprite;

    sf::Texture* texture;
    sf::Texture defaultTexture;
    sf::Texture tearTexture;
    sf::Texture skeletonBulletTexture;

    sf::Vector2f direction;
    float speed;
    bool isAlive;
    float aliveTimer;
    float maxAliveTime;
    int damage;
    sf::IntRect frameRect;
    sf::Clock animationClock;

public:
    Bullet();
    ~Bullet();

    void Initialize(const sf::Vector2f &position, sf::Vector2f &direction, int damage = 10, float speed = 0.2f, float maxAliveTime = 1000, Type type = Default);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void AnimateBullet(float deltaTime);
    bool CheckCollision(const sf::RectangleShape& target) const;
    float GetDamage() const;
    void SetAlive(bool isAlive);
    bool IsAlive() const;

    sf::FloatRect GetGlobalBounds() {
        return bulletShape.getGlobalBounds();
    };
};

#endif //BULLET_H
