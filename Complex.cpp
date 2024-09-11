#include "Complex.h"
#include <utility>

void swap(Complex& a, Complex& b) noexcept {
    std::swap(a.a_, b.a_);
    std::swap(a.b_, b.b_);
}

long long abs(const Complex& a) {
    return a.getA() * a.getA() + a.getB() * a.getB();
}

Complex::Complex(const long long& a, const long long& b)
: a_(new long long(a)), b_(new long long(b)) {};
Complex::Complex(Complex&& other) noexcept
: a_(std::exchange(other.a_, nullptr)),
b_(std::exchange(other.b_, nullptr)) {}
Complex::Complex(const Complex& other)
: a_(new long long(*other.a_)), b_(new long long(*other.b_)) {}

Complex& Complex::operator=(Complex other) noexcept {
    swap(*this, other);
    return *this;
}

Complex& Complex::operator+=(const Complex& other) {
    *this->a_ += *other.a_;
    *this->b_ += *other.b_;
    return *this;
}
Complex& Complex::operator-=(const Complex& other) {
    *this->a_ -= *other.a_;
    *this->b_ -= *other.b_;
    return *this;
}
Complex& Complex::operator*=(const Complex& other) {
    const auto ta = *this->a_;
    const auto tb = *this->b_;

    const auto oa = *other.a_;
    const auto ob = *other.b_;

    *this->a_ = (ta * oa + tb * ob);
    *this->b_ = (ta * ob + tb * oa);

    return *this;
}

Complex Complex::operator-() const {
    constexpr Complex reverse(-*this->a_, -*this->b_);
    return reverse;
}

Complex Complex::operator+(Complex other) const {
    other += *this;
    return other;
}
Complex Complex::operator-(Complex other) const {
    return -(other-=*this);
}
Complex Complex::operator*(Complex other) const {
    return other *= *this;
}

bool Complex::operator==(const Complex& other) const {
    return (*this->a_ == *other.a_) && (*this->b_ == *other.b_);
}

long long Complex::getA() const {
    return *this->a_;
}
long long Complex::getB() const {
    return *this->b_;
}
