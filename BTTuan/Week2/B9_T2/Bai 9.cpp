#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// ── Đọc mảng số thực ─────────────────────────────────────────────────────────
bool readArray(const string& path, vector<double>& arr) {
    ifstream fin(path);
    if (!fin) {
        cerr << "[ERROR] Khong mo duoc file: " << path << "\n";
        return false;
    }
    int n;
    if (!(fin >> n) || n < 0) {
        cerr << "[ERROR] n khong hop le.\n";
        return false;
    }
    arr.resize(n);
    for (int i = 0; i < n; ++i) {
        if (!(fin >> arr[i])) {
            cerr << "[ERROR] Doc phan tu thu " << i << " that bai.\n";
            return false;
        }
    }
    return true;
}

// ── Đọc k từ config ──────────────────────────────────────────────────────────
bool readK(const string& path, int& k) {
    ifstream fin(path);
    if (!fin) {
        cerr << "[ERROR] Khong mo duoc file config: " << path << "\n";
        return false;
    }
    if (!(fin >> k) || k <= 0) {
        cerr << "[ERROR] k phai la so nguyen duong.\n";
        return false;
    }
    return true;
}

// ── Tính histogram ────────────────────────────────────────────────────────────
// Trả về vector counts kích thước k
// bins[b] = [minVal + b*width, minVal + (b+1)*width)
// Phần tử == maxVal thuộc bin cuối (k-1)
vector<int> buildHistogram(const vector<double>& arr, int k,
                            double& minVal, double& maxVal, double& width) {
    minVal = *min_element(arr.begin(), arr.end());
    maxVal = *max_element(arr.begin(), arr.end());
    width  = (maxVal - minVal) / k;

    vector<int> counts(k, 0);

    for (double x : arr) {
        if (x == maxVal) {
            counts[k - 1]++;           // Phần tử max → bin cuối
            continue;
        }
        int bin = (int)((x - minVal) / width);
        // Đảm bảo trong phạm vi (phòng lỗi floating point)
        if (bin < 0)     bin = 0;
        if (bin >= k)    bin = k - 1;
        counts[bin]++;
    }
    return counts;
}

// ── Ghi histogram ─────────────────────────────────────────────────────────────
bool writeHistogram(const string& path,
                    int k, double minVal, double width,
                    const vector<int>& counts) {
    ofstream fout(path);
    if (!fout) {
        cerr << "[ERROR] Khong mo duoc file output: " << path << "\n";
        return false;
    }
    fout << fixed << setprecision(6);
    for (int b = 0; b < k; ++b) {
        double lo = minVal + b * width;
        double hi = minVal + (b + 1) * width;
        fout << "[" << lo << ", " << hi << "): " << counts[b] << "\n";
    }
    return true;
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    const string INPUT_ARR    = "input.txt";
    const string INPUT_CONFIG = "config.txt";
    const string OUTPUT       = "histogram.txt";

    vector<double> arr;
    if (!readArray(INPUT_ARR, arr)) return 1;

    int k;
    if (!readK(INPUT_CONFIG, k)) return 1;

    // Case biên: mảng rỗng
    if (arr.empty()) {
        ofstream fout(OUTPUT);
        if (!fout) { cerr << "[ERROR] Khong mo duoc file output.\n"; return 1; }
        fout << "EMPTY\n";
        cout << "Mang rong → EMPTY\n";
        return 0;
    }

    // Case biên: tất cả phần tử bằng nhau → width = 0
    // Xử lý: 1 bin duy nhất chứa toàn bộ phần tử
    double minVal, maxVal, width;
    if (*min_element(arr.begin(), arr.end()) ==
        *max_element(arr.begin(), arr.end())) {
        ofstream fout(OUTPUT);
        if (!fout) { cerr << "[ERROR] Khong mo duoc file output.\n"; return 1; }
        fout << fixed << setprecision(6);
        double v = arr[0];
        fout << "[" << v << ", " << v << "]: " << arr.size() << "\n";
        cout << "Tat ca phan tu bang nhau = " << v << "\n";
        return 0;
    }

    vector<int> counts = buildHistogram(arr, k, minVal, maxVal, width);
    if (!writeHistogram(OUTPUT, k, minVal, width, counts)) return 1;

    cout << "Da ghi histogram " << k << " bins vao " << OUTPUT << "\n";
    return 0;
}
