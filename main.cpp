#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include "vector2.hpp"
#include "mobile.hpp"
#include "flock.hpp"
#include "hsl.hpp"

#define WIDTH 800
#define HEIGHT 600
#define FRAMERATE 60

sf::VertexArray boidShape(int x, int y, float angle) {
    int boidWidth = 3;
    int boidHeight = 10;

    // set the center position
    sf::Vertex v1(sf::Vector2f(x, y));
    sf::Vertex v2(
        sf::Vector2f(v1.position.x - boidWidth, v1.position.y - boidHeight));
    sf::Vertex v3(
        sf::Vector2f(v1.position.x + boidWidth, v1.position.y - boidHeight));

    // setting color
    v1.color = v2.color = v3.color = HSL::fromHex("#3589f2").toRGB();

    // set the angle
    sf::Transform transform;
    transform.rotate(angle, (v2.position.x + v3.position.x) / 2,
                     v1.position.y - 5);
    v1.position = transform.transformPoint(v1.position);
    v2.position = transform.transformPoint(v2.position);
    v3.position = transform.transformPoint(v3.position);

    // appending them into vertex array
    sf::VertexArray array(sf::Triangles, 3);
    array.append(v1);
    array.append(v2);
    array.append(v3);
    return array;
}

void draw(sf::RenderWindow& window, Flock &flock) {
    for (auto& mobile : flock.flock) {
        sf::VertexArray shape = boidShape(mobile.position.x, mobile.position.y,
                                          mobile.angle() * 180 / M_PI);
        window.draw(shape);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BOIDS");

    // Create mobiles
    Flock flock(WIDTH, HEIGHT);
    flock.generate(400);

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
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space)
                    flock.update();
            }
        }

        window.clear(HSL::fromHex("#282C34").toRGB());
        draw(window, flock);
        window.display();

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            flock.update();
        }
    }
}
