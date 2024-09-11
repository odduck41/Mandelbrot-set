#pragma once

class Complex {
  public:
    Complex() = default;
    Complex(const Complex&);
    explicit Complex(const long long&, const long long&);
    Complex(Complex&&) noexcept;

    Complex& operator=(Complex) noexcept;

    Complex& operator+=(const Complex&);
    Complex& operator-=(const Complex&);
    Complex& operator*=(const Complex&);

    Complex operator-() const;

    Complex operator+(Complex) const;
    Complex operator-(Complex) const;
    Complex operator*(Complex) const;

    bool operator==(const Complex&) const;

    [[nodiscard]] long long getA() const;
    [[nodiscard]] long long getB() const;

    friend void swap(Complex&, Complex&) noexcept;
    friend long long abs(const Complex&);
  private:
    long long* a_{};
    long long* b_{};
};

