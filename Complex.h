#pragma once

struct ComplexPart {
    double* b_{};
    ComplexPart operator-() const;
    ~ComplexPart();
};

ComplexPart operator""_i(unsigned long long);

class Complex {
  public:
    Complex() = default;
    Complex(const Complex&);
    explicit Complex(const double&, const double&);
    explicit Complex(const double&);
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
    [[nodiscard]] double getA() const;
    [[nodiscard]] double getB() const;

    friend void swap(Complex&, Complex&) noexcept;
    friend double abs(const Complex&);
    ~Complex();
  private:
    double* a_{};
    double* b_{};
};

