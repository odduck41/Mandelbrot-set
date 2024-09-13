#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <cmath>
#include "Complex.h"

#define STEPS 256

sf::Vector2f operator* (sf::Vector2f a, const long long& b) {
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

void generatePart(sf::Image& image, long long startY, long long height, long long width,
    const double& one, const sf::Vector2f& center) {
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
    const int numThreads = std::thread::hardware_concurrency();


    sf::RenderWindow window({1500, 1500}, "Set");
    sf::Vector2f beg{-100, -100};
    sf::Vector2f now;
    bool resized = false;
    sf::Image ff;
    ff.loadFromFile("..\\start.png");
    std::vector<sf::Image> poses;
    poses.push_back(ff);
    std::vector<sf::Vector2f> centers = {{1050, 750}};
    std::vector<double> ones = {500};
    bool flag = false;
    sf::Clock ck;
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
                float mX = sf::Mouse::getPosition(window).x;
                float mY = sf::Mouse::getPosition(window).y;
                if (beg == sf::Vector2f{-100, -100} || !flag) {
                    beg = sf::Vector2f{mX, mY};
                }
                float nX, nY;
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
                now = beg + sf::Vector2f{std::max(nX, nY), std::max(nX, nY)};
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)
                && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (beg != sf::Vector2f{-100, -100}) {
                    double w = 1500 / ((now - beg) * 2).x;
                    center -= (beg - (now - beg));
                    center.x *= w;
                    center.y *= w;
                    one *= w;
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
            int partHeight = 1500 / numThreads;
            for (int i = 0; i < numThreads; ++i) {
                int startY = i * partHeight;
                int heightToProcess = (i == numThreads - 1) ? (1500 - startY) : partHeight;
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
        if (beg != sf::Vector2f{-100.f, -100.f}) {
            sf::RectangleShape a;
            if (now == sf::Vector2f{-100.f, -100.f}) {
                now = {(float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y};
            }
            a.setSize((now - beg) * 2);
            a.setPosition(beg - (now - beg));
            a.setFillColor(sf::Color(3, 144, 252, 100));
            a.setOutlineColor(sf::Color(3, 78, 252, 200));
            a.setOutlineThickness(3);
            window.draw(a);
        }
        // window.draw(c);
        window.display();
    }
}