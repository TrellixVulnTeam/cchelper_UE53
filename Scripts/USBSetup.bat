@echo off
wmic LOGICALDISK where driveType=2 get deviceID > wmic.txt
for /f "skip=1" %%b in ('type wmic.txt') do (set usbDrive=%%b)
del wmic.txt
if "%usbDrive%" == "" (
    echo USB drive not found
) else (
    echo Setting up USB drive %usbDrive%...
    copy Install.bat %usbDrive%
    copy Uninstall.bat %usbDrive%
    mkdir %usbDrive%\Release
    copy ..\x64\Release\CCHelper.exe %usbDrive%\Release
    copy *.dll %usbDrive%\Release
    rd /s /q %usbDrive%\python
    robocopy python %usbDrive%\python /S /NFL /NDL
    echo That's all, folks!
)