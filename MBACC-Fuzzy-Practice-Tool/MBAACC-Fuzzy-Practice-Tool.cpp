#include <Windows.h>
#include "tlhelp32.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <iostream>
#include <ctime>
#include <vector>

enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, HITCONFIRM, CUSTOM };
enum eEnemyDefenseBiasSetting { UNLIKELY, EVEN, LIKELY, OFF };

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

void ApplyPreset(int nPresetSetting, bool* bSwitchToCrouch, int* nDelayFrames, int* nEnemyStatusSetting, int* nEnemyDefenseSetting, int* nEnemyDefenseBiasSetting)
{
    switch (nPresetSetting)
    {
    case ePresetSettings::DEFAULT:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
        break;
    case ePresetSettings::FUZZY:
        *bSwitchToCrouch = true;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::STAND;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
        break;
    case ePresetSettings::HITCONFIRM:
        *bSwitchToCrouch = false;
        *nDelayFrames = 0;
        *nEnemyStatusSetting = eEnemyStatus::CROUCH;
        *nEnemyDefenseSetting = eEnemyDefense::ALLGUARD;
        *nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::LIKELY;
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    HANDLE hProcess = 0x0;
    std::cout << "Looking for MBAA.exe...";
    while (hProcess == 0x0)
        hProcess = GetProcessByName(L"MBAA.exe");
    DWORD dwBaseAddress = GetBaseAddressByName(hProcess, L"MBAA.exe");

    const DWORD dwP2Offset = 0xAFC;
    const DWORD dwPlayerState = 0x155140;  //0:STAND 13:CROUCH 17:STANDGUARDING
    const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
    const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE

    int nReadResult;
    int nWriteBuffer;
    
    bool bUpPressed = false;
    bool bDownPressed = false;
    bool bRightPressed = false;
    bool bLeftPressed = false;

    bool bSwitchToCrouch = false;
    bool bRandomBlock = false;
    int nDelayFrames = 0;
    int nPresetSetting = ePresetSettings::DEFAULT;
    int nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
    int nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
    int nEnemyStatusSetting = eEnemyStatus::STAND;
    std::vector<std::string> vPresetSettings = { "Default", "Fuzzy Overhead", "Hitconfirming from blockstring", "Custom" };
    std::vector<std::string> vEnemyDefenseSettings = { "No Guard", "All Guard", "Status Guard", "All Shield", "Status Shield"};
    std::vector<std::string> vEnemyDefenseBiasSettings = { "Unlikely", "Even", "Likely", "Off" };
    std::vector<std::string> vEnemyStatusSettings = { "Stand", "Jump", "Crouch" };

    int nCursorIndex = 1;

    int nDebugBias = 0;
    int nDebugFrameCount = 0;

    std::srand(std::time(nullptr));

    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    system("cls");
    std::cout << "Fang's Extended Training Mode v1.0" << std::endl;

    while (1)
    {
        // Loop till the dummy is blocking
        while (1)
        {
            Sleep(16);
            nDebugFrameCount++;

            std::cout.flush();

            SetConsoleCursorPosition(hOut, { 0, 2 });
            std::cout << (nCursorIndex == 0 ? "=>  " : "    ") << "Preset:\t\t\t\t<- " << vPresetSettings[nPresetSetting] << " ->                                        ";

            SetConsoleCursorPosition(hOut, { 0, 4 });
            std::cout << (nCursorIndex == 1 ? "=>  " : "    ") << "Switch to crouching after blocking:\t<- " << (bSwitchToCrouch ? "On " : "Off") << " ->      ";

            SetConsoleCursorPosition(hOut, { 0, 5 });
            std::cout << (nCursorIndex == 2 ? "=>  " : "    ") << "Delay before crouching:\t\t<- " << nDelayFrames << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 7 });
            std::cout << (nCursorIndex == 3 ? "=>  " : "    ") << "Enemy Status:\t\t\t<- " << vEnemyStatusSettings[nEnemyStatusSetting] << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 9 });
            std::cout << (nCursorIndex == 4 ? "=>  " : "    ") << "Enemy Defense:\t\t\t<- " << vEnemyDefenseSettings[nEnemyDefenseSetting] << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 10 });
            std::cout << (nCursorIndex == 5 ? "=>  " : "    ") << "Random Bias:\t\t\t<- " << vEnemyDefenseBiasSettings[nEnemyDefenseBiasSetting] << " ->              ";

            SetConsoleCursorPosition(hOut, { 60, 0 });
            std::cout << "Bias:" << nDebugBias << " FrameCount:" << nDebugFrameCount;

            if (GetAsyncKeyState(VK_ESCAPE))
                goto CLEANUP;

            if (GetAsyncKeyState(VK_UP))
            {
                if (!bUpPressed)
                {
                    nCursorIndex = max(0, nCursorIndex - 1);
                    bUpPressed = true;
                }
            }
            else
                bUpPressed = false;

            if (GetAsyncKeyState(VK_DOWN))
            {
                if (!bDownPressed)
                {
                    nCursorIndex++;
                    bDownPressed = true;
                }
            }
            else
                bDownPressed = false;

            
            if (GetAsyncKeyState(VK_RIGHT))
            {
                if (!bRightPressed)
                {
                    bRightPressed = true;
                    if (nCursorIndex != 0)
                        nPresetSetting = ePresetSettings::CUSTOM;

                    switch (nCursorIndex)
                    {
                    case 0:
                        nPresetSetting = min(nPresetSetting + 1, vPresetSettings.size() - 2);
                        ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyDefenseBiasSetting);
                        break;
                    case 1:
                        bSwitchToCrouch = !bSwitchToCrouch;
                        break;
                    case 2:
                        nDelayFrames++;
                        break;
                    case 3:
                        nEnemyStatusSetting = min(nEnemyStatusSetting + 1, vEnemyStatusSettings.size() - 1);
                        break;
                    case 4:
                        nEnemyDefenseSetting = min(nEnemyDefenseSetting + 1, vEnemyDefenseSettings.size() - 1);
                        break;
                    case 5:
                        nEnemyDefenseBiasSetting = max(nEnemyDefenseBiasSetting - 1, 0);
                        break;
                    default:
                        break;
                    }
                }
            }
            else
                bRightPressed = false;

            if (GetAsyncKeyState(VK_LEFT))
            {
                if (!bLeftPressed)
                {
                    bLeftPressed = true;
                    if (nCursorIndex != 0)
                        nPresetSetting = ePresetSettings::CUSTOM;

                    switch (nCursorIndex)
                    {
                    case 0:
                        nPresetSetting = max(nPresetSetting - 1, 0);
                        ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyDefenseBiasSetting);
                        break;
                    case 1:
                        bSwitchToCrouch = !bSwitchToCrouch;
                        break;
                    case 2:
                        nDelayFrames = max(0, nDelayFrames - 1);
                        break;
                    case 3:
                        nEnemyStatusSetting = max(nEnemyStatusSetting - 1, 0);
                        break;
                    case 4:
                        nEnemyDefenseSetting = max(nEnemyDefenseSetting - 1, 0);
                        break;
                    case 5:
                        nEnemyDefenseBiasSetting = min(nEnemyDefenseBiasSetting + 1, vEnemyDefenseBiasSettings.size() - 1);
                        break;
                    default:
                        break;
                    }
                }
            }
            else
                bLeftPressed = false;


            nWriteBuffer = nEnemyStatusSetting;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

            nWriteBuffer = nEnemyDefenseSetting;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

            int nRandVal = std::rand() % 4;
            if (nDebugFrameCount % 10 == 0 && nRandVal < nEnemyDefenseBiasSetting + 1)
            {
                nDebugBias--;
                nWriteBuffer = eEnemyDefense::NOGUARD;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
            }

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
            if (nReadResult == eEnemyStance::STANDGUARDING)
                break;
        }

        if (bSwitchToCrouch)
        {
            Sleep(16 * nDelayFrames);

            nWriteBuffer = eEnemyStatus::CROUCH;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

            nWriteBuffer = eEnemyDefense::STATUSGUARD;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

            // Loop until the dummy returns to neutral
            while (1)
            {
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
                if (nReadResult == eEnemyStance::CROUCHING)
                    break;
            }
        }
    }

CLEANUP:
    CloseHandle(hProcess);
    return 0;
}
