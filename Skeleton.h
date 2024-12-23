#ifndef SKELETON_H
#define SKELETON_H
#include <SFML/Graphics.hpp>

class Skeleton {
private:
    sf::Texture texture;

    sf::Text healthText;
    sf::Font font;

public:
    sf::Sprite sprite;
    sf::RectangleShape boundingBox;
    sf::Vector2i size;

    int health;

public:
    Skeleton();
    ~Skeleton();
    void ChangeHealth(int hp);
    void Initialize();
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
};



#endif //SKELETON_H
