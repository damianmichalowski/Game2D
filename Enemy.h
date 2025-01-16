#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
#include "Player.h"

class Player;

class Enemy {
protected:
    sf::RectangleShape hitBox;
    sf::RectangleShape visionBox;
    sf::Sprite sprite;
    sf::Vector2i spriteSize;
    sf::Texture texture;
    float posX, posY;
    sf::Vector2f direction;
    float inactiveTimer;
    float maxInactiveCooldown;

    int health;
    int damage;
    float speed;
    bool isAlive;

    //@todo przenieść do klasy UI
    sf::Text healthText;
    sf::Font font;

    //@todo przenieść do klasy animator
    float animationTimer;
    float animationSpeed;
    int currentFrame;
    int frameCount;

public:
    Enemy(float x, float y);
    ~Enemy();
    virtual void Initialize();
    virtual void Load();
    virtual void Update(float deltaTime, Player& player);
    virtual void Draw(sf::RenderWindow& window);
    virtual void HandleAnimation(float deltaTime, bool isPlayerInVision);
    virtual void CheckIsPlayerCollision(Player &player, bool isImmortal);
    // virtual void Attack();
    // virtual void Die();

    void TakeDamage(int damage);
    bool IsAlive() const {
        return isAlive;
    }
    sf::FloatRect GetGlobalBounds() const {
        return hitBox.getGlobalBounds();
    }

    sf::RectangleShape GetHitbox() const {
        return hitBox;
    }
};

#endif // ENEMY_H
