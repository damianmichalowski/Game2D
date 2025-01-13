#include "Room.h"
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Skeleton.h"

Room::Room()
    : TILE_SIZE(32), ROOM_WIDTH(13), ROOM_HEIGHT(7),
      NUM_OBSTACLES(4), enemiesSize(0), difficult(Difficulty::Easy) {
}

Room::Room(int width, int height, int size, Difficulty difficulty)
        : TILE_SIZE(32), ROOM_WIDTH(width), ROOM_HEIGHT(height),
          NUM_OBSTACLES(4), enemiesSize(size), difficult(difficulty) {
}

Room::~Room() {
}

void Room::Initialize() {
    sf::Vector2i doorTop(ROOM_WIDTH / 2, 0);                  // Górne drzwi
    sf::Vector2i doorBottom(ROOM_WIDTH / 2, ROOM_HEIGHT - 1); // Dolne drzwi
    sf::Vector2i doorLeft(0, ROOM_HEIGHT / 2);                // Lewe drzwi
    sf::Vector2i doorRight(ROOM_WIDTH - 1, ROOM_HEIGHT / 2);  // Prawe drzwi

    //przeszkody
    std::vector<sf::Vector2i> obstacles;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    //szukanie pustych pol na mapie i dodanie indeksów do przeszkód
    while (obstacles.size() < NUM_OBSTACLES) {
        int x = std::rand() % (ROOM_WIDTH - 4) + 2;
        int y = std::rand() % (ROOM_HEIGHT - 4) + 2;

        obstacles.push_back(sf::Vector2i(x, y));
    }

    for (int y = 0; y < ROOM_HEIGHT; ++y) {
        for (int x = 0; x < ROOM_WIDTH; ++x) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            if (x == 0 || x == ROOM_WIDTH - 1 || y == 0 || y == ROOM_HEIGHT - 1) {
                // Ściany
                if ((x == doorTop.x && y == doorTop.y) ||
                    (x == doorBottom.x && y == doorBottom.y) ||
                    (x == doorLeft.x && y == doorLeft.y) ||
                    (x == doorRight.x && y == doorRight.y)) {
                    tile.setFillColor(sf::Color::Green); // Drzwi
                    } else {
                        tile.setFillColor(sf::Color(100, 100, 100)); // Ściany
                    }
            } else if (std::find(obstacles.begin(), obstacles.end(), sf::Vector2i(x, y)) != obstacles.end()) {
                tile.setFillColor(sf::Color::Red); // Przeszkody
            } else {
                tile.setFillColor(sf::Color::Black); // Podłoga
            }


            tile.setOutlineColor(sf::Color::White);
            tile.setOutlineThickness(-1); // Czarna obramówka
            tiles.push_back(tile);
        }
    }

    GenerateEnemies(difficult);
}

void Room::Load() {
}

void Room::Update(const float &deltaTime, Player& player) {
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime, player);
    }

    player.CheckBulletCollisions(deltaTime, enemies);
}

void Room::Draw(sf::RenderWindow &window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }

    for (const auto& enemy : enemies) {
        enemy->Draw(window);
    }
}


void Room::GenerateEnemies(Difficulty difficulty) {
    int enemiesCount = 0;

    if (difficulty == Difficulty::Easy) {
        enemiesCount = 2;
    } else if (difficulty == Difficulty::Medium) {
        enemiesCount = 5;
    } else if (difficulty == Difficulty::Hard) {
        enemiesCount = 8;
    }

    std::vector<sf::Vector2i> availablePositions;

    for (int y = 1; y < ROOM_HEIGHT - 1; ++y) {
        for (int x = 1; x < ROOM_WIDTH - 1; ++x) {

            bool isOccupied = false;
            for (const auto& obstacle : obstacles) {
                if (obstacle.getPosition().x == x * TILE_SIZE && obstacle.getPosition().y == y * TILE_SIZE) {
                    isOccupied = true;
                    break;
                }
            }

            if (!isOccupied) {
                availablePositions.push_back(sf::Vector2i(x, y));
            }
        }
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int i = 0; i < enemiesCount; i++) {
        int randomIndex = std::rand() % availablePositions.size();
        sf::Vector2i spawnPosition = availablePositions[randomIndex];

        availablePositions.erase(availablePositions.begin() + randomIndex);

        Skeleton* enemy = new Skeleton(spawnPosition.x * TILE_SIZE, spawnPosition.y * TILE_SIZE);
        enemy->Load();
        enemy->Initialize();
        enemies.push_back(enemy);
    }
}

