#pragma once
#include "Item.h"
#include "Room.h"

class TreasureRoom : public Room {
private:
    float openNextRoomTimer;
    std::vector<std::unique_ptr<Item>> items;
    bool isNextRoomOpen;
    bool isCleared;

public:
    TreasureRoom(const int currentRoom,sf::Vector2i prevDoor = sf::Vector2i(0, 0));
    ~TreasureRoom();
    void Initialize() override;
    void Update(const float &deltaTime, Player &player) override;
    void Draw(sf::RenderWindow &window) override;

    void GenerateItems();
    void CheckItemCollection(Player& player);
};
