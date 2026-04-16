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

SampleVector buildPrefixSum(const SampleVector& samples) {
    int n = samples.size();
    // Tạo mảng prefix có kích thước n + 1 để dễ tính toán (tránh lỗi l - 1 < 0)
    SampleVector prefix(n + 1, 0.0);
    
    for (int i = 0; i < n; ++i) {
        // Cộng dồn năng lượng: P[i+1] = P[i] + (samples[i]^2)
        prefix[i + 1] = prefix[i] + (samples[i] * samples[i]);
    }
    return prefix;
}

double getSegmentEnergy(const SampleVector& prefix, int l, int r) {
    if (l < 0 || r >= prefix.size() - 1 || l > r) return 0.0;
    // Năng lượng đoạn [l, r] = P[r+1] - P[l]
    return prefix[r + 1] - prefix[l];
}

SampleVector calculateRollingEnergy(const SampleVector& samples, int k) {
    int n = samples.size();
    SampleVector rolling_energy;
    
    if (n == 0 || k <= 0 || k > n) return rolling_energy; // Xử lý case biên

    double current_window_energy = 0.0;

    // 1. Tính năng lượng cho cửa sổ đầu tiên (từ 0 đến k-1)
    for (int i = 0; i < k; ++i) {
        current_window_energy += (samples[i] * samples[i]);
    }
    rolling_energy.push_back(current_window_energy);

    // 2. Trượt cửa sổ từ k đến n - 1
    for (int i = k; i < n; ++i) {
        // Cộng năng lượng mẫu mới vào, trừ năng lượng mẫu cũ (i-k) ra
        current_window_energy += (samples[i] * samples[i]);
        current_window_energy -= (samples[i - k] * samples[i - k]);
        
        rolling_energy.push_back(current_window_energy);
    }
    
    return rolling_energy;
}

SegmentResult findBestSegment(const SampleVector& energy_array) {
    if (energy_array.empty()) return {-1, -1, 0.0};

    double current_max = energy_array[0];
    double global_max = energy_array[0];
    
    int temp_start = 0;
    int best_start = 0;
    int best_end = 0;

    for (int i = 1; i < energy_array.size(); ++i) {
        // Nếu việc cộng thêm energy_array[i] làm tổng hiện tại bé hơn chính energy_array[i]
        // Nghĩa là ta nên "chặt đứt" dãy cũ và bắt đầu đếm lại dãy mới từ i
        if (energy_array[i] > current_max + energy_array[i]) {
            current_max = energy_array[i];
            temp_start = i; // Đánh dấu điểm bắt đầu mới
        } else {
            current_max += energy_array[i];
        }

        // Cập nhật kỷ lục nếu tìm được chuỗi có tổng lớn hơn
        if (current_max > global_max) {
            global_max = current_max;
            best_start = temp_start;
            best_end = i;
        }
    }

    return {best_start, best_end, global_max};
}

SegmentResult detectCrescendo(const SampleVector& samples) {
    if (samples.empty()) return {-1, -1, 0.0};

    int current_len = 1;
    int max_len = 1;
    
    int temp_start = 0;
    int best_start = 0;
    int best_end = 0;

    for (int i = 1; i < samples.size(); ++i) {
        // Lấy trị tuyệt đối vì ta quan tâm đến CƯỜNG ĐỘ âm thanh to dần
        double current_amp = std::abs(samples[i]);
        double prev_amp = std::abs(samples[i - 1]);

        if (current_amp >= prev_amp) {
            current_len++; // Nếu đang to dần lên thì tăng độ dài chuỗi
        } else {
            current_len = 1; // Bị ngắt quãng, reset lại độ dài
            temp_start = i;  // Bắt đầu đếm chuỗi mới
        }

        // Cập nhật kỷ lục
        if (current_len > max_len) {
            max_len = current_len;
            best_start = temp_start;
            best_end = i;
        }
    }

    // Giá trị trả về ở đây lưu ĐỘ DÀI của đoạn crescendo
    return {best_start, best_end, (double)max_len}; 
}
