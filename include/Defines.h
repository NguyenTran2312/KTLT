#ifndef DEFINES_H
#define DEFINES_H

// Chứa bí danh, hằng số và mã lỗi
using SampleVector = std::vector<double>;
const double EPSILON = 1e-9;
const int DEFAULT_SAMPLE_RATE = 44100;
const double SILENCE_THRESHOLD = 0.001;

enum class AudioStatus {
    SUCCESS = 0,
    ERR_FILE_NOT_FOUND = 1,
    ERR_INVALID_FORMAT = 2,
    ERR_MISSING_DATA = 3,
    ERR_SAMPLE_RATE_INVALID = 4
};

#endif