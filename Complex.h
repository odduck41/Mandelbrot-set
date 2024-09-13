#pragma once

struct ComplexPart {
    long double b_{};
    ComplexPart operator-() const;
    ~ComplexPart() = default;
};

ComplexPart operator""_i(unsigned long long);

class Complex {
  public:
    Complex() = default;
    Complex(const Complex&);
    explicit Complex(const long double&, const long double&);
    explicit Complex(const long double&);
    explicit Complex(const ComplexPart&);
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
    bool operator> (const Complex&) const;
    bool operator< (const Complex&) const;
    bool operator>=(const Complex &) const;
    bool operator<=(const Complex &) const;
    [[nodiscard]] long double getA() const;
    [[nodiscard]] long double getB() const;

    friend void swap(Complex&, Complex&) noexcept;
    friend long double abs(const Complex&);
    ~Complex() = default;
  private:
    long double a_{};
    long double b_{};
};

