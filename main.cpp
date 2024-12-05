#include <SFML/Graphics.hpp>

auto main() -> int {
    //-----------------INIT-------------------
    auto window = sf::RenderWindow(
        sf::VideoMode(800, 600), "MyGame",
        sf::Style::Default,
        sf::ContextSettings(0,0,0)
    );
    auto circle = sf::CircleShape(50.0f, 7);
    circle.setPosition(sf::Vector2f(100, 100));
    circle.setFillColor(sf::Color::Red);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Yellow);

    auto rectangle = sf::RectangleShape(sf::Vector2f(200, 2));
    rectangle.setPosition(sf::Vector2f(100, 100));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOrigin(rectangle.getSize() / 2.0f);
    rectangle.rotate(45);
    //-----------------INIT-------------------

    // Main Loop
    while (window.isOpen()){
        //-----------------UPDATE-------------------
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        //-----------------UPDATE-------------------


        //-----------------DRAW-------------------
        window.clear(sf::Color::Black);
        window.draw(circle);
        window.draw(rectangle);
        window.display();
        //-----------------DRAW-------------------
    }
}