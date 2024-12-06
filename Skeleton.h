#ifndef SKELETON_H
#define SKELETON_H
#include <SFML/Graphics.hpp>

class Skeleton {
private:
    sf::Texture texture;

public:
    sf::Sprite sprite;

    sf::RectangleShape boundingBox;
    sf::Vector2i size;
public:
    void Initialize();
    void Load();
    void Update();
    void Draw(sf::RenderWindow& window);
};



#endif //SKELETON_H
