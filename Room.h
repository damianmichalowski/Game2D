#ifndef ROOM_H
#define ROOM_H
#include <SFML/Graphics.hpp>

#include "Difficulty.h"
#include "Enemy.h"
#include "Player.h"

class Player;
class Enemy;

class Room {
private:
    const int TILE_SIZE;         // Rozmiar kafelka w pikselach
    const int ROOM_WIDTH;        // Liczba kafelków w poziomie
    const int ROOM_HEIGHT;        // Liczba kafelków w pionie
    const int NUM_OBSTACLES;
    std::vector<sf::RectangleShape> obstacles;
    std::vector<sf::RectangleShape> tiles;


    enum ConnectionDirections { NORTH, EAST, SOUTH, WEST };
    sf::RectangleShape roomShape;
    std::vector<sf::RectangleShape> walls;
    sf::RectangleShape exit;
    int enemiesSize;
    Difficulty difficult;
    std::vector<Enemy*> enemies;
    bool isCleared;
    //std::vector<Item> items;

public:
    Room();
    Room(int ROOM_WIDTH, int ROOM_HEIGHT, int enemiesSize, Difficulty difficult);
    ~Room();
    void Initialize();
    void Load();
    void Update(const float& deltaTime, Player& player);
    void Draw(sf::RenderWindow& window);

    void GenerateWalls();
    void GenerateExit();
    void GenerateEnemies(Difficulty difficulty);
    void CheckCollision(const sf::FloatRect& playerBounds);
    void CheckIfCleared();

    int GetRoomWidthPX() const {
        return ROOM_WIDTH * TILE_SIZE;
    }

    int GetRoomHeightPX() const {
        return ROOM_HEIGHT * TILE_SIZE;
    }

    int GetTileSize() const {
        return TILE_SIZE;
    }
};

#endif // ROOM_H