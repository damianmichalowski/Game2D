#include "Player.h"
#include <iostream>
#include "Math.h"

void Player::Initialize() {

}

void Player::Load(){
    if (!texture.loadFromFile("../Assets/Player/Textures/BODY_male.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
    }

    std::cerr << "Player texture loaded !" << std::endl;
    sprite.setTexture(texture);

    // max x: 8, y: 3
    int spriteXIndex = 0;
    int spriteYIndex = 2;

    sprite.setTextureRect(sf::IntRect(spriteXIndex * 64, spriteYIndex * 64, 64, 64));
    sprite.setScale(sf::Vector2f(2,2));
    sprite.setPosition(sf::Vector2f(1000,600));
}


void Player::Update(Skeleton& skeleton) {
    sf::Vector2f position = sprite.getPosition();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        sprite.setPosition(position + sf::Vector2(1.0f,0.f));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        sprite.setPosition(position + sf::Vector2(-1.0f,0.f));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        sprite.setPosition(position + sf::Vector2(0.f,-1.f));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        sprite.setPosition(position + sf::Vector2(0.f,1.f));

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        bullets.push_back(sf::RectangleShape(sf::Vector2f(10,10)));

        int i = bullets.size() - 1;
        bullets[i].setPosition(sprite.getPosition());
    }

    for (sf::RectangleShape& bullet: bullets) {
        sf::Vector2f bulletDirection = skeleton.sprite.getPosition() - bullet.getPosition();
        bulletDirection = Math::NormalizeVector(bulletDirection);
        bullet.setPosition(bullet.getPosition() + bulletDirection * bulletSpeed);
    }
}


void Player::Draw(sf::RenderWindow& window){
    window.draw(sprite);

    for (const sf::RectangleShape& bullet: bullets) {
        window.draw(bullet);
    }
}
