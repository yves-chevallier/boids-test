#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include "vector2.hpp"
#include "mobile.hpp"
#include "flock.hpp"

#define WIDTH 800
#define HEIGHT 600
#define FRAMERATE 60

void draw(sf::RenderWindow& window, Flock &flock) {
    sf::CircleShape shape(5);
    shape.setFillColor(sf::Color::Red);
    for (auto& mobile : flock.flock) {
        shape.setPosition(mobile.position.x, mobile.position.y);
        window.draw(shape);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BOIDS");

    // Create mobiles
    Flock flock(WIDTH, HEIGHT);
    flock.generate(100);

    // Time management
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / FRAMERATE);
    window.setFramerateLimit(FRAMERATE);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        draw(window, flock);
        window.display();

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            flock.update();
        }
    }
}
