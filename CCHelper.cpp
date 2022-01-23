// CCHelper.cpp : Defines the entry point for the application.
//
// This program performs several initialization tasks for the Casagrande-Cesi
// Computer Science labs computers. So far it does the following:
//   - Reset Google Chrome's profiles
//   - Add several entries to the Windows start menu
//
// This software is meant to be executed whenever the normal PC users logs
// into Windows: the Install.bat script places it in the Startup folder
// for the current user, whereas the Uninstall.bat scripts removes it
// (including the menu entries).
//
// The program is meant to be executed on Windows >= 8.
//
// by Paolo Bernardi

#include "framework.h"
#include "CCHelper.h"

// Windows header files
#include <shellapi.h>
#include <ShlObj.h>

// C++ standard header files
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <codecvt>

using namespace std;

const wstring CHROME_X86 = L"C:\\Program Files(x86)\\Google\\Chrome\\Application\\chrome.exe";
const wstring CHROME = L"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";

// Thanks to http://freshtechies.blogspot.com/2008/12/vc-recursive-folder-deletion-by.html
bool deleteDir(wstring csDeleteFolderPath_i)
{
	// Making the directory name double null terminated
	int nFolderPathLen = csDeleteFolderPath_i.length();
	TCHAR* pszFrom = new TCHAR[nFolderPathLen + 2];
	wcscpy_s(pszFrom, nFolderPathLen + 2, csDeleteFolderPath_i.c_str());
	pszFrom[nFolderPathLen] = 0;
	pszFrom[++nFolderPathLen] = 0;
	SHFILEOPSTRUCT stSHFileOpStruct = { 0 };
	// Delete operation
	stSHFileOpStruct.wFunc = FO_DELETE;
	// Folder name as double null terminated string
	stSHFileOpStruct.pFrom = pszFrom;
	// Do not prompt the user
	stSHFileOpStruct.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
	// Delete operation can be undo (to recycle bin)
	stSHFileOpStruct.fFlags = FOF_ALLOWUNDO;
	//Check for any operation is aborted
	stSHFileOpStruct.fAnyOperationsAborted = FALSE;
	int nFileDeleteOprnRet = SHFileOperation(&stSHFileOpStruct);
	delete[]pszFrom;
	if (0 != nFileDeleteOprnRet)
	{
		// Failed deletion
		return false;
	}
	// Deletion was successfull
	return true;
}

// Converts a wstring to a string 
string ws2s(const wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}

// Converts a string to a wstring
wstring s2ws(const string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}

// Checks whether the specified file exists
bool fileExists(const wstring name) {
	ifstream f(name);
	return f.good();
}

// Returns the full Google Chrome's exe path or an empty string if not found
wstring getChromeExe() {
	if (fileExists(CHROME_X86)) return CHROME_X86;
	else if (fileExists(CHROME)) return CHROME;
	else return wstring();
}

// Create a new start menu entry:
// - exePath: the full exe file path
// - arguments: the arguments to be passed to the exe
// - shortcutFile: something like Pippo.lnk
// - shortcutDescription: the shortcut title
bool createStartMenuEntry(wstring exePath, wstring arguments, wstring shortcutFile, wstring shortcutDescription) {
	bool res = false;
	PWSTR startMenuPath = NULL;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Programs, 0, NULL, &startMenuPath);
	if (!fileExists(wstring(startMenuPath) + L"\\" + shortcutFile)) {
		wstring linkPath = wstring(startMenuPath) + L"\\" + shortcutFile;
		HRESULT result = CoInitialize(NULL);
		if (SUCCEEDED(result)) {
			IShellLinkW* shellLink = NULL;
			result = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLinkW, (void**)&shellLink);
			if (SUCCEEDED(result)) {
				shellLink->SetPath(exePath.c_str());
				shellLink->SetArguments(arguments.c_str());
				shellLink->SetDescription(shortcutDescription.c_str());
				shellLink->SetIconLocation(exePath.c_str(), 0);
				IPersistFile* persistFile;
				result = shellLink->QueryInterface(IID_IPersistFile, (void**)&persistFile);
				if (SUCCEEDED(result)) {
					result = persistFile->Save(linkPath.c_str(), TRUE);
					persistFile->Release();
					//MessageBox(0, L"OK!", L"Link", MB_OK);
					res = true;
				}
				shellLink->Release();
			}
		}
	}
	CoTaskMemFree(startMenuPath);
	return res;
}

// Reset Google Chrome's configuration
void resetChrome() {
	// Delete existing Google Chrome profiles, if any
	wchar_t *localAppDataWC = new wchar_t[512];
	size_t numOfElems;
	_wdupenv_s(&localAppDataWC, &numOfElems, L"LOCALAPPDATA");
	if (!localAppDataWC) {
		return;
	}
	wstring localAppData(localAppDataWC);
	wstring userData = localAppData + L"\\Google\\Chrome\\User Data";
	deleteDir(userData);
	// Recreate Google Chrome default profiles by starting it with --no-startup-window,
	// NOT REALLY NEEDED, I ACTUALLY JUST WANTED TO TRY AND CHANGE THE PREFERENCES
	// BUT SECURE PREFERENCES (AND THE SHORT TIME AT HAND) GOT IN MY WAY
	//wstring chromeExe = getChromeExe();
	//if (!chromeExe.empty()) {
	//	wstring chromeCmd = chromeExe;
	//	ShellExecuteW(0, 0, (LPWSTR)chromeExe.c_str(), L"--no-startup-window", 0, SW_SHOW);
	//}
}

// Create the desired startup menu entries
void createEntries() {
	wstring chromeExe = getChromeExe();
	createStartMenuEntry(chromeExe, L"https://www.casagrandecesi.edu.it/", L"Casagrande-Cesi.lnk", L"IIS \"Casagrande-Cesi\"");
	createStartMenuEntry(chromeExe, L"https://replit.com/languages/python3/", L"Python3 REPL.lnk", L"Python3 REPL");
	createStartMenuEntry(chromeExe, L"https://py3.codeskulptor.org/", L"CodeSkulptor.lnk", L"CodeSkulptor");
}

// Main entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	resetChrome();
	createEntries();
}