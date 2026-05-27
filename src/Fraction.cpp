#include <iostream>
#include <stdexcept>
#include <cmath>
#include <climits>
#include <algorithm>
using namespace std;

// Tìm UCLN bằng thuật toán Euclid đệ quy
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
    if (d == 0) throw runtime_error("Mau so = 0");
    num = n;
    den = d;
    simplify();
}

void Fraction::simplify() {
    int g = gcd(abs(num), abs(den));
    num /= g;
    den /= g;
    // Chuẩn hóa: mẫu luôn dương
    if (den < 0) {
        num *= -1;
        den *= -1;
    }
    // Tử bằng 0 thì mẫu bằng 1
    if (num == 0) {
        den = 1;
    }
}

// Tạo Fraction từ long long, ép về int sau khi rút gọn
// Tránh tràn số khi tích num*den vượt phạm vi int 32-bit
static Fraction makeFraction(long long n, long long d) {
    if (d == 0) throw runtime_error("Mau so = 0");
    // Rút gọn trước khi ép về int để giảm nguy cơ overflow
    long long g = __gcd(abs(n), abs(d));
    n /= g;
    d /= g;
    if (d < 0) { n = -n; d = -d; }
    return Fraction((int)n, (int)d);
}

// Toán tử cộng: a/b + c/d = (a*d + c*b) / (b*d)
// Dùng long long trung gian tránh tràn int 32-bit
Fraction operator+(const Fraction& a, const Fraction& b) {
    long long n = (long long)a.num * b.den + (long long)b.num * a.den;
    long long d = (long long)a.den * b.den;
    return makeFraction(n, d);
}

// Toán tử trừ: a/b - c/d = (a*d - c*b) / (b*d)
Fraction operator-(const Fraction& a, const Fraction& b) {
    long long n = (long long)a.num * b.den - (long long)b.num * a.den;
    long long d = (long long)a.den * b.den;
    return makeFraction(n, d);
}

// Toán tử nhân: a/b * c/d = (a*c) / (b*d)
Fraction operator*(const Fraction& a, const Fraction& b) {
    long long n = (long long)a.num * b.num;
    long long d = (long long)a.den * b.den;
    return makeFraction(n, d);
}

// Toán tử chia: a/b ÷ c/d = (a*d) / (b*c)
Fraction operator/(const Fraction& a, const Fraction& b) {
    if (b.num == 0) throw runtime_error("Khong chia duoc cho 0");
    long long n = (long long)a.num * b.den;
    long long d = (long long)a.den * b.num;
    return makeFraction(n, d);
}

// So sánh 2 phân số (đã simplify nên so sánh trực tiếp num và den)
bool operator==(const Fraction& a, const Fraction& b) {
    return a.num == b.num && a.den == b.den;
}

// Ghi phân số dạng num/den
ostream& operator<<(ostream& out, const Fraction& f) {
    out << f.num << "/" << f.den;
    return out;
}
