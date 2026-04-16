#include <iostream>
#include "SignalProcessor.h"
#include "Defines.h"

int main() {
    // Mảng biên độ âm thanh giả lập
    SampleVector audio_samples = {0.1, 0.5, 0.8, -1.2, 0.3, 0.6, 0.9, -0.1};

    std::cout << "--- TEST KADANE (Tren mang nang luong) ---" << std::endl;
    // Giả sử lấy mẫu âm thanh làm mảng tính toán
    SegmentResult best = findBestSegment(audio_samples);
    std::cout << "Doan nang luong Max tu index " << best.start_index 
              << " den " << best.end_index << " | Tong = " << best.value << std::endl;

    std::cout << "\n--- TEST CRESCENDO ---" << std::endl;
    SegmentResult cres = detectCrescendo(audio_samples);
    std::cout << "Doan to dan dai nhat tu index " << cres.start_index 
              << " den " << cres.end_index << " | So mau = " << cres.value << std::endl;

    std::cout << "\n--- TEST SLIDING WINDOW (k = 3) ---" << std::endl;
    std::vector<double> roll_energy = calculateRollingEnergy(audio_samples, 3);
    for (int i = 0; i < roll_energy.size(); ++i) {
        std::cout << "Window " << i << ": " << roll_energy[i] << std::endl;
    }

    return 0;
}