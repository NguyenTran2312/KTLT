#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include "Defines.h"
#include <vector>
#include <string>

// Cấu trúc lưu trữ kết quả phân tích đoạn con (Kadane, Crescendo)
struct SegmentResult {
    int start_index;
    int end_index;
    double value;
};

// Cấu trúc mới: Gom toàn bộ thông tin của một cửa sổ trượt tại một thời điểm
struct WindowStats {
    int window_index;
    int start_sample;
    int end_sample;
    double rolling_energy;
    double rolling_mean;
    double rolling_min;  
    double rolling_max;
};

// 1. Thuật toán Kadane gốc cho mảng số nguyên
int kadane(const std::vector<int>& nums);

// 2. Thuật toán LIS (Longest Increasing Subsequence)
int longestIncreasingSubsequence(const std::vector<int>& nums);

// --- Các hàm giải thuật hệ thống nâng cấp ---
SampleVector buildPrefixSum(const SampleVector& samples);
double getSegmentEnergy(const SampleVector& prefix, int l, int r);

// Gộp 2 hàm cũ thành 1 hàm Sliding Window tối ưu duy nhất
std::vector<WindowStats> calculateSlidingWindow(const SampleVector& samples, int k);

// Hàm Kadane phân tích chuyên sâu trên mảng Năng lượng cửa sổ để tìm Best Segment
SegmentResult findBestSegment(const std::vector<WindowStats>& windows);

// Hàm phát hiện chuỗi tăng liên tiếp dài nhất
SegmentResult detectCrescendo(const SampleVector& samples);

#endif