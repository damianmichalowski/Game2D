#include "TreasureRoom.h"

TreasureRoom::TreasureRoom(const int roomCount) : Room(Difficulty::Easy, roomCount), openNextRoomTimer(0) {
    std::cout << "TreasureRoom constructor" << std::endl;
}

TreasureRoom::~TreasureRoom() {
}

void TreasureRoom::Initialize() {
    std::cout << "TreasureRoom initialization" << std::endl;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sprite.setPosition(sf::Vector2f(0, 0));
    InitializeDoors();
    GenerateTiles();

    sprite.setTexture(texture);

    float scaleX = static_cast<float>(ROOM_WIDTH * TILE_SIZE) / texture.getSize().x;
    float scaleY = static_cast<float>(ROOM_HEIGHT * TILE_SIZE) / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);

    isCleared = false;

    roomNumberText.setFont(font);
    roomNumberText.setCharacterSize(16);
    roomNumberText.setFillColor(sf::Color::White);
    if(Globals::IsDebugMode()) {
        roomNumberText.setString("Room " + std::to_string(roomCount));
    } else {
        roomNumberText.setString("Room " + std::to_string(++roomCount));
    }
    roomNumberText.setPosition(32.f, 32.f * 7 - 25);
}

void TreasureRoom::Update(const float &deltaTime, Player& player) {

    player.CheckBulletCollisions(deltaTime, tiles);

    openNextRoomTimer += deltaTime;
    if (openNextRoomTimer >= 4000 && !isCleared) {
        OpenRandomDoor();
        isCleared = true;
    }
}

void TreasureRoom::Draw(sf::RenderWindow &window) {
    window.draw(sprite);

    for (const auto& tile : tiles) {
        window.draw(tile);
    }

    window.draw(doorClosedSprite);
    window.draw(doorDieSprite);
    window.draw(doorDickSprite);
    window.draw(doorHeartSprite);

    if (isCleared) {
        window.draw(doorOpenSprite);
        if(Globals::IsDebugMode()) {
            window.draw(openDoorRect);
        }
    }
}