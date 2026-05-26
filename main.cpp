// ==========================================
// File: main1.cpp (Đã điều chỉnh)
// ==========================================
#include <iostream>
#include <vector>
#include <string>

#include "AudioModel.h"
#include "IO.h"
#include "Statistics.h"
#include "SignalProcessor.h"
#include "Polynomial.h"
#include "Defines.h"

using namespace std;

int main() {
    AudioFile audio;
    int k = 0, d = 0;

    cout << "--- KIEM TRA PIPELINE ---" << endl;

    if (!readConfig("data/config.txt", k, d)) {
        cerr << "Khong tim thay file config.txt" << endl;
        return 1;
    }
    cout << "Config: k=" << k << ", d=" << d << endl;

    if (readAudioCSV("data/audio.csv", audio) != AudioStatus::SUCCESS) {
        cerr << "Loi doc file audio.csv!" << endl;
        return 1;
    }
    
    audio.sample_rate = DEFAULT_SAMPLE_RATE;
    if (!audio.isValid()) {
        cerr << "Du lieu audio khong hop le." << endl;
        return 1;
    }

    vector<string> channel_names;
    vector<Polynomial> best_polys;
    bool is_first_channel = true; 

    for (int i = 0; i < audio.getChannelCount(); ++i) {
        string name = audio.getChannelName(i);
        SampleVector samples = audio.getAllSamples(i);
        channel_names.push_back(name);
        
        cout << "\nDang xu ly: " << name << endl;

        // 1. Tiền xử lý dữ liệu (Z-Score)
        SampleVector normalized = standardizeZScore(samples);
        
        // 2. Chạy Sliding Window tối ưu (Tính song song Energy và Mean)
        vector<WindowStats> windows = calculateSlidingWindow(normalized, k);
        
        // Ghi báo cáo năng lượng và trung bình trượt dạng bảng phẳng CSV
        writeEnergyReport("output/energy_report.csv", name, windows, !is_first_channel);
        
        // 3. Phân tích chuỗi con bằng thuật toán Kadane và phát hiện Crescendo
        SegmentResult best_segment = findBestSegment(windows);
        SegmentResult crescendo = detectCrescendo(normalized);
        
        // Ghi kết quả nhận diện nâng cao ra file TXT báo cáo sạch
        writeBestSegment("output/best_segment.txt", name, best_segment, crescendo, k, !is_first_channel);
        
        if (best_segment.start_index != -1) {
            SampleVector prefix_energy = buildPrefixSum(normalized);
            double check_energy = getSegmentEnergy(prefix_energy, best_segment.start_index, best_segment.end_index);
            cout << "   -> [Prefix Sum Check] Nang luong doan tot nhat: " << check_energy << endl;
        }

        // 4. Lấy dữ liệu đoạn tốt nhất phục vụ xấp xỉ đa thức
        vector<double> best_x;
        vector<double> best_y;
        for (int j = best_segment.start_index; j <= best_segment.end_index && j < samples.size(); ++j) {
            best_x.push_back(j);
            best_y.push_back(samples[j]); 
        }

        Polynomial poly = polyFit(best_x, best_y, d);
        best_polys.push_back(poly);

        cout << "Polynomial cho Best Segment (" << name << "): ";
        printPolynomial(poly);
        cout << " -> Hoan thanh kenh " << name << endl;
        
        is_first_channel = false; 
    }

    // 5. Xuất báo cáo đa thức nội suy cấu trúc phân số
    writePolyApprox("output/poly_approx.csv", "output/poly_coefficients.txt", channel_names, audio, best_polys);

    cout << "\n--- TAT CA KET QUA DA DUOC LUU TAI THU MUC HIEN TAI ---" << endl;

    return 0;
}