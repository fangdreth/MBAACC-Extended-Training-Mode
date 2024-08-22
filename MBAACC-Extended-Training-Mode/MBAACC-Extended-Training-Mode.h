#pragma once
#include <Windows.h>
#include "tlhelp32.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <iostream>
#include <chrono>
#include <set>
#include <unordered_set>
#include <conio.h>
#include <WinUser.h>
#include <strsafe.h>
#include <algorithm>

#include "json.hpp"

#include "..\Common\Common.h"
#include "..\Common\CharacterData.h"
#include "..\Common\SharedMemoryHelper.h"
#include "PointerManager.h"
#include "FrameDisplay.h"

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
        sVersion = json["tag_name"];
    }
    catch (...)
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

DWORD GetProcessPID(const wchar_t* name)
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
        return pid;
    }

    return 0;
}

std::string GetProcessPath(HANDLE hMBAAHandle)
{
    const int nPathSize = 256;
    DWORD dwPathSize = nPathSize;
    char path[nPathSize];
   
    if (QueryFullProcessImageNameA(hMBAAHandle, 0, path, &dwPathSize)) 
        {
        std::string sTempPathString = std::string(path);
        size_t lastBackslash = sTempPathString.find_last_of("\\/");
        if (lastBackslash != std::string::npos) 
        {
            return sTempPathString.substr(0, lastBackslash) + "\\";
        }
        return "";
    }
    
    return "";
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

void CreateRegistryKey()
{
    try
    {
        SECURITY_DESCRIPTOR SD;
        SECURITY_ATTRIBUTES SA;
        InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
        //SetSecurityDescriptorDacl(&SD, true, 0, false);
        SA.nLength = sizeof(SA);
        SA.lpSecurityDescriptor = &SD;
        SA.bInheritHandle = false;

        DWORD dwFunc;
        HKEY hKey;
        LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
        LONG openResult = RegCreateKeyExW(HKEY_CURRENT_USER, sk, 0, (LPTSTR)NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &SA, &hKey, &dwFunc);
        RegCloseKey(hKey);
    }
    catch (...)
    {
    }
}

void ReadFromRegistry(std::wstring sKey, int* nValue)
{
    try
    {
        DWORD dwValue = NULL;
        HKEY hKey;
        LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
        DWORD dwType = REG_DWORD;
        DWORD dwSize = sizeof(nValue);

        LONG openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
        openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        if (openResult == 0)
            *nValue = (int)dwValue;

        RegCloseKey(hKey);
    }
    catch (...)
    {
    }
}

void SetRegistryValue(std::wstring sKey, int nValue)
{
    try
    {
        HKEY hKey;
        LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode\\";

        LONG openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_WRITE, &hKey);
        openResult = RegSetValueEx(hKey, sKey.c_str(), 0, REG_DWORD, (unsigned char*)&nValue, sizeof(nValue));
        
        RegCloseKey(hKey);
    }
    catch (...)
    {
    }
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
    int nWriteBuffer = 13 - min(nValue, MAX_BULLETS);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1SionBullets), &nWriteBuffer, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1SionBullets + dwP2Offset), &nWriteBuffer, 4, 0);
}

void SetRoaVisibleCharge(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nVisible)
{
    nVisible = nVisible == -2 ? 9 : max(nVisible, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge), &nVisible, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge + dwP2Offset), &nVisible, 4, 0);
}

void SetRoaHiddenCharge(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nHidden)
{
    // for some reason it matters the order that hidden is first
    nHidden = nHidden == -2 ? 9 : max(nHidden, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge), &nHidden, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge + dwP2Offset), &nHidden, 4, 0);
}

void SetFMaidsHearts(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nHearts)
{
    if (nHearts == -1)
        nHearts = 0;
    else
        nHearts = 5 - min(nHearts, MAX_HEARTS);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1FMaidsHearts), &nHearts, 4, 0);
}

void SetRyougiKnife(HANDLE hMBAAHandle, DWORD dwBaseAddress, bool bInfinite)
{
    int nTemp = bInfinite ? 0 : 1;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RyougiKnife), &nTemp, 4, 0);
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

BYTE arrKeysHeld[256] = { 0 };
void ResetKeysHeld()
{
    std::fill(std::begin(arrKeysHeld), std::end(arrKeysHeld), 1);
}

int KeyJustPressed()
{
    //http://www.kbdedit.com/manual/low_level_vk_list.html

    // there're some weird collisions between keys
    // like numpad-/ and the arrow keys,
    // but all of the normal keys work as expected.
    // I'm fine with it, personally.
    for (int i = 0x00; i <= 0xFF; i++)
    {
        if (GetAsyncKeyState(i) & 0x8000)
        {
            if (arrKeysHeld[i] == 0)
                return i;
        }
        else
            arrKeysHeld[i] = 0;
    }

    return 0;
}