#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <cmath>
#include "Complex.h"

// class Selected : sf::RectangleShape {
//     public:
//
// };

class Window : sf::RenderWindow {
    public:
        Window(): sf::RenderWindow({1000, 1000}, "Set") {}
        void addObj(const std::string& name, sf::RectangleShape* shape) {
            obj[name] = shape;
        }
        void rmObj(const std::string& name) {
            obj.erase(name);
        }
        void connect(const sf::Event::EventType& type, const std::function<void()> &todo) {
            todo_[type] = todo;
        }

        void loop() {
            while (this->isOpen()) {
                sf::Event event{};
                while (this->pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        this->close();
                        return;
                    }
                    for (auto&[ev, func]: todo_) {
                        if (event.type == ev) {
                            func();
                        }
                    }
                }
                this->clear();
                for (auto& [_, shape]: obj) {
                    this->draw(*shape);
                }
                this->display();
            }
        }
        ~Window() override {
            loop();
        }
    private:
        std::map<sf::Event::EventType, std::function<void()>> todo_;
        std::map<std::string, sf::RectangleShape*> obj;
};

long long step(const Complex& C, Complex& nw, long long steps = 0) {
    if (steps == 100) {
        return steps;
    }
    if (abs(nw) > 4) {
        return steps;
    }
    nw *= nw;
    nw += C;
    return step(C, nw, steps + 1);
}

int main() {
    // sf::RectangleShape e({100, 100});
    // Window w;
    // e.setPosition(100, 100);
    // e.setFillColor(sf::Color::Green);
    // w.addObj("fff", &e);
    // e.setFillColor(sf::Color::Cyan);
    // w.loop();
    sf::RenderWindow window({1500, 1500}, "Set");
    sf::Vector2f beg{-100, -100};
    sf::Vector2f now;
    // c.setPosition(500, 500);
    // c.setFillColor(sf::Color::White);
    bool flag = false;
    bool resized = true;
    sf::Image ff;
    sf::RectangleShape nw;
    while (window.isOpen()) {
        sf::RectangleShape a;
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                flag = true;
                float mX = sf::Mouse::getPosition(window).x;
                float mY = sf::Mouse::getPosition(window).y;
                if (beg == sf::Vector2f{-100, -100}) {
                    beg = sf::Vector2f{mX, mY};
                } else {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                        if (mX >= beg.x) {
                            if (mY > beg.y) {
                                mY = mX - beg.x + beg.y;
                            } else {
                                mY = beg.x - mX + beg.y;
                            }
                        } else {
                            if (mY > beg.y) {
                                mY = beg.x - mX + beg.y;
                            } else {
                                mY = mX - beg.x + beg.y;
                            }
                        }
                    }
                    now = sf::Vector2f{mX, mY};
                }
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                beg = {-100, -100};
                if (flag) {
                    resized = true;
                    flag = false;
                }
            }
        }
        window.clear();
        if (resized) {
            ff.create(1500, 1500);
            for (long long x = 0; x < 1500; ++x) {
                for (long long y = 0; y < 1500; ++y) {
                    Complex zero(0, 0);
                    auto color = 100 - step(Complex(((double)x - 1050) / 500, ((double)y - 750) / 500), zero);
                    ff.setPixel(x, y, sf::Color(((color * 256) / 100) % 256, 0, ((color * 256) / 100) % 256));
                }
            }
        }



        resized = false;
        sf::Texture l;
        l.loadFromImage(ff);
        sf::Sprite s;
        s.setTexture(l);
        s.setPosition(0, 0);
        window.draw(s);
        if (beg != sf::Vector2f{-100.f, -100.f}) {
            a.setSize(now - beg);
            a.setPosition(beg);
            a.setFillColor(sf::Color(3, 144, 252, 100));
            a.setOutlineColor(sf::Color(3, 78, 252, 200));
            a.setOutlineThickness(3);
            window.draw(a);
        }
        // window.draw(c);
        window.display();
    }
}