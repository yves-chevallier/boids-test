#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>

struct HSL {
    double hue;
    double saturation;
    double luminance;

    HSL() : hue(0), saturation(0), luminance(0) {}

    HSL(int H, int S, int L) {
        // Range control for hue.
        if (H <= 360 && H >= 0) {
            hue = H;
        } else {
            if (H > 360) {
                hue = H % 360;
            } else if (H < 0 && H > -360) {
                hue = -H;
            } else if (H < 0 && H < -360) {
                hue = -(H % 360);
            }
        }

        // Range control for saturation.
        if (S <= 100 && S >= 0) {
            saturation = S;
        } else {
            if (S > 100) {
                saturation = S % 100;
            } else if (S < 0 && S > -100) {
                saturation = -S;
            } else if (S < 0 && S < -100) {
                saturation = -(S % 100);
            }
        }

        // Range control for luminance
        if (L <= 100 && L >= 0) {
            luminance = L;
        } else {
            if (L > 100) {
                luminance = L % 100;
            }
            if (L < 0 && L > -100) {
                luminance = -L;
            }
            if (L < 0 && L < -100) {
                luminance = -(L % 100);
            }
        }
    }
    static HSL fromHex(const std::string& hex) {
        if (hex.size() != 7) {
            throw std::invalid_argument("Invalid hex string");
        }

        if (hex[0] != '#') {
            throw std::invalid_argument("Invalid hex string");
        }

        int r = std::stoi(hex.substr(1, 2), nullptr, 16);
        int g = std::stoi(hex.substr(3, 2), nullptr, 16);
        int b = std::stoi(hex.substr(5, 2), nullptr, 16);

        return HSL::fromRGB(sf::Color(r, g, b));
    }
    static HSL fromRGB(const sf::Color& C) {
        const double epsilon = std::numeric_limits<double>::epsilon();

        // Trivial cases.
        if (C == sf::Color::White) {
            return HSL(0, 0, 100);
        }

        if (C == sf::Color::Black) {
            return HSL(0, 0, 0);
        }

        if (C == sf::Color::Red) {
            return HSL(0, 100, 50);
        }

        if (C == sf::Color::Yellow) {
            return HSL(60, 100, 50);
        }

        if (C == sf::Color::Green) {
            return HSL(120, 100, 50);
        }

        if (C == sf::Color::Cyan) {
            return HSL(180, 100, 50);
        }

        if (C == sf::Color::Blue) {
            return HSL(240, 100, 50);
        }

        if (C == sf::Color::Cyan) {
            return HSL(300, 100, 50);
        }

        double R = C.r / 255.0;
        double G = C.g / 255.0;
        double B = C.b / 255.0;

        double max = std::max(std::max(R, G), B);
        double min = std::min(std::min(R, G), B);

        HSL A;
        double l = ((max + min) / 2.0);
        double s = 0;

        if (max - min <= epsilon) {
            A.hue = 0;
            A.saturation = 0;
        } else {
            double diff = max - min;

            s = diff / (A.luminance < 0.5 ? max + min : 2 - max - min);

            double diffR = (((max - R) * 60) + (diff / 2.0)) / diff;
            double diffG = (((max - G) * 60) + (diff / 2.0)) / diff;
            double diffB = (((max - B) * 60) + (diff / 2.0)) / diff;

            if (max - R <= epsilon) {
                A.hue = diffB - diffG;
            } else if (max - G <= epsilon) {
                A.hue = (1 * 360) / 3.0 + (diffR - diffB);
            } else if (max - B <= epsilon) {
                A.hue = (2 * 360) / 3.0 + (diffG - diffR);
            }

            if (A.hue <= 0 || A.hue >= 360) {
                fmod(A.hue, 360);
            }

            s *= 100;
        }

        l *= 100;
        A.saturation = s;
        A.luminance = l;
        return A;
    }

    sf::Color toRGB() {
        // Reconvert to range [0,1]
        double h = hue / 360.0;
        double s = saturation / 100.0;
        double l = luminance / 100.0;

        if (s <= std::numeric_limits<double>::epsilon()) {
            return sf::Color(l * 255, l * 255, l * 255);
        }

        float arg2 = l < 0.5 ? l * (1 + s) : (l + s) - (s * l);
        float arg1 = 2 * l - arg2;

        return sf::Color(255 * hueToRGB(arg1, arg2, (h + 1.0 / 3.0)), 255 * hueToRGB(arg1, arg2, h),
                         255 * hueToRGB(arg1, arg2, (h - 1.0 / 3.0)));
    }

   protected:
    double hueToRGB(double arg1, double arg2, double h) {
        if (h < 0) h += 1;
        if (h > 1) h -= 1;
        if ((6 * h) < 1) {
            return (arg1 + (arg2 - arg1) * 6 * h);
        }
        if ((2 * h) < 1) {
            return arg2;
        }
        if ((3 * h) < 2) {
            return (arg1 + (arg2 - arg1) * ((2.0 / 3.0) - h) * 6);
        }
        return arg1;
    }
};
