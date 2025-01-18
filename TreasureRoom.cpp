#include "TreasureRoom.h"

TreasureRoom::TreasureRoom(const int currentRoom, sf::Vector2i prevDoor) : Room(Difficulty::Easy, currentRoom, prevDoor), openNextRoomTimer(0) {
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
    isNextRoomOpen = false;

    roomNumberText.setFont(font);
    roomNumberText.setCharacterSize(16);
    roomNumberText.setFillColor(sf::Color::White);
    if(Globals::IsDebugMode()) {
        roomNumberText.setString("Room " + std::to_string(currentRoom));
    } else {
        roomNumberText.setString("Room " + std::to_string(currentRoom));
    }
    roomNumberText.setPosition(32.f, 32.f * 7 - 25);

    GenerateItems();
}

void TreasureRoom::Update(const float &deltaTime, Player& player) {
    player.CheckBulletCollisions(deltaTime, tiles);

    openNextRoomTimer += deltaTime;
    if (openNextRoomTimer >= 1000 && !isNextRoomOpen) {
        OpenPrevDoor();
        OpenRandomDoor();
        isNextRoomOpen = true;
    }

    CheckItemCollection(player);
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

    window.draw(doorOpenSprite);
    if(currentRoom > 0) {
        window.draw(prevDoorSprite);
    };

    if(Globals::IsDebugMode()) {
        window.draw(openDoorRect);
        if(currentRoom > 0) {
            window.draw(prevDoorRect);
        };
    }


    window.draw(roomNumberText);

    if(!isCleared) {
        for (auto& item : items) {
            if(!item->IsCollected()) {
                item->Draw(window);
            }
        }
    }else {
        items.clear();
    }


    items.erase(
     std::remove_if(items.begin(), items.end(), [](const std::unique_ptr<Item>& item) {
         return item->IsCollected();
     }),
     items.end());
}

void TreasureRoom::GenerateItems() {
    float y = 32.f*3;
    float first = 32.f*4;
    float second = 32.f*6;
    float third = 32.f*8;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int firstRandom = std::rand() % 2;
    int secondRandom = std::rand() % 3;
    int thirdRandom = std::rand() % 3;

    if(firstRandom == 0) {
        items.push_back(std::make_unique<Item>(Item::Type::HealthPotion, sf::Vector2f(first, y)));
    } else {
        items.push_back(std::make_unique<Item>(Item::Type::LongerBulletALive, sf::Vector2f(first, y)));
    }

    if (secondRandom == 0) {
        items.push_back(std::make_unique<Item>(Item::Type::SpeedBoost, sf::Vector2f(second, y)));
    } else if (secondRandom == 1) {
        items.push_back(std::make_unique<Item>(Item::Type::DecreaseSpeed, sf::Vector2f(second, y)));
    }else {

    }

    if(thirdRandom == 0) {
        items.push_back(std::make_unique<Item>(Item::Type::DamageBoost, sf::Vector2f(third, y)));
    } else if(thirdRandom == 1) {
        items.push_back(std::make_unique<Item>(Item::Type::FireRateSpeed, sf::Vector2f(third, y)));
    } else {

    }

}


void TreasureRoom::CheckItemCollection(Player& player) {
    for (auto& item : items) {
        if (!item->IsCollected() && player.GetGlobalBounds().intersects(item->GetGlobalBounds())) {
            std::cout << "Collision with item" << std::endl;
            item->Collect(item->GetType(), player);
            isCleared = true;
        }
    }
}