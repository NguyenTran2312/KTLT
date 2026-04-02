#include <iostream>
#include <vector>
#include <algorithm>
#include "SignalProcessor.h"

using namespace std;

// ==========================================
// 1. Thuật toán Kadane
// Bài toán: Tìm mảng con liên tiếp có tổng lớn nhất
// Độ phức tạp: O(N) thời gian, O(1) không gian
// ==========================================
int kadane(const vector<int>& nums) {
    if (nums.empty()) return 0;

    int current_max = nums[0];
    int global_max = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        // Chọn giữa việc bắt đầu một mảng con mới tại i, hoặc tiếp tục cộng dồn vào mảng con cũ
        current_max = max(nums[i], current_max + nums[i]);
        // Cập nhật tổng lớn nhất toàn cục
        global_max = max(global_max, current_max);
    }

    return global_max;
}

// ==========================================
// 2. Thuật toán LIS (Longest Increasing Subsequence)
// Bài toán: Tìm độ dài của dãy con tăng ngặt dài nhất (không cần liên tiếp)
// Độ phức tạp: O(N log N) thời gian, O(N) không gian
// ==========================================
int longestIncreasingSubsequence(const vector<int>& nums) {
    if (nums.empty()) return 0;

    vector<int> tails; // Mảng lưu phần tử cuối cùng nhỏ nhất của các dãy con tăng có độ dài khác nhau

    for (int x : nums) {
        // Dùng Binary Search để tìm vị trí đầu tiên trong 'tails' có giá trị >= x
        auto it = lower_bound(tails.begin(), tails.end(), x);

        // Nếu x lớn hơn tất cả các phần tử trong tails, thêm x vào cuối (mở rộng độ dài dãy LIS)
        if (it == tails.end()) {
            tails.push_back(x);
        } 
        // Nếu không, thay thế phần tử tìm được bằng x để tối ưu cho các phần tử phía sau
        else {
            *it = x;
        }
    }

    return tails.size(); // Kích thước của tails chính là độ dài của LIS
}
