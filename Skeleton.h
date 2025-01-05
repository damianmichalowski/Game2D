#ifndef SKELETON_H
#define SKELETON_H
#include <SFML/Graphics.hpp>
#include "Player.h"

class Player;

class Skeleton {
private:
    sf::Texture texture;

    sf::Text healthText;
    sf::Font font;
    float speed;
    sf::Vector2f direction;

    //animation
    float animationTimer;
    float animationSpeed;
    int currentFrame;
    int frameCount;

    void HandleAnimation(float deltaTime, bool isPlayerInVision);

public:
    sf::Sprite sprite;
    sf::RectangleShape boundingBox;
    sf::Vector2i size;

    sf::RectangleShape boundingVisionBox;

    int health;

public:
    Skeleton();
    ~Skeleton();
    void ChangeHealth(int hp);
    void Initialize();
    void Load();
    void Update(float deltaTime, Player& player);
    void Draw(sf::RenderWindow& window);
};



#endif //SKELETON_H
