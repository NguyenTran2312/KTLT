#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <iostream>
#include "Fraction.h"
#include "Defines.h"

using namespace std;

// ==============================================================
// Struct Polynomial
// Lưu đa thức dưới dạng vector hệ số phân số
// coeffs[i] = hệ số của x^i  (i = 0, 1, 2, ..., degree)
// Ví dụ: P(x) = 1/2 + 3/4*x + 2/1*x^2
//        coeffs = { Fraction(1,2), Fraction(3,4), Fraction(2,1) }
// ==============================================================
struct Polynomial {
    vector<Fraction> coeffs;
};

// ==============================================================
// Tính giá trị đa thức P(x) tại x bằng thuật toán Horner
// - Trả về double (không phải Fraction) để tránh bùng nổ mẫu số
// - Độ phức tạp: O(N) thời gian, O(1) không gian
// ==============================================================
double evaluate(const Polynomial& p, double x);

// ==============================================================
// Tính đạo hàm P'(x) theo quy tắc: (c*x^i)' = c*i * x^(i-1)
// - Trả về Polynomial mới bậc = degree - 1
// - Hằng số (bậc 0) → trả về Polynomial { Fraction(0,1) }
// - Độ phức tạp: O(N) thời gian, O(N) không gian
// ==============================================================
Polynomial derivative(const Polynomial& p);

// ==============================================================
// In đa thức ra console từ bậc cao xuống bậc thấp
// Ví dụ: 2/1*x^2 + 3/4*x^1 + 1/2
// ==============================================================
void printPolynomial(const Polynomial& p);

// ==============================================================
// Hồi quy đa thức bậc 'degree' từ tập điểm {(x[k], y[k])}
// bằng phương pháp Bình phương tối thiểu (Least Squares)
// + Khử Gauss có chọn trục (Partial Pivoting)
// - Hệ số trả về dạng Fraction(round(a*1000000), 1000000)
// - Độ phức tạp: O(N²*n + N³) thời gian, O(N²) không gian
//   (N = degree+1, n = số điểm dữ liệu)
// ==============================================================
Polynomial polyFit(const vector<double>& x,
                   const vector<double>& y,
                   int degree);

#endif
