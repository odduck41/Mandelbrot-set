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
        std::thread t(&Mandelbrot::part_, this, y, heightToProcess);
        threads.push_back(std::move(t));
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

void App::loop_() {
    bool realised = true;
    while (this->isOpen()) {
        sf::Event event{};
        while (this->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->close();
                return;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (realised) {
                    delete selector_;
                    selector_ = nullptr;
                    realised = false;
                }
                if (selector_ == nullptr) {
                    selector_ = new Selector(Vector2d(sf::Mouse::getPosition(*this)));
                } else {
                    selector_->update(Vector2d(sf::Mouse::getPosition(*this)));
                }
            } else if (selector_ != nullptr
            && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)
            && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                center_ -= Vector2d(selector_->getPosition());
                (center_ *= width_) /= selector_->getSide();

                (scale_ *= width_) /= selector_->getSide();

                this->nextStep();

                realised = true;
                delete selector_;
                selector_ = nullptr;
            } else if (event.type == sf::Event::KeyPressed &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
            && sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                this->prevStep();
            } else {
                realised = true;
            }
        }
        this->clear();
        states_.back().draw(*this);
        if (selector_ != nullptr) {
            selector_->draw(*this);
        }
        this->display();
    }
}

App::App() : sf::RenderWindow({width_, height_}, "Mandelbrot") {
    nextStep();
}

void App::nextStep() {
    sf::Image image;
    image.create(width_, height_);
    states_.emplace_back(image, center_, scale_);
}

void App::prevStep() {
    if (states_.size() == 1) return;
    states_.pop_back();
    center_ = states_.back().getCenter();
    scale_ = states_.back().getScale();
}

Selector::Selector(const Vector2d& beg): beg_(beg), size_(beg) {
    this->setFillColor(sf::Color(3, 144, 252, 100));
    this->setOutlineColor(sf::Color(3, 78, 252, 200));
    this->setOutlineThickness(3);

}

void Selector::update(const Vector2d& now) {
    if (const double mX = now.x; mX - beg_.x < 0) {
        size_.x = beg_.x - mX;
    } else {
        size_.x = mX - beg_.x;
    }

    if (const double mY = now.y; mY - beg_.y < 0) {
        size_.y = beg_.y - mY;
    } else {
        size_.y = mY - beg_.y;
    }

    size_ = beg_ + Vector2d{std::max(size_.x, size_.y), std::max(size_.x, size_.y)};

    this->setPosition(sf::Vector2f(beg_ - (size_ - beg_)));
    this->setSize(sf::Vector2f((size_ - beg_) * 2));
}

void Selector::draw(sf::RenderWindow& window) const {
    window.draw(*this);
}
