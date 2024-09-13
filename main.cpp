#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <cmath>
#include "Complex.h"

#define STEPS 256

typedef sf::Vector2<double> Vector2d;

Vector2d operator* (Vector2d a, const double& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

inline long long step(const Complex& C) {
    Complex nw(0, 0);
    long long steps = 0;
    for (; steps < STEPS && abs(nw) <= 4; ++steps) {
        nw *= nw;
        nw += C;
    }
    return steps;
}

void generatePart(sf::Image& image, const long long startY, const long long height, const long long width,
    const double& one, const Vector2d& center) {
    for (long long y = startY; y < startY + height; ++y) {
        for (long long x = 0; x < width; ++x) {
            Complex C(
                        (static_cast<double>(x) - center.x) / one,
                        (static_cast<double>(y) - center.y) / one
                    );

            auto color = STEPS - step(C);

            image.setPixel(x, y, sf::Color(
                (4 * color) % 256,
                (6 * color) % 256,
                (8 * color) % 256)
            );
        }
    }
}

int main() {
    const auto numThreads = std::thread::hardware_concurrency();


    sf::RenderWindow window({1500, 1500}, "Set");
    Vector2d beg{-100, -100};
    Vector2d now;
    bool resized = false;
    sf::Image ff;
    ff.loadFromFile("..\\start.png");
    std::vector<sf::Image> poses;
    poses.push_back(ff);
    std::vector<Vector2d> centers = {{1050, 750}};
    std::vector<double> ones = {500};
    bool flag = false;
    bool focused = true;
    while (window.isOpen()) {
        auto one = ones.back();
        auto center = centers.back();
        sf::Event event{};
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::LostFocus) {
                focused = false;
            } else if (event.type == sf::Event::GainedFocus) {
                focused = true;
            }
            if (!focused) break;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                double mX = static_cast<double>(sf::Mouse::getPosition(window).x);
                double mY = static_cast<double>(sf::Mouse::getPosition(window).y);
                if (beg == Vector2d{-100, -100} || !flag) {
                    beg = Vector2d{mX, mY};
                }
                double nX, nY;
                if (mX - beg.x < 0) {
                    nX = -(mX - beg.x);
                } else {
                    nX = mX - beg.x;
                }
                if (mY - beg.y < 0) {
                    nY = -(mY - beg.y);
                } else {
                    nY = mY - beg.y;
                }
                flag = true;
                now = beg + Vector2d{std::max(nX, nY), std::max(nX, nY)};
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (beg != Vector2d{-100, -100}) {
                    double w = ((now - beg) * 2).x;
                    center -= (beg - (now - beg));
                    center.x *= 1500;
                    center.x /= w;
                    center.y *= 1500;
                    center.y /= w;
                    one *= 1500;
                    one /= w;
                    beg = {-100, -100};
                    flag = false;
                    resized = true;
                }
            } else if(event.type == sf::Event::KeyPressed &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                flag = false;
                if (poses.size() > 1) {
                    beg = {-100.f, -100.f};
                    centers.pop_back();
                    ones.pop_back();
                    poses.pop_back();
                }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    flag = false;
                    center.y += 100;
                    resized = true;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    flag = false;
                    center.y -= 100;
                    resized = true;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    flag = false;
                    center.x += 100;
                    resized = true;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    flag = false;
                    center.x -= 100;
                    resized = true;
                } else {
                    flag = false;
                }
        }

        window.clear();
        if (resized) {
            ff.create(1500, 1500);

            std::vector<std::thread> threads;
            auto partHeight = 1500 / numThreads;
            for (auto i = 0ull; i < numThreads; ++i) {
                auto startY = i * partHeight;
                auto heightToProcess = (i == numThreads - 1) ? (1500 - startY) : partHeight;
                threads.emplace_back(generatePart,
                    std::ref(ff), startY, heightToProcess, 1500, one, center);

            }

            for (auto& thread : threads) {
                thread.join();
            }
            poses.push_back(ff);
            centers.push_back(center);
            ones.push_back(one);
            beg = {-100.f, -100.f};
            resized = false;
        }
        sf::Texture l;
        sf::Sprite s;
        l.loadFromImage(poses.back());
        s.setTexture(l);
        s.setPosition(0, 0);
        window.draw(s);
        if (beg != Vector2d{-100.f, -100.f}) {
            sf::RectangleShape a;
            if (now == Vector2d{-100.f, -100.f}) {
                now = {static_cast<double>(sf::Mouse::getPosition(window).x),
                    static_cast<double>(sf::Mouse::getPosition(window).y)};
            }
            a.setSize(sf::Vector2f((now - beg) * 2));
            a.setPosition(sf::Vector2f(beg - (now - beg)));
            a.setFillColor(sf::Color(3, 144, 252, 100));
            a.setOutlineColor(sf::Color(3, 78, 252, 200));
            a.setOutlineThickness(3);
            window.draw(a);
        }
        // window.draw(c);
        window.display();
    }
}