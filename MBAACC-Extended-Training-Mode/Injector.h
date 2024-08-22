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

#include "Logger.h"

#define RED "\x1b[31;1m"
#define GREEN "\x1b[32;1m"
#define CYAN "\x1b[36;1m"
#define WHITE "\x1b[37;1m"

#define RESET "\x1b[0m"

static int inject(unsigned long procID, char* dllPath) 
{

	unsigned long pid = procID;
	const char* path = dllPath;

	std::ifstream f(dllPath);

	if(!f.good())
	{
		LogInfo("Failed to find DLL in melty path");
	}

	HANDLE injectorProcHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, false, pid);
	if (injectorProcHandle == NULL)
	{
		LogInfo("OpenProcess failed for DLL injection");
		return 1;
	}

	PVOID dllNameAdr = VirtualAllocEx(injectorProcHandle, NULL, strlen(path) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (dllNameAdr == NULL)
	{
		LogInfo("VirtualAllocEx failed for DLL injection");
		return 1;
	}

	if (WriteProcessMemory(injectorProcHandle, dllNameAdr, path, strlen(path) + 1, NULL) == 0)
	{
		LogInfo("WriteProcessMemory failed for DLL injection");
		return 1;
	}

	HANDLE tHandle = CreateRemoteThread(injectorProcHandle, 0, 0, (LPTHREAD_START_ROUTINE)(void*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), dllNameAdr, 0, 0);
	if (tHandle == NULL)
	{
		LogInfo("CreateRemoteThread failed for DLL injection");
		return 1;
	}
}

static void InjectIntoMBAA(unsigned long nPID, std::string sDLLPath) 
{
	try
	{
		if (inject(nPID, sDLLPath.data()))
			LogInfo("Failure during injection");
	}
	catch (...)
	{
		LogInfo("Failure during injection");
	}
}
