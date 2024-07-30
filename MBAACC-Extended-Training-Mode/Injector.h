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
