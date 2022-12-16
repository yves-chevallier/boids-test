#pragma once

#include "vector2.hpp"
#include "mobile.hpp"

#include <random>
#include <vector>

class Flock {
public:
    using value_type = Mobile;
    using vector_type = Vector2<value_type>;

    // World dimensions
    float width;
    float height;

    // Boid parameters
    float margin;
    float turnFactor;
    float maxVelocity;

    // Wrap around the world (toroidal world) or void walls
    bool wrap;

    std::vector<value_type> flock;

    Flock(float width, float height);
    void generate(size_t count);

    void update();
};
