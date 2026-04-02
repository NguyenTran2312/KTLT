#include <iostream>
#include <vector>
#include <numeric>   // Cho std::accumulate
#include <cmath>     // Cho std::pow, std::sqrt
#include <algorithm> // Cho std::min_element, std::max_element
#include <stdexcept> // Cho std::invalid_argument
#include "Statistics.h"


// 1. Tính giá trị trung bình (Mean)
double calculateMean(const SampleVector& arr) {
    if (arr.empty()) 
    throw std::invalid_argument("Mảng không được rỗng!");
    
    // std::accumulate tính tổng các phần tử trong mảng
    double sum = std::accumulate(arr.begin(), arr.end(), 0.0);
    return sum / arr.size();
}

// 2. Tính độ lệch chuẩn (Standard Deviation)
double calculateStd(const SampleVector& arr, double mean) {
    if (arr.size() <= 1) return 0.0; // Tránh chia cho 0
    
    double sq_sum = 0.0;
    for (double val : arr) {
        sq_sum += std::pow(val - mean, 2);
    }
    
    // Sử dụng hiệu chỉnh Bessel (N-1) cho mẫu (Sample Standard Deviation)
    return std::sqrt(sq_sum / (arr.size() - 1)); 
}

// 3. Tìm giá trị nhỏ nhất (Min)
double calculateMin(const SampleVector& arr) {
    if (arr.empty()) throw std::invalid_argument("Mảng không được rỗng!");
    return *std::min_element(arr.begin(), arr.end());
}

// 4. Tìm giá trị lớn nhất (Max)
double calculateMax(const SampleVector& arr) {
    if (arr.empty()) throw std::invalid_argument("Mảng không được rỗng!");
    return *std::max_element(arr.begin(), arr.end());
}

// 5. Chuẩn hóa Z-Score
SampleVector standardizeZScore(const SampleVector& arr) {
    if (arr.empty()) return {};
    
    double mean = calculateMean(arr);
    double stdDev = calculateStd(arr, mean);
    
    SampleVector z_scores;
    z_scores.reserve(arr.size()); // Tối ưu hóa cấp phát bộ nhớ
    
    if (stdDev == 0.0) {
        // Trả về một vector có kích thước arr.size(), tất cả phần tử là 0.0
        return SampleVector(arr.size(), 0.0); 
    }

    // Công thức Z-score: z = (x - mean) / stdDev
    for (double val : arr) {
        z_scores.push_back((val - mean) / stdDev);
    }
    
    return z_scores;
}


