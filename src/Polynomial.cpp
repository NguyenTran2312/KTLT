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

// Tính giá trị đa thức tại x
// Tính giá trị đa thức tại x (tất cả đều ở dạng Fraction)
Fraction evaluate(const Polynomial& p, const Fraction& x) {
    if (p.coeffs.empty()) {
        return Fraction(0, 1);
    }

    // Bắt đầu từ hệ số bậc cao nhất (an)
    Fraction result = p.coeffs[p.coeffs.size() - 1];

    // Duyệt lặp ngược từ bậc (n-1) về bậc 0 theo thuật toán Horner
    for (int i = (int)p.coeffs.size() - 2; i >= 0; i--) {
        result = result * x + p.coeffs[i];
    }

    return result;
}

// Đạo hàm
Polynomial derivative(const Polynomial& p) {
    Polynomial res;

    // Nếu là hằng số → đạo hàm = 0
    if (p.coeffs.size() <= 1) {
        res.coeffs.push_back(Fraction(0,1));
        return res;
    }

    // Áp dụng công thức: c * n
    for (int i = 1; i < p.coeffs.size(); i++) {
        Fraction c = p.coeffs[i];
        res.coeffs.push_back(Fraction(c.num * i, c.den));
    }

    return res;
}

// Ghi đa thức
void printPolynomial(const Polynomial& p) {
    
    // Duyệt từng hệ số 
    for (int i = 0; i < p.coeffs.size(); i++) {
        cout << p.coeffs[i] << "*x^" << i;
        
        // Tránh "+" ở cuối 
        if (i != p.coeffs.size() - 1) cout << " + ";
    }
    cout << endl;
}

Polynomial polyFit(const vector<double>& x, const vector<double>& y, int degree) {
    Polynomial p;
    int n = x.size();
    if (n == 0 || degree < 0) {
        p.coeffs.push_back(Fraction(0, 1));
        return p;
    }

    int N = degree + 1;
    vector<vector<double>> M(N, vector<double>(N, 0.0));
    vector<double> B(N, 0.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < n; ++k) {
                M[i][j] += pow(x[k], i + j);
            }
        }
        for (int k = 0; k < n; ++k) {
            B[i] += y[k] * pow(x[k], i);
        }
    }

    vector<double> a(N, 0.0);
    for (int i = 0; i < N; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < N; ++k) {
            if (abs(M[k][i]) > abs(M[maxRow][i])) maxRow = k;
        }
        swap(M[i], M[maxRow]);
        swap(B[i], B[maxRow]);

        if (abs(M[i][i]) < EPSILON) continue;

        for (int k = i + 1; k < N; ++k) {
            double t = M[k][i] / M[i][i];
            for (int j = 0; j < N; ++j) M[k][j] -= t * M[i][j];
            B[k] -= t * B[i];
        }
    }

    for (int i = N - 1; i >= 0; --i) {
        if (abs(M[i][i]) < EPSILON) {
            a[i] = 0;
        } else {
            a[i] = B[i];
            for (int j = i + 1; j < N; ++j) {
                a[i] -= M[i][j] * a[j];
            }
            a[i] /= M[i][i];
        }
    }

    for (int i = 0; i < N; ++i) {
        p.coeffs.push_back(Fraction((long long)round(a[i] * 1000000.0), 1000000));
    }

    return p;
}