# 🔬 Windows User-Mode Injection & Kernel-Mode Rootkit Lab

Dự án này được xây dựng nhằm mục đích **nghiên cứu học thuật, giáo dục và thử nghiệm an toàn thông tin** (Cybersecurity Research). Dự án tập trung vào việc mô phỏng lại các kỹ thuật tấn công găm mã (DLL Injection) ở mức User-mode và cơ chế ẩn mình cơ bản của mã độc nhằm phục vụ cho việc phát triển các giải pháp phòng thủ, nhận diện phần mềm độc hại.

---

## ⚠️ TUYÊN BỐ TỪ CHỐI TRÁCH NHIỆM (DISCLAIMER)

**QUAN TRỌNG:** Mã nguồn trong kho lưu trữ này chỉ được cung cấp cho mục đích giáo dục và nghiên cứu học tập phi thương mại. 

* **Mục đích duy nhất:** Giúp các kỹ sư bảo mật và sinh viên hiểu rõ cách thức hoạt động của mã độc mức thấp để xây dựng hệ thống phòng thủ tốt hơn.
* **Từ chối trách nhiệm:** Tác giả dự án **KHÔNG** chịu bất kỳ trách nhiệm nào đối với những thiệt hại, tổn thất hoặc hậu quả pháp lý phát sinh nếu bất kỳ cá nhân hay tổ chức nào sử dụng mã nguồn này vào mục đích xấu, phá hoại hoặc bất hợp pháp.
* **Khuyến cáo:** Mọi hành vi thử nghiệm phải được diễn ra trong môi trường Lab cô lập (máy ảo VirtualBox/VMware), không được tự ý thực thi trên hệ thống thật hoặc máy tính của người khác khi chưa được cấp phép.

---

## 🛠️ Cấu Trúc Dự Án

* `injector.c`: Mã nguồn chương trình găm mã (User-mode Injector) viết bằng ngôn ngữ C.
* `MyPayload.c`: Mã nguồn file DLL (chứa mảng Shellcode mẫu kết nối ngầm Reverse Shell).
* `mydriver.c`: Mã nguồn Driver thử nghiệm chạy ở tầng nhân (Kernel-mode Ring 0).

## 🚀 Kịch Bản Thử Nghiệm (Lab Setup)

1. **Môi trường máy tấn công:** Kali Linux (Sử dụng Metasploit Framework `multi/handler` để hứng kết nối).
2. **Môi trường nạn nhân:** Máy ảo Windows 10 Pro (Đã bật chế độ `testsigning on` để nạp thử nghiệm driver).
3. **Trình biên dịch:** Dev-C++ (TDM-GCC 64-bit trên Windows) hoặc MinGW-w64 (Biên dịch chéo trên Kali Linux).
**Dự án đang trong giai đoạn thử nghiệm**
## 📜 Giấy Phép (License)

Dự án này được phát hành dưới bản quyền **MIT License** - Xem chi tiết tại file `LICENSE`.
