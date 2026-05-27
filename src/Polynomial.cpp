#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "Fraction.h"
#include "Defines.h"

using namespace std;

struct Polynomial {
    vector<Fraction> coeffs; // coeffs[i] = hệ số của x^i
};

/*
    Phương pháp Horner:
    Biến đổi đa thức từ dạng:
        P(x) = a0 + a1*x + a2*x^2 + ... + an*x^n
    Thành dạng lồng nhau:
        P(x) = (...((an*x + a(n-1))*x + ... + a1)*x + a0)
    Giảm phức tạp từ O(n^2) xuống O(n)
*/

double evaluate(const Polynomial& p, double x) {
    if (p.coeffs.empty()) return 0.0;

    // Bắt đầu từ hệ số bậc cao nhất (an), chuyển sang double
    double result = 0.0;
    for (int i = (int)p.coeffs.size() - 1; i >= 0; i--) {
        // Chuyển Fraction → double để tránh tràn mẫu số
        double c = (double)p.coeffs[i].num / p.coeffs[i].den;
        result = result * x + c;
    }
    return result;
}

// Tính đạo hàm P'(x) theo quy tắc hạ bậc: (c * x^i)' = c*i * x^(i-1)
// Trả về Polynomial mới có bậc giảm 1
Polynomial derivative(const Polynomial& p) {
    Polynomial res;

    // Nếu là hằng số (bậc 0) → đạo hàm = 0
    if (p.coeffs.size() <= 1) {
        res.coeffs.push_back(Fraction(0, 1));
        return res;
    }

    // Áp dụng công thức: hệ số x^i → hệ số mới = c * i, bậc giảm xuống i-1
    for (int i = 1; i < (int)p.coeffs.size(); i++) {
        Fraction c = p.coeffs[i];
        // Dùng long long tránh tràn khi c.num * i vượt int 32-bit
        long long newNum = (long long)c.num * i;
        res.coeffs.push_back(Fraction((int)newNum, c.den));
    }
    return res;
}

// Ghi đa thức
void printPolynomial(const Polynomial& p) {
    if (p.coeffs.empty()) {
        cout << "0" << endl;
        return;
    }

    bool first = true;
    // In từ bậc cao nhất xuống bậc 0
    for (int i = (int)p.coeffs.size() - 1; i >= 0; i--) {
        if (!first) cout << " + ";
        cout << p.coeffs[i];
        if (i > 0) cout << "*x^" << i;
        first = false;
    }
    cout << endl;
}

/*
    Hồi quy đa thức bậc 'degree' theo phương pháp Bình phương tối thiểu (Least Squares)
    Xây dựng hệ phương trình pháp tuyến: M * a = B
        M[i][j] = sum(x[k]^(i+j))
        B[i]    = sum(y[k] * x[k]^i)
    Giải hệ bằng khử Gauss có chọn trục (Partial Pivoting) để ổn định số học
    Hệ số thực được làm tròn thành Fraction với mẫu 1000000 (6 chữ số thập phân)
*/
Polynomial polyFit(const vector<double>& x, const vector<double>& y, int degree) {
    Polynomial p;
    int n = (int)x.size();

    // Case biên: không có dữ liệu hoặc bậc âm
    if (n == 0 || degree < 0) {
        p.coeffs.push_back(Fraction(0, 1));
        return p;
    }

    int N = degree + 1;
    vector<vector<double>> M(N, vector<double>(N, 0.0));
    vector<double> B(N, 0.0);

    // Xây dựng ma trận pháp tuyến
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < n; ++k)
                M[i][j] += pow(x[k], i + j);
        for (int k = 0; k < n; ++k)
            B[i] += y[k] * pow(x[k], i);
    }

    // Khử Gauss có chọn trục (Partial Pivoting)
    for (int i = 0; i < N; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < N; ++k)
            if (abs(M[k][i]) > abs(M[maxRow][i])) maxRow = k;
        swap(M[i], M[maxRow]);
        swap(B[i], B[maxRow]);
        if (abs(M[i][i]) < EPSILON) continue;
        for (int k = i + 1; k < N; ++k) {
            double t = M[k][i] / M[i][i];
            for (int j = 0; j < N; ++j) M[k][j] -= t * M[i][j];
            B[k] -= t * B[i];
        }
    }

    // Thế ngược để tìm hệ số
    vector<double> a(N, 0.0);
    for (int i = N - 1; i >= 0; --i) {
        if (abs(M[i][i]) < EPSILON) {
            a[i] = 0;
        } else {
            a[i] = B[i];
            for (int j = i + 1; j < N; ++j) a[i] -= M[i][j] * a[j];
            a[i] /= M[i][i];
        }
    }

    // Chuyển hệ số double → Fraction(num, 1000000)
    // Dùng long long trung gian để tránh tràn khi round, sau đó ép về int an toàn
    const int DENOM = 1000000;
    for (int i = 0; i < N; ++i) {
        long long num = llround(a[i] * DENOM);
        // Clamp về phạm vi int nếu hệ số quá lớn (fallback về 3 chữ số thập phân)
        if (num > INT_MAX || num < INT_MIN) {
            num = llround(a[i] * 1000.0);
            p.coeffs.push_back(Fraction((int)num, 1000));
        } else {
            p.coeffs.push_back(Fraction((int)num, DENOM));
        }
    }
    return p;
}
