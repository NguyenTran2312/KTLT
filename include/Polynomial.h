#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include "Fraction.h"

// =======================
// Polynomial struct
// coeffs[i] = coefficient of x^i
// =======================
struct Polynomial {
    std::vector<Fraction> coeffs;
};

// =======================
// Function declarations
// =======================

// Evaluate polynomial at x (Horner's method)
Fraction evaluate(const Polynomial& p, const Fraction& x);

// Compute derivative of polynomial
Polynomial derivative(const Polynomial& p);

// Print polynomial
void printPolynomial(const Polynomial& p);

Polynomial polyFit(const std::vector<double>& x, const std::vector<double>& y, int degree);


#endif
