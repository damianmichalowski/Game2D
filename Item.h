#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"
#include "Math.h"
#include <string>
class Item {
public:
    enum class Type {
        HealthPotion,
        SpeedBoost,
        DamageBoost,
        FireRateSpeed,
        LongerBulletALive,
        DecreaseSpeed,
    };

private:
    Type type;
    sf::Sprite sprite;
    sf::RectangleShape hitBox;
    bool collected;
    sf::Texture texture;
    sf::Vector2f position;
    std::string text;

public:
    Item(Type type, sf::Vector2f position);
    ~Item();

    void Draw(sf::RenderWindow& window);

    bool IsCollected() const {
        return collected;
    }

    void Collect(Type type, Player& player);

    Type GetType() const {
        return type;
    }

    sf::FloatRect GetGlobalBounds() const {
        return hitBox.getGlobalBounds();
    }

    void setPosition(sf::Vector2f newPosition) {
        position = newPosition;
    }

    std::string GetText() const {
        return text;
    }
};
