#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define WIDTH 100
#define HEIGHT 100
using namespace std;


int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RTREE",
                            sf::Style::Close);

    sf::Shader shader;
    shader.loadFromFile("shader.frag", sf::Shader::Fragment);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        sf::CircleShape circle(50);
        circle.setOrigin(50, 50);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(100,100);
        size_t count = circle.getPointCount();
        sf::VertexArray objects(sf::TriangleFan, count+1);

        // float radius = 50;
        // for (int i = 0; i < 60; i++) {
        //     float angle = i * 2 * M_PI / 60;
        //     sf::Vertex v;

        //     v.position = sf::Vector2f(50 + cos(angle)*radius, 50 + sin(angle)*radius);
        //     v.color = sf::Color::Red;
        //     objects.append(v);
        //     //std::cout << point.x << " " << point.y << std::endl;
        // }
        objects.append(circle.getPosition());
        for (int i = 0; i < count; i++) {
           objects.append(circle.getPoint(i));

        }

        for (int i = 0; i < objects.getVertexCount(); i++) {
            objects[i].color = sf::Color::Red;
            cout << i << " " << objects[i].position.x << " " << objects[i].position.y << endl;
        }
        return 0;
        window.clear();
        window.draw(objects);
        window.display();
    }
}
