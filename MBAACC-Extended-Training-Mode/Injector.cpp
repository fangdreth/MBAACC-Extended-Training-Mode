
#include "Injector.h"

#include "..\Common\Common.h"
#include "Logger.h"
#include <iostream>
//#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>

int inject(unsigned long procID, std::wstring dllPath)
{

	unsigned long pid = procID;
	size_t dllPathLength = (dllPath.length() + 1) * sizeof(wchar_t);

	std::ifstream f(dllPath);

	if (!f.good())
	{
		LogError("Failed to find DLL in melty path");
	}

	HANDLE injectorProcHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, false, pid);
	if (injectorProcHandle == NULL)
	{
		LogError("OpenProcess failed for DLL injection");
		return 1;
	}

	PVOID dllNameAdr = VirtualAllocEx(injectorProcHandle, NULL, dllPathLength, MEM_COMMIT, PAGE_READWRITE);
	if (dllNameAdr == NULL)
	{
		LogError("VirtualAllocEx failed for DLL injection");
		return 1;
	}

	if (WriteProcessMemory(injectorProcHandle, dllNameAdr, dllPath.c_str(), dllPathLength, NULL) == 0)
	{
		LogError("WriteProcessMemory failed for DLL injection");
		return 1;
	}

	//HANDLE tHandle = CreateRemoteThread(injectorProcHandle, 0, 0, (LPTHREAD_START_ROUTINE)(void*)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"), dllNameAdr, 0, 0);
	HANDLE tHandle = CreateRemoteThread(injectorProcHandle, 0, 0, (LPTHREAD_START_ROUTINE)(void*)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"), dllNameAdr, 0, 0);
	if (tHandle == NULL)
	{
		LogError("CreateRemoteThread failed for DLL injection");
		return 1;
	}

	return 0;
}

bool InjectIntoMBAA(unsigned long nPID, std::wstring sDLLPath)
{
	int nReturn;
	try
	{
		nReturn = inject(nPID, sDLLPath);
		if (nReturn)
			LogError("Failure during injection");
	} catch (...)
	{
		LogError("Failure during injection");
	}

	return !nReturn;
}
