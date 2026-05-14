#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cctype>
#include <stdexcept>
using namespace std;

// ── Tiện ích: trim ────────────────────────────────────────────────────────────
string trim(const string& s) {
    int l = 0, r = (int)s.size() - 1;
    while (l <= r && isspace((unsigned char)s[l])) ++l;
    while (r >= l && isspace((unsigned char)s[r])) --r;
    return (l > r) ? "" : s.substr(l, r - l + 1);
}

// ── Struct điểm ──────────────────────────────────────────────────────────────
struct Point { double x, y; };

// ── Đọc points.csv (x,y mỗi dòng) ───────────────────────────────────────────
bool readPoints(const string& path, vector<Point>& pts) {
    ifstream fin(path);
    if (!fin) {
        cerr << "[ERROR] Khong mo duoc file: " << path << "\n";
        return false;
    }
    string line;
    int lineNo = 0;
    while (getline(fin, line)) {
        ++lineNo;
        line = trim(line);
        if (line.empty()) continue;               // bỏ dòng trống
        istringstream ss(line);
        string sx, sy;
        if (!getline(ss, sx, ',') || !getline(ss, sy)) {
            cerr << "[WARN] Dong " << lineNo << " khong dung dinh dang, bo qua.\n";
            continue;
        }
        try {
            Point p;
            p.x = stod(trim(sx));
            p.y = stod(trim(sy));
            pts.push_back(p);
        } catch (...) {
            cerr << "[WARN] Parse that bai dong " << lineNo << ", bo qua.\n";
        }
    }
    return true;
}

// ── Đọc queries.csv (mỗi dòng 1 số) ─────────────────────────────────────────
bool readQueries(const string& path, vector<double>& queries) {
    ifstream fin(path);
    if (!fin) {
        cerr << "[ERROR] Khong mo duoc file: " << path << "\n";
        return false;
    }
    string line;
    int lineNo = 0;
    while (getline(fin, line)) {
        ++lineNo;
        line = trim(line);
        if (line.empty()) continue;
        try {
            queries.push_back(stod(line));
        } catch (...) {
            cerr << "[WARN] Parse that bai dong " << lineNo << " ('" << line << "'), bo qua.\n";
        }
    }
    return true;
}

// ── Nội suy tuyến tính tại x_q ───────────────────────────────────────────────
// Giả sử pts đã sắp xếp tăng theo x, không trùng x.
// Trả về true nếu trong khoảng, false nếu ngoài.
bool interpolate(const vector<Point>& pts, double xq, double& yq) {
    int n = (int)pts.size();
    if (n == 0) return false;

    double xMin = pts.front().x;
    double xMax = pts.back().x;

    // Ngoài khoảng
    if (xq < xMin || xq > xMax) return false;

    // Tìm đoạn [pts[i], pts[i+1]] chứa xq bằng binary search
    // (hoặc linear scan — n nhỏ)
    // lower_bound theo x
    int lo = 0, hi = n - 1;
    // Nếu xq == xMax → trả về điểm cuối
    if (xq == xMax) { yq = pts.back().y; return true; }

    // Tìm i sao cho pts[i].x <= xq < pts[i+1].x
    while (lo + 1 < hi) {
        int mid = (lo + hi) / 2;
        if (pts[mid].x <= xq) lo = mid;
        else                   hi = mid;
    }
    // lo và lo+1 là đoạn nội suy
    double x0 = pts[lo].x,   y0 = pts[lo].y;
    double x1 = pts[lo+1].x, y1 = pts[lo+1].y;

    // Nội suy tuyến tính: y = y0 + (xq - x0) * (y1 - y0) / (x1 - x0)
    yq = y0 + (xq - x0) * (y1 - y0) / (x1 - x0);
    return true;
}

// ── Ghi kết quả ──────────────────────────────────────────────────────────────
bool writeResult(const string& path,
                 const vector<double>& queries,
                 const vector<Point>& pts) {
    ofstream fout(path);
    if (!fout) {
        cerr << "[ERROR] Khong mo duoc file output: " << path << "\n";
        return false;
    }
    fout << fixed << setprecision(6);
    fout << "x_q,y_interp\n";

    for (double xq : queries) {
        double yq;
        if (interpolate(pts, xq, yq))
            fout << xq << "," << yq << "\n";
        else
            fout << xq << ",OUT_OF_RANGE\n";
    }
    return true;
}

// ── main ──────────────────────────────────────────────────────────────────────
int main() {
    const string INPUT_PTS  = "points.csv";
    const string INPUT_QRY  = "queries.csv";
    const string OUTPUT     = "interpolated.csv";

    vector<Point>  pts;
    vector<double> queries;

    if (!readPoints(INPUT_PTS, pts))   return 1;
    if (!readQueries(INPUT_QRY, queries)) return 1;

    // Case biên: không có điểm nào → mọi query là OUT_OF_RANGE
    if (pts.empty()) {
        cerr << "[WARN] Khong co diem noi suy nao → moi query la OUT_OF_RANGE.\n";
    }

    // Case biên: chỉ 1 điểm → chỉ xq == x0 mới nội suy được
    if (pts.size() == 1) {
        cerr << "[WARN] Chi co 1 diem → chi noi suy tai dung x = " << pts[0].x << ".\n";
    }

    if (!writeResult(OUTPUT, queries, pts)) return 1;

    cout << "Da xu ly " << queries.size() << " query, ghi vao " << OUTPUT << "\n";
    return 0;
}
