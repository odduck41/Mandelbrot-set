#include "App.h"

Mandelbrot::Mandelbrot(const sf::Image& img, const Vector2d& center, const double& scale)
: image_(img), center_(center), scale_(scale) {}

void Mandelbrot::draw(sf::RenderWindow& window) {
    if (!generated_) {
        generate_();
    }

    sf::Texture texture;
    texture.loadFromImage(image_);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition({0, 0});

    window.draw(sprite);
}

void Mandelbrot::generate_() {
    generated_ = true;
    std::vector<std::thread> threads;

    const auto part_ = image_.getSize().y / threadAmount;
    for (unsigned i = 0; i < threadAmount; ++i) {
        const auto y = i * part_;
        unsigned heightToProcess;
        if (i + 1 == threadAmount) {
            heightToProcess = image_.getSize().y - y;
        } else {
            heightToProcess = part_;
        }
        threads.emplace_back(part_, y, heightToProcess);
    }

    for (auto& thread: threads) {
        thread.join();
    }
}

void Mandelbrot::part_(const unsigned& from, const unsigned& h) {
    for (long long y = from; y < from + h; ++y) {
        for (long long x = 0; x < image_.getSize().x; ++x) {
            Complex C(
                (static_cast<double>(x) - center_.x) / scale_,
                (static_cast<double>(y) - center_.y) / scale_
            );

            const auto color = 256 - steps_(C);
            image_.setPixel(x, y, sf::Color(
                (4 * color) % 256,
                (6 * color) % 256,
                (8 * color) % 256)
            );
        }
    }
}

long long Mandelbrot::steps_(const Complex &C) {
    Complex nw(0, 0);
    long long steps = 0;
    for (; steps < 256 && abs(nw) <= 4; ++steps) {
        nw *= nw;
        nw += C;
    }
    return steps;
}

App::~App() {
    loop_();
}

void App::connect(const sf::Event::EventType& ev, const std::function<void()>& func) {
    onEvent[ev] = func;
}

void App::loop_() {
    while (this->isOpen()) {
        sf::Event event{};
        while (this->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->close();
                return;
            }

            if (onEvent.contains(event.type)) {
                onEvent[event.type]();
            }
        }
        states_.back().draw(*this);
    }
}

App::App() : sf::RenderWindow({width_, height_}, "Mandelbrot") {

}
