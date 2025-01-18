#include "Item.h"

#include <iostream>


Item::Item(Type type, sf::Vector2f position): type(type), position(position), collected(false){
    switch (type) {
        case Type::HealthPotion:
            if(!texture.loadFromFile("../Assets/Items/heart.png")) {
                std::cerr << "Failed to load healthPotionTexture" << std::endl;
            }
            text = "HP UP!";
            break;
        case Type::SpeedBoost:
            if(!texture.loadFromFile("../Assets/Items/shoes.png")) {
                std::cerr << "Failed to load battery texture" << std::endl;
            }
            text = "SPEED UP!";
            break;
        case Type::DamageBoost:
            if(!texture.loadFromFile("../Assets/Items/eyeRed.png")) {
                std::cerr << "Failed to load eyeRed texture" << std::endl;
            }
            text = "DAMAGE UP!";
        break;
        case Type::FireRateSpeed:
            if(!texture.loadFromFile("../Assets/Items/eyeBlue.png")) {
                std::cerr << "Failed to load eyeBlue texture" << std::endl;
            }
        text = "FASTER SHOOTING!";
        break;
        case Type::LongerBulletALive:
            if(!texture.loadFromFile("../Assets/Items/eyeGreen.png")) {
                std::cerr << "Failed to load eyeGreen texture" << std::endl;
            }
            text = "BULLET ALIVE UP!";
        break;
        case Type::DecreaseSpeed:
            if(!texture.loadFromFile("../Assets/Items/mashroom.png")) {
                std::cerr << "Failed to load mushroom texture" << std::endl;
            }
            text = "SPEED DECREASED HA HA!";
        break;
    }

    sprite.setTexture(texture);
    sf::Vector2u textureSize = texture.getSize();
    float scaleX = 16.f / textureSize.x;
    float scaleY = 16.f / textureSize.y;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(position);
    hitBox.setSize(sf::Vector2f(16.f, 16.f));
    hitBox.setPosition(position);
    hitBox.setFillColor(sf::Color::Transparent);

    if(Globals::IsDebugMode()) {
        hitBox.setOutlineColor(sf::Color::Red);
        hitBox.setOutlineThickness(1.f);
    }
}

Item::~Item() {
    std::cout << "item zniszczony" << std::endl;
}

void Item::Draw(sf::RenderWindow &window) {
    if (!collected) {
        window.draw(sprite);
        window.draw(hitBox);
    }
}


void Item::Collect(Type type, Player& player) {
    collected = true;

    std::cout<<"Item collected"<<std::endl;
    switch (type) {
        case Type::HealthPotion:
            player.IncreaseHealth(1);
        break;
        case Type::SpeedBoost:
            player.IncreaseSpeed(1.2f);
        break;
        case Type::DamageBoost:
            player.IncreaseDamage(10);
        break;
        case Type::FireRateSpeed:
        player.IncreaseFireRate(1.2f);
        break;
        case Type::LongerBulletALive:
            player.IncreaseBulletAlive(1.2f);
        break;
        case Type::DecreaseSpeed:
            player.DecreaseSpeed(1.3f);
        break;
    };
}


