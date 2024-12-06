#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "Skeleton.h"

class Player {
private:
    sf::Texture texture;
    std::vector<sf::RectangleShape> bullets;
    float bulletSpeed = 1.0f;

    sf::RectangleShape boundingBox;
    sf::Vector2i size;

public:
    sf::Sprite sprite;

public:
    void Initialize(); // once at start
    void Load(); // once at start
    void Update(Skeleton& skeleton); // once per frame
    void Draw(sf::RenderWindow& window); // once per frame
};



#endif //PLAYER_H
