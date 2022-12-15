#include "vector2.hpp"
#include "flock.hpp"
#include "mobile.hpp"
#include <random>
#include <vector>

struct MobileGenerator {
    Flock &flock;

    std::uniform_real_distribution<> height;
    std::uniform_real_distribution<> width;
    std::uniform_real_distribution<> speed;
    std::random_device rd;
    std::mt19937 gen;

    MobileGenerator(Flock &flock) : flock(flock), height(0,flock.height), width(0,flock.width), speed(-1.f, 1.f), gen(rd()) {}
    MobileGenerator(MobileGenerator const&m) : flock(m.flock), height(m.height), width(m.width), speed(m.speed), gen(rd()) {}

    auto operator() () { return Mobile(flock, Vector2<float>(width(gen), height(gen)), Vector2<float>(speed(gen), speed(gen))); }
};

Flock::Flock(float width, float height) :
        width(width), height(height), margin(10), turnFactor(0.01), maxVelocity(10) {}

void Flock::generate(size_t count) {
        MobileGenerator generator(*this);
        std::generate_n(std::back_inserter(flock), count, generator);
    }

void Flock::update() {
        std::for_each(flock.begin(), flock.end(), [&](Mobile& mobile) {
            mobile.bounce(margin, turnFactor);
            mobile.wrap();
            mobile.update();
        });
    }
