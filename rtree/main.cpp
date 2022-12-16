#include <SFML/Graphics.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

using point_2d = bg::model::d2::point_xy<double>;
using box = bg::model::box<point_2d>;

struct Foo {
    mutable bool selected = false;
    point_2d position;
    mutable sf::CircleShape circle;

    Foo(double x, double y) : position(x, y) {
        circle.setRadius(2);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition(position.x(), position.y());
    }
    auto get() const { return position; }
    void draw() const {
        if (selected)
            circle.setFillColor(sf::Color::Red);
        else
            circle.setFillColor(sf::Color::Blue);

        circle.setPosition(position.x(), position.y());
    }
    struct ByPos {
        using result_type = point_2d;
        result_type const& operator()(Foo const& f) const { return f.position; }
    };
};

auto intersecting(auto const& search, auto const& tree, float radius) {
    std::vector<std::reference_wrapper<Foo const>> result;
    tree.query(bgi::satisfies([&](auto const& f) {
                   return bg::distance(f.position, search) < radius ;
               }),
               std::back_inserter(result));
    return result;
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4.0;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RTREE",
                            sf::Style::Close, settings);
    window.setMouseCursorVisible(false);
    // Create points
    bgi::rtree<Foo, bgi::quadratic<16>, Foo::ByPos> rtree;
    for (int i = 0; i < 100; ++i) {
        rtree.insert({(double)(rand() % WIDTH), (double)(rand() % HEIGHT)});
    }

    // box with given distance from center
    auto makebox = [&](double d, point_2d c = {}) {
        return box{{c.x() - d, c.y() - d}, {c.x() + d, c.y() + d}};
    };
    sf::CircleShape spot(50);
    spot.setOrigin(spot.getRadius(), spot.getRadius());
    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta > 0) {
                    spot.setRadius(spot.getRadius() + 10);
                } else {
                    spot.setRadius(spot.getRadius() - 10);
                    if (spot.getRadius() < 10) spot.setRadius(10);
                }
                spot.setOrigin(spot.getRadius(), spot.getRadius());
            }
        }

        spot.setFillColor(sf::Color(255, 0, 0, 50));

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        spot.setPosition(mousePos.x, mousePos.y);
        auto key = makebox(spot.getRadius(), {mousePos.x, mousePos.y});

        for (auto const& foo : rtree) {
            foo.selected = false;
        }

        for (Foo const& foo : intersecting(point_2d(mousePos.x, mousePos.y), rtree, spot.getRadius())) {
            foo.selected = true;
        }

        window.clear(sf::Color::Green);
        sf::CircleShape circle(50);
        circle.setPosition(100,100);
        sf::VertexArray objects(sf::TriangleStrip, circle.getPointCount());
        for (int i = 0; i < circle.getPointCount(); i++) {
           std::cout << circle.getPoint(i).x << " " << circle.getPoint(i).y << std::endl;
           objects.append(sf::Vector2f(circle.getPoint(i).x, circle.getPoint(i).y));

        }
        objects.setFillColor(sf::Color::Red);
        //return 0;

        // for (auto& foo : rtree) {
        //     foo.draw();
        //     circle.setPosition(foo.position.x(), foo.position.y());
        //     for (int i = 0; i < circle.getPointCount(); ++i) {
        //         auto p = circle.getPoint(i);
        //         objects.append(sf::Vector2f(p.x, p.y));
        //     }
        for (int i = 0; i < objects.getVertexCount(); i++) {
            objects[i].color = sf::Color::Blue;

        }
        // }
        window.draw(objects);
        window.draw(spot);
        window.display();
    }
}
