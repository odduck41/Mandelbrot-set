#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <vector>
#include "Complex.h"
#define WIDTH (long long)600
#define HEIGHT (long long)600

typedef sf::Vector2<double> Vector2d;

const auto threadAmount = std::thread::hardware_concurrency();

inline Vector2d operator* (Vector2d a, const double& b) {
    a.x *= b;
    a.y *= b;
    return a;
}


inline Vector2d& operator*= (Vector2d& a, const double& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

inline Vector2d& operator/= (Vector2d& a, const double& b) {
	  a.x /= b;
	  a.y /= b;
	  return a;
}

class Mandelbrot {
  public:
    Mandelbrot() = default;
    Mandelbrot(const Vector2d&, const double&);
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
    double scale_ = WIDTH / 3.;
    bool generated_{};
};

class Selector final : public sf::RectangleShape {
  public:
    Selector() = default;
    explicit Selector(const Vector2d&);
    void update(const Vector2d&);
    void draw(sf::RenderWindow&) const;
    [[nodiscard]] Vector2d getBegin() const {
        return beg_;
    }
    [[nodiscard]] double getSide() const {
        return (size_.x - beg_.x) * 2;
    }
  private:
    Vector2d beg_;
    Vector2d size_;
};

class App final : public sf::RenderWindow {
  public:
    App();
    void nextStep();
    void prevStep();
    ~App() override;
  private:
    void loop_();
    static constexpr long long height_ = HEIGHT;
    static constexpr long long width_ = WIDTH;
    std::vector<Mandelbrot> states_;
    Vector2d center_{width_ * 1050. / 1500., height_ / 2.};
    double scale_{width_ * 1. / 3};
    Selector* selector_{};
};
