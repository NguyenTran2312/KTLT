#include "fraction.h"
#include <iostream>
using namespace std;

// Constructor mặc định
Fraction::Fraction() {
    numerator = 0;
    denominator = 1;
}

// Constructor có tham số
Fraction::Fraction(int num, int den) {
    numerator = num;
    denominator = den;
}

// Hàm tìm ước chung lớn nhất (gcd)
int gcd(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// Rút gọn phân số
void Fraction::simplify() {
    int g = gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;
}

// In phân số
void Fraction::print() const {
    cout << numerator << "/" << denominator << endl;
}