#ifndef AUDIO_MODEL_H
#define AUDIO_MODEL_H

#include <string>
#include <vector>
#include "Defines.h"    // Để có SampleVector
#include "Polynomial.h" // Để có Polynomial

// =======================
// Channel struct đại diện cho một kênh âm thanh
// Chứa tên kênh, vector mẫu và đa thức xấp xỉ
// =======================
struct Channel {
    std::string name;
    SampleVector samples;
    Polynomial approx;
    
    int getSampleCount() const;
    void addSample(double value);
};
// =======================
// AudioFile struct đại diện cho một file âm thanh
// Chứa tên file, sample rate và vector các kênh âm thanh
// =======================
struct AudioFile {
    std::string filename;
    int sample_rate;
    std::vector<Channel> channels;
    
    int findChannelIndex(const std::string& channel_name) const;
    void addSampleToChannel(const std::string& channel_name, double value);
    int getChannelCount() const;
    double getSample(int channelIdx, int sampleIdx) const;
    std::string getChannelName(int channelIdx) const;
    SampleVector getAllSamples(int channelIdx) const;
    bool isValid() const;
};

#endif
