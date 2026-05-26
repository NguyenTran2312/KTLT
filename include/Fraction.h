#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>

// =======================
// Fraction struct
// =======================
struct Fraction {
    long long num, den;

    // Constructor
    Fraction(long long n = 0, long long d = 1);

    // Simplify fraction
    void simplify();
};

// =======================
// Operator overloads
// =======================

// Arithmetic
Fraction operator+(const Fraction& a, const Fraction& b);
Fraction operator-(const Fraction& a, const Fraction& b);
Fraction operator*(const Fraction& a, const Fraction& b);
Fraction operator/(const Fraction& a, const Fraction& b);

// Comparison
bool operator==(const Fraction& a, const Fraction& b);

// Output
std::ostream& operator<<(std::ostream& out, const Fraction& f);

#endif
