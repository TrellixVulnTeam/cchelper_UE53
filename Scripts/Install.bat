@echo off
echo ------------------------------------------
echo            Installing CCHelper...
echo ------------------------------------------

echo *** INSTALLING CCHELPER ON THE LOCAL DISK ***
mkdir "%LOCALAPPDATA%\CCHelper"
copy Release\* "%LOCALAPPDATA%\CCHelper"

echo *** AUTOMATIC STARTUP SHORTCUT CREATION ***
set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"
echo Set oWS = WScript.CreateObject("WScript.Shell") >> %SCRIPT%
echo sLinkFile = "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\CCHelper.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "%LOCALAPPDATA%\CCHelper\CCHelper.exe" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%
cscript /nologo %SCRIPT%
del %SCRIPT%

echo *** PYTHON INSTALLATION ***
set PYTHON_DIR="%USERPROFILE%\Desktop\python"
echo %PYTHON_DIR%
if exist %PYTHON_DIR% (
   rd /s /q %PYTHON_DIR%
)
robocopy python %PYTHON_DIR% /S /NFL /NDL