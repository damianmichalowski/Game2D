#include "Skeleton.h"
#include <iostream>

void Skeleton::Load() {
    Enemy::Load();
    if (!texture.loadFromFile("../Assets/Skeleton/Textures/BODY_skeleton.png")) {
        std::cerr << "Failed to load skeleton.png" << std::endl;
    }

    sprite.setTexture(texture);
}
