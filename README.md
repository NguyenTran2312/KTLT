# Hướng dẫn Biên dịch và Thực thi (Build & Run Guide)

Tài liệu này cung cấp các bước chi tiết để biên dịch và chạy dự án Xử lý Tín hiệu Âm thanh Đa kênh (Audio Signal Processing Pipeline) viết bằng C++.

## 1. Yêu cầu hệ thống (Prerequisites)

* Trình biên dịch C++ hỗ trợ chuẩn C++11 trở lên (khuyến nghị GCC/MinGW).
* Terminal hoặc Command Prompt (PowerShell trên Windows, hoặc Bash trên Linux/macOS).

## 2. Cấu trúc thư mục hiện tại

Đảm bảo bạn đang mở Terminal tại thư mục gốc của dự án. Cấu trúc cây thư mục cần có các thành phần chính sau trước khi thực thi:

* `include/` : Chứa các file header (`.h`).
* `src/` : Chứa mã nguồn thực thi các module (`.cpp`).
* `data/` : Chứa dữ liệu đầu vào (`audio.csv`, `config.txt` và các file test case).
* `output/`: Thư mục trống để chương trình xuất kết quả.
* `main1.cpp`: File luồng chính (Pipeline) của chương trình.

## 3. Hướng dẫn Biên dịch (Build)

Mở Terminal tại thư mục gốc và thực thi dòng lệnh sau để biên dịch toàn bộ dự án:

```bash
g++ main.cpp src/AudioModel.cpp src/Fraction.cpp src/IO.cpp src/Polynomial.cpp src/SignalProcessor.cpp src/Statistics.cpp -I include -o main

```

**Giải thích các cờ (flags) biên dịch:**

* `g++`: Gọi trình biên dịch GNU C++.
* `main.cpp src/*.cpp`: Chỉ định tất cả các file mã nguồn cần được liên kết (linked) lại với nhau.
* `-I include`: Báo cho trình biên dịch biết thư mục chứa các file header để tham chiếu (Include path).
* `-o main`: Đặt tên cho file thực thi đầu ra là `main`.

*Lưu ý:* Nếu không có bất kỳ thông báo lỗi nào hiện ra và Terminal chuyển sang dòng lệnh mới, quá trình biên dịch đã thành công.

## 4. Hướng dẫn Thực thi (Run)

Sau khi biên dịch thành công, file thực thi `main` sẽ xuất hiện tại thư mục gốc. Thực thi bằng lệnh tương ứng với hệ điều hành:

* **Trên Windows (PowerShell/CMD):**
```bash
.\main

```


* **Trên Linux/macOS:**
```bash
./main

```



## 5. Cấu hình Đầu vào (Input)

Trước khi chạy, hãy đảm bảo các file sau tồn tại trong thư mục `data/`:

1. **`config.txt`**: Chứa 2 số nguyên cách nhau bởi khoảng trắng.
* Số thứ 1: Kích thước cửa sổ trượt $k$ (`window_size`).
* Số thứ 2: Bậc đa thức xấp xỉ $d$ (`poly_degree`).
* *Ví dụ:* `3 2` (Cửa sổ 3 mẫu, xấp xỉ đa thức bậc 2).


2. **`audio.csv`**: Dữ liệu tín hiệu âm thanh theo định dạng: `Channel,Index,Value`.

## 6. Kiểm tra Đầu ra (Output)

Chương trình sẽ tự động phân tích và lưu các kết quả vào thư mục `output/`. Bạn cần kiểm tra 3 file sau:

1. **`energy_report.csv`**: Bảng dữ liệu thống kê năng lượng tính theo thuật toán cửa sổ trượt (Sliding Window) của tất cả các kênh.
2. **`best_segment.txt`**: Báo cáo phân đoạn có tổng năng lượng lớn nhất (tìm bằng thuật toán Max sliding window ) kèm chỉ số start/end.
3. **`poly_approx.csv`**: Hệ số đa thức được tính toán bằng thuật toán Hồi quy bình phương tối thiểu (Khử Gauss) dựa trên đoạn tín hiệu tối ưu nhất.
