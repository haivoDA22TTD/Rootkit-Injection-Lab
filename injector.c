#include <windows.h>
#include <stdio.h>

int main() {
    // 1. Nhập thông tin từ bàn phím cho đơn giản
    DWORD pid;
    char dllPath[256];

    printf("Nhap PID cua tien trinh muc tieu (VD: Notepad): ");
    scanf("%lu", &pid);
    
    printf("Nhap duong dan day du cua file DLL: ");
    scanf("%s", dllPath);

    // 2. Mở tiến trình mục tiêu
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("[-] Khong the mo tien trinh. Loi: %lu\n", GetLastError());
        system("pause");
        return 1;
    }

    // 3. Cấp phát bộ nhớ trong tiến trình đó
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (remoteMem == NULL) {
        printf("[-] Cap phat bo nho that bai!\n");
        CloseHandle(hProcess);
        system("pause");
        return 1;
    }

    // 4. Ghi đường dẫn DLL vào bộ nhớ vừa cấp phát
    SIZE_T bytesWritten;
    WriteProcessMemory(hProcess, remoteMem, dllPath, strlen(dllPath) + 1, &bytesWritten);

    // 5. Lấy địa chỉ hàm LoadLibraryA
    PTHREAD_START_ROUTINE pLoadLibrary = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    // 6. Tạo Thread từ xa để ép tiến trình nạp DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibrary, remoteMem, 0, NULL);

    if (hThread != NULL) {
        printf("[+] Inject DLL thanh cong rui nhe!\n");
        CloseHandle(hThread);
    } else {
        printf("[-] Tao thread that bai. Loi: %lu\n", GetLastError());
    }

    CloseHandle(hProcess);
    system("pause");
    return 0;
}
