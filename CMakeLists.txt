cmake_minimum_required(VERSION 3.29)
project(MyIsaacGame)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS FALSE)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)

FetchContent_MakeAvailable(SFML)

add_executable(MyIsaacGame main.cpp
        Player.cpp
        Player.h
        Skeleton.cpp
        Skeleton.h
        Math.cpp
        Math.h
        FrameRate.cpp
        FrameRate.h
        Bullet.cpp
        Bullet.h
        Enemy.cpp
        Enemy.h
        Dungeon.cpp
        Dungeon.h
        Room.cpp
        Room.h
        Animate.cpp
        Animate.h
        Difficulty.h
        Globals.cpp
        Globals.h
        Obstacle.cpp
        Obstacle.h
        FireObstacle.cpp
        FireObstacle.h
        RockObstacle.cpp
        RockObstacle.h
        TreasureRoom.cpp
        TreasureRoom.h
        Item.cpp
        Item.h
        Charger.cpp
        Charger.h
        BombMan.cpp
        BombMan.h
)
target_link_libraries(MyIsaacGame sfml-graphics sfml-window sfml-system sfml-audio)