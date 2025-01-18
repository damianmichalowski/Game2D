#ifndef DUNGEON_H
#define DUNGEON_H
#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include <iostream>
#include "TreasureRoom.h"


class Enemy;
class Dungeon;
class Player;

class Dungeon {
private:
    int roomsNum;
    std::vector<std::unique_ptr<Room>> rooms;
    std::vector<sf::Vector2i> openDoors;
    std::vector<sf::Vector2i> prevDoors;
    int currentRoom;
    Player player;
    float enterRoomTimer;
    float maxRoomCooldown;

    const int roomWidth = 32*13;
    const int roomHeight = 32*7;

    public:
    Dungeon();
    ~Dungeon();
    void Initialize();
    void Load();
    void Update(float& deltaTime);
    void Draw(sf::RenderWindow& window);
    void CreateNextRoom();
    void MoveToNextRoom();
    void BackToPrevRoom();
    void SetPlayerPositionInNextRoom(sf::Vector2i  prevDoor);
    void SetPlayerPositionInPrevRoom(sf::Vector2i  currentDoor);
};

#endif // DUNGEON_H