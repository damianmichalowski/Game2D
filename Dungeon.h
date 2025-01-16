#ifndef DUNGEON_H
#define DUNGEON_H
#include "Player.h"
#include "Room.h"
#include "Enemy.h"

class Enemy;
class Dungeon;
class Player;

class Dungeon {
private:
    int roomsNum;
    std::vector<Room> rooms;
    std::vector<sf::Vector2i> openDoors;
    int currentRoom;
    Player player;
    float enterRoomTimer;
    float maxRoomCooldown;


    public:
    Dungeon();
    ~Dungeon();
    void Initialize();
    void Load();
    void Update(float& deltaTime);
    void Draw(sf::RenderWindow& window);
    void CreateNextRoom();
};

#endif // DUNGEON_H