#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>

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
double evaluate(const Polynomial& p, double x) {
    double result = 0;

    // Duyệt từ bậc cao xuống
    for (int i = p.coeffs.size() - 1; i >= 0; i--) {
        
        // Chuyển phân số thành số thực
        double c = (double)p.coeffs[i].num / p.coeffs[i].den;
        result = result * x + c;
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
