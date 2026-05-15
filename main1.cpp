#include <iostream>
#include <vector>
#include <string>

#include "AudioModel.h"
#include "IO.h"
#include "Statistics.h"
#include "SignalProcessor.h"
#include "Polynomial.h"

using namespace std;

int main() {
    AudioFile audio;
    int k = 0, d = 0;

    cout << "--- KIEM TRA PIPELINE (SINGLE FOLDER MODE) ---" << endl;

    // 1. Đọc config
    if (!readConfig("config.txt", k, d)) {
        cerr << "Khong tim thay file config.txt trong cung thu muc!" << endl;
        return 1;
    }
    cout << "Config: k=" << k << ", d=" << d << endl;

    // 2. Đọc Audio CSV
    if (readAudioCSV("audio.csv", audio) != AudioStatus::SUCCESS) {
        cerr << "Loi doc file audio.csv!" << endl;
        return 1;
    }
    
    // Gán sample rate giả định nếu chưa có
    audio.sample_rate = 44100;

    if (!audio.isValid()) {
        cerr << "Du lieu audio khong hop le (Check sample_rate hoac so luong mau)." << endl;
        return 1;
    }

    // 3. Xử lý dữ liệu
    for (int i = 0; i < audio.getChannelCount(); ++i) {
        string name = audio.getChannelName(i);
        SampleVector samples = audio.getAllSamples(i);
        
        cout << "\nDang xu ly: " << name << endl;

        // Chuan hoa và tính toán
        SampleVector normalized = standardizeZScore(samples);
        SampleVector energy = calculateRollingEnergy(normalized, k);
        
        // Xuất báo cáo (File sẽ xuất hiện ngay trong thư mục này)
        writeEnergyReport("energy_" + name + ".csv", energy);
        
        SegmentResult best = findBestSegment(energy);
        writeBestSegment("best_" + name + ".txt", best, name);
        
        cout << " -> Hoan thanh kenh " << name << ". Da xuat file bao cao." << endl;
    }

    // 4. Xuất đa thức xấp xỉ
    writePolyApprox("poly_approx.csv", audio.channels);

    cout << "\n--- TAT CA KET QUA DA DUOC LUU TAI THU MUC HIEN TAI ---" << endl;

    return 0;
}

