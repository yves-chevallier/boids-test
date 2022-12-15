#pragma once

#include <boost/circular_buffer.hpp>

#include "vector2.hpp"

class Flock;

class Mobile {
   public:
    using value_type = float;
    using vector_type = Vector2<value_type>;


    vector_type position;
    vector_type velocity;

    Mobile(Flock &flock, vector_type position);

    float angle();
    float speed();

    float angleTo(Mobile &other);
    float distanceTo(Mobile &other);

    void bounce(float margin, float turnFactor);
    void wrap();

    void update();

   protected:
    bool is_wrap;
    float max_velocity;

    boost::circular_buffer<vector_type> history;
    size_t max_history;

    Flock &flock;
};
