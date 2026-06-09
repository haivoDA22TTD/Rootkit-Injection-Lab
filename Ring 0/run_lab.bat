@echo off
:: Di chuyển quyền thực thi về thư mục hiện tại
cd /d "%~dp0"

echo ===================================================
echo       TIEN TRINH TU DONG NAP GHOST ROOTKIT
echo ===================================================

echo [*] Dang dung va xoa Driver cu (neu co)...
sc stop MyKernelRootkit >nul 2>&1
sc delete MyKernelRootkit >nul 2>&1

echo [*] Dang dang ky Driver moi vao He thong...
:: Lệnh này lấy file kernel_rootkit.sys nằm chung thư mục với file .bat này
sc create MyKernelRootkit type= kernel binPath= "%~dp0kernel_rootkit.sys"

echo [*] Dang kich hoat Driver vao Kernel Ring 0...
sc start MyKernelRootkit

echo ===================================================
echo Hoan thanh! Hay kiem tra Log trong DebugView.
pause
