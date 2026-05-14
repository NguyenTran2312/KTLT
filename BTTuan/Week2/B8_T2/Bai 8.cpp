#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>
using namespace std;

// ── Tiện ích: trim khoảng trắng hai đầu ─────────────────────────────────────
string trim(const string& s) {
    int l = 0, r = (int)s.size() - 1;
    while (l <= r && isspace((unsigned char)s[l])) ++l;
    while (r >= l && isspace((unsigned char)s[r])) --r;
    return s.substr(l, r - l + 1);
}

// ── Kiểm tra token missing ───────────────────────────────────────────────────
bool isMissingToken(const string& raw) {
    string t = trim(raw);
    // Chuyển lowercase để so sánh
    string low = t;
    transform(low.begin(), low.end(), low.begin(),
              [](unsigned char c){ return tolower(c); });
    return low.empty() || low == "na" || low == "nan" || low == "n/a";
}

// ── Struct lưu từng dòng ─────────────────────────────────────────────────────
struct Entry {
    bool   valid;   // false nếu missing
    double value;
};

// ── Đọc file CSV 1 cột ───────────────────────────────────────────────────────
bool readCSV(const string& path, vector<Entry>& entries) {
    ifstream fin(path);
    if (!fin) {
        cerr << "[ERROR] Khong mo duoc file: " << path << "\n";
        return false;
    }
    string line;
    while (getline(fin, line)) {
        Entry e;
        if (isMissingToken(line)) {
            e.valid = false;
            e.value = 0.0;
        } else {
            try {
                size_t pos;
                e.value = stod(trim(line), &pos);
                // Đảm bảo toàn bộ chuỗi là số
                string rest = trim(line).substr(pos);
                e.valid = rest.empty();
                if (!e.valid) { e.value = 0.0; } // không parse được
            } catch (...) {
                e.valid = false;
                e.value = 0.0;
            }
        }
        entries.push_back(e);
    }
    return true;
}

// ── Tính median từ vector đã có giá trị hợp lệ ──────────────────────────────
double computeMedian(vector<double> valid) {
    // valid đã được truyền theo value (copy)
    sort(valid.begin(), valid.end());
    int n = (int)valid.size();
    if (n == 0) return 0.0;
    if (n % 2 == 1) return valid[n / 2];
    return (valid[n / 2 - 1] + valid[n / 2]) / 2.0;
}

// ── Ghi file imputed ─────────────────────────────────────────────────────────
bool writeImputed(const string& path,
                  const vector<Entry>& entries,
                  double median) {
    ofstream fout(path);
    if (!fout) {
        cerr << "[ERROR] Khong mo duoc file output: " << path << "\n";
        return false;
    }
    fout << fixed << setprecision(6);
    fout << "i,value_imputed\n";
    for (int i = 0; i < (int)entries.size(); ++i) {
        double val = entries[i].valid ? entries[i].value : median;
        fout << i << "," << val << "\n";
    }
    return true;
}

// ── Ghi summary ──────────────────────────────────────────────────────────────
bool writeSummary(const string& path,
                  int countValid,
                  int countMissing,
                  double median) {
    ofstream fout(path);
    if (!fout) {
        cerr << "[ERROR] Khong mo duoc file summary: " << path << "\n";
        return false;
    }
    fout << fixed << setprecision(6);
    fout << "count_valid="   << countValid   << "\n";
    fout << "count_missing=" << countMissing << "\n";
    if (countValid == 0)
        fout << "median=N/A\n";
    else
        fout << "median=" << median << "\n";
    return true;
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    const string INPUT        = "data.csv";
    const string OUT_IMPUTED  = "imputed.csv";
    const string OUT_SUMMARY  = "summary.txt";

    vector<Entry> entries;
    if (!readCSV(INPUT, entries)) return 1;

    // Tách giá trị hợp lệ để tính median
    vector<double> validVals;
    for (auto& e : entries)
        if (e.valid) validVals.push_back(e.value);

    int countValid   = (int)validVals.size();
    int countMissing = (int)entries.size() - countValid;

    // Case biên: không có giá trị hợp lệ → median = 0 (không có gì để impute)
    double median = (countValid > 0) ? computeMedian(validVals) : 0.0;

    if (!writeImputed(OUT_IMPUTED, entries, median)) return 1;
    if (!writeSummary(OUT_SUMMARY, countValid, countMissing, median)) return 1;

    cout << "count_valid="   << countValid   << "\n";
    cout << "count_missing=" << countMissing << "\n";
    if (countValid > 0)
        cout << fixed << setprecision(6) << "median=" << median << "\n";
    else
        cout << "median=N/A\n";

    return 0;
}
