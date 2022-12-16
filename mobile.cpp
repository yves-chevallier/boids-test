#include "mobile.hpp"
#include "flock.hpp"

Mobile::Mobile(Flock &flock, vector_type position, vector_type speed)
    : position(position), velocity(speed), is_wrap(false), max_velocity(0.1),
      max_history(100), flock(flock)
{
    history.set_capacity(max_history);
}

float Mobile::speed()
{
    return velocity.magnitude();
}

float Mobile::angle() { return velocity.angle(); }
float Mobile::angleTo(Mobile &other) { return position.angle(other.position); }

float Mobile::distanceTo(Mobile &other)
{
    return is_wrap ? position.toroidal_distance(other.position, flock.width,
                                                flock.height )
                : position.distance(other.position);
}

/**
 * The Mobile would bounce on the edge of the map with
 * a turn factor, at a certain distance (margin) of
 * the edge.
 */
void Mobile::bounce(float margin, float turnFactor)
{
    if (position.x < margin) velocity.x += turnFactor;
    if (position.y < margin) velocity.y += turnFactor;
    if (position.x > flock.width - margin) velocity.x -= turnFactor;
    if (position.y > flock.height- margin) velocity.y -= turnFactor;
}

/**
 * The Mobile suddently appear at the opposite of the map
 * if it crosses the boundaries.
 */
void Mobile::wrap()
{
    if (position.x < 0) position.x += flock.width;
    if (position.y < 0) position.y += flock.height;

    if (position.x > flock.width) position.x -= 1.0;
    if (position.y > flock.height) position.y -= 1.0;
}


void Mobile::update()
{
    if (is_wrap)
        wrap();
    else
        bounce(speed() * 5.0, speed() / 5.0);

    // Update position
    velocity.limit(max_velocity);
    position += velocity;

    // Record previous position
    if (history.size() > max_history)
        history.pop_front();

    history.push_back(position);
}

// Add a small random velocity to the Mobile
void Mobile::roam() {
    float chance = rand() / (float)RAND_MAX;
    if (chance > 0.80) {
        float step = 0.2;
        float vx = (rand() / (float)RAND_MAX - 0.5) * step;
        float vy = (rand() / (float)RAND_MAX - 0.5) * step;
        velocity += Vector2<float>(vx, vy);
        velocity.limit(max_velocity);
    }
}
