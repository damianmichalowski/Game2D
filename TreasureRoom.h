#pragma once
#include "Room.h"

class TreasureRoom : public Room {
private:
    float openNextRoomTimer;

public:
    TreasureRoom(const int roomCount);
    ~TreasureRoom();
    void Initialize() override;
    void Update(const float &deltaTime, Player &player) override;
    void Draw(sf::RenderWindow &window) override;

};
