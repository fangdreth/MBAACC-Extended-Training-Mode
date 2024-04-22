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

void ApplyPreset(int nPresetSetting, bool* bSwitchToCrouch, int* nDelayFrames, int* nEnemyStatusSetting, int* nEnemyDefenseSetting, int* nEnemyDefenseBiasSetting, int* nEnemyGuardLevelSetting, bool* bExGuard, int* nReversalPattern, int* nReversalDelayFrames)
{
    switch (nPresetSetting)
    {
    case ePresetSettings::DEFAULT:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
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
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
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
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::LIKELY;
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
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
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
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
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
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
        *nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::INF;
        *bExGuard = false;
        *nReversalPattern = 36;
        *nReversalDelayFrames = 6;
        break;
    default:
        break;
    }
}