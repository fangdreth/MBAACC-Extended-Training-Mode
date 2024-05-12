#pragma once
#include "Constants.h"

static DWORD GetSubMenuEnumAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x84;

    return dwTempAddress;
}

static DWORD GetEnemySettingsMenuTitleStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionInfoStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionActionAStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionActionBStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionActionCStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionRecoverCStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyActionRecoverDStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseAllGuardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseStatusGuardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseAllShieldStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseStatusShieldStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseDodgeStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeComboStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyDefenseTypeOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryNeutralStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryBackStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryForwardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryRandom1StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetAirRecoveryRandom2StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryOffStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryNeutralStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryBackStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryForwardStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryRandom1StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDownRecoveryRandom2StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetNoRecoverStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetRecover25StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetRecover50StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetRecover75StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetRecover100StringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetLifeIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetLifeOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetTrainingMenuStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryAllFastStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryAllLateStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryAllRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryRandomFastStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryRandomLateStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetThrowRecoveryRandomRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageIndexAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageOptionXAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageNormalStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageAllStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReduceDamageRandomStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetDefaultStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemySettingsCursorAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetEnemyStatusAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetMagicCircuitAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetReturnToMainMenuStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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
}

static DWORD GetViewScreenStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
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

static DWORD GetCommandListStringAddress(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTempAddress + 0x2C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}