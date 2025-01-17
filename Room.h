#ifndef ROOM_H
#define ROOM_H
#include <SFML/Graphics.hpp>

#include "Difficulty.h"
#include "Enemy.h"
#include "Player.h"
#include "Globals.h"
#include "Obstacle.h"
#include "FireObstacle.h"
#include "RockObstacle.h"

class Player;
class Enemy;

class Room {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    const int TILE_SIZE;         // Rozmiar kafelka w pikselach
    const int ROOM_WIDTH;        // Liczba kafelków w poziomie
    const int ROOM_HEIGHT;        // Liczba kafelków w pionie
    int NUM_OBSTACLES;
    std::vector<Obstacle*> obstacles;
    std::vector<sf::RectangleShape> tiles;
    sf::Vector2i doorTop;
    sf::Vector2i doorBottom;
    sf::Vector2i doorLeft;
    sf::Vector2i doorRight;
    sf::Texture doorOpenTexture, prevDoorTexture, doorClosedTexture, dickDoorTexture, heartDoorTexture, dieDoorTexture;
    sf::Sprite doorOpenSprite,prevDoorSprite, doorClosedSprite, doorDickSprite, doorHeartSprite, doorDieSprite;
    Difficulty difficult;
    std::vector<Enemy*> enemies;
    bool isCleared;
    sf::Vector2i openedDoor;
    sf::Vector2i prevDoor;
    //testing
    sf::RectangleShape openDoorRect;
    sf::RectangleShape prevDoorRect;
    bool openedDoorCollision = false;
    //std::vector<Item> items;

    //fire animation
    float animationTimer = 0.f;
    const float animationDuration = 200;
    int currentFrame = 0;

    int currentRoom;
    sf::Font font;
    sf::Text roomNumberText;

public:
    Room(Difficulty difficult, const int currentRoom, sf::Vector2i prevDoor = sf::Vector2i(0, 0));
    virtual ~Room();
    virtual void Initialize();
    void Load();
    virtual void Update(const float& deltaTime, Player& player);
    virtual void Draw(sf::RenderWindow& window);
    void InitializeDoors();
    void GenerateObstacles();
    bool IsWallTile(int x, int y) const;
    bool IsExitTile(int x, int y) const;
    bool IsObstacleTile(int x, int y) const;
    void GenerateTiles();
    void GenerateEnemies(Difficulty difficulty);
    bool IsRoomCleared() const;
    void OpenRandomDoor();
    void OpenPrevDoor();
    void SetDoorPositionAndRotation(sf::Sprite& doorSprite, sf::Vector2i door, float rotation);
    bool IsPlayerEnterNewRoom(Player& player) const;
    bool IsPlayerEnterPrevRoom(Player& player) const;
    void ClearDeadEnemies();

    int GetRoomWidthPX() const {
        return ROOM_WIDTH * TILE_SIZE;
    }

    int GetRoomHeightPX() const {
        return ROOM_HEIGHT * TILE_SIZE;
    }

    std::vector<Obstacle*> GetObstacles() const{
        return obstacles;
    }

    sf::Vector2i GetOpenDoor() const {
        return openedDoor;
    }

    sf::Vector2i GetPrevDoor() const {
        return prevDoor;
    }

    sf::Vector2i GetTopDoor() const {
        return doorTop;
    }

    sf::Vector2i GetBottomDoor() const {
        return doorBottom;
    }

    sf::Vector2i GetLeftDoor() const {
        return doorLeft;
    }

    sf::Vector2i GetRightDoor() const {
        return doorRight;
    }
};

#endif // ROOM_H