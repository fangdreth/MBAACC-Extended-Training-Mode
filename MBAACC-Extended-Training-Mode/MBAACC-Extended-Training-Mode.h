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
#include <format>
#include <regex>

#include "json.hpp"

#include "..\Common\Common.h"
#include "..\Common\CharacterData.h"
#include "PointerManager.h"
#include "FrameDisplay.h"
#include "Injector.h"
#include "Logger.h"

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

LONG ReadFromRegistry(std::wstring sKey, uint8_t* nValue)
{
    LONG openResult = -1;

    try
    {
        DWORD dwValue = NULL;
        HKEY hKey;
        LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
        DWORD dwType = REG_DWORD;
        DWORD dwSize = sizeof(nValue);

        openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
        if (openResult == 0)
            openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
        if (openResult == 0)
            *nValue = (int)dwValue;

        RegCloseKey(hKey);
    }
    catch (...)
    {
    }

    return openResult;
}

LONG SetRegistryValue(std::wstring sKey, int nValue)
{
    LONG openResult = -1;

    try
    {
        HKEY hKey;
        LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode\\";

        openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_WRITE, &hKey);
        if (openResult == 0)
            openResult = RegSetValueEx(hKey, sKey.c_str(), 0, REG_DWORD, (unsigned char*)&nValue, sizeof(nValue));

        RegCloseKey(hKey);
    }
    catch (...)
    {
    }
    return openResult;
}

LONG SetRegistryValue(std::wstring sKey, bool bValue)
{
    return SetRegistryValue(sKey, bValue ? 1 : 0);
}

LONG ReadFromRegistry(std::wstring sKey, bool* bValue)
{
    uint8_t nValue = 0;
    LONG openResult = ReadFromRegistry(sKey, &nValue);
    if (openResult == 0)
        *bValue = nValue > 0 ? true : false;
    return openResult;
}

void SetHealth(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Health), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RedHealth), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Health), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RedHealth), &nValue, 4, 0);
}

void SetMeter(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue, int nP1Moon, int nP2Moon)
{
    int nWriteBuffer = nP1Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Meter), &nWriteBuffer, 4, 0);

    nWriteBuffer = nP2Moon == 2 ? min(nValue, 20000) : nValue;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Meter), &nWriteBuffer, 4, 0);
}

void SetGuard(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nLevel, int nP1Moon, int nP2Moon)
{
    int nWriteBuffer = vGuardLevelLookupTable[nP1Moon * 5 + nLevel];
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1GuardAmount), &nWriteBuffer, 4, 0);
    
    nWriteBuffer = vGuardLevelLookupTable[nP2Moon * 5 + nLevel];
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2GuardAmount), &nWriteBuffer, 4, 0);
}

void SetSionBullets(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    int nWriteBuffer = 13 - min(nValue, MAX_BULLETS);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1SionBullets), &nWriteBuffer, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2SionBullets), &nWriteBuffer, 4, 0);
}

void SetRoaVisibleCharge(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nVisible)
{
    nVisible = nVisible == -2 ? 9 : max(nVisible, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge), &nVisible, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RoaVisibleCharge), &nVisible, 4, 0);
}

void SetRoaHiddenCharge(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nHidden)
{
    // for some reason it matters the order that hidden is first
    nHidden = nHidden == -2 ? 9 : max(nHidden, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge), &nHidden, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RoaHiddenCharge), &nHidden, 4, 0);
}

void SetFMaidsHearts(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nHearts)
{
    if (nHearts == -1)
        nHearts = 0;
    else
        nHearts = 5 - min(nHearts, MAX_HEARTS);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1FMaidsHearts), &nHearts, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2FMaidsHearts), &nHearts, 4, 0);
}

void SetRyougiKnife(HANDLE hMBAAHandle, DWORD dwBaseAddress, bool bInfinite)
{
    int nTemp = bInfinite ? 0 : 1;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RyougiKnife), &nTemp, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RyougiKnife), &nTemp, 4, 0);
}

void SetP1X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nValue, 4, 0);
}

void SetP2X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2X), &nValue, 4, 0);
}

void SetP3X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP3X), &nValue, 4, 0);
}

void SetP4X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4X), &nValue, 4, 0);
}

uint8_t arrKeysHeld[256] = { 0 };
void ResetKeysHeld()
{
    std::fill(std::begin(arrKeysHeld), std::end(arrKeysHeld), 1);
}

uint8_t KeyJustPressed()
{
    //http://www.kbdedit.com/manual/low_level_vk_list.html

    // there're some weird collisions between keys
    // like numpad-/ and the arrow keys,
    // but all of the normal keys work as expected.
    // I'm fine with it, personally.
    uint8_t i = 0;
    do
    {
        // key pressed AND key isn't garbage
        if (GetAsyncKeyState(i) & 0x8000 && MapVirtualKeyW(i, MAPVK_VK_TO_VSC) != 0)
        {
            if (arrKeysHeld[i] == 0)
                return i;
        }
        else
            arrKeysHeld[i] = 0;
        i++;
    } while (i);

    return 0;
}

std::array<uint8_t, 4> CreateColorArray2(int nHighlightID)
{
    switch (nHighlightID)
    {
    default:
    case NO_HIGHLIGHT:
        return { 255, 255, 255, 0 };
    case RED_HIGHLIGHT:
        return { 255, 90, 90, 1 };
    case YELLOW_HIGHLIGHT:
        return { 255, 255, 0, 1 };
    case GREEN_HIGHLIGHT:
        return { 60, 255, 60, 1 };
    case BLUE_HIGHLIGHT:
        return { 90, 90, 255, 1 };
    case PURPLE_HIGHLIGHT:
        return { 255, 90, 255, 1 };
    case BLACK_HIGHLIGHT:
        return { 60, 60, 60, 1 };
    }
}

#define ID_EDIT 1
#define ID_BUTTON 2
HWND hEdit;
uint32_t nDialogOutput;
bool bSubmitPressed = false;    // <- this is such a dumb way to do it but I'm sick of winapi bs
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndButton;

    switch (msg)
    {
    case WM_CREATE:
    {
        // Create an Edit box
        hEdit = CreateWindow(TEXT("EDIT"), TEXT(""),
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            10, 10, 150, 20, hwnd, (HMENU)ID_EDIT, NULL, NULL);

        EnableWindow(hEdit, TRUE);
        SetFocus(hEdit);
        SendMessage(hEdit, EM_SETREADONLY, FALSE, 0);

        // Create a button
        hwndButton = CreateWindowW(TEXT("BUTTON"), TEXT("Submit"),
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            10, 35, 80, 25, hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        break;
    }

    case WM_COMMAND:
    {
        if (LOWORD(wParam) == ID_BUTTON)
        {
            bool bGoodInput = true;
            wchar_t buffer[256];
            
            GetWindowTextW(hEdit, buffer, 256);
            std::wstring wsBuffer = std::wstring(buffer);

            if (wsBuffer.substr(0, 2) == L"0x")
                wsBuffer.erase(0, 2);

            if (wsBuffer.length() > 8)
            {
                bGoodInput = false;
                MessageBox(NULL, L"Number is too large.  Value should be between 0 and FFFFFFFF.", L"", NULL);
                break;
            }
            
            std::wregex hexRegex(L"[0-9A-Fa-f]+$");
            if (!std::regex_match(wsBuffer, hexRegex))
            {
                MessageBox(NULL, L"Number is invalid.  Value should be a hex number containing only 0-9 and A-F", L"", NULL);
                bGoodInput = false;
                break;
            }

            nDialogOutput = std::stoi(wsBuffer, nullptr, 16);
            bSubmitPressed = true;
            PostMessage(hwnd, WM_CLOSE, 0, 0);
        }
        
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

uint32_t PromptForNumber(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    const uint8_t nOne = 1;
    const uint8_t nZero = 0;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nOne, 1, 0);

    bSubmitPressed = false;
    WNDCLASS wc = { 0 };
    wc.lpszClassName = L"Edit Control";
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = TEXT("MyWindowClass");
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowExW(0, TEXT("MyWindowClass"), TEXT("erm"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 200, 100,
        NULL, NULL, GetModuleHandleW(NULL), NULL);
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nZero, 1, 0);

    if (bSubmitPressed)
        return nDialogOutput;
    else
        return -1;
}