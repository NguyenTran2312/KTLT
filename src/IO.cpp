#include "IO.h"
#include "AudioModel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;
// =======================
// 1. Read config 
// Đọc 2 tham số window_size và poly_degree từ file 
// =======================
bool readConfig(const std::string& filepath, int& window_size, int& poly_degree){
    ifstream fileIn(filepath);

    // Kiểm tra nếu file không mở được
    if (!fileIn.is_open()) {
        cerr << "Error opening config file: " << filepath << endl;
        return false;
    }
    // Đọc window_size và poly_degree từ file
    fileIn >> window_size >> poly_degree;
    fileIn.close();
    return true;
}
// =======================
// 2. Read audio data from CSV
// Đọc file CSV chứa dữ liệu âm thanh dạng: channal, index, value
// Mục tiêu: parse từng dòng và đưa value vào đúng channel tronh AudioFile 
AudioStatus readAudioCSV(const string& filepath, AudioFile& audio){
    ifstream fileIn(filepath);
    if (!fileIn.is_open()) {
        cerr << "Error opening audio file: " << filepath << endl;
        return AudioStatus::FILE_ERROR;
    }
    string line;
    // Bỏ qua dòng header nếu có
    getline(fileIn, line);
    // Đọc từng dòng dữ liệu
    while (getline(fileIn, line)) {

        // Bỏ qua dòng trống
        if (line.empty()) continue;
        stringstream ss(line);
        string channel_name;
        string index_str, value_str;
        // Đọc channel, index, value từ dòng
        if (!getline(ss, channel_name, ',') || !getline(ss, index_str, ',') || !getline(ss, value_str)) {
            cerr << "Error parsing line: " << line << endl;
            continue; // Bỏ qua dòng lỗi    
        }
        try {
            int index = stoi(index_str);
            double value = stod(value_str);
            // Đưa value vào đúng channel trong AudioFile
            audio.addSampleToChannel(channel_name, value);
        }
        catch (...) {
            cerr << "Invalid data in line: " << line << endl;
            // Tự động gán bằng 0 nếu dữ liệu khuyết
            audio.addSampleToChannel(channel_name, 0.0);
        }       
    }
    fileIn.close();
    return AudioStatus::SUCCESS;
}

// =======================
// 3. Write output data to CSV
// Ghi kết quả phân tích vào file CSV gồm:
// - Rolling energy cho mỗi mẫu âm thanh
// - Thông tin về đoạn âm thanh có năng lượng cao nhất (start_index, end_index, energy)
// - Hệ số của đa thức bậc n đã được xấp xấp cho mỗi kênh âm thanh
// =======================
bool writeEnergyReport(const string& filepath, const SampleVector& rolling_energy){
    ofstream fileOut(filepath);
    if (!fileOut.is_open()) {
        cerr << "Error opening output file: " << filepath << endl;
        return false;
    }
    for (int i = 0; i < rolling_energy.size(); i++) {
        fileOut << i << "," << rolling_energy[i] << endl;
    }
    fileOut.close();
    return true;
}
// Ghi thông tin về đoạn âm thanh có năng lượng cao nhất (start_index, end_index, energy)
bool writeBestSegment(const string& filepath, SegmentResult segment, const string& channel_name){
    ofstream fileOut(filepath);
    if (!fileOut.is_open()) {
        cerr << "Error opening output file: " << filepath << endl;
        return false;
    }
    fileOut << "Best Segment for Channel: " << channel_name << endl;
    fileOut << "Start Index: " << segment.start_index << endl;
    fileOut << "End Index: " << segment.end_index << endl;
    fileOut << "Energy: " << segment.value << endl;
    fileOut.close();
    return true;

}
// Ghi hệ số của đa thức bậc n đã được xấp xấp cho mỗi kênh âm thanh
bool writePolyApprox(const string& filepath, const vector<Channel>& channels){
    ofstream fileOut(filepath);
    if (!fileOut.is_open()) {
        cerr << "Error opening output file: " << filepath << endl;
        return false;
    }
    for (const auto& ch : channels) {
        fileOut << "Channel: " << ch.name << endl;
        fileOut << "Polynomial Coefficients: ";
        
        for (const auto& coeff : ch.approx.coeffs) {
            fileOut << coeff << " ";
        }
        fileOut << endl;
    }
    fileOut.close();
    return true;
}
