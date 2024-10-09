#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#pragma comment(lib, "urlmon.lib")

int main(int argc, const char* argv[])
{
	std::string sAppName = argv[0];
	std::string sInstallPath = argv[1];
	std::string sGithubDownloadPath = argv[2];
    std::string sEXEName = argv[3];

    MessageBoxA(NULL, "", "attach here", NULL);

	HRESULT hr = S_OK;
	for (int i = 3; i < argc; i++)
	{
		std::string sDownloadURL = sGithubDownloadPath + argv[i];
		std::string sSavePath = sInstallPath + argv[i];
		hr = URLDownloadToFileA(NULL, sDownloadURL.c_str(), sSavePath.c_str(), 0, NULL);
	}
	if (SUCCEEDED(hr))
	{
        // additional information
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        // set the size of the structures
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string sCmdArgs = sEXEName;

        // start the program up
        CreateProcessA((sInstallPath + sEXEName).c_str(),   // the path
            (LPSTR)sCmdArgs.c_str(),        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
        // Close process and thread handles. 
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
	}
	else
		MessageBoxA(NULL, "", "There was a failure during the update.", MB_ICONERROR);
}