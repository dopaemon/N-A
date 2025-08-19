# Hệ thống kiểm tra - thông báo trẻ em bị bỏ quên trên xe ôtô
## Giả lập
- sử dụng nền tảng công cụ giả lập trước khi triển khai thực tế:
  + [Wokwi](https://wokwi.com/projects/439611244632673281)
  + NodeRED
## Thành phần yêu cầu:
- Các thành phần yêu cầu cơ bản trong hệ thống:
  + ESP32 S3 (1 Cái)
  + DHT22 (1 Cái)
  + Cảm biến PIR (2 Cái)
  + Cảm biến MPU6050 (1 Cái)
  + Cảm biến thời gian thực DS1307 - RTC (1 Cái)
  + Buzzer Cảnh Báo 5V (1 Cái)
  + LED Cảnh báo (3 Cái, Khác Màu)
  + Trở 1KΩ (3 Cái)
  + Momentary (2 Cái)
  + SSD1306 I2C (1 Cái)
## Mindmap
```mindthemaps
                                     +======================================================+
                                     |  HỆ THỐNG CẢNH BÁO TRẺ EM BỊ BỎ QUÊN TRÊN XE HƠI     |
                                     +======================================================+
                                                      |
                      ---------------------------------------------------------------------------------------------------------------------
                      |                          |                                |                              |                        | 
                  1. Tổng quan              2. Cấu trúc hệ thống           3. Công nghệ sử dụng             4. Phương thức cảnh báo    5. Công cụ giả lập
                      |                          |                                |                              |                        |
                      |                          |                                |                              |                        |
 ----------------------------------    ---------------------------------    ---------------------------     -------------------      -----------------------
 |                                |    |                               |    |                         |     |                 |     |                       |
 | - Thực trạng tại VN            |    | - Hệ thống cảm biến           |    | - Xử lý trung tâm       |     | - Cảnh báo      |     | + Wokwi               |
 | - Tai nạn đã ghi nhận          |    |  + Trọng lượng ghế            |    | + ESP32 S3              |     | + Âm thanh      |     | + Node-RED + MQTT     |
 | - Nguyên nhân phổ biến         |    |  + PIR (chuyển động)          |    | + Onion Omega 2+        |     | + Tin nhắn SMS  |     | + Tinkercad / Proteus |
 | - Hậu quả nghiêm trọng         |    |  + Nhiệt độ, CO2              |    | + Xử lý ảnh (OpenCV)    |     | + Mobile App    |     -------------------------
 | - Nhu cầu giải pháp cấp bách   |    |  + Camera AI                  |    ---------------------------     -------------------     
 ----------------------------------    |                               |    
                                       |                               |    
                                        -------------------------------    
                                                     |
                                        ---------------------------------
                                       |                                 |
                                 ------------------------------------------------------------------
                                |  - Nhận diện trẻ em              - Kiểm tra trạng thái xe        |
                                |    + Hình ảnh, nhiệt, cử động      + Động cơ đã tắt              |
                                |    + Hệ thống ghế                  + Xe đã khóa                  |
                                |                                    + Không phát hiện người lớn   |
                                 ------------------------------------------------------------------
                                                      |
                                                      |
                                                      |
                                        -------------------------------------------------
                                       |                                                 |
                                   5. Đánh giá hệ thống                          6. Hạn chế & cải tiến
                                       |                                                 |
                       ------------------------------------          --------------------------------------------
                      |                    |               |        |                        |                   |
                 - Tốc độ phản hồi   - Độ chính xác   - Hiệu suất |  - Sai số phát hiện   - Cảm biến giả    - Kết nối yếu
                 - Độ bền cảm biến  - An toàn        - Tiêu thụ   |  - Nhiễu từ ngoại cảnh- Trẻ em ngủ yên  - Không có sóng
                 - Tương thích xe   - Khả năng bảo trì năng lượng |  - Không phân biệt trẻ em / vật thể khác |  - Bảo mật hệ thống

                                                                                   |
                                                                                   |
                                                                      -------------------------------
                                                                     |                              |
                                                          - Hướng cải tiến hệ thống     - Triển khai thực tế
                                                                     |                              |
                                                         + Học máy (ML) tự cải thiện      + Lắp đặt trong xe hơi mới
                                                        + Phân tích âm thanh em bé        + Hệ thống tiêu chuẩn hóa
                                                         + Kết nối hệ thống cứu hộ        + Can thiệp từ xa bằng IoT

```
