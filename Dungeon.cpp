#include "Dungeon.h"

#include <iostream>

#include "TreasureRoom.h"

Dungeon::Dungeon(): currentRoom(0),maxRoomCooldown(2000){
}

Dungeon::~Dungeon() {
    rooms.clear();
}

void Dungeon::Initialize() {
    //@todo tutaj będzie losowanie room od 5 do 10
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    roomsNum = std::rand() %  6 + 5;

   // Room room(Difficulty::Easy, currentRoom);
    rooms.push_back(std::make_unique<Room>(Difficulty::Easy, currentRoom));

    player.Load();
    rooms[currentRoom]->Load();
    rooms[currentRoom]->Initialize();
    player.Initialize(*rooms[currentRoom]);
}

void Dungeon::Load() {

}

void Dungeon::Update(float& deltaTime) {
    player.Update(deltaTime);
    rooms[currentRoom]->Update(deltaTime, player);


    enterRoomTimer+=deltaTime;
    if(rooms[currentRoom]->IsPlayerEnterNewRoom(player) && (enterRoomTimer >= maxRoomCooldown)) {
        std::cout<<"Wszedł"<<std::endl;
        enterRoomTimer = 0;
        openDoors.emplace_back(rooms[currentRoom]->GetOpenDoor());
        CreateNextRoom();
    }
}

void Dungeon::Draw(sf::RenderWindow &window) {
    rooms[currentRoom]->Draw(window);
    player.Draw(window);
}

void Dungeon::CreateNextRoom() {
    Difficulty difficulty = static_cast<Difficulty>(currentRoom % 3);
    currentRoom++;
    if (currentRoom == 1) {
        rooms.push_back(std::make_unique<TreasureRoom>(currentRoom)); // TreasureRoom
    } else {
        rooms.push_back(std::make_unique<Room>(difficulty, currentRoom)); // Room
    }

    const sf::Vector2i& prevDoor = openDoors[currentRoom - 1];
    const int roomWidth = rooms[currentRoom]->GetRoomWidthPX();
    const int roomHeight = rooms[currentRoom]->GetRoomHeightPX();

    if (prevDoor == rooms[currentRoom - 1]->GetTopDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2, roomHeight - 33 - 15));
    } else if (prevDoor == rooms[currentRoom - 1]->GetBottomDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth / 2, 33));
    } else if (prevDoor == rooms[currentRoom - 1]->GetLeftDoor()) {
        player.SetPosition(sf::Vector2f(roomWidth - 32 - 15, roomHeight / 2));
    } else if (prevDoor == rooms[currentRoom - 1]->GetRightDoor()) {
        player.SetPosition(sf::Vector2f(33, roomHeight / 2));
    }

    rooms[currentRoom]->Load();
    rooms[currentRoom]->Initialize();
    player.SetCurrentRoom(*rooms[currentRoom]);
}