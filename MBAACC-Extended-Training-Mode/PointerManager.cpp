#include "PointerManager.h"

#ifndef POINTERMANAGER_H
#define POINTERMANAGER_H

PointerManager::PointerManager()
{
    m_hMBAAHandle = 0x0;
    m_dwBaseAddress = 0x0;

    m_dwEnemySettingsMenuTitle = 0x0;

    m_dwEnemyActionOffString = 0x0;
    m_dwEnemyActionAString = 0x0;
    m_dwEnemyActionBString = 0x0;
    m_dwEnemyActionCString = 0x0;
    m_dwEnemyRecoverCString = 0x0;
    m_dwEnemyRecoverDString = 0x0;

    m_dwEnemyDefenseTypeStringAddress = 0x0;
    m_dwEnemyDefenseTypeNormalStringAddress = 0x0;
    m_dwEnemyDefenseTypeComboStringAddress = 0x0;
    m_dwEnemyDefenseTypeRandomStringAddress = 0x0;

    m_dwEnemyActionInfoStringAddress = 0x0;

    m_dwEnemyDefenseOffString = 0x0;
    m_dwEnemyDefenseAllGuardString = 0x0;
    m_dwEnemyDefenseStatusGuardString = 0x0;
    m_dwEnemyDefenseAllShieldString = 0x0;
    m_dwEnemyDefenseStatusShieldString = 0x0;
    m_dwEnemyDefenseDodgeString = 0x0;

    m_dwAirRecoveryString = 0x0;
    m_dwAirRecoveryOffString = 0x0;
    m_dwAirRecoveryNeutralString = 0x0;
    m_dwAirRecoveryBackString = 0x0;
    m_dwAirRecoveryForwardString = 0x0;
    m_dwAirRecoveryRandom1String = 0x0;
    m_dwAirRecoveryRandom2String = 0x0;

    m_dwDownRecoveryString = 0x0;
    m_dwDownRecoveryOffString = 0x0;
    m_dwDownRecoveryNeutralString = 0x0;
    m_dwDownRecoveryBackString = 0x0;
    m_dwDownRecoveryForwardString = 0x0;
    m_dwDownRecoveryRandom1String = 0x0;
    m_dwDownRecoveryRandom2String = 0x0;

    m_dwThrowRecoveryString = 0x0;
    m_dwThrowRecoveryNormalString = 0x0;
    m_dwThrowRecoveryAllFastString = 0x0;
    m_dwThrowRecoveryAllLateString = 0x0;
    m_dwThrowRecoveryAllRandomString = 0x0;
    m_dwThrowRecoveryRandomFastString = 0x0;
    m_dwThrowRecoveryRandomLateString = 0x0;
    m_dwThrowRecoveryRandomRandomString = 0x0;

    m_dwReduceDamageString = 0x0;
    m_dwReduceDamageNormalString = 0x0;
    m_dwReduceDamageAllString = 0x0;
    m_dwReduceDamageRandomString = 0x0;

    m_dwEnemyActionString = 0x0;
    m_dwEnemyDefenseString = 0x0;

    m_dwEnemyActionOptionX = 0x0;
    m_dwEnemyDefenseOptionX = 0x0;
    m_dwEnemyDefenseTypeOptionX = 0x0;
    m_dwAirRecoveryOptionX = 0x0;
    m_dwDownRecoveryOptionX = 0x0;
    m_dwThrowRecoveryOptionX = 0x0;

    m_dwDefaultString = 0x0;

    m_dwEnemySettingsCursor = 0x0;

    m_dwEnemyActionIndex = 0x0;
    m_dwEnemyDefenseIndex = 0x0;
    m_dwEnemyDefenseTypeIndex = 0x0;
    m_dwAirRecoveryIndex = 0x0;
    m_dwDownRecoveryIndex = 0x0;
    m_dwThrowRecoveryIndex = 0x0;
    m_dwReduceDamageIndex = 0x0;
    m_dwEnemyStatusIndex = 0x0;
}

PointerManager::~PointerManager()
{
}

void PointerManager::InitializeMainPointers(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    m_hMBAAHandle = hMBAAHandle;
    m_dwBaseAddress = dwBaseAddress;

    m_dwViewScreenStringAddress = GetViewScreenStringAddress();
    m_dwTrainingMenuString = GetTrainingMenuStringAddress();

    m_dwSubMenuAddress = GetSubMenuEnumAddress();

    m_dwReturnToMainMenuString = GetReturnToMainMenuStringAddress();
}

void PointerManager::InitializeBattleSettingsPointers(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    m_hMBAAHandle = hMBAAHandle;
    m_dwBaseAddress = dwBaseAddress;

    m_dwNoRecoverString = GetNoRecoverStringAddress();
    m_dwRecover25String = GetRecover25StringAddress();
    m_dwRecover50String = GetRecover50StringAddress();
    m_dwRecover75String = GetRecover75StringAddress();
    m_dwRecover100String = GetRecover100StringAddress();

    do
    {
        m_dwLifeIndex = GetLifeIndexAddress();
    } while (m_dwLifeIndex == 0x58);
}

void PointerManager::InitializeEnemySettingsPointers(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
    m_hMBAAHandle = hMBAAHandle;
    m_dwBaseAddress = dwBaseAddress;

    m_dwEnemySettingsMenuTitle = GetEnemySettingsMenuTitleStringAddress();

    m_dwEnemyActionOffString = GetEnemyActionOffStringAddress();
    m_dwEnemyActionAString = GetEnemyActionActionAStringAddress();
    m_dwEnemyActionBString = GetEnemyActionActionBStringAddress();
    m_dwEnemyActionCString = GetEnemyActionActionCStringAddress();
    m_dwEnemyRecoverCString = GetEnemyActionRecoverCStringAddress();
    m_dwEnemyRecoverDString = GetEnemyActionRecoverDStringAddress();

    m_dwEnemyDefenseTypeStringAddress = GetEnemyDefenseTypeStringAddress();
    m_dwEnemyDefenseTypeNormalStringAddress = GetEnemyDefenseTypeNormalStringAddress();
    m_dwEnemyDefenseTypeComboStringAddress = GetEnemyDefenseTypeComboStringAddress();
    m_dwEnemyDefenseTypeRandomStringAddress = GetEnemyDefenseTypeRandomStringAddress();

    m_dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress();

    m_dwEnemyDefenseOffString = GetEnemyDefenseOffStringAddress();
    m_dwEnemyDefenseAllGuardString = GetEnemyDefenseAllGuardStringAddress();
    m_dwEnemyDefenseStatusGuardString = GetEnemyDefenseStatusGuardStringAddress();
    m_dwEnemyDefenseAllShieldString = GetEnemyDefenseAllShieldStringAddress();
    m_dwEnemyDefenseStatusShieldString = GetEnemyDefenseStatusShieldStringAddress();
    m_dwEnemyDefenseDodgeString = GetEnemyDefenseDodgeStringAddress();

    m_dwAirRecoveryString = GetAirRecoveryStringAddress();
    m_dwAirRecoveryOffString = GetAirRecoveryOffStringAddress();
    m_dwAirRecoveryNeutralString = GetAirRecoveryNeutralStringAddress();
    m_dwAirRecoveryBackString = GetAirRecoveryBackStringAddress();
    m_dwAirRecoveryForwardString = GetAirRecoveryForwardStringAddress();
    m_dwAirRecoveryRandom1String = GetAirRecoveryRandom1StringAddress();
    m_dwAirRecoveryRandom2String = GetAirRecoveryRandom2StringAddress();

    m_dwDownRecoveryString = GetDownRecoveryStringAddress();
    m_dwDownRecoveryOffString = GetDownRecoveryOffStringAddress();
    m_dwDownRecoveryNeutralString = GetDownRecoveryNeutralStringAddress();
    m_dwDownRecoveryBackString = GetDownRecoveryBackStringAddress();
    m_dwDownRecoveryForwardString = GetDownRecoveryForwardStringAddress();
    m_dwDownRecoveryRandom1String = GetDownRecoveryRandom1StringAddress();
    m_dwDownRecoveryRandom2String = GetDownRecoveryRandom2StringAddress();

    m_dwThrowRecoveryString = GetThrowRecoveryStringAddress();
    m_dwThrowRecoveryNormalString = GetThrowRecoveryNormalStringAddress();
    m_dwThrowRecoveryAllFastString = GetThrowRecoveryAllFastStringAddress();
    m_dwThrowRecoveryAllLateString = GetThrowRecoveryAllLateStringAddress();
    m_dwThrowRecoveryAllRandomString = GetThrowRecoveryAllRandomStringAddress();
    m_dwThrowRecoveryRandomFastString = GetThrowRecoveryRandomFastStringAddress();
    m_dwThrowRecoveryRandomLateString = GetThrowRecoveryRandomLateStringAddress();
    m_dwThrowRecoveryRandomRandomString = GetThrowRecoveryRandomRandomStringAddress();

    m_dwReduceDamageString = GetReduceDamageStringAddress();
    m_dwReduceDamageNormalString = GetReduceDamageNormalStringAddress();
    m_dwReduceDamageAllString = GetReduceDamageAllStringAddress();
    m_dwReduceDamageRandomString = GetReduceDamageRandomStringAddress();

    m_dwEnemyActionString = GetEnemyActionStringAddress();
    m_dwEnemyDefenseString = GetEnemyDefenseStringAddress();

    m_dwEnemyActionOptionX = GetEnemyActionOptionXAddress();
    m_dwEnemyDefenseOptionX = GetEnemyDefenseOptionXAddress();
    m_dwEnemyDefenseTypeOptionX = GetEnemyDefenseTypeOptionXAddress();
    m_dwAirRecoveryOptionX = GetAirRecoveryOptionXAddress();
    m_dwDownRecoveryOptionX = GetDownRecoveryOptionXAddress();
    m_dwThrowRecoveryOptionX = GetThrowRecoveryOptionXAddress();

    m_dwDefaultString = GetDefaultStringAddress();

    m_dwEnemySettingsCursor = GetEnemySettingsCursorAddress();

    do
    {
        m_dwEnemyActionIndex = GetEnemyActionIndexAddress();
        m_dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress();
        m_dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress();
        m_dwAirRecoveryIndex = GetAirRecoveryIndexAddress();
        m_dwDownRecoveryIndex = GetDownRecoveryIndexAddress();
        m_dwThrowRecoveryIndex = GetThrowRecoveryIndexAddress();
        m_dwReduceDamageIndex = GetReduceDamageIndexAddress();
        m_dwEnemyStatusIndex = GetEnemyStatusAddress();
    } while (m_dwEnemyActionIndex == 0x58 || m_dwEnemyDefenseIndex == 0x58 || m_dwEnemyDefenseTypeIndex == 0x58 || m_dwAirRecoveryIndex == 0x58 || m_dwDownRecoveryIndex == 0x58 || m_dwThrowRecoveryIndex == 0x58 || m_dwReduceDamageIndex == 0x58 || m_dwEnemyStatusIndex == 0x58);
}

#endif
