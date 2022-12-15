#pragma once

#include "vector2.hpp"
#include "mobile.hpp"

#include <random>
#include <vector>

class Flock {
public:
    using value_type = Mobile;
    using vector_type = Vector2<value_type>;

    float width;
    float height;
    float margin;
    float turnFactor;
    float maxVelocity;

    std::vector<value_type> flock;

    Flock(float width, float height);
    void generate(size_t count);

    void update();
};
