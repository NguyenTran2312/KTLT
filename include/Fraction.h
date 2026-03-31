#ifndef FRACTION_H
#define FRACTION_H

struct Fraction {
    int numerator;     // tử số
    int denominator;   // mẫu số

    // Constructor mặc định
    Fraction();

    // Constructor có tham số
    Fraction(int num, int den);

    // Hàm rút gọn phân số
    void simplify();

    // Hàm in phân số
    void print() const;
};

#endif