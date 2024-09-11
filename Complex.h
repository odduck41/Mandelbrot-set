#pragma once

struct ComplexPart {
    long long* b_{};
    ComplexPart operator-() const;
    ~ComplexPart();
};

ComplexPart operator""_i(unsigned long long);

class Complex {
  public:
    Complex() = default;
    Complex(const Complex&);
    explicit Complex(const long long&, const long long&);
    explicit Complex(const long long&);
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
    [[nodiscard]] long long getA() const;
    [[nodiscard]] long long getB() const;

    friend void swap(Complex&, Complex&) noexcept;
    friend long long abs(const Complex&);
    ~Complex();
  private:
    long long* a_{};
    long long* b_{};
};

