#include "FrameRate.h"
#include <iostream>

FrameRate::FrameRate() : timer(0) {
}

FrameRate::~FrameRate() {
}

void FrameRate::Initialize() {
}

void FrameRate::Load() {
    if (font.loadFromFile("../Assets/Fonts/Oswald-Regular.ttf")) {
        std::cout << "Font loaded" << std::endl;
        frameRateText.setFont(font);
    } else {
        std::cerr << "Failed to load Oswald font." << std::endl;
    }
}

void FrameRate::Update(float deltaTime) {
    timer += deltaTime;

    if(timer >= 100.0) {
        float fps = 1000.0;
        frameRateText.setString("FPS: " + std::to_string((int)(fps / deltaTime)) + " Frame Time: " + std::to_string((int)deltaTime));
        timer = 0;
    }
}

void FrameRate::Draw(sf::RenderWindow &window) {
    window.draw(frameRateText);
}
