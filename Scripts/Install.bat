@echo off
echo ------------------------------------------
echo            Installing CCHelper...
echo ------------------------------------------
copy Release\CCHelper.exe "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup"
set PYTHON_DIR="%USERPROFILE%\Desktop\python"
echo %PYTHON_DIR%
if exist %PYTHON_DIR% (
   rd /s /q %PYTHON_DIR%
)
robocopy python %PYTHON_DIR% /S /NFL /NDL