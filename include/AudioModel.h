#ifndef AUDIO_MODEL_H
#define AUDIO_MODEL_H

#include <string>
#include <vector>
#include "Defines.h"
#include "Polynomial.h"

using std::string;
using std::vector;

// =======================
// Channel struct
// =======================
struct Channel {
    string name;
    SampleVector samples;
    Polynomial approx;

    int getSampleCount() const;
    void addSample(double value);
};

// =======================
// AudioFile struct
// =======================
struct AudioFile {
    string filename;
    int sample_rate;
    vector<Channel> channels;

    int findChannelIndex(const string& channel_name) const;
    void addSampleToChannel(const string& channel_name, double value);
    int getChannelCount() const;
    double getSample(int channelIndex, int sampleIndex) const;
    string getChannelName(int channelIndex) const;
    SampleVector getAllSamples(int channelIndex) const;
    bool isValid() const;
};

#endif