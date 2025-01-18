#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Room.h"

class Player;
class Room;

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

    Room* currentRoom;

public:
    Enemy(float x, float y);
    virtual ~Enemy();
    virtual void Initialize(Room& room);
    virtual void Load();
    virtual void Update(float& deltaTime, Player& player);
    virtual void Draw(sf::RenderWindow& window);
    virtual void HandleAnimation(float& deltaTime, bool isPlayerInVision);
    virtual void HandleMovement(float& deltaTime, Player& player);
    virtual bool CheckCollision(const sf::Vector2f& newPosition);
    void SetCurrentRoom(Room& room);
    virtual void CheckIsPlayerCollision(Player &player, bool isImmortal);
    void TakeDamage(int damage);
    bool IsAlive() const { return isAlive; }
    sf::Vector2f GetCenterHitBox() const;
    sf::Vector2f GetCenterSprite() const;
    sf::RectangleShape GetHitBox() const { return hitBox;}
    sf::FloatRect GetGlobalBounds() const { return hitBox.getGlobalBounds();}
    virtual void CheckBulletCollisions(float& deltaTime, Player& player ,std::vector<sf::RectangleShape>& tiles);
};

#endif // ENEMY_H
