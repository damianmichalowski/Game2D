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
    Skeleton();
    ~Skeleton();

    void Initialize();
    void Load();
    void Update(double deltaTime);
    void Draw(sf::RenderWindow& window);
};



#endif //SKELETON_H
