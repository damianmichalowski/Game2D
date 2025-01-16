#ifndef ROOM_H
#define ROOM_H
#include <SFML/Graphics.hpp>

#include "Difficulty.h"
#include "Enemy.h"
#include "Player.h"
#include "Globals.h"

class Player;
class Enemy;

class Room {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    const int TILE_SIZE;         // Rozmiar kafelka w pikselach
    const int ROOM_WIDTH;        // Liczba kafelków w poziomie
    const int ROOM_HEIGHT;        // Liczba kafelków w pionie
    const int NUM_OBSTACLES;
    std::vector<sf::Vector2i> obstacles;
    sf::Texture fireTexture;
    std::vector<sf::Sprite> fireSprites;
    std::vector<sf::RectangleShape> tiles;
    sf::Vector2i doorTop;
    sf::Vector2i doorBottom;
    sf::Vector2i doorLeft;
    sf::Vector2i doorRight;
    sf::Texture doorOpenTexture, doorClosedTexture, dickDoorTexture, heartDoorTexture, dieDoorTexture;
    sf::Sprite doorOpenSprite, doorClosedSprite, doorDickSprite, doorHeartSprite, doorDieSprite;
    Difficulty difficult;
    std::vector<Enemy*> enemies;
    bool isCleared;
    sf::Vector2i openedDoor;
    bool openedDoorCollision = false;
    //std::vector<Item> items;

    //fire animation
    float animationTimer = 0.f;
    const float animationDuration = 200;
    int currentFrame = 0;

    //testing
    sf::RectangleShape openDoorRect;

public:
    Room();
    Room(Difficulty difficult);
    ~Room();
    void Initialize();
    void Load();
    void Update(const float& deltaTime, Player& player);
    void Draw(sf::RenderWindow& window);
    void InitializeDoors();
    void GenerateObstacles();
    bool IsWallTile(int x, int y) const;
    bool IsExitTile(int x, int y) const;
    bool IsObstacleTile(int x, int y) const;
    void GenerateTiles();
    void GenerateEnemies(Difficulty difficulty);
    bool IsRoomCleared() const;
    void OpenRandomDoor();
    bool IsPlayerEnterNewRoom(Player& player) const;
    void HandleFireAnimation(const float &deltaTime);
    void ClearDeadEnemies();

    int GetRoomWidthPX() const {
        return ROOM_WIDTH * TILE_SIZE;
    }

    int GetRoomHeightPX() const {
        return ROOM_HEIGHT * TILE_SIZE;
    }

    std::vector<sf::Vector2i> GetObstacles() const{
        return obstacles;
    }

    sf::Vector2i GetOpenDoor() const {
        return openedDoor;
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