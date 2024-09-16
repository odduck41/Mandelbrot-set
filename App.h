#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Complex.h"
#define WIDTH (long long)1500
#define HEIGHT (long long)1500


typedef sf::Vector2<double> Vector2d;

inline Vector2d operator* (Vector2d a, const double& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

class App : public sf::RenderWindow {
    public:
        App();
        void loop();
        ~App() override;
    private:

};


class Mandelbrot {
    public:
        void draw(sf::Image&);
    private:
        void part();
        long long steps(const Complex& C);
};