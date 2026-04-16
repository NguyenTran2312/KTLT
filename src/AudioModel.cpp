#include "AudioModel.h"

// =======================
// Channel
// =======================
int Channel::getSampleCount() const {
    return samples.size();
}

void Channel::addSample(double value) {
    samples.push_back(value);
}

// =======================
// AudioFile
// =======================
int AudioFile::findChannelIndex(const string& channel_name) const {
    for (int i = 0; i < channels.size(); i++) {
        if (channels[i].name == channel_name) {
            return i;
        }
    }
    return -1;
}

void AudioFile::addSampleToChannel(const string& channel_name, double value) {
    int idx = findChannelIndex(channel_name);

    if (idx == -1) {
        Channel new_channel;
        new_channel.name = channel_name;
        new_channel.samples.push_back(value);
        channels.push_back(new_channel);
    } else {
        channels[idx].samples.push_back(value);
    }
}

int AudioFile::getChannelCount() const {
    return channels.size();
}

double AudioFile::getSample(int channelIndex, int sampleIndex) const {
    return channels[channelIndex].samples[sampleIndex];
}

string AudioFile::getChannelName(int channelIndex) const {
    return channels[channelIndex].name;
}

SampleVector AudioFile::getAllSamples(int channelIndex) const {
    return channels[channelIndex].samples;
}

bool AudioFile::isValid() const {
    if (channels.empty()) return false;

    for (const auto& ch : channels) {
        if (ch.samples.empty()) return false;
    }
    return true;
}