#ifndef FRAMERATE_H
#define FRAMERATE_H
#include <SFML/Graphics.hpp>


class FrameRate {
private:
    sf::Text frameRateText;
    sf::Font font;
    double timer;

public:
    FrameRate();
    ~FrameRate();

    void Initialize(sf::View &view);
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
};



#endif //FRAMERATE_H
