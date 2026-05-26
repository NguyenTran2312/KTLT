#ifndef IO_H
#define IO_H

#include <string>
#include <vector>
#include "AudioModel.h"   // Để dùng struct AudioFile và Channel
#include "Defines.h"      // Để dùng enum AudioStatus và các bí danh như SampleVector
#include "SignalProcessor.h"

// =================================================================
// 1. CÁC HÀM ĐỌC DỮ LIỆU (INPUT)
// =================================================================

bool readConfig(const std::string& filepath, int& window_size, int& poly_degree);
AudioStatus readAudioCSV(const std::string& filepath, AudioFile& audio);

// =================================================================
// 2. CÁC HÀM XUẤT DỮ LIỆU (OUTPUT)
// =================================================================

//Ghi báo cáo năng lượng cuộn (Rolling Energy) của các mẫu
bool writeEnergyReport(const std::string& filepath, const std::string& channel_name, const SampleVector& rolling_energy, bool append);

// Ghi thông tin về đoạn âm thanh có năng lượng cao nhất (start_index, end_index, value)
bool writeBestSegment(const std::string& filepath, SegmentResult segment, const std::string& channel_name, bool append);

//Ghi các hệ số đa thức xấp xỉ dưới dạng phân số cho tất cả các kênh
void writePolyApprox(const std::string& filename, const std::vector<std::string>& channel_names, const AudioFile& audio, const std::vector<Polynomial>& polys);
#endif
