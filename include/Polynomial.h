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
double evaluate(const Polynomial& p, double x);

// Compute derivative of polynomial
Polynomial derivative(const Polynomial& p);

// Print polynomial
void printPolynomial(const Polynomial& p);

#endif