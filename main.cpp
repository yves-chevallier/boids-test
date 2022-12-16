#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "flock.hpp"
#include "hsl.hpp"
#include "mobile.hpp"
#include "vector2.hpp"

#define WIDTH 800
#define HEIGHT 600
#define FRAMERATE 120


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

float radius = 10;
void insertCircle(sf::VertexArray &array, sf::Vector2f position, float radius, sf::Color color) {
    float perimeter = 2 * M_PI * radius;
    size_t count = perimeter / 10;

    for (int i = 0; i < count; i += 1) {
        sf::Vertex v0;
        v0.position = sf::Vector2f(position.x, position.y);
        v0.color = color;
        array.append(v0);

        sf::Vertex v1;
        float angle = i * 2 * M_PI / count;
        v1.position = sf::Vector2f(position.x + cos(angle) * radius, position.y + sin(angle) * radius);
        v1.color = color;
        array.append(v1);

        sf::Vertex v2;
        angle = (i + 1) * 2 * M_PI / count;
        v2.position = sf::Vector2f(position.x + cos(angle) * radius, position.y + sin(angle) * radius);
        v2.color = color;
        array.append(v2);
    }
}

void insertBoid(sf::VertexArray& array, int x, int y, float angle) {
    int width = 2, height = 7;

    // Set the triangle shape. Origin is the front of the Boid.
    sf::Vertex v1(sf::Vector2f(x, y));
    sf::Vertex v2(sf::Vector2f(v1.position.x - width, v1.position.y - height));
    sf::Vertex v3(sf::Vector2f(v1.position.x + width, v1.position.y - height));

    // Setting color
    v1.color = v2.color = v3.color = HSL::fromHex("#3589f2").toRGB();

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

    // Add radius
    sf::Color c(HSL::fromHex("#E24471").toRGB());
    c.a = 10;
    insertCircle(array, v1.position, radius, c);
}

void draw(sf::RenderWindow& window, Flock& flock) {
    sf::VertexArray boids(sf::Triangles, 3);

    for (auto& mobile : flock.flock) {
        insertBoid(boids, mobile.position.x, mobile.position.y, mobile.angle());
    }
    window.draw(boids);
}


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2.0;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BOIDS", sf::Style::Close, settings);
    // Create mobiles
    Flock flock(WIDTH, HEIGHT);
    flock.generate(5000);

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
    fps.setPosition(10, 10);
    fps.setCharacterSize(16);
    fps.setFillColor(HSL::fromHex("#AFE0DC").toRGB());

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    flock.wrap = !flock.wrap;
                };
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    radius += 10;
                } else {
                    radius -= 10;
                }
            }
        }

        window.clear(HSL::fromHex("#282C34").toRGB());

        timeSinceLastDraw = clock.restart();
        timeSinceLastUpdate += timeSinceLastDraw;
        double timeFps = 1.f / timeSinceLastDraw.asSeconds();
        fps.setString("FPS: " + to_string_with_precision(timeFps, 0));

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            flock.update();
        }
        draw(window, flock);
        window.draw(fps);
        window.display();
    }
}
