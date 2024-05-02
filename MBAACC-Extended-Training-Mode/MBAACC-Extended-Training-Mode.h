#pragma once
#include <Windows.h>
#include "tlhelp32.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <iostream>
#include <chrono>
#include "json.hpp"

#include "Constants.h"
#include "CharacterData.h"
#include "PointerManager.h"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string GetLatestVersion()
{
    char pcCommand[1024];
    strcpy_s(pcCommand, ("curl -s " + GITHUB_LATEST).c_str());
    std::string sOutputJSONBuffer = exec(pcCommand);
    auto json = nlohmann::json::parse(sOutputJSONBuffer);
    std::string sVersion = "";
    try
    {
        sVersion = json["name"];
    }
    catch(...)
    { 
    }
    
    return sVersion;
}

DWORD GetBaseAddressByName(HANDLE pHandle, const wchar_t* name)
{
    HMODULE hMods[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(pHandle, hMods, sizeof(hMods), &cbNeeded))
    {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(pHandle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                std::wstring wstrModName = szModName;
                std::wstring wstrModContain = L"MBAA.exe";
                if (wstrModName.find(wstrModContain) != std::string::npos)
                {
                    return (DWORD)hMods[i];
                }
            }
        }
    }

    return 0;
}

HANDLE GetProcessByName(const wchar_t* name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (std::wcscmp((process.szExeFile), name) == 0)
            {
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
        return OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    }

    return nullptr;
}

void SetHealth(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Health), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RedHealth), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Health + dwP2Offset), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RedHealth + dwP2Offset), &nValue, 4, 0);
}

void SetMeter(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue, int nP1Moon, int nP2Moon)
{
    int nWriteBuffer = nP1Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Meter), &nWriteBuffer, 4, 0);

    nWriteBuffer = nP2Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Meter + dwP2Offset), &nWriteBuffer, 4, 0);
}

void SetGuard(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nLevel, int nP1Moon, int nP2Moon)
{
    int nWriteBuffer = vGuardLevelLookupTable[nP1Moon * 5 + nLevel];
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1GuardAmount), &nWriteBuffer, 4, 0);
    
    nWriteBuffer = vGuardLevelLookupTable[nP2Moon * 5 + nLevel];
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1GuardAmount + dwP2Offset), &nWriteBuffer, 4, 0);
}

void SetSionBullets(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    int nWriteBuffer = 13 - nValue;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1SionBullets), &nWriteBuffer, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1SionBullets + dwP2Offset), &nWriteBuffer, 4, 0);
}

void SetP1X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nValue, 4, 0);
}

void SetP2X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset), &nValue, 4, 0);
}

void SetP3X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 2), &nValue, 4, 0);
}

void SetP4X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 3), &nValue, 4, 0);
}

void SetRoaCharge(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nVisible, int nHidden)
{
    // for some reason it matters the order that hidden/visible is set

    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge), &nHidden, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge + dwP2Offset), &nHidden, 4, 0);

    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge), &nVisible, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge + dwP2Offset), &nVisible, 4, 0);
}

/*DWORD GetViewScreenStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34d7fc;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x34), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetSubMenuEnumAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x84;

    return dwTempAddress;
}

DWORD GetEnemySettingsMenuTitleStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x60), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetEnemyActionIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetEnemyActionOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetEnemyDefenseIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetEnemyDefenseOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetEnemyActionInfoStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xDC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x90), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xE58), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x36;

    return dwTempAddress;
}

DWORD GetEnemyActionStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetEnemyActionOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionActionAStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionActionBStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionActionCStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionRecoverCStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x1C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionRecoverDStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseAllGuardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStatusGuardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseAllShieldStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStatusShieldStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseDodgeStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x24), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeComboStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetAirRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetAirRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetAirRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetAirRecoveryOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryNeutralStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryBackStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryForwardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryRandom1StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryRandom2StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetDownRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetDownRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetDownRecoveryOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryNeutralStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryBackStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryForwardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryRandom1StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryRandom2StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetNoRecoverStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover25StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover50StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover75StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover100StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetLifeIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetLifeOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetTrainingMenuStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x60;

    return dwTempAddress;
}

DWORD GetThrowRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetThrowRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetThrowRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetThrowRecoveryNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetThrowRecoveryAllFastStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetThrowRecoveryAllLateStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetThrowRecoveryAllRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetThrowRecoveryRandomFastStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetThrowRecoveryRandomLateStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetThrowRecoveryRandomRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetReduceDamageStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetReduceDamageIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetReduceDamageOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

DWORD GetReduceDamageNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetReduceDamageAllStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetReduceDamageRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDefaultStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x30), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetEnemySettingsCursorAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x40;

    return dwTempAddress;
}

DWORD GetEnemyStatusAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetMagicCircuitAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetReturnToMainMenuStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x40), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x24), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}*/