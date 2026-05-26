#include "SignalProcessor.h"
#include "Statistics.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

// Prefix Sum 1D dùng để truy vấn nhanh O(1)
SampleVector buildPrefixSum(const SampleVector& samples) {
    int n = samples.size();
    SampleVector prefix(n + 1, 0.0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + (samples[i] * samples[i]);
    }
    return prefix;
}

double getSegmentEnergy(const SampleVector& prefix, int l, int r) {
    if (l < 0 || r >= (int)prefix.size() - 1 || l > r) return 0.0;
    return prefix[r + 1] - prefix[l];
}


vector<WindowStats> calculateSlidingWindow(const SampleVector& samples, int k) {
    vector<WindowStats> windows;
    int n = samples.size();
    if (n == 0 || k <= 0 || k > n) return windows;

    double current_sum = 0.0;
    double current_energy = 0.0;

    // 1. Tính toán cho cửa sổ đầu tiên (mẫu 0 đến k-1)
    SampleVector first_window(samples.begin(), samples.begin() + k);
    for (int i = 0; i < k; ++i) {
        current_sum += samples[i];
        current_energy += (samples[i] * samples[i]);
    }
    
    // Gọi hàm Min/Max của bạn
    double first_min = calculateMin(first_window);
    double first_max = calculateMax(first_window);
    
    windows.push_back({0, 0, k - 1, current_energy, current_sum / k, first_min, first_max});

    // 2. Trượt cửa sổ sang các vị trí tiếp theo
    for (int i = k; i < n; ++i) {
        current_sum += (samples[i] - samples[i - k]);
        current_energy += (samples[i] * samples[i]) - (samples[i - k] * samples[i - k]);
        
        int w_idx = i - k + 1;
        
        // Trích xuất cửa sổ hiện tại để tính Min/Max
        SampleVector current_window(samples.begin() + w_idx, samples.begin() + i + 1);
        double w_min = calculateMin(current_window);
        double w_max = calculateMax(current_window);

        windows.push_back({w_idx, w_idx, i, current_energy, current_sum / k, w_min, w_max});
    }
    return windows;
}


// Hàm quét tìm Cửa sổ trượt có tổng năng lượng lớn nhất
SegmentResult findBestSegment(const vector<WindowStats>& windows) {
    if (windows.empty()) return {-1, -1, 0.0};

    double max_energy = windows[0].rolling_energy;
    int best_index = 0;

    // Duyệt qua mảng các cửa sổ để tìm đỉnh năng lượng
    for (size_t i = 1; i < windows.size(); ++i) {
        if (windows[i].rolling_energy > max_energy) {
            max_energy = windows[i].rolling_energy;
            best_index = i;
        }
    }

    // Trích xuất dải mẫu gốc của cửa sổ đó
    int start_sample = windows[best_index].start_sample;
    int end_sample = windows[best_index].end_sample;

    return {start_sample, end_sample, max_energy};
}

// Phát hiện Crescendo (Chuỗi biên độ tăng dài nhất)
SegmentResult detectCrescendo(const SampleVector& samples) {
    if (samples.empty()) return {-1, -1, 0.0};

    int current_len = 1;
    int max_len = 1;
    
    int temp_start = 0;
    int best_start = 0;
    int best_end = 0;

    for (int i = 1; i < (int)samples.size(); ++i) {
        double curr = abs(samples[i]);
        double prev = abs(samples[i - 1]); // Lấy trực tiếp từ i-1 để tránh lệch nhịp biến toàn cục

        if (curr >= prev * 0.98) {
            current_len++;
        } else {
            current_len = 1;
            temp_start = i; // Chuỗi tăng mới bắt đầu tính từ vị trí i
        }

        // Cập nhật khi tìm thấy độ dài lớn hơn hẳn
        if (current_len > max_len) {
            max_len = current_len;
            best_start = temp_start;
            best_end = i;
        }
    }
    
    return {best_start, best_end, (double)max_len};
}