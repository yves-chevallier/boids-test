#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>


#define WIDTH 800
#define HEIGHT 600
#define FRAMERATE 120

void insertBoid(sf::VertexArray& array, int x, int y, float angle) {
    int width = 3, height = 10;

    // Set the triangle shape. Origin is the front of the Boid.
    sf::Vertex v1(sf::Vector2f(x, y));
    sf::Vertex v2(sf::Vector2f(v1.position.x - width, v1.position.y - height));
    sf::Vertex v3(sf::Vector2f(v1.position.x + width, v1.position.y - height));

    // Setting color
    v1.color = v2.color = v3.color = sf::Color::White;

    // Set the orientation
    sf::Transform transform;
    angle = angle * 180 / M_PI;
    angle -= 90; // because the angle is calculated from the x axis
    transform.rotate(angle, (v2.position.x + v3.position.x) / 2, v1.position.y - height / 2);
    v1.position = transform.transformPoint(v1.position);
    v2.position = transform.transformPoint(v2.position);
    v3.position = transform.transformPoint(v3.position);

    // Appending them into vertex array
    array.append(v1);
    array.append(v2);
    array.append(v3);
}


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4.0;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BOIDS", sf::Style::Close, settings);

    // Time management
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / FRAMERATE);
    sf::Time timeSinceLastDraw = sf::Time::Zero;
    window.setFramerateLimit(FRAMERATE);

    // Fps text
    sf::Font font;
    if (!font.loadFromFile("assets/collegiate.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    sf::Text fps;
    fps.setFont(font);
    fps.setCharacterSize(20);
    fps.setFillColor(sf::Color::White);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();

        timeSinceLastDraw = clock.restart();
        timeSinceLastUpdate += timeSinceLastDraw;
        double timeFps = 1.f / timeSinceLastDraw.asMilliseconds() * 1000;

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            fps.setString(std::to_string(std::round(timeFps)));
        }

        sf::VertexArray boids(sf::Triangles, 3);

        for (int i = 0; i < 1000; i++) {
            insertBoid(boids, (float)(rand() % WIDTH), (float)(rand() % HEIGHT), (float)(rand() % 360));
        }
        window.draw(boids);
        window.draw(fps);
        window.display();
    }
}
