#pragma once
#include "..\Common\Common.h"
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
#include <thread>
#include <timeapi.h>
#include <sstream>
#include <iostream>
#include "resource.h"
#pragma comment(lib, "winmm.lib")

#include "json.hpp"

#include "..\Common\CharacterData.h"
#include "PointerManager.h"
#include "FrameDisplay.h"
#include "Injector.h"
#include "Logger.h"

HANDLE hMBAAHandle = 0x0;
DWORD dwBaseAddress = 0;

uint8_t nFreezeKey = nDefaultFreezeKey;
uint8_t nFrameStepKey = nDefaultFrameStepKey;
uint8_t nHitboxDisplayKey = nDefaultHitboxDisplayKey;
uint8_t nFrameDataDisplayKey = nDefaultFrameDataDisplayKey;
uint8_t nHighlightsOnKey = nDefaultHighlightsOnKey;
uint8_t nSaveStateKey = nDefaultSaveStateKey;
uint8_t nPrevSaveSlotKey = nDefaultPrevSaveSlotKey;
uint8_t nNextSaveSlotKey = nDefaultNextSaveSlotKey;
uint8_t nFrameBarScrollLeftKey = nDefaultFrameBarScrollLeftKey;
uint8_t nFrameBarScrollRightKey = nDefaultFrameBarScrollRightKey;
uint8_t nRNGIncKey = nDefaultRNGIncKey;
uint8_t nRNGDecKey = nDefaultRNGIncKey;
uint8_t nReversalKey = nDefaultReversalKey;
uint8_t nSlowKey = nDefaultSlowKey;
uint8_t nNextFrameKey = nDefaultNextFrameKey;
uint8_t nPrevFrameKey = nDefaultPrevFrameKey;
uint8_t nResetKey = nDefaultResetKey;

bool bFreezeKeySet = false;
bool bFrameStepKeySet = false;
bool bHitboxDisplayKeySet = false;
bool bFrameDataDisplayKeySet = false;
bool bHighlightsOnKeySet = false;
bool bSaveStateKeySet = false;
bool bPrevSaveSlotKeySet = false;
bool bNextSaveSlotKeySet = false;
bool bFrameBarScrollLeftKeySet = false;
bool bFrameBarScrollRightKeySet = false;
bool bRNGIncKeySet = false;
bool bRNGDecKeySet = false;
bool bReversalKeySet = false;
bool bSlowKeySet = false;
bool bNextFrameKeySet = false;
bool bPrevFrameKeySet = false;
bool bResetKeySet = false;

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

void WriteCharacterMemory(HANDLE hMBAAHandle, DWORD address, void* pData, uint8_t nSize, uint8_t nCharacter)
{
    WriteProcessMemory(hMBAAHandle, (LPVOID)(address + nCharacter * 0xAFC), pData, nSize, 0);
}

void ReadCharacterMemory(HANDLE hMBAAHandle, DWORD address, void* pData, uint8_t nSize, uint8_t nCharacter)
{
    ReadProcessMemory(hMBAAHandle, (LPVOID)(address + nCharacter * 0xAFC), pData, nSize, 0);
}

void SetHealth(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue, uint8_t nP1Controlled, uint8_t nP2Controlled)
{
    /*WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Health), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RedHealth), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Health), &nValue, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RedHealth), &nValue, 4, 0);*/
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1Health, &nValue, 4, nP1Controlled);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1Health, &nValue, 4, nP2Controlled);

    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1RedHealth, &nValue, 4, nP1Controlled);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1RedHealth, &nValue, 4, nP2Controlled);
}

void SetMeter(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue, int nP1Moon, int nP2Moon, uint8_t nP1Controlled, uint8_t nP2Controlled)
{
    int nWriteBuffer = nP1Moon == 2 ? min(nValue, 20000) : nValue;
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Meter), &nWriteBuffer, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1Meter, &nWriteBuffer, 4, nP1Controlled);

    nWriteBuffer = nP2Moon == 2 ? min(nValue, 20000) : nValue;
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Meter), &nWriteBuffer, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1Meter, &nWriteBuffer, 4, nP2Controlled);
}

void SetGuard(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nLevel, int nP1Moon, int nP2Moon, uint8_t nP1Controlled, uint8_t nP2Controlled)
{
    int nWriteBuffer = vGuardLevelLookupTable[nP1Moon * 5 + nLevel];
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1GuardAmount), &nWriteBuffer, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1GuardAmount, &nWriteBuffer, 4, nP1Controlled);
    
    nWriteBuffer = vGuardLevelLookupTable[nP2Moon * 5 + nLevel];
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2GuardAmount), &nWriteBuffer, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1GuardAmount, &nWriteBuffer, 4, nP2Controlled);
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
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1FMaidsHisuiLeadHearts), &nHearts, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2FMaidsHisuiLeadHearts), &nHearts, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1FMaidsKohaLeadHearts), &nHearts, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2FMaidsKohaLeadHearts), &nHearts, 4, 0);
}

void SetRyougiKnife(HANDLE hMBAAHandle, DWORD dwBaseAddress, bool bInfinite)
{
    int nTemp = bInfinite ? 0 : 1;
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RyougiKnife), &nTemp, 4, 0);
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2RyougiKnife), &nTemp, 4, 0);
}

void SetP1X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nValue, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1X, &nValue, 4, 0);
}

void SetP2X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2X), &nValue, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1X, &nValue, 4, 1);
}

void SetP3X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP3X), &nValue, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1X, &nValue, 4, 2);
}

void SetP4X(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nValue)
{
    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4X), &nValue, 4, 0);
    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1X, &nValue, 4, 3);
}

void GetP1Guts(float* pP1Guts, uint8_t nCharacter)
{
    float f1, f2, f3, f4;

    DWORD dwTempAddress = dwP1Struct + 0xAFC * nCharacter + 0x33C;
    DWORD dwGutsAddress = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &dwGutsAddress, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x18), &f1, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x1C), &f2, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x20), &f3, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x24), &f4, 4, 0);

    pP1Guts[0] = f1;
    pP1Guts[1] = f2 * (31.0f / 32.0f);
    pP1Guts[2] = f3 * (30.0f / 32.0f);
    pP1Guts[3] = f4 * (29.0f / 32.0f);
}

void GetP2Guts(float* pP2Guts, uint8_t nCharacter)
{
    float f1, f2, f3, f4;

    DWORD dwTempAddress = dwP2Struct * nCharacter + 0x33C;
    DWORD dwGutsAddress = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &dwGutsAddress, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x18), &f1, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x1C), &f2, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x20), &f3, 4, 0);
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwGutsAddress + 0x24), &f4, 4, 0);

    pP2Guts[0] = f1;
    pP2Guts[1] = f2 * (31.0f / 32.0f);
    pP2Guts[2] = f3 * (30.0f / 32.0f);
    pP2Guts[3] = f4 * (29.0f / 32.0f);
}

uint8_t arrKeysHeld[256] = { 0 };
void ResetKeysHeld()
{
    std::fill(std::begin(arrKeysHeld), std::end(arrKeysHeld), 1);
}

int KeyJustPressed()
{

    // check if any joy inputs are pressed
    short pressed = KeyState::pressedButtons();
    if (pressed) {
        // this should change to support these keys, however i dont know how yall want to do that
    }


    //http://www.kbdedit.com/manual/low_level_vk_list.html

    // there're some weird collisions between keys
    // like numpad-/ and the arrow keys,
    // but all of the normal keys work as expected.
    // I'm fine with it, personally.
    for(int i=0; i<256; i++) {
        // key pressed AND key isn't garbage
        if (GetAsyncKeyState(i) & 0x8000 && MapVirtualKeyW(i, MAPVK_VK_TO_VSC) != 0)
        {
            if (arrKeysHeld[i] == 0)
                return i;
        }
        else
            arrKeysHeld[i] = 0;
    }

    return 0;
}

bool IsControllerNeutral()
{
    // do this to prevent arrows or buttons being bound
    for (int i = 0; i < 16; i++)
    {
        Sleep(1);
        uint64_t nInputsDirectionThruStart = 0;
        uint16_t nInputsFn1Fn2 = 0;
        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1DirectionInput), &nInputsDirectionThruStart, 8, 0);
        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1DirectionInput + 0x8), &nInputsFn1Fn2, 2, 0);
        if (nInputsDirectionThruStart != 0 || nInputsFn1Fn2)
            return false;
    }
    return true;
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
uint64_t nDialogOutput;
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
            char buffer[256];
            
            GetWindowTextA(hEdit, buffer, 256);
            std::string sBuffer = std::string(buffer);

            if (sBuffer.substr(0, 2) == "0x")
                sBuffer.erase(0, 2);

            if (sBuffer.length() > 8)
            {
                bGoodInput = false;
                MessageBox(NULL, L"Number is too large.  Value should be between 0 and FFFFFFFF.", L"", NULL);
                break;
            }
            
            std::regex hexRegex("[0-9A-Fa-f]+$");
            if (!std::regex_match(sBuffer, hexRegex))
            {
                MessageBox(NULL, L"Number is invalid.  Value should be a hex number containing only 0-9 and A-F", L"", NULL);
                bGoodInput = false;
                break;
            }

            nDialogOutput = std::stoull(sBuffer, nullptr, 16);
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

    HWND hwnd = CreateWindowExW(0, TEXT("MyWindowClass"), TEXT(""),
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

void __stdcall ___netlog(const char* msg)
{
    const char* ipAddress = "127.0.0.1";
    unsigned short port = 17474;

    int msgLen = strlen(msg);

    const char* message = msg;

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        return;
    }

    SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sendSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return;
    }

    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress, &destAddr.sin_addr) <= 0)
    {
        closesocket(sendSocket);
        WSACleanup();
        return;
    }

    int sendResult = sendto(sendSocket, message, strlen(message), 0, (sockaddr*)&destAddr, sizeof(destAddr));
    if (sendResult == SOCKET_ERROR)
    {
        closesocket(sendSocket);
        WSACleanup();
        return;
    }

    closesocket(sendSocket);
    WSACleanup();
}

void __stdcall netlog(const char* format, ...) {
    static char buffer[1024]; // no more random char buffers everywhere.
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    ___netlog(buffer);
    va_end(args);
}

long long getMicroSec() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

std::wstring getDLLPath() {
    wchar_t buffer[1024];
    if (!GetTempPathW(1024, buffer)) {
        printf(RED "Failed to get temp path\n" RESET);
        return L"";
    }
    std::wstring dllPath = std::wstring(buffer) + std::wstring(L"\\Extended-Training-Mode-DLL.dll");
    return dllPath;
}

bool writeDLL() {

    std::wstring dllPath = getDLLPath();

    HINSTANCE hInstance = GetModuleHandle(NULL);

    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_DLL1), L"DLL");
    if (!hRes) {
        printf(RED "Failed to FindResource %d\n" RESET, GetLastError());
        return false;
    }

    HGLOBAL hData = LoadResource(hInstance, hRes);
    if (!hData) {
        printf(RED "Failed to LoadResource\n" RESET);
        return false;
    }
    void* pData = LockResource(hData);
    if (!pData) {
        printf(RED "Failed to LockResource\n" RESET);
        return false;
    }

    DWORD dwSize = SizeofResource(hInstance, hRes);

    std::ofstream outFile(dllPath, std::ios::binary);
    if (outFile) {
        outFile.write(reinterpret_cast<char*>(pData), dwSize);
    } else {
        printf(RED "Failed to open dll file\n" RESET);
        return false;
    }

    return true;
}
