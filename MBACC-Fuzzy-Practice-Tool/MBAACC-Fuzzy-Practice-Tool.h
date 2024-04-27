#pragma once
#include <Windows.h>
#include "tlhelp32.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <iostream>

#include "Constants.h"
#include "CharacterData.h"
#include "Menu.h"

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

void ApplyPreset(int nPresetSetting, bool* bSwitchToCrouch, int* nDelayFrames, int* nEnemyStatusSetting, int* nEnemyDefenseSetting, int* nEnemyGuardLevelSetting, bool* bExGuard, int* nReversalPattern, int* nReversalDelayFrames)
{
    switch (nPresetSetting)
    {
    case ePresetSettings::DEFAULT:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::ONEHUNDRED;
        *bExGuard = false;
        *nReversalPattern = -1;
        nReversalDelayFrames = 0;
        break;
    case ePresetSettings::FUZZY:
        *bSwitchToCrouch = true;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = -1;
        nReversalDelayFrames = 0;
        break;
    case ePresetSettings::BLOCKSTRING:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::CROUCH;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = -1;
        nReversalDelayFrames = 0;
        break;
    case ePresetSettings::HEATOS:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = 260;
        *nReversalDelayFrames = 0;
        break;
    case ePresetSettings::FUZZYMASH:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::CROUCH;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = 4;
        *nReversalDelayFrames = 6;
        break;
    case ePresetSettings::FUZZYJUMP:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::CROUCH;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = 36;
        *nReversalDelayFrames = 6;
        break;
    default:
        break;
    }
}

void SetHealth(HANDLE hProcess, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Health), &nValue, 4, 0);
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1RedHealth), &nValue, 4, 0);
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Health + dwP2Offset), &nValue, 4, 0);
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1RedHealth + dwP2Offset), &nValue, 4, 0);
}

void SetMeter(HANDLE hProcess, DWORD dwBaseAddress, int nValue, int nP1Moon, int nP2Moon)
{
    int nWriteBuffer = nP1Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Meter), &nWriteBuffer, 4, 0);

    nWriteBuffer = nP2Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Meter + dwP2Offset), &nWriteBuffer, 4, 0);
}

DWORD GetMainViewScreenStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34d7fc;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x34), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetSubMenuEnumAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x84;

    return dwTempAddress;
}

DWORD GetEnemySettingsMenuTitleStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x60), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetEnemyActionIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetEnemyDefenseIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetEnemyActionInfoStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xDC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x90), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xE58), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x36;

    return dwTempAddress;
}

DWORD GetEnemyActionStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetEnemyActionActionAStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionActionBStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionActionCStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionRecoverCStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x1C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyActionRecoverDStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseOffStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseAllGuardStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStatusGuardStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseAllShieldStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseStatusShieldStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseDodgeStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x24), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeNormalStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeComboStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeRandomStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetEnemyDefenseTypeIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetAirRecoveryIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetAirRecoveryStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetAirRecoveryOffStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryNeutralStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryBackStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryForwardStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryRandom1StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetAirRecoveryRandom2StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

DWORD GetDownRecoveryStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

DWORD GetDownRecoveryOffStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryNeutralStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryBackStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryForwardStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryRandom1StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetDownRecoveryRandom2StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetNoRecoverStringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover25StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover50StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover75StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetRecover100StringAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

DWORD GetLifeIndexAddress(HANDLE hProcess, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = 0x34D7FC;
    int nReadResult = 0;

    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hProcess, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}