#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "Bullet.h"
#include "Room.h"

class Enemy;
class Room;

class Player {
private:
    sf::RectangleShape hitBox;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i spriteSize;
    sf::Vector2i size;

    std::vector<Bullet*> bullets;

    float maxFireRate;
    float fireRateTimer;

    float playerSpeed;
    sf::Vector2f direction;

    //movement animation
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    int frameCount;

    //health
    bool immortal;
    int maxHealth;
    int currentHealth;
    sf::Texture heartTexture;
    std::vector<sf::Sprite> hearts;
    float takeDamageCooldownTimer;
    float takeDamageCooldown;
    float damageTimer;
    sf::Color originalColor;

    Room* currentRoom;

    void HandleMovement(float& deltaTime);
    bool CheckCollision(const sf::Vector2f& newPosition);
    void HandleAnimation(float& deltaTime, sf::Vector2f& direction);
    void HandleShooting(const float& deltaTime);
    //void CheckBulletCollisions(const float& deltaTime, Enemy& skeleton);


public:
    Player();
    ~Player();
    void Initialize(Room& room);
    void Load();
    void Update(float& deltaTime);
    void Draw(sf::RenderWindow& window);
    void TakeDamage(int damage);
    void CheckBulletCollisions(const float& deltaTime, std::vector<Enemy*>& enemies);
    void SetPosition(sf::Vector2f newPosition);
    void SetCurrentRoom(Room& room);
    sf::Vector2f GetCenterHitBox() const;
    sf::Vector2f GetCenterSprite() const;

    sf::RectangleShape GetHitBox() {
        return hitBox;
    }

    sf::FloatRect GetGlobalBounds() const {
        return hitBox.getGlobalBounds();
    }

    sf::Vector2f getPosition() const {
        return hitBox.getPosition();
    }

    bool IsImmortal() const {
        return immortal;
    }

};


#endif //PLAYER_H
