/**
 * Performances test when drawing a lot of circles
 * Use a vertex array to draw circles
 */

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <sstream>

#include "../hsl.hpp"

#define WIDTH 800
#define HEIGHT 800
using namespace std;

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

void addCircle(sf::VertexArray &array, sf::Vector2f position, float radius, sf::Color color) {
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

#define FRAMERATE 120

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RTREE", sf::Style::Close);

    // Fps text
    sf::Font font;
    if (!font.loadFromFile("../assets/collegiate.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    sf::Text fps;
    fps.setFont(font);
    fps.setPosition(10, 10);
    fps.setCharacterSize(16);
    fps.setFillColor(sf::Color::White);

    // Time management
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / FRAMERATE);
    sf::Time timeSinceLastDraw = sf::Time::Zero;
    window.setFramerateLimit(FRAMERATE);

    float vr = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        size_t count = 15;
        sf::Color color = sf::Color::Red;
        color.a = 50;
        sf::Color color2 = sf::Color::Blue;
        color2.a = 20;

        sf::VertexArray objects(sf::Triangles);

        for (int k = 0; k < 5; k++) {
            HSL hsl = HSL(0, 100, 50);
            hsl.hue = k * 360 / 5;
            sf::Color c = hsl.toRGB();
            c.a = 10;
            for (int j = 20; j < 400; j += 5) {
                for (int i = 0; i < count; i++) {
                    float angle = i * 2 * M_PI / count;
                    angle += (i % 2 ? vr : -vr) * k * (k % 2 ? 1 : -1);
                    addCircle(objects, sf::Vector2f(WIDTH / 2 + cos(angle) * j, HEIGHT / 2 + sin(angle) * j), j / 5, c);
                }
            }
        }


        timeSinceLastDraw = clock.restart();
        timeSinceLastUpdate += timeSinceLastDraw;
        double timeFps = 1.f / timeSinceLastDraw.asSeconds();
        fps.setString("verticles: " + to_string_with_precision(objects.getVertexCount(), 0) + " fps: " + to_string_with_precision(timeFps, 0));

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            vr += 0.001;
        }

        window.clear();
        window.draw(objects);
        window.draw(fps);
        window.display();
    }
}
