#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Enemy.h"
#include "Bullet.h"
#include "Room.h"
#include <iostream>

class Enemy;
class Room;

class Player {
private:
    sf::SoundBuffer buffer;
    sf::Sound shootingSound;
    sf::Sound hurtSound;

    sf::RectangleShape hitBox;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2i spriteSize;
    sf::Vector2i size;

    std::vector<Bullet*> bullets;
    sf::Vector2f shootingDirection;
    float maxFireRate;
    float fireRateTimer;
    float fireSpeed;
    float bulletMaxAliveTime;

    float playerSpeed;
    sf::Vector2f direction;
    int damage;

    //movement animation
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    int frameCount;

    //health
    bool immortal;
    int currentHealth;
    sf::Texture heartTexture;
    std::vector<sf::Sprite> hearts;
    float takeDamageCooldownTimer;
    float takeDamageCooldown;
    float damageTimer;
    sf::Color originalColor;

    Room* currentRoom;

    sf::Sprite dieImageSprite;
    sf::Texture dieImageTexture;
    bool isAlive;

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
    void CheckBulletCollisions(const float& deltaTime, std::vector<Enemy*>& enemies,std::vector<sf::RectangleShape>& tiles);
    void CheckBulletCollisions(const float& deltaTime, std::vector<sf::RectangleShape>& tiles);
    void SetPosition(sf::Vector2f newPosition);
    void SetCurrentRoom(Room& room);
    sf::Vector2f GetCenterHitBox() const;
    sf::Vector2f GetCenterSprite() const;
    void PlayShootingSound();
    void PlayHurtSound();

    void SaveToFile(std::ofstream &file);

    void LoadFromFile(std::ifstream &file);

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

    bool IsAlive() const {
        return isAlive;
    }

    void IncreaseHealth(int hearts) {
        currentHealth += hearts;
    }

    void IncreaseSpeed(float factor) {
        playerSpeed *= factor;
        std::cout << "player speed increased: " << playerSpeed << std::endl;
    }

    void IncreaseDamage(int damageIncrease) {
        damage += damageIncrease;
        std::cout << "player damage increased: " << damage << std::endl;
    }

    void IncreaseFireRate(float factor) {
        maxFireRate *= (1 / factor);
        std::cout << "player maxFireRate: " << maxFireRate << std::endl;
    }

    void IncreaseBulletAlive(float factor) {
        bulletMaxAliveTime *= factor;
        std::cout << "bulletMaxAliveTime increased: " << bulletMaxAliveTime << std::endl;
    }

    void DecreaseSpeed(float factor) {
        playerSpeed *= (1/factor);
        std::cout << "player speed decreased: " << playerSpeed << std::endl;
    }
};


#endif //PLAYER_H
