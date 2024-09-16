#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include "Complex.h"
#include <vector>
#define WIDTH (long long)1500
#define HEIGHT (long long)1500

typedef sf::Vector2<double> Vector2d;

const auto threadAmount = std::thread::hardware_concurrency();

inline Vector2d operator* (Vector2d a, const double& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

class Mandelbrot {
  public:
    Mandelbrot() = default;
    Mandelbrot(const sf::Image&, const Vector2d&, const double&);
    void draw(sf::RenderWindow&);
    [[nodiscard]] Vector2d getCenter() const {
       return center_;
    }
    [[nodiscard]] double getScale() const {
      return scale_;
    }
  private:
    void part_(const unsigned&, const unsigned&);
    void generate_();
    static long long steps_(const Complex& C);
    sf::Image image_;
    Vector2d center_;
    double scale_ = 800. / 3;
    bool generated_{};
};

class App final : public sf::RenderWindow {
  public:
    App();
    ~App() override;
  private:
    void loop_();
    static constexpr long long height_ = 800;
    static constexpr long long width_ = 800;
    std::vector<Mandelbrot> states_;
};
