#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

// Tìm UCLN
int gcd(int a, int b) {
    return b == 0 ? abs(a) : gcd(b, a % b);
}

// Lớp phân số
struct Fraction {
    int num, den;

    // Constructor
    Fraction(int n = 0, int d = 1);

    // Rút gọn
    void simplify();
};

// Định nghĩa hàm 
Fraction::Fraction(int n, int d) {   
    if(d == 0) throw runtime_error("Mau so = 0");
    num = n;
    den = d;
    simplify();
}

void Fraction::simplify() {
    int g = gcd(abs(num), abs(den));
    num /= g;
    den /= g;

    // Chuẩn hóa mẫu
    if (den < 0) {
        num *= -1;
        den *= -1;
    }

    // Tử bằng 0 thì mẫu bằng 1
    if (num == 0) {
        den = 1;
    }
}

// Toán tử cộng cho phân số
Fraction operator+(const Fraction& a, const Fraction& b) {
    return Fraction(
        a.num * b.den + b.num * a.den,
        a.den * b.den
    );
}

// Toán tử trừ cho phân số
Fraction operator-(const Fraction& a, const Fraction& b) {
    return Fraction(
        a.num * b.den - b.num * a.den,
        a.den * b.den
    );
}

// Toán tử nhân cho phân số
Fraction operator*(const Fraction& a, const Fraction& b) {
    return Fraction(
        a.num * b.num,
        a.den * b.den
    );
}

// Toán tử chia cho phân số
Fraction operator/(const Fraction& a, const Fraction& b) {
    if (b.num == 0)
        throw runtime_error("Không chia được cho 0");

    return Fraction(
        a.num * b.den,
        a.den * b.num
    );
}

// So sánh 2 phân số
bool operator==(const Fraction& a, const Fraction& b) {
    return a.num == b.num && a.den == b.den;
}

ostream& operator<<(ostream& out, const Fraction& f) {
    out << f.num << "/" << f.den;
    return out;
}
