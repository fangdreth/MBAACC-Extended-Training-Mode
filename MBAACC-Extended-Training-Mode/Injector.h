#pragma once
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>


#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define CYAN "\x1b[36;1m"
#define WHITE "\x1b[37;1m"

#define RESET "\x1b[0m"

static void initConsole() {

#ifdef _WIN32

#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

	DWORD outMode = 0;
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (stdoutHandle == INVALID_HANDLE_VALUE) {
		exit(GetLastError());
	}

	if (!GetConsoleMode(stdoutHandle, &outMode)) {
		exit(GetLastError());
	}

	outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	if (!SetConsoleMode(stdoutHandle, outMode)) {
		exit(GetLastError());
	}

#endif
}

static void exitWithCode(int code = 0, std::string msg = std::string("Press enter to exit")) {
	std::cout << (code ? RED : WHITE);
	std::cout << msg << RESET << "\n";

	if (msg != "Press enter to exit") {
		std::cout << WHITE << "Press enter to exit\n" << RESET;
	}
	getchar();
	exit(code);
}

static std::pair<int, std::string> getPID(std::string& processName) {

	std::vector<int> res;

	HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
	if (hndl)
	{

		PROCESSENTRY32 process;
		process.dwSize = sizeof(PROCESSENTRY32);

		Process32First(hndl, &process);

		do
		{
			if (process.szExeFile == std::wstring(processName.begin(), processName.end()))
			{
				res.push_back(process.th32ProcessID);
			}
		} while (Process32Next(hndl, &process));

		CloseHandle(hndl);
	}

	if (res.size() == 0) {
		std::cerr << RED << "Unable to find " << processName << ", is it running?\n" << RESET;
		exitWithCode(1);
	}

	std::string processPath;

	for (auto it = res.begin(); it != res.end();) {

		HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, *it);

		char filename[256];

		if (!GetModuleFileNameExA(processHandle, NULL, filename, 256)) {
			std::cerr << "Error getting module filename for process\n";
			exitWithCode(1);
		}

		processPath = std::string(filename);
		break;
	}

	std::pair<int, std::string> result(res[0], processPath);

	std::cerr << GREEN << "Found " << processName << " at " << processPath << "\n" << RESET;


	return result;
}

static std::string getPathOnly(const std::string& s) {

	char buffer[256];

	memset(buffer, '\0', 256);

	const char* temp = s.c_str();

	memcpy(buffer, s.c_str(), std::strrchr(temp, '\\') + 1 - temp);

	return std::string(buffer);
}

static bool fileExists(const std::string& s) {
	std::ifstream f(s.c_str());
	return f.good();
}

static bool inject(unsigned long procID, const char* dllPath) {

	unsigned long pid = procID;
	const char* path = dllPath;

	std::ifstream f(dllPath);

	if(!f.good())
	{
		printf("%sFailed to find DLL in melty path%s\n", RED, RESET);
		printf("%s%s%s\n", RED, dllPath, RESET);
		return false;
	}

	HANDLE injectorProcHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, false, pid);
	if (injectorProcHandle == NULL)
	{
		printf("%sOpenProcess failed for DLL injection%s\n", RED, RESET);
		return false;
	}

	PVOID dllNameAdr = VirtualAllocEx(injectorProcHandle, NULL, strlen(path) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (dllNameAdr == NULL) 
	{
		printf("%sVirtualAllocEx failed for DLL injection%s\n", RED, RESET);
		return false;
	}

	if (WriteProcessMemory(injectorProcHandle, dllNameAdr, path, strlen(path) + 1, NULL) == 0) 
	{
		printf("%sWriteProcessMemory failed for DLL injection%s\n", RED, RESET);
		return false;
	}

	HANDLE tHandle = CreateRemoteThread(injectorProcHandle, 0, 0, (LPTHREAD_START_ROUTINE)(void*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), dllNameAdr, 0, 0);
	if (tHandle == NULL) 
	{
		printf("%sCreateRemoteThread failed for DLL injection%s\n", RED, RESET);
		return false;
	}

	return true;
}

static int WH_Inject(unsigned long nPID, std::string sDLLPath) {

	bool result = inject(nPID, sDLLPath.c_str());

	if (!result) {
		std::cout << std::endl;
		return 1;
	}

	printf("%sinjection ran successfully%s\n", GREEN, RESET);
	std::cout << std::endl;
	return 0;
}
