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

    sf::RectangleShape boundingBox;
    sf::Vector2i size;

public:
    sf::Sprite sprite;

public:
    Player();
    ~Player();

    void Initialize(); // once at start
    void Load(); // once at start
    void Update(float deltaTime, Skeleton& skeleton, sf::Vector2f& mousePosition); // once per frame
    void Draw(sf::RenderWindow& window); // once per frame
};



#endif //PLAYER_H
