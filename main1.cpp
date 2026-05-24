// ==========================================
// File: main1.cpp
// ==========================================
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
    
    audio.sample_rate = 44100;
    if (!audio.isValid()) {
        cerr << "Du lieu audio khong hop le." << endl;
        return 1;
    }

    vector<string> channel_names;
    vector<Polynomial> best_polys;
    bool is_first_channel = true; // Biến kiểm soát việc ghi đè hay ghi tiếp

    for (int i = 0; i < audio.getChannelCount(); ++i) {
        string name = audio.getChannelName(i);
        SampleVector samples = audio.getAllSamples(i);
        channel_names.push_back(name);
        
        cout << "\nDang xu ly: " << name << endl;

        SampleVector normalized = standardizeZScore(samples);
        SampleVector energy = calculateRollingEnergy(normalized, k);
        
        // Gọi hàm ghi file mới, truyền thêm is_first_channel để biết có cần tạo mới/append không
        writeEnergyReport("output/energy_report.csv", name, energy, !is_first_channel);
        
        SegmentResult best = findBestSegment(energy, k);
        writeBestSegment("output/best_segment.txt", best, name, !is_first_channel);
        
        vector<double> best_x;
        vector<double> best_y;
        for (int j = best.start_index; j <= best.end_index && j < samples.size(); ++j) {
            best_x.push_back(j);
            best_y.push_back(samples[j]); 
        }

        Polynomial poly = polyFit(best_x, best_y, d);
        best_polys.push_back(poly);

        cout << "Polynomial cho Best Segment (" << name << "): ";
        printPolynomial(poly);
        cout << " -> Hoan thanh kenh " << name << endl;
        
        is_first_channel = false; // Các kênh sau sẽ được append vào cuối file
    }

    writePolyApprox("output/poly_approx.csv", channel_names, best_polys);

    cout << "\n--- TAT CA KET QUA DA DUOC LUU TAI THU MUC HIEN TAI ---" << endl;

    return 0;
}
