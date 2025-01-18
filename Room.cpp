#include "Room.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "BombMan.h"
#include "Charger.h"
#include "Dungeon.h"
#include "Skeleton.h"

Room::Room(Difficulty difficulty, const int currentRoom,sf::Vector2i prevDoor)
        : TILE_SIZE(32), ROOM_WIDTH(13), ROOM_HEIGHT(7),
          NUM_OBSTACLES(6), difficult(difficulty), currentRoom(currentRoom), prevDoor(prevDoor) {
}

Room::~Room() {
}

void Room::Initialize() {
    std::cout << "Initializing room..." << std::endl;
    std::cout << "New Room prev door after INIT: " << prevDoor.x << " " <<prevDoor.y << std::endl;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    NUM_OBSTACLES = std::rand() % 9;

    sprite.setPosition(sf::Vector2f(0, 0));
    InitializeDoors();
    GenerateObstacles();
    GenerateTiles();

    sprite.setTexture(texture);

    float scaleX = static_cast<float>(ROOM_WIDTH * TILE_SIZE) / texture.getSize().x;
    float scaleY = static_cast<float>(ROOM_HEIGHT * TILE_SIZE) / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);

    GenerateEnemies(difficult, currentRoom);
    isCleared = false;

    roomNumberText.setFont(font);
    roomNumberText.setCharacterSize(16);
    roomNumberText.setFillColor(sf::Color::White);
    if(Globals::IsDebugMode()) {
        roomNumberText.setString("Room " + std::to_string(currentRoom));
    } else {
        roomNumberText.setString("Room " + std::to_string(currentRoom));
    }
    roomNumberText.setPosition(32.f, 32.f * 7 - 25);
}

void Room::Load() {
    if(!texture.loadFromFile("../Assets/Map/background.png")) {
        std::cerr << "Failed to load background texture in Room" << std::endl;
    }
    if (!doorOpenTexture.loadFromFile("../Assets/Map/Doors/openDoor.png")) {
        std::cerr << "Failed to load open door texture in Room" << std::endl;
    }
    if (!prevDoorTexture.loadFromFile("../Assets/Map/Doors/prevDoor.png")) {
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
    if (!font.loadFromFile("../Assets/Fonts/VipnagorgiallaBd.otf")) {
        std::cerr << "Failed to load VipnagorgiallaBd font" << std::endl;
    }
}

void Room::Update(float &deltaTime, Player& player) {
    for (auto& enemy : enemies) {
        enemy->Update(deltaTime, player);
        enemy->CheckBulletCollisions(deltaTime, player, tiles);
    }

    player.CheckBulletCollisions(deltaTime, enemies, tiles);

    if (IsRoomCleared() && !isCleared) {
        std::cout << "Cleared!" << std::endl;
        OpenPrevDoor();
        OpenRandomDoor();
        isCleared = true;
    }

    for (auto& obstacle : obstacles) {
        obstacle->Update(deltaTime);
    }

    ClearDeadEnemies();
}

void Room::Draw(sf::RenderWindow &window) {
    window.draw(sprite);

    for (const auto& tile : tiles) {
        window.draw(tile);
    }

    for (auto& obstacle : obstacles) {
        obstacle->Draw(window);
    }

    window.draw(doorClosedSprite);
    window.draw(doorDieSprite);
    window.draw(doorDickSprite);
    window.draw(doorHeartSprite);

    if (isCleared) {
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
    }

    for (const auto& enemy : enemies) {
        enemy->Draw(window);
    }

    window.draw(roomNumberText);
}

void Room::InitializeDoors() {
    doorTop = sf::Vector2i(ROOM_WIDTH / 2, 0);
    doorBottom = sf::Vector2i(ROOM_WIDTH / 2, ROOM_HEIGHT - 1);
    doorLeft = sf::Vector2i(0, ROOM_HEIGHT / 2);
    doorRight = sf::Vector2i(ROOM_WIDTH - 1, ROOM_HEIGHT / 2);

    doorOpenSprite.setTexture(doorOpenTexture);
    doorOpenSprite.setPosition(1000, 0);

    prevDoorSprite.setTexture(prevDoorTexture);
    prevDoorSprite.setPosition(1000, 0);

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

    while (obstacles.size() < NUM_OBSTACLES) {
        int x = std::rand() % (ROOM_WIDTH - 4) + 2;
        int y = std::rand() % (ROOM_HEIGHT - 4) + 2;

        bool positionOccupied = false;
        for (const auto& obstacle : obstacles) {
            if (obstacle->GetPosition().x == x && obstacle->GetPosition().y == y) {
                positionOccupied = true;
                break;
            }
        }

        if (!positionOccupied) {
            if (std::rand() % 2 == 0) {
                obstacles.push_back(new RockObstacle({x, y}));
            } else {
                obstacles.push_back(new FireObstacle({x, y}));
            }
        }
    }
}

bool Room::IsWallTile(int x, int y) const {
    return (x == 0 || x == (ROOM_WIDTH - 1) * TILE_SIZE || y == 0 || y == (ROOM_HEIGHT - 1) * TILE_SIZE);
}

bool Room::IsExitTile(int x, int y) const {
    return (x == doorTop.x && y == doorTop.y) || (x == doorBottom.x && y == doorBottom.y) || (x == doorLeft.x && y == doorLeft.y) || (x == doorRight.x && y == doorRight.y);
}
bool Room::IsObstacleTile(int x, int y) const {
    for (auto& obstacle : obstacles) {
        if (obstacle->GetPosition() == sf::Vector2i(x, y)) {
            return true;
        }
    }
    return false;
}

void Room::GenerateTiles() {
    tiles.clear();

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
                    tile.setFillColor(sf::Color::Transparent); // Floor
                }

                tile.setOutlineColor(sf::Color(255,255,255));
                tile.setOutlineThickness(-1); //Borders
            } else {
                tile.setFillColor(sf::Color::Transparent);
            }

            tiles.push_back(tile);
        }
    }
}

void Room::GenerateEnemies(Difficulty difficulty, int currentRoom) {
    const std::unordered_map<Difficulty, int> difficultyToEnemies = {
        {Difficulty::Easy, 2},
        {Difficulty::Medium, 4},
        {Difficulty::Hard, 6}
    };

    int enemiesCount = difficultyToEnemies.at(difficulty);

    std::vector<sf::Vector2i> availablePositions;
    for (int y = 2; y < ROOM_HEIGHT - 2; ++y) {
        for (int x = 2; x < ROOM_WIDTH - 2; ++x) {
            if (!IsObstacleTile(x, y)) {
                availablePositions.emplace_back(x, y);
            }
        }
    }

    int skeletonCount = 0;

    for (int i = 0; i < enemiesCount; ++i) {
        int randomIndex = std::rand() % availablePositions.size();
        sf::Vector2i spawnPosition = availablePositions[randomIndex];
        availablePositions.erase(availablePositions.begin() + randomIndex);

        if ((difficulty == Difficulty::Medium && skeletonCount < 1) ||
            (difficulty == Difficulty::Hard && skeletonCount < 2)) {
            auto* skeleton = new Skeleton(spawnPosition.x * TILE_SIZE, spawnPosition.y * TILE_SIZE);
            skeleton->Load();
            skeleton->Initialize(std::ref(*this));
            enemies.push_back(skeleton);
            ++skeletonCount;
        } else {
            int randomType = std::rand() % 3;

            if (randomType == 1) {
                auto* bombMan = new BombMan(spawnPosition.x * TILE_SIZE, spawnPosition.y * TILE_SIZE);
                bombMan->Load();
                bombMan->Initialize(std::ref(*this));
                enemies.push_back(bombMan);
            } else if (randomType == 2) {
                auto* charger = new Charger(spawnPosition.x * TILE_SIZE, spawnPosition.y * TILE_SIZE);
                charger->Load();
                charger->Initialize(std::ref(*this));
                enemies.push_back(charger);
            } else {
                auto* skeleton = new Skeleton(spawnPosition.x * TILE_SIZE, spawnPosition.y * TILE_SIZE);
                skeleton->Load();
                skeleton->Initialize(std::ref(*this));
                enemies.push_back(skeleton);
                ++skeletonCount;
            }
        }
    }
}


bool Room::IsRoomCleared() const {
    for (auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            return false;
        }
    }
    return true;
}

void Room::OpenRandomDoor() {
    int randomDoor;
    sf::FloatRect openDoorBounds = doorOpenSprite.getLocalBounds();
    sf::Vector2i newDoorPosition;
    bool isValidPosition = false;

    do {
        randomDoor = std::rand() % 4;
        switch (randomDoor) {
            case 0: newDoorPosition = doorTop; break;
            case 1: newDoorPosition = doorBottom; break;
            case 2: newDoorPosition = doorLeft; break;
            case 3: newDoorPosition = doorRight; break;
        }
        if (newDoorPosition != prevDoor) {
            isValidPosition = true;
        }
    } while (!isValidPosition);

    openedDoor = newDoorPosition;
    doorOpenSprite.setOrigin(openDoorBounds.width / 2.0f, openDoorBounds.height / 2.0f);

    switch (randomDoor) {
        case 0: // Top
            doorOpenSprite.setPosition((doorTop.x + 0.5f) * TILE_SIZE, (doorTop.y + 0.5f) * TILE_SIZE);
            doorOpenSprite.setRotation(0);
            break;
        case 1: // Bottom
            doorOpenSprite.setPosition((doorBottom.x + 0.5f) * TILE_SIZE, (doorBottom.y + 0.5f) * TILE_SIZE);
            doorOpenSprite.setRotation(180);
            break;
        case 2: // Left
            doorOpenSprite.setPosition((doorLeft.x + 0.5f) * TILE_SIZE, (doorLeft.y + 0.5f) * TILE_SIZE);
            doorOpenSprite.setRotation(270);
            break;
        case 3: // Right
            doorOpenSprite.setPosition((doorRight.x + 0.5f) * TILE_SIZE, (doorRight.y + 0.5f) * TILE_SIZE);
            doorOpenSprite.setRotation(90);
            break;
    }

    std::cout << "Opened door posX: " << openedDoor.x << std::endl;
    std::cout << "Opened door posY: " << openedDoor.y << std::endl;

    openDoorRect = sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    openDoorRect.setPosition(openedDoor.x * TILE_SIZE, openedDoor.y * TILE_SIZE);
    openDoorRect.setOutlineThickness(3);

    if (Globals::IsDebugMode()) {
        openDoorRect.setOutlineColor(sf::Color::Green);
    }
}

void Room::OpenPrevDoor() {
    if(currentRoom == 0) return;

    prevDoorSprite.setOrigin(0, 0);
    prevDoorSprite.setRotation(0);

    prevDoorSprite.setPosition(prevDoor.x * TILE_SIZE, prevDoor.y * TILE_SIZE);

    if (prevDoor == doorTop) {
        prevDoor = doorBottom;
        SetDoorPositionAndRotation(prevDoorSprite, prevDoor, 180);
    } else if (prevDoor == doorBottom) {
        prevDoor = doorTop;
        SetDoorPositionAndRotation(prevDoorSprite, prevDoor, 0);
    } else if (prevDoor == doorLeft) {
        prevDoor = doorRight;
        SetDoorPositionAndRotation(prevDoorSprite, prevDoor, 90);
    } else if (prevDoor == doorRight) {
        prevDoor = doorLeft;
        SetDoorPositionAndRotation(prevDoorSprite, prevDoor, 270);
    }

    prevDoorRect = sf::RectangleShape(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    prevDoorRect.setPosition(prevDoor.x * TILE_SIZE, prevDoor.y * TILE_SIZE);
    prevDoorRect.setOutlineThickness(3);

    if(Globals::IsDebugMode()) {
        prevDoorRect.setOutlineColor(sf::Color::Blue);
    }

    std::cout << "open prev after OPEN door posX: " << prevDoor.x << std::endl;
    std::cout << "open prev afrer OPEN posY: " << prevDoor.y << std::endl;
}

void Room::SetDoorPositionAndRotation(sf::Sprite& doorSprite, sf::Vector2i door, float rotation) {
    sf::FloatRect bounds = doorSprite.getLocalBounds();
    doorSprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    doorSprite.setPosition((door.x + 0.5f) * TILE_SIZE, (door.y + 0.5f) * TILE_SIZE);
    doorSprite.setRotation(rotation);
}

bool Room::IsPlayerEnterNewRoom(Player& player) const {
    if(player.GetGlobalBounds().intersects(openDoorRect.getGlobalBounds())) {
        std::cout << "Collision with nextRoom" << std::endl;
        return true;
    }
    return false;
};

bool Room::IsPlayerEnterPrevRoom(Player& player) const {
    if(player.GetGlobalBounds().intersects(prevDoorRect.getGlobalBounds())) {
        std::cout << "Collision z prev door" << std::endl;
        return true;
    }
    return false;
};


void Room::ClearDeadEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](Enemy* enemy) {
        bool toRemove = !enemy->IsAlive();
        if (toRemove) delete enemy;
        return toRemove;
    }),
   enemies.end());
}
