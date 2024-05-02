#pragma once
#include "Constants.h"

static HANDLE m_hMBAAHandle = 0x0;
static DWORD m_dwBaseAddress = 0x0;

class PointerManager
{
public:
	PointerManager();
	~PointerManager();
	void InitializePointers(HANDLE hMBAAHandle, DWORD dwBaseAddress);
	
	DWORD m_dwEnemySettingsMenuTitle;

	DWORD m_dwEnemyActionOffString;
	DWORD m_dwEnemyActionAString;
	DWORD m_dwEnemyActionBString;
	DWORD m_dwEnemyActionCString;
	DWORD m_dwEnemyRecoverCString;
	DWORD m_dwEnemyRecoverDString;

	DWORD m_dwEnemyDefenseTypeStringAddress;
	DWORD m_dwEnemyDefenseTypeNormalStringAddress;
	DWORD m_dwEnemyDefenseTypeComboStringAddress;
	DWORD m_dwEnemyDefenseTypeRandomStringAddress;

	DWORD m_dwEnemyActionInfoStringAddress;

	DWORD m_dwEnemyDefenseOffString;
	DWORD m_dwEnemyDefenseAllGuardString;
	DWORD m_dwEnemyDefenseStatusGuardString;
	DWORD m_dwEnemyDefenseAllShieldString;
	DWORD m_dwEnemyDefenseStatusShieldString;
	DWORD m_dwEnemyDefenseDodgeString;

	DWORD m_dwAirRecoveryString;
	DWORD m_dwAirRecoveryOffString;
	DWORD m_dwAirRecoveryNeutralString;
	DWORD m_dwAirRecoveryBackString;
	DWORD m_dwAirRecoveryForwardString;
	DWORD m_dwAirRecoveryRandom1String;
	DWORD m_dwAirRecoveryRandom2String;

	DWORD m_dwDownRecoveryString;
	DWORD m_dwDownRecoveryOffString;
	DWORD m_dwDownRecoveryNeutralString;
	DWORD m_dwDownRecoveryBackString;
	DWORD m_dwDownRecoveryForwardString;
	DWORD m_dwDownRecoveryRandom1String;
	DWORD m_dwDownRecoveryRandom2String;

	DWORD m_dwThrowRecoveryString;
	DWORD m_dwThrowRecoveryNormalString;
	DWORD m_dwThrowRecoveryAllFastString;
	DWORD m_dwThrowRecoveryAllLateString;
	DWORD m_dwThrowRecoveryAllRandomString;
	DWORD m_dwThrowRecoveryRandomFastString;
	DWORD m_dwThrowRecoveryRandomLateString;
	DWORD m_dwThrowRecoveryRandomRandomString;

	DWORD m_dwReduceDamageString;
	DWORD m_dwReduceDamageNormalString;
	DWORD m_dwReduceDamageAllString;
	DWORD m_dwReduceDamageRandomString;

	DWORD m_dwEnemyActionString;
	DWORD m_dwEnemyDefenseString;

	DWORD m_dwEnemyActionOptionX;
	DWORD m_dwEnemyDefenseOptionX;
	DWORD m_dwEnemyDefenseTypeOptionX;
	DWORD m_dwAirRecoveryOptionX;
	DWORD m_dwDownRecoveryOptionX;
	DWORD m_dwThrowRecoveryOptionX;

	DWORD m_dwDefaultString;

	DWORD m_dwEnemySettingsCursor;

	DWORD m_dwEnemyActionIndex;
	DWORD m_dwEnemyDefenseIndex;
	DWORD m_dwEnemyDefenseTypeIndex;
	DWORD m_dwAirRecoveryIndex;
	DWORD m_dwDownRecoveryIndex;
	DWORD m_dwThrowRecoveryIndex;
	DWORD m_dwReduceDamageIndex;
	DWORD m_dwEnemyStatusIndex;

    DWORD m_dwNoRecoverString;
    DWORD m_dwRecover25String;
    DWORD m_dwRecover50String;
    DWORD m_dwRecover75String;
    DWORD m_dwRecover100String;
    DWORD m_dwLifeIndex;

    DWORD m_dwViewScreenStringAddress;
    DWORD m_dwTrainingMenuString;

    DWORD m_dwSubMenuAddress;

    DWORD m_dwReturnToMainMenuString;
};

static DWORD GetSubMenuEnumAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x84;

    return dwTempAddress;
}

static DWORD GetEnemySettingsMenuTitleStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x60), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetEnemyActionIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetEnemyActionOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetEnemyActionInfoStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xDC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x90), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xE58), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x36;

    return dwTempAddress;
}

static DWORD GetEnemyActionStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetEnemyActionOffStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyActionActionAStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyActionActionBStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyActionActionCStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyActionRecoverCStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x1C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyActionRecoverDStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseOffStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseAllGuardStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseStatusGuardStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseAllShieldStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseStatusShieldStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseDodgeStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x24), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeNormalStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeComboStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeRandomStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetEnemyDefenseTypeOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetAirRecoveryIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetAirRecoveryOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetAirRecoveryStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetAirRecoveryOffStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetAirRecoveryNeutralStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetAirRecoveryBackStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetAirRecoveryForwardStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetAirRecoveryRandom1StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetAirRecoveryRandom2StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetDownRecoveryOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetDownRecoveryStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetDownRecoveryOffStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryNeutralStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryBackStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryForwardStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryRandom1StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDownRecoveryRandom2StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetNoRecoverStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetRecover25StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetRecover50StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetRecover75StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetRecover100StringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetLifeIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetLifeOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetTrainingMenuStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x60;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryNormalStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryAllFastStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryAllLateStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryAllRandomStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryRandomFastStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryRandomLateStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x14), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetThrowRecoveryRandomRandomStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x20), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x18), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetReduceDamageStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetReduceDamageIndexAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetReduceDamageOptionXAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x5C;

    return dwTempAddress;
}

static DWORD GetReduceDamageNormalStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetReduceDamageAllStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetReduceDamageRandomStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x28), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x64), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x4;

    return dwTempAddress;
}

static DWORD GetDefaultStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x30), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}

static DWORD GetEnemySettingsCursorAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xCC), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x40;

    return dwTempAddress;
}

static DWORD GetEnemyStatusAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetMagicCircuitAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0xC8), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x58;

    return dwTempAddress;
}

static DWORD GetReturnToMainMenuStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x40), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x24), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;

    return dwTempAddress;
}

static DWORD GetViewScreenStringAddress()
{
    DWORD dwTempAddress = dwBasePointer;
    int nReadResult = 0;

    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x10), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x0), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x4C), &nReadResult, 4, 0);
    dwTempAddress = nReadResult;
    ReadProcessMemory(m_hMBAAHandle, (LPVOID)(dwTempAddress + 0x34), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x24;

    return dwTempAddress;
}