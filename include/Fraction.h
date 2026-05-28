#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>

// =======================
// Fraction struct
// =======================
struct Fraction {
    int num, den;

    // Constructor
    Fraction(int n = 0, int d = 1);

    // Simplify fraction
    void simplify();
};

// =======================
// Operator overloads
// =======================

static Fraction makeFraction(long long n, long long d);

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
