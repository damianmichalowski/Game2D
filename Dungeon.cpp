#include "Dungeon.h"

#include <fstream>

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
Dungeon::Dungeon() : currentRoom(0), maxRoomCooldown(1000), enterRoomTimer(0), score(0) {
    std::ifstream file("isGameSaved.dat");
    if (!file.is_open()) {
        std::cerr << "Error opening file isGameSaved.dat" << std::endl;
    }
    file.read(reinterpret_cast<char*>(&isGameSaved), sizeof(isGameSaved));
    file.close();

    if(isGameSaved) {
        LoadFromFile("../Saves/SaveFile.dat");
    }

    std::cout << "score: " << score <<"isGameSaved: "  << isGameSaved << std::endl;
}

Dungeon::~Dungeon() {
    rooms.clear();
}

void Dungeon::Initialize() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    roomsNum = std::rand() % 6 + 5;

    rooms.push_back(std::make_unique<Room>(Difficulty::Easy, currentRoom));

    player.Load();
    rooms[currentRoom]->Load();
    rooms[currentRoom]->Initialize();
    player.Initialize(*rooms[currentRoom]);

    if (!font.loadFromFile("../Assets/Fonts/VipnagorgiallaBd.otf")) {
        std::cerr << "Failed to load VipnagorgiallaBd font" << std::endl;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color::White);
    if(Globals::IsDebugMode()) {
        scoreText.setString("High Score" + std::to_string(score));
    } else {
        scoreText.setString("High Score " + std::to_string(score));
    }
    scoreText.setPosition(32.f*8, 32.f * 7 - 25);
}

void Dungeon::Load() {
}

void Dungeon::Update(float& deltaTime) {
    player.Update(deltaTime);
    rooms[currentRoom]->Update(deltaTime, player);

    enterRoomTimer += deltaTime;

    if (rooms[currentRoom]->IsPlayerEnterNewRoom(player) && enterRoomTimer >= maxRoomCooldown) {
        std::cout << "Enter new room" << std::endl;
        enterRoomTimer = 0;

        if(currentRoom == rooms.size() - 1) {
            openDoors.emplace_back(rooms[currentRoom]->GetOpenDoor());
            CreateNextRoom();
        } else {
            MoveToNextRoom();
        }
    }

    if (rooms[currentRoom]->IsPlayerEnterPrevRoom(player) && enterRoomTimer >= maxRoomCooldown) {
        std::cout << "Back prev room" << std::endl;
        enterRoomTimer = 0;
        BackToPrevRoom();
    }
}

void Dungeon::Draw(sf::RenderWindow& window) {
    rooms[currentRoom]->Draw(window);
    player.Draw(window);
    window.draw(scoreText);
}

void Dungeon::CreateNextRoom() {
    if (currentRoom > score) {
        score++;
        scoreText.setString("High Score " + std::to_string(score));
    }
    currentRoom++;
    SaveToFile("../Saves/SaveFile.dat");

    sf::Vector2i prevDoor = rooms[currentRoom - 1]->GetOpenDoor();
    prevDoors.emplace_back(prevDoor);

    if (currentRoom % 3 == 0) {
        rooms.push_back(std::make_unique<TreasureRoom>(currentRoom, prevDoor));
    } else {
        Difficulty difficulty;
        if (currentRoom < 6) {
            difficulty = Difficulty::Easy;
        } else if (currentRoom < 12) {
            difficulty = Difficulty::Medium;
        } else {
            difficulty = Difficulty::Hard;
        }
        rooms.push_back(std::make_unique<Room>(difficulty, currentRoom, prevDoor));
    }

    rooms[currentRoom]->Load();
    rooms[currentRoom]->Initialize();
    player.SetCurrentRoom(*rooms[currentRoom]);
    SetPlayerPositionInNextRoom(prevDoor);
}

void Dungeon::MoveToNextRoom() {
    currentRoom++;
    player.SetCurrentRoom(*rooms[currentRoom]);
    SetPlayerPositionInNextRoom(prevDoors[currentRoom - 1]);
}

void Dungeon::BackToPrevRoom() {
    currentRoom--;
    player.SetCurrentRoom(*rooms[currentRoom]);
    SetPlayerPositionInPrevRoom(prevDoors[currentRoom]);
    std::cout << "prevDoors[currentRoom]" << prevDoors[currentRoom].x << " " << prevDoors[currentRoom].y << std::endl;
}

void Dungeon::SetPlayerPositionInNextRoom(sf::Vector2i prevDoor) {
    if (prevDoor == rooms[currentRoom]->GetTopDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2, roomHeight - 33 - 15));
    } else if (prevDoor == rooms[currentRoom]->GetBottomDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2, 33));
    } else if (prevDoor == rooms[currentRoom]->GetLeftDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth - 32 - 15, roomHeight / 2));
    } else if (prevDoor == rooms[currentRoom]->GetRightDoor()) {
        player.SetPosition(sf::Vector2f(33, roomHeight / 2));
    }
}

void Dungeon::SetPlayerPositionInPrevRoom(sf::Vector2i  currentDoor) {
    if (currentDoor == rooms[currentRoom]->GetTopDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2,  33));
    } else if (currentDoor == rooms[currentRoom]->GetBottomDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2, roomHeight - 33 - 15));
    } else if (currentDoor == rooms[currentRoom]->GetLeftDoor()) {
        player.SetPosition(sf::Vector2f(33, roomHeight / 2));
    } else if (currentDoor == rooms[currentRoom]->GetRightDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth - 32 - 15, roomHeight / 2));
    }
}

void Dungeon::SaveToFile(const std::string& pathFile) {
    std::ofstream isSavedFile("isGameSaved.dat", std::ios::binary);
    std::ofstream file(pathFile, std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "File could not be opened" << std::endl;
    }
    file.write(reinterpret_cast<char*>(&score), sizeof(score));
    file.close();
    isSavedFile.write(reinterpret_cast<char*>(&isGameSaved), sizeof(isGameSaved));
    isSavedFile.close();
    isGameSaved = true;
    std::cout << "I am saving game" << std::endl;
}

void Dungeon::LoadFromFile(const std::string& pathFile) {
    std::ofstream isSavedFile("isGameSaved.dat", std::ios::binary);
    std::ifstream file(pathFile, std::ios::binary);
    if(!file.is_open()) {
        std::cerr << "File could not be opened" << std::endl;
    }
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.close();
    isSavedFile.write(reinterpret_cast<char*>(&isGameSaved), sizeof(isGameSaved));
    isSavedFile.close();
    isGameSaved = false;
    std::cout << "I loded game" << std::endl;
}