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

sf::Vector2f operator* (sf::Vector2f a, const long long& b) {
    a.x *= b;
    a.y *= b;
    return a;
}

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
    bool resized = true;
    sf::Image ff;
    std::vector<sf::Image> poses;
    std::vector<sf::Vector2f> centers = {{1050, 750}};
    std::vector<double> ones = {500};
    bool flag = false;
    while (window.isOpen()) {
        auto one = ones.back();
        auto center = centers.back();
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
                }
                now = sf::Vector2f{std::max(mX, mY), std::max(mX, mY)};
            } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (beg != sf::Vector2f{-100, -100}) {
                    double w = 1500 / ((now - beg) * 2).x;
                    center -= (beg - (now - beg));
                    center.x *= w;
                    center.y *= w;
                    one *= w;
                    beg = {-100, -100};
                    if (flag) {
                        resized = true;
                        flag = false;
                    }
                }
            } else if(event.type == sf::Event::KeyPressed &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                if (poses.size() > 1) {
                    centers.pop_back();
                    ones.pop_back();
                    poses.pop_back();
                }
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                center.y += 100;
                resized = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                center.y -= 100;
                resized = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                center.x += 100;
                resized = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                center.x -= 100;
                resized = true;
            }
        }
        window.clear();
        if (resized) {
            ff.create(1500, 1500);
            for (long long x = 0; x < 1500; ++x) {
                for (long long y = 0; y < 1500; ++y) {
                    Complex zero(0, 0);
                    auto color = 100 - step(Complex(((double)x - center.x) / one, ((double)y - center.y) / one), zero);
                    ff.setPixel(x, y, sf::Color(((color * 130) / 100) % 256, ((color * 130) / 100) % 256, ((color * 324) / 100) % 256));
                }
            }
            poses.push_back(ff);
            centers.push_back(center);
            ones.push_back(one);
            resized = false;
        }

        sf::Texture l;
        sf::Sprite s;
        l.loadFromImage(poses.back());
        s.setTexture(l);
        s.setPosition(0, 0);
        window.draw(s);
        if (beg != sf::Vector2f{-100.f, -100.f}) {
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