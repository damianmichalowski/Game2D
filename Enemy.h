#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
#include "Player.h"

class Player;

class Enemy {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float posX, posY;
    sf::Vector2f direction;
    float inactiveTimer;
    float maxInactiveCooldown;

    sf::RectangleShape boundingBox;
    sf::RectangleShape visionBox;

    sf::Vector2i size;

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
    virtual void CheckIsPlayerCollision(sf::Sprite &sprite, Player &player, bool isImmortal);
    // virtual void Attack();
    // virtual void Die();

    void TakeDamage(int damage);
    // void SetPosition(float x, float y);
    // void Move(float deltaX, float deltaY);
    // bool IsWithinRange(const sf::Vector2f& position) const;
    bool IsAlive() const {
        return isAlive;
    }
    sf::FloatRect GetGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    sf::Sprite GetSprite() const {
        return sprite;
    }
};

#endif // ENEMY_H
