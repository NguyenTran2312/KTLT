#include <iostream>
#include "AudioModel.h"

using namespace std;

// TRIỂN KHAI CÁC PHƯƠNG THỨC CHO STRUCT CHANNEL
// Lấy số lượng mẫu trong kênh âm thanh
int Channel::getSampleCount() const{
    return samples.size();
}
// Thêm mẫu vào kênh âm thanh
void Channel::addSample(double value){
    samples.push_back(value);
}
// TRIỂN KHAI CÁC PHƯƠNG THỨC CHO STRUCT AUDIOFILE
// Tìm kiếm chỉ số (index) của một kênh dựa trên tên kênh
int AudioFile::findChannelIndex(const std::string& channel_name) const {
    for (int i = 0; i < (int)channels.size(); i++) {
        if (channels[i].name == channel_name) {
            return i;
        }
    }
    return -1; // Trả về -1 nếu không tìm thấy kênh 
}
// Thêm mẫu vào kênh tương ứng, nếu chưa có thì tạo mới 
void AudioFile::addSampleToChannel(const std::string& channel_name, double value) {
    int idx = findChannelIndex(channel_name);

    if (idx == -1) {
        // Channel chưa tồn tại, tạo mới channel
        Channel new_channel;
        new_channel.name = channel_name;
        new_channel.addSample(value);
        channels.push_back(new_channel);
    } else {
        // Channel đã tồn tại, thêm mẫu vào channel đó
        channels[idx].addSample(value);
    }
}
// Trả về tổng số kênh đang quản 
int AudioFile::getChannelCount() const {
    return static_cast<int>(channels.size());
}
// Truy xuất một mẫu cụ thể dựa vào index kênh và index mẫu
double AudioFile::getSample(int channelIndex, int sampleIndex) const {
    return channels[channelIndex].samples[sampleIndex];
}    
// Lấy tên của kênh dựa vào index 
string AudioFile::getChannelName(int channelIndex) const {
    return channels[channelIndex].name;
} 
// Trả về toàn bộ mảng mẫu của một kênh 
SampleVector AudioFile::getAllSamples(int channelIndex) const {
    return channels[channelIndex].samples;
}    
// Kiểm tra tính toàn vẹn của dữ liệu trước khi xử lý 
bool AudioFile::isValid() const {
    // File phải có sample rate dương và có ít nhất một kênh 
    if (sample_rate <= 0 || channels.empty()) {
        return false;
    }

    // Mỗi kênh phải chứa ít nhất một mẫu âm thanh 
    for (const auto& ch : channels) {
        if (ch.samples.empty()) {
            return false;
        }
    }
    return true;
}
