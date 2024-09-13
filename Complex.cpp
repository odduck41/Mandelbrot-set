#include "Complex.h"
#include <utility>

inline void swap(Complex& a, Complex& b) noexcept {
    std::swap(a.a_, b.a_);
    std::swap(a.b_, b.b_);
}

inline long double abs(const Complex& a) {
    return a.getA() * a.getA() + a.getB() * a.getB();
}


inline ComplexPart ComplexPart::operator-() const {
    return ComplexPart{-this->b_};
}

inline ComplexPart operator ""_i(const unsigned long long b_) {
    return ComplexPart{static_cast<long double>(b_)};
}


inline Complex::Complex(const long double& a, const long double& b)
: a_(a), b_(b) {}

inline Complex::Complex(const long double& a): a_(a), b_(0) {}

inline Complex::Complex(const ComplexPart& b): a_(0), b_(b.b_) {}

inline Complex::Complex(Complex&& other) noexcept
: a_(std::exchange(other.a_, 0)),
b_(std::exchange(other.b_, 0)) {}

inline Complex::Complex(const Complex& other) = default;

inline Complex& Complex::operator=(Complex other) noexcept {
    swap(*this, other);
    return *this;
}


inline Complex& Complex::operator+=(const Complex& other) {
    this->a_ += other.a_;
    this->b_ += other.b_;
    return *this;
}

inline Complex& Complex::operator-=(const Complex& other) {
    this->a_ -= other.a_;
    this->b_ -= other.b_;
    return *this;
}

inline Complex& Complex::operator*=(const Complex& other) {
    const auto ta = this->a_;
    const auto tb = this->b_;

    const auto oa = other.a_;
    const auto ob = other.b_;

    this->a_ = (ta * oa - tb * ob);
    this->b_ = (tb * oa + ta * ob);

    return *this;
}


inline Complex Complex::operator-() const {
    const Complex reverse(-this->a_, -this->b_);
    return reverse;
}

inline Complex Complex::operator+(Complex other) const {
    other += *this;
    return other;
}

inline Complex Complex::operator-(Complex other) const {
    return -(other-=*this);
}

inline Complex Complex::operator*(Complex other) const {
    return other *= *this;
}


inline bool Complex::operator==(const Complex& other) const {
    return (this->a_ == other.a_) && (this->b_ == other.b_);
}

inline bool Complex::operator>(const Complex& other) const {
    return abs(*this) > abs(other);
}

inline bool Complex::operator<(const Complex& other) const {
    return abs(*this) < abs(other);
}

inline bool Complex::operator>=(const Complex& other) const {
    return *this > other || *this == other;
}

inline bool Complex::operator<=(const Complex& other) const {
    return *this < other || *this == other;
}


inline long double Complex::getA() const {
    return this->a_;
}

inline long double Complex::getB() const {
    return this->b_;
}
