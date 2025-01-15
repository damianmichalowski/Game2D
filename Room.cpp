#include "Room.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Skeleton.h"

Room::Room()
    : TILE_SIZE(32), ROOM_WIDTH(13), ROOM_HEIGHT(7),
      NUM_OBSTACLES(4), difficult(Difficulty::Easy) {
}

Room::Room(int width, int height, int size, Difficulty difficulty)
        : TILE_SIZE(32), ROOM_WIDTH(width), ROOM_HEIGHT(height),
          NUM_OBSTACLES(4), difficult(difficulty) {
}

Room::~Room() {
}

void Room::Initialize() {
    sprite.setPosition(sf::Vector2f(0, 0));
    InitializeDoors();
    GenerateObstacles();
    GenerateTiles();

    sprite.setTexture(texture);

    float scaleX = static_cast<float>(ROOM_WIDTH * TILE_SIZE) / texture.getSize().x;
    float scaleY = static_cast<float>(ROOM_HEIGHT * TILE_SIZE) / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);

    GenerateEnemies(difficult);
    isCleared = false;
}

void Room::Load() {
    if(!texture.loadFromFile("../Assets/Map/background.png")) {
        std::cerr << "Failed to load background texture in Room" << std::endl;
    }
    if(!fireTexture.loadFromFile("../Assets/Map/fire.png")) {
        std::cerr << "Failed to load fire texture in Room" << std::endl;
    }
    if (!doorOpenTexture.loadFromFile("../Assets/Map/Doors/openDoor.png")) {
        std::cerr << "Failed to load open door texture in Room" << std::endl;
    }
    if (!doorClosedTexture.loadFromFile("../Assets/Map/Doors/closedDoor.png")) {
        std::cerr << "Failed to load closed door texture in Room" << std::endl;
    }
    if (!dickDoorTexture.loadFromFile("../Assets/Map/Doors/dickDoor.png")) {
        std::cerr << "Failed to load *** door texture in Room" << std::endl;
    }
    if (!dieDoorTexture.loadFromFile("../Assets/Map/Doors/dieDoor.png")) {
        std::cerr << "Failed to load die door texture in Room" << std::endl;
    }
    if (!heartDoorTexture.loadFromFile("../Assets/Map/Doors/heartDoor.png")) {
        std::cerr << "Failed to load heart door texture in Room" << std::endl;
    }
}

void Room::Update(const float &deltaTime, Player& player) {
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime, player);
    }

    player.CheckBulletCollisions(deltaTime, enemies);

    if(!isCleared) {
        IsCleared();
    }

    HandleFireAnimation(deltaTime);
}

void Room::Draw(sf::RenderWindow &window) {
    window.draw(sprite);

    for (const auto& tile : tiles) {
        window.draw(tile);
    }

    for (const auto& fireSprite : fireSprites) {
        window.draw(fireSprite);
    }

    for (const auto& enemy : enemies) {
        enemy->Draw(window);
    }

    if (isCleared) {
        window.draw(doorOpenSprite);  // Drzwi otwarte
    } else {
        window.draw(doorClosedSprite);  // Drzwi zamknięte
        window.draw(doorDieSprite);
        window.draw(doorDickSprite);
        window.draw(doorHeartSprite);
    }
}

void Room::InitializeDoors() {
    doorTop = sf::Vector2i(ROOM_WIDTH / 2, 0);
    doorBottom = sf::Vector2i(ROOM_WIDTH / 2, ROOM_HEIGHT - 1);
    doorLeft = sf::Vector2i(0, ROOM_HEIGHT / 2);
    doorRight = sf::Vector2i(ROOM_WIDTH - 1, ROOM_HEIGHT / 2);

    // Drzwi górne (bez rotacji)
    doorOpenSprite.setTexture(doorOpenTexture);
    doorOpenSprite.setPosition(doorTop.x * TILE_SIZE, doorTop.y * TILE_SIZE);

    doorClosedSprite.setTexture(doorClosedTexture);
    doorClosedSprite.setPosition(doorTop.x * TILE_SIZE, doorTop.y * TILE_SIZE);

    doorDickSprite.setTexture(dickDoorTexture);
    sf::FloatRect dickBounds = doorDickSprite.getLocalBounds();
    doorDickSprite.setOrigin(dickBounds.width / 2.0f, dickBounds.height / 2.0f);
    doorDickSprite.setPosition((doorBottom.x + 0.5f) * TILE_SIZE, (doorBottom.y + 0.5f) * TILE_SIZE);
    doorDickSprite.setRotation(180);

    doorHeartSprite.setTexture(heartDoorTexture);
    sf::FloatRect heartBounds = doorHeartSprite.getLocalBounds();
    doorHeartSprite.setOrigin(heartBounds.width / 2.0f, heartBounds.height / 2.0f);
    doorHeartSprite.setPosition((doorLeft.x + 0.5f) * TILE_SIZE, (doorLeft.y + 0.5f) * TILE_SIZE);
    doorHeartSprite.setRotation(270);

    doorDieSprite.setTexture(dieDoorTexture);
    sf::FloatRect dieBounds = doorDieSprite.getLocalBounds();
    doorDieSprite.setOrigin(dieBounds.width / 2.0f, dieBounds.height / 2.0f);
    doorDieSprite.setPosition((doorRight.x + 0.5f) * TILE_SIZE, (doorRight.y + 0.5f) * TILE_SIZE);
    doorDieSprite.setRotation(90);
}


void Room::GenerateObstacles() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    obstacles.clear();
    fireSprites.clear();

    while (obstacles.size() < NUM_OBSTACLES) {
        int x = std::rand() % (ROOM_WIDTH - 4) + 2;
        int y = std::rand() % (ROOM_HEIGHT - 4) + 2;
        obstacles.push_back(sf::Vector2i(x, y));

        sf::Sprite fireSprite;
        fireSprite.setTexture(fireTexture);


        fireSprite.setTextureRect(sf::IntRect(currentFrame * 24, 0, 24, 32));
        fireSprite.setPosition(x * TILE_SIZE + 15, y * TILE_SIZE + 4);
        fireSprite.setOrigin(fireSprite.getLocalBounds().width / 2.f, fireSprite.getLocalBounds().height / 2.f);
        fireSprite.setScale(2.2f, 1.7f);

        fireSprites.push_back(fireSprite);
    }
}

bool Room::IsWallTile(int x, int y) const {
    return (x == 0 || x == ROOM_WIDTH - 1 || y == 0 || y == ROOM_HEIGHT - 1);
}

bool Room::IsExitTile(int x, int y) const {
    return (x == doorTop.x && y == doorTop.y) || (x == doorBottom.x && y == doorBottom.y) || (x == doorLeft.x && y == doorLeft.y) || (x == doorRight.x && y == doorRight.y);
}

bool Room::IsObstacleTile(int x, int y) const {
    return std::find(obstacles.begin(), obstacles.end(), sf::Vector2i(x, y)) != obstacles.end();
}

void Room::GenerateTiles() {
    for (int y = 0; y < ROOM_HEIGHT; ++y) {
        for (int x = 0; x < ROOM_WIDTH; ++x) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            if(Globals::IsDebugMode()) {
                if (IsWallTile(x, y)) {
                    if (IsExitTile(x,y)) {
                        tile.setFillColor(sf::Color::Green); // Doors
                    } else {
                        tile.setFillColor(sf::Color(100, 100, 100)); // Walls
                    }
                } else if (IsObstacleTile(x, y)) {
                        tile.setFillColor(sf::Color::Red); // Obstacles
                } else {
                    tile.setFillColor(sf::Color(0,0,0)); // Floor
                }

                tile.setOutlineColor(sf::Color(255,255,255));
                tile.setOutlineThickness(-1); //Borders
            } else {
                tile.setFillColor(sf::Color(0,0,0,0));
            }

            tiles.push_back(tile);
        }
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
                if (obstacle.x == x * TILE_SIZE && obstacle.y == y * TILE_SIZE) {
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

void Room::IsCleared() {
    isCleared = true;  // Zakładamy, że pokój jest oczyszczony

    for (auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            isCleared = false;  // Znaleziono żywego przeciwnika, więc pokój nie jest oczyszczony
            break;
        }
    }

    if (isCleared) {
        std::cout << "Cleared!" << std::endl;
    }
}

void Room::HandleFireAnimation(const float &deltaTime) {
    animationTimer += deltaTime;
    if (animationTimer >= animationDuration) {
        animationTimer = 0.f;  // Resetowanie timera
        currentFrame = (currentFrame + 1) % 8;  // Zmieniamy klatkę (8 klatek w cyklu)

        // Ustawienie nowej klatki dla każdego sprite'a ognia
        for (auto& fireSprite : fireSprites) {
            fireSprite.setTextureRect(sf::IntRect(currentFrame * 24, 0, 24, 32));  // Zmiana klatki
        }
    }
};
