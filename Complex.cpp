#include "Complex.h"
#include <utility>

void swap(Complex& a, Complex& b) noexcept {
    std::swap(a.a_, b.a_);
    std::swap(a.b_, b.b_);
}

long double abs(const Complex& a) {
    return a.getA() * a.getA() + a.getB() * a.getB();
}


ComplexPart ComplexPart::operator-() const {
    return ComplexPart{new long double(-(*(this->b_)))};
}

ComplexPart::~ComplexPart() {
    delete b_;
}

ComplexPart operator ""_i(const unsigned long long b_) {
    return ComplexPart{new long double(static_cast<long double>(b_))};
}


Complex::Complex(const long double& a, const long double& b)
: a_(new long double(a)), b_(new long double(b)) {}

Complex::Complex(const long double& a): a_(new long double(a)), b_(new long double(0)) {}

Complex::Complex(const ComplexPart& b): a_(new long double(0)), b_(new long double(*b.b_)) {}

Complex::Complex(Complex&& other) noexcept
: a_(std::exchange(other.a_, nullptr)),
b_(std::exchange(other.b_, nullptr)) {}

Complex::Complex(const Complex& other)
: a_(new long double(*other.a_)), b_(new long double(*other.b_)) {}

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

    *this->a_ = (ta * oa - tb * ob);
    *this->b_ = (tb * oa + ta * ob);

    return *this;
}


Complex Complex::operator-() const {
    const Complex reverse(-*this->a_, -*this->b_);
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

bool Complex::operator>(const Complex& other) const {
    return abs(*this) > abs(other);
}

bool Complex::operator<(const Complex& other) const {
    return abs(*this) < abs(other);
}

bool Complex::operator>=(const Complex& other) const {
    return *this > other || *this == other;
}

bool Complex::operator<=(const Complex& other) const {
    return *this < other || *this == other;
}


long double Complex::getA() const {
    return *this->a_;
}

long double Complex::getB() const {
    return *this->b_;
}

Complex::~Complex() {
    delete a_;
    delete b_;
}
