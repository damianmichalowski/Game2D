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
    sf::Texture texture;
    std::vector<Bullet*> bullets;

    float maxFireRate;
    float fireRateTimer;

    float playerSpeed;
    bool immortal;
    sf::Vector2f direction;

    sf::Sprite sprite;
    sf::Vector2i spriteSize;
    sf::Vector2i size;
    sf::RectangleShape boundingBox;

    //movement animation
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    int frameCount;

    //health
    int maxHealth;
    int currentHealth;
    sf::Texture heartTexture;
    std::vector<sf::Sprite> hearts;
    float takeDamageCooldownTimer;
    float takeDamageCooldown;

    Room* currentRoom;

    void HandleMovement(float& deltaTime);
    void HandleAnimation(float& deltaTime, sf::Vector2f& direction);
    void HandleObstacles(sf::Vector2f& newPosition);
    void HandleShooting(const float& deltaTime);
    //void CheckBulletCollisions(const float& deltaTime, Enemy& skeleton);


public:
    Player();
    ~Player();
    void Initialize(Room& room);
    void Load();
    void Update(float& deltaTime);
    void Draw(sf::RenderWindow& window, sf::View& view);
    void TakeDamage(int heartsTaken);
    void CheckBulletCollisions(const float& deltaTime, std::vector<Enemy*>& enemies);
    void ChangeRoom(Player& player, Room& newRoom);

    sf::Sprite GetSprite() {
        return sprite;
    }

    sf::FloatRect GetGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    bool IsImmortal() const {
        return immortal;
    }

    sf::Vector2f GetCenterOfSprite() const;
};



#endif //PLAYER_H
