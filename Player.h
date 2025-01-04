#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Skeleton.h"
#include "Bullet.h"

class Player {
private:
    sf::Texture texture;
    std::vector<Bullet> bullets;

    float maxFireRate;
    float fireRateTimer;

    float playerSpeed;
    bool immortal;

    sf::RectangleShape boundingBox;
    sf::Vector2i size;

    //movement animation
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    int frameCount;
    sf::Vector2i spriteSize;

    //health
    int maxHealth;
    int currentHealth;
    sf::Texture heartTexture;
    std::vector<sf::Sprite> hearts;
    float heartAttackCooldownTimer;
    float heartAttackCooldown;

    void HandleMovement(float deltaTime);
    void HandleAnimation(float deltaTime);
    void HandleShooting(float deltaTime, sf::Vector2f& mousePosition);
    void CheckBulletCollisions(Skeleton& skeleton, float deltaTime);
    void HandleHeartAttack(int hearts);

public:
    sf::Sprite sprite;

    Player();
    ~Player();

    void Initialize();
    void Load();
    void Update(float deltaTime, Skeleton& skeleton, sf::Vector2f& mousePosition);
    void Draw(sf::RenderWindow& window);
};



#endif //PLAYER_H
