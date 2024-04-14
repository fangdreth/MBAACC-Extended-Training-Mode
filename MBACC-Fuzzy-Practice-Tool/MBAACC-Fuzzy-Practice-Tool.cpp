#include <Windows.h>
#include "tlhelp32.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
#include <iostream>

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

int main(int argc, char* argv[])
{
    HANDLE hProcess = 0x0;
    std::cout << "Looking for MBAA.exe...";
    while (hProcess == 0x0)
        hProcess = GetProcessByName(L"MBAA.exe");
    DWORD dwBaseAddress = GetBaseAddressByName(hProcess, L"MBAA.exe");

    DWORD dwP2Offset = 0xAFC;
    DWORD dwPlayerState = 0x155140;
    DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
    DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE

    enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
    enum eEnemyDefense { OFF, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
    enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };

    int nReadResult;
    int nWriteBuffer;
    int nDelayFrames = 0;

    bool bUpPressed = false;
    bool bDownPressed = false;

    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    system("cls");
    std::cout << "Fang's Fuzzy Practice Tool v1.0" << std::endl;

    while (1)
    {
        nWriteBuffer = eEnemyStatus::STAND;
        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

        nWriteBuffer = eEnemyDefense::ALLGUARD;
        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

        while (1)
        {
            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
            if (nReadResult == eEnemyStance::STANDGUARDING)
                break;

            //system("cls");
            std::cout.flush();
            SetConsoleCursorPosition(hOut, { 0, 5 });
            std::cout << "Delay before crouching: " << nDelayFrames << "              ";

            if (GetAsyncKeyState(VK_ESCAPE))
                goto CLEANUP;

            if (GetAsyncKeyState(VK_UP))
            {
                if (!bUpPressed)
                {
                    nDelayFrames++;
                    bUpPressed = true;
                }
            }
            else
                bUpPressed = false;

            if (GetAsyncKeyState(VK_DOWN))
            {
                if (!bDownPressed)
                {
                    nDelayFrames = max(0, nDelayFrames - 1);
                    bDownPressed = true;
                }
            }
            else
                bDownPressed = false;

        }

        Sleep(16 * nDelayFrames);

        nWriteBuffer = eEnemyStatus::CROUCH;
        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

        nWriteBuffer = eEnemyDefense::STATUSGUARD;
        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

        while (1)
        {
            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
            if (nReadResult == eEnemyStance::CROUCHING)
                break;
        }

        
    }

CLEANUP:
    CloseHandle(hProcess);
    return 0;
}
