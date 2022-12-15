#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>

struct HSL {
    double hue;
    double saturation;
    double luminance;

    HSL();
    HSL(int H, int S, int L);

    static HSL fromHex(const std::string& hex);
    static HSL fromRGB(const sf::Color& rgb);
    sf::Color toRGB();

   protected:
    double hueToRGB(double arg1, double arg2, double H);
};
