@echo off
echo ------------------------------------------
echo           Uninstalling CCHelper...
echo ------------------------------------------
rd /s /q "%LOCALAPPDATA%\CCHelper"
del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\CCHelper.lnk"
del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Casagrande-Cesi.lnk"
del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Python3 REPL.lnk"
del "%APPDATA%\Microsoft\Windows\Start Menu\Programs\CodeSkulptor.lnk"