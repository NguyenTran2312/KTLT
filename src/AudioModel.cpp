#include <iostream>
#include <string>
#include <vector>
#include "AudioModel.h"

using namespace std;

// =======================
// Channel struct đại diện cho một kênh âm thanh
// Chứa tên kênh, vector mẫu và đa thức xấp xỉ
// =======================
struct Channel {
    string name;
    SampleVector samples;
    Polynomial approx;
    
    // Lấy số lượng mẫu trong kênh âm thanh
    int getSampleCount() const{
        return samples.size();
    }
    // Thêm mẫu vào kênh âm thanh
    void addSample(double value){
        samples.push_back(value);
    }
};
// =======================
// AudioFile struct đại diện cho một file âm thanh
// Chứa tên file, sample rate và vector các kênh âm thanh
// =======================
struct AudioFile {
    string filename;
    int sample_rate;
    vector<Channel> channels;
    
    // Tìm channel theo tên
    int findChannelIndex(const string& channel_name) const{
        for (int i = 0; i < channels.size(); i++) {
            if (channels[i].name == channel_name) {
                return i;
            }
        }
        return -1; // Không tìm thấy channel
    }
    // Thêm mẫu vào channel
    // Nếu channel chưa tồn tại, tạo mới channel
    // Nếu channel đã tồn tại, thêm mẫu vào channel đó
    void addSampleToChannel(const string& channel_name, double value){
        int idx = findChannelIndex(channel_name);

        if (idx == -1) {
            // Channel chưa tồn tại, tạo mới channel
            Channel new_channel;
            new_channel.name = channel_name;
            new_channel.samples.push_back(value);
            channels.push_back(new_channel);
        } else {
            // Channel đã tồn tại, thêm mẫu vào channel đó
            channels[idx].samples.push_back(value);
        }
    }
    // Lấy số lượng channel trong file âm thanh
    int getChannelCount() const{
        return channels.size();
    }
    // Lấy 1 sample cụ thể
    double getSample(int channelIndex, int sampleIndex) const{
        return channels[channelIndex].samples[sampleIndex];
    }
    // Lấy tên channel theo index
    string getChannelName(int channelIndex) const{
        return channels[channelIndex].name;
    }
    // Lấy tất cả mẫu của channel theo index
    SampleVector getAllSamples(int channelIndex) const{
        return channels[channelIndex].samples;
    }
    // Kiểm tra tính hợp lệ của AudioFile
    // Điều kiện hợp lệ: file phải có ít nhất 1 channel và mỗi channel phải có ít nhất 1 sample
    bool isValid() const {
        if (channels.empty()) return false;

        for (const auto& ch : channels) {
            if (ch.samples.empty()) return false;
        }
        return true;
    }
};
