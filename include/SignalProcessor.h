#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H
#include "Defines.h"
#include <vector>

struct SegmentResult {
    int start_index;
    int end_index;
    double value;
};

// 1. Thuật toán Kadane
// Bài toán: Tìm mảng con liên tiếp có tổng lớn nhất
// Độ phức tạp: O(N) thời gian, O(1) không gian
int kadane(const std::vector<int>& nums);

// 2. Thuật toán LIS (Longest Increasing Subsequence)
// Bài toán: Tìm độ dài của dãy con tăng ngặt dài nhất (không cần liên tiếp)
// Độ phức tạp: O(N log N) thời gian, O(N) không gian

int longestIncreasingSubsequence(const std::vector<int>& nums);

// Các hàm thuật toán lõi (Tham số const & để chống copy mảng tốn RAM)
SampleVector buildPrefixSum(const SampleVector& samples);
double getSegmentEnergy(const SampleVector& prefix, int l, int r);
SampleVector calculateRollingEnergy(const SampleVector& samples, int k);
SampleVector calculateRollingMean(const SampleVector& samples, int k);
SegmentResult findBestSegment(const SampleVector& energy_array);
SegmentResult detectCrescendo(const SampleVector& samples);

#endif
