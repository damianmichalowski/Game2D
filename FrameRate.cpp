#include "FrameRate.h"
#include <iostream>

FrameRate::FrameRate() : timer(0) {
}

FrameRate::~FrameRate() {
}

void FrameRate::Initialize(sf::View &view) {
    frameRateText.setFont(font);
    frameRateText.setCharacterSize(9);
    frameRateText.setFillColor(sf::Color::Green);
    frameRateText.setPosition(view.getSize().x - 28, 2);
}

void FrameRate::Load() {
    if (!font.loadFromFile("../Assets/Fonts/Oswald-ExtraLight.ttf")) {
        std::cout << "Failed to load Font Oswald in frameRate" << std::endl;
    }
}

void FrameRate::Update(float deltaTime) {
    timer += deltaTime;

    if(timer >= 100.0) {
        float fps = 1000.0;
        frameRateText.setString("fps: " + std::to_string((int)(fps / deltaTime)) + "\ntick: " + std::to_string((int)deltaTime));
        timer = 0;
    }
}

void FrameRate::Draw(sf::RenderWindow &window) {
    window.draw(frameRateText);
}
