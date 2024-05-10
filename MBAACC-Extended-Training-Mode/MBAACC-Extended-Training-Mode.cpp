#include "MBAACC-Extended-Training-Mode.h"
#include "Logger.h"

// the -1 when loading the menu glitch is in full force

int main(int argc, char* argv[])
{
    HANDLE hMBAAHandle = 0x0;
    DWORD dwExitCode = 0;
    DWORD dwBaseAddress = 0;

    int nReadResult = 0;
    int nWriteBuffer = 0;
    
    bool bPaused = false;
    bool bAPressed = false;

    bool bOnCSS = false;
    bool bOnExtendedSettingsMenu = false;

    int nP1CharacterID = 0;
    int nP2CharacterID = 0;
    int nP1Moon = 0;
    int nP2Moon = 0;
    int nP1CharacterNumber = 0;
    int nP2CharacterNumber = 0;
    bool bSwitchToCrouch = false;
    
    //int nSwitchBlockDelayFrames = 0;

    int nExGuardSetting = eEnemyOffOnRandom::OFF;
    int nCustomGuard = eEnemyGuardLevelSettings::ONEHUNDRED;
    int nReversalPattern = 0;
    int nReversalDelayFrames = 0;
    int nTempReversalDelayFrames = 0;
    bool bDelayingReversal = false;
    bool bReversaled = false;
    //bool bRandomReversal = false;
    //bool bRepeatReversal = true;
    int nReversalType = REVERSAL_NORMAL;
    int nReversalIndex1 = 0;
    int nReversalIndex2 = 0;
    int nReversalIndex3 = 0;
    int nReversalIndex4 = 0;
    std::vector<std::string> vPatternNames = GetEmptyPatternList();
    int nMot = 0;
    int nOldMot = 0;
    int nP2Y = 0;
    int nBurstCooldown = 0;

    int nOldEnemyActionIndex = -1;
    int nOldPresetIndex = -1;
    int nOldEnemyDefenseIndex = -1;
    int nOldEnemyDefenseTypeIndex = -1;
    int nOldAirRecoveryIndex = -1;
    int nOldDownRecoveryIndex = -1;
    int nOldThrowRecoveryIndex = -1;
    int nOldReduceDamageIndex = -1;
    int nOldLifeIndex = -1;
    int nCurrentSubMenu = eMenu::MAIN;
    int nOldCurrentSubMenu = -1;

    int nFrameCounter = 0;
    int nOldFrameCounter = 0;
    
    int nStoredEnemyAction = 0;
    int nStoredEnemyDefense = 0;
    int nStoredEnemyDefenseType = 0;
    int nStoredAirRecovery = 0;
    int nStoredDownRecovery = 0;
    int nStoredThrowRecovery = 0;
    int nStoredReduceDamage = 0;
    
    int nGameCursorIndex = 0;
    int nOldGameCursorIndex = 0;
    int nEnemySettingsCursor = 0;
    int nOldEnemySettingsCursor = 0;
    
    int nCustomMeter = 10000;
    int nCustomHealth = 11400;
    int nHealthRefillTimer = 0;
    int nCharSpecificsRefillTimer = 0;
    bool bLifeRecover = true;
    int nSionBullets = 14;
    int nRoaVisibleCharge = -1;
    int nRoaHiddenCharge = -1;
    int nSionBulletsRefillTimer = 0;
    int nRoaHiddenChargeRefillTimer = 0;
    int nRoaVisibleChargeRefillTimer = 0;

    int nExtendedSettingsPage = 1;

    int nGameMode = 0;
    
    bool bPositionsLocked = false;
    int nP1X = P1_DEFAULT_X;
    int nP2X = P2_DEFAULT_X;
    int nP3X = P3_DEFAULT_X;
    int nP4X = P4_DEFAULT_X;
    bool bP3Exists = false;
    bool bP4Exists = false;

    int nHitsTillBurst = TOO_HIGH_TO_BURST;

    bool bInfGuard = true;
    
    bool bNeedToAnnounceNewVersion = false;
    int nMovingMessageIndex = 0;
    int nStartingTime = 0;
    std::string sOnlineVersion;
    int nCurrentTime;

    HANDLE hConsoleHandle;

    std::srand((unsigned int)std::time(nullptr));

    hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
  
    InitializeLogger(hConsoleHandle);

    try
    {
        sOnlineVersion = GetLatestVersion();
        if (sOnlineVersion != "" && sOnlineVersion != VERSION)
        {
            bNeedToAnnounceNewVersion = true;
            LogInfo("New version " + sOnlineVersion + " found online");
        }
        else
        {
            LogInfo("Latest version installed");
        }
        
    }
    catch (...)
    {
        sOnlineVersion = "";
        LogError("Cannot fetch latest version");
    }

    while (1)
    {
        nCurrentTime = std::time(nullptr);
        
        
        SetConsoleCursorPosition(hConsoleHandle, { 0, 0 });
        std::cout << "===========================================================================" << std::endl;
        std::cout << "|   Fang's Extended Training Mode Mod " << VERSION << "                               |  " << std::endl;
        std::cout << "|                                                                         |" << std::endl;
        std::cout << "|   " << GITHUB_RELEASE << "   |" << std::endl;
        if (bNeedToAnnounceNewVersion && nCurrentTime % 3 != 0)
            std::cout << "|   NEW VERSION " << sOnlineVersion << " AVAILABLE ON GITHUB                                 |" << std::endl;
        else
            std::cout << "|                                                                         |" << std::endl;
        std::cout << "===========================================================================" << std::endl;

        GetExitCodeProcess(hMBAAHandle, &dwExitCode);

        if (hMBAAHandle == 0x0 || dwExitCode != 259)
        {
            // Print the variable values when MBAA closes, possibly from a crash
            if (hMBAAHandle != 0x0)
                CloseLog(bPaused, bAPressed, bOnCSS, bOnExtendedSettingsMenu, nP1CharacterID, nP2CharacterID, nP1Moon, nP2Moon, nP1CharacterNumber, nP2CharacterNumber, bSwitchToCrouch, nExGuardSetting, nCustomGuard, nReversalPattern, nReversalDelayFrames, nTempReversalDelayFrames, bDelayingReversal, bReversaled, nReversalType, nReversalIndex1, nReversalIndex2, nReversalIndex3, nReversalIndex4, vPatternNames, nMot, nOldMot, nP2Y, nBurstCooldown ,nOldEnemyActionIndex, nOldPresetIndex, nOldEnemyDefenseIndex, nOldEnemyDefenseTypeIndex, nOldAirRecoveryIndex, nOldDownRecoveryIndex, nOldThrowRecoveryIndex, nOldReduceDamageIndex, nOldLifeIndex, nCurrentSubMenu, nOldCurrentSubMenu, nFrameCounter, nOldFrameCounter, nStoredEnemyAction, nStoredEnemyDefense, nStoredEnemyDefenseType, nStoredAirRecovery, nStoredDownRecovery, nStoredThrowRecovery, nStoredReduceDamage, nGameCursorIndex, nOldGameCursorIndex, nEnemySettingsCursor, nOldEnemySettingsCursor, nCustomMeter, nCustomHealth, nHealthRefillTimer, nCharSpecificsRefillTimer, bLifeRecover, nSionBullets, nRoaVisibleCharge, nRoaHiddenCharge, nSionBulletsRefillTimer, nRoaHiddenChargeRefillTimer, nRoaVisibleChargeRefillTimer, nExtendedSettingsPage, bPositionsLocked, nP1X, nP2X, nP3X, nP4X, bP3Exists, bP4Exists, nHitsTillBurst, bInfGuard, nGameMode);

            hMBAAHandle = 0x0;

            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::string sLookingForMelty = "Looking for MBAA.exe                 ";
            for (int i = 0; i < nCurrentTime % 8; i++)
                sLookingForMelty[20 + i] = '.';
            std::cout << sLookingForMelty;
            SetConsoleCursorPosition(hConsoleHandle, { 0, 8 });
            std::cout << "                                              ";

            hMBAAHandle = GetProcessByName(L"MBAA.exe");

            // don't do anything until we re-attach to mbaa
            if (hMBAAHandle == 0x0)
            {
                Sleep(100);
                continue;
            }
            LogInfo("Attached to MBAA");

            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::cout << "                                              ";

            

            Sleep(100);
            dwBaseAddress = GetBaseAddressByName(hMBAAHandle, L"MBAA.exe");
            LogInfo("Got BaseAddressByName");

            InitializeCharacterMaps();
        }

        // check this to prevent attaching to netplay
        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGameMode), &nReadResult, 4, 0);
        nGameMode = nReadResult;
        if (nGameMode != 0)
        {
            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::cout << "Cannot attach to versus mode....                 ";
            //LogInfo("MBAA is in versus mode");
            continue;
        }
        else
        {
            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::cout << "Attached to MBAA.exe                                              ";
            //LogInfo("MBAA is in training mode");
        }

        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
        if (nReadResult == 0)
        {
            nReversalIndex1 = 0;
            nReversalIndex2 = 0;
            nReversalIndex3= 0;
            vPatternNames = GetEmptyPatternList();

            // TODO: add fun CSS features here
        }
        else
        {
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1APressed), &nReadResult, 4, 0);
            bAPressed = (nReadResult == 1 ? true : false);

            // these flags are used to determine if assist chars exist
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Exists + dwP2Offset * 2), &nReadResult, 4, 0);
            bP3Exists = (nReadResult == 1 ? true : false);
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Exists + dwP2Offset * 3), &nReadResult, 4, 0);
            bP4Exists = (nReadResult == 1 ? true : false);

            // update the location variables if not locked
            if (!bPositionsLocked)
            {
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nReadResult, 4, 0);
                nP1X = nReadResult;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset), &nReadResult, 4, 0);
                nP2X = nReadResult;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 2), &nReadResult, 4, 0);
                nP3X = nReadResult;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 3), &nReadResult, 4, 0);
                nP4X = nReadResult;
            }

            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
            bPaused = (nReadResult == 1 ? true : false);

            // This is the big if-else
            if (bPaused)
            {
                DWORD dwTrainingMenuString = GetTrainingMenuStringAddress(hMBAAHandle, dwBaseAddress);
                DWORD dwViewScreenStringAddress = GetViewScreenStringAddress(hMBAAHandle, dwBaseAddress);
                DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hMBAAHandle, dwBaseAddress);

                // Replace the menu title and VIEW SCREEN
                char pcTrainingMenu[19];
                strcpy_s(pcTrainingMenu, ("EXTENDED MOD " + VERSION).c_str());
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwViewScreenStringAddress), &pcExtendedSettings_18, 18, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwTrainingMenuString), &pcTrainingMenu, 19, 0);

                //DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hMBAAHandle, dwBaseAddress);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                nOldCurrentSubMenu = nCurrentSubMenu;
                nCurrentSubMenu = nReadResult;

                // Main Start Menu
                if (nCurrentSubMenu == eMenu::MAIN)
                {
                    // these are used in the menus.
                    nOldEnemyActionIndex = -1;
                    nOldEnemyDefenseIndex = -1;
                    nOldEnemyDefenseTypeIndex = -1;
                    nOldAirRecoveryIndex = -1;
                    nOldDownRecoveryIndex = -1;
                    nOldThrowRecoveryIndex = -1;
                    nOldReduceDamageIndex = -1;
                    nOldLifeIndex = -1;

                    // restore the Battle Settings menu after clobbering it in EXTENDED SETTINGS
                    nWriteBuffer = nStoredEnemyDefense;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredEnemyDefenseType;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyDefenseType), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredEnemyAction;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredAirRecovery;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwAirRecovery), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredDownRecovery;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwDownRecovery), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredThrowRecovery;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwThrowRecovery), &nWriteBuffer, 4, 0);
                    nWriteBuffer = nStoredReduceDamage;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwReduceRecovery), &nWriteBuffer, 4, 0);
                        
                    // Replace the RETURN TO MAIN MENU option with fancy scrolling text
                    // this is 100% unnecessary but I did it for fun
                    DWORD dwReturnToMainMenuString = GetReturnToMainMenuStringAddress(hMBAAHandle, dwBaseAddress);
                    if (bNeedToAnnounceNewVersion)
                    {
                        // assemble the string for the message
                        std::string sNewVersionMessage = "             VERSION " + sOnlineVersion + " AVAILABLE NOW ON GITHUB";
                        int nMessageLength = sNewVersionMessage.size();
                        sNewVersionMessage += "             " + sNewVersionMessage;

                        if (nStartingTime == 0 || nCurrentSubMenu != nOldCurrentSubMenu || nMovingMessageIndex >= nMessageLength + 16)
                            nStartingTime = std::time(nullptr);

                        // move the index based on the timer
                        int nCurrentTime = std::time(nullptr);
                        nMovingMessageIndex = (nCurrentTime - nStartingTime) * 2;

                        // chop the string up
                        std::string sTempNewVersionMessage = "[" + sNewVersionMessage.substr(nMovingMessageIndex, 16) + "]";

                        //send it
                        char pcNewVersionMessage[19];
                        strcpy_s(pcNewVersionMessage, (sTempNewVersionMessage).c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReturnToMainMenuString), &pcNewVersionMessage, 19, 0);
                    }
                    else if (sOnlineVersion != "")
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReturnToMainMenuString), &pcLatestVersion_19, 19, 0);
                    }
                    else
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReturnToMainMenuString), &pcOffline_8, 8, 0);
                    }
                }

                // Turn VIEW SCREEN into EXTENDED SETTINGS
                if (nCurrentSubMenu == eMenu::VIEW_SCREEN)
                {
                    nWriteBuffer = eMenu::ENEMY_SETTINGS;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nWriteBuffer, 4, 0);

                    bOnExtendedSettingsMenu = true;
                }
                else if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                    bOnExtendedSettingsMenu = false;

                if (bOnExtendedSettingsMenu)
                {
                    DWORD dwEnemySettingsMenuTitle = GetEnemySettingsMenuTitleStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionOffString = GetEnemyActionOffStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionAString = GetEnemyActionActionAStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionBString = GetEnemyActionActionBStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionCString = GetEnemyActionActionCStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyRecoverCString = GetEnemyActionRecoverCStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyRecoverDString = GetEnemyActionRecoverDStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyDefenseTypeStringAddress = GetEnemyDefenseTypeStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseTypeNormalStringAddress = GetEnemyDefenseTypeNormalStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseTypeComboStringAddress = GetEnemyDefenseTypeComboStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseTypeRandomStringAddress = GetEnemyDefenseTypeRandomStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyDefenseOffString = GetEnemyDefenseOffStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseAllGuardString = GetEnemyDefenseAllGuardStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseStatusGuardString = GetEnemyDefenseStatusGuardStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseAllShieldString = GetEnemyDefenseAllShieldStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseStatusShieldString = GetEnemyDefenseStatusShieldStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseDodgeString = GetEnemyDefenseDodgeStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwAirRecoveryString = GetAirRecoveryStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryOffString = GetAirRecoveryOffStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryNeutralString = GetAirRecoveryNeutralStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryBackString = GetAirRecoveryBackStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryForwardString = GetAirRecoveryForwardStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryRandom1String = GetAirRecoveryRandom1StringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryRandom2String = GetAirRecoveryRandom2StringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwDownRecoveryString = GetDownRecoveryStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryOffString = GetDownRecoveryOffStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryNeutralString = GetDownRecoveryNeutralStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryBackString = GetDownRecoveryBackStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryForwardString = GetDownRecoveryForwardStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryRandom1String = GetDownRecoveryRandom1StringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryRandom2String = GetDownRecoveryRandom2StringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwThrowRecoveryString = GetThrowRecoveryStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryNormalString = GetThrowRecoveryNormalStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryAllFastString = GetThrowRecoveryAllFastStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryAllLateString = GetThrowRecoveryAllLateStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryAllRandomString = GetThrowRecoveryAllRandomStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryRandomFastString = GetThrowRecoveryRandomFastStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryRandomLateString = GetThrowRecoveryRandomLateStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryRandomRandomString = GetThrowRecoveryRandomRandomStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwReduceDamageString = GetReduceDamageStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwReduceDamageNormalString = GetReduceDamageNormalStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwReduceDamageAllString = GetReduceDamageAllStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwReduceDamageRandomString = GetReduceDamageRandomStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionString = GetEnemyActionStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseString = GetEnemyDefenseStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionOptionX = GetEnemyActionOptionXAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseOptionX = GetEnemyDefenseOptionXAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyDefenseTypeOptionX = GetEnemyDefenseTypeOptionXAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwAirRecoveryOptionX = GetAirRecoveryOptionXAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwDownRecoveryOptionX = GetDownRecoveryOptionXAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwThrowRecoveryOptionX = GetThrowRecoveryOptionXAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwDefaultString = GetDefaultStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemySettingsCursor = GetEnemySettingsCursorAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionIndex;
                    DWORD dwEnemyDefenseIndex;
                    DWORD dwEnemyDefenseTypeIndex;
                    DWORD dwAirRecoveryIndex;
                    DWORD dwDownRecoveryIndex;
                    DWORD dwThrowRecoveryIndex;
                    DWORD dwReduceDamageIndex;
                    DWORD dwEnemyStatusIndex;
                    do
                    {
                        dwEnemyActionIndex = GetEnemyActionIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwThrowRecoveryIndex = GetThrowRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwReduceDamageIndex = GetReduceDamageIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwEnemyStatusIndex = GetEnemyStatusAddress(hMBAAHandle, dwBaseAddress);

                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                        nCurrentSubMenu = nReadResult;

                    } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58 || dwThrowRecoveryIndex == 0x58 || dwReduceDamageIndex == 0x58 || dwEnemyStatusIndex == 0x58) && nCurrentSubMenu == eMenu::ENEMY_SETTINGS);

                    if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                        continue;

                    // Index values for menu items
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nReadResult, 4, 0);
                    nOldEnemySettingsCursor = nEnemySettingsCursor;
                    nEnemySettingsCursor = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nReadResult, 4, 0);
                    int nEnemyActionIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseTypeIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nReadResult, 4, 0);
                    int nAirRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nReadResult, 4, 0);
                    int nDownRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nReadResult, 4, 0);
                    int nThrowRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nReadResult, 4, 0);
                    int nReduceDamageIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyStatusIndex), &nReadResult, 4, 0);
                    int nEnemyStatusIndex = nReadResult;

                    // Reset hits till burst if status is not manual
                    if (nEnemyStatusIndex != eEnemyStatus::MANUAL)
                        nHitsTillBurst = TOO_HIGH_TO_BURST;

                    // Replace static menu fields
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset_17, 17, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings_18, 18, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageString), &pcBlank_1, 1, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDefaultString), &pcBlank_1, 1, 0);
                    if (nExtendedSettingsPage == REVERSALS_PAGE)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcReversalType_14, 14, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcReversalSlot1_16, 16, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcReversalSlot2_16, 16, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcReversalSlot3_16, 16, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcReversalSlot4_16, 16, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcReversalDelay_15, 15, 0);
                    }
                    else if (nExtendedSettingsPage == STATS_PAGE)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcReversalType_14, 14, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcExGuard_9, 9, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcGuardBar_10, 10, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcMeter_18, 18, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcHealth_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcHitsUntilBurst_17, 17, 0);
                    }
                    else if (nExtendedSettingsPage == POSITIONS_PAGE)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcASlow_12, 12, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcP1XLoc_11, 11, 0);
                        if (bP3Exists)
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcAssistLoc_13, 13, 0);
                        else
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcP2XLoc_11, 11, 0);
                        if (bP4Exists)
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcAssistLoc_13, 13, 0);
                        else
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryNormalString), &pcInvert_7, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcInvert_7, 7, 0);
                    }
                    else if (nExtendedSettingsPage == CHARACTER_SPECIFICS)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcSionBullets_13, 13, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcRoaVisibleCharge_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcRoaHiddenCharge_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcHitsUntilBurst_17, 17, 0);
                    }

                    // Skip DEFAULT
                    if (nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 12)
                    {
                        nWriteBuffer = 14;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                        nEnemySettingsCursor = 14;
                        nOldEnemySettingsCursor = 14;
                    }
                    else if (nOldEnemySettingsCursor == 14 && nEnemySettingsCursor == 12)
                    {
                        nWriteBuffer = 10;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                        nEnemySettingsCursor = 10;
                        nOldEnemySettingsCursor = 10;
                    }

                    // Skipping blank menu items
                    if (nExtendedSettingsPage == REVERSALS_PAGE)
                    {
                        // don't skip anything
                    }
                    else if (nExtendedSettingsPage == STATS_PAGE)
                    {
                        // don't skip anything
                    }
                    else if (nExtendedSettingsPage == POSITIONS_PAGE)
                    {
                        if (bPositionsLocked && nOldEnemySettingsCursor == 0 && nEnemySettingsCursor == 2)
                        {
                            nWriteBuffer = 10;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 10;
                            nOldEnemySettingsCursor = 10;
                        }
                        if (bPositionsLocked && nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 8)
                        {
                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 0;
                            nOldEnemySettingsCursor = 0;
                        }
                        if (!bP3Exists && nOldEnemySettingsCursor == 2 && nEnemySettingsCursor == 3)
                        {
                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 5;
                            nOldEnemySettingsCursor = 5;
                        }
                        else if (!bP3Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 3)
                        {
                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 2;
                            nOldEnemySettingsCursor = 2;
                        }
                        if (!bP4Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 8;
                            nOldEnemySettingsCursor = 8;
                        }
                        else if (!bP4Exists && nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 5;
                            nOldEnemySettingsCursor = 5;
                        }
                    }
                    else if (nExtendedSettingsPage == CHARACTER_SPECIFICS)
                    {
                        if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                        {
                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 8;
                            nOldEnemySettingsCursor = 8;
                        }
                        else if (nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 3;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 3;
                            nOldEnemySettingsCursor = 3;
                        }
                    }

                    // Left/Right on menu handlers
                    if (nExtendedSettingsPage == REVERSALS_PAGE)
                    {
                        if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                            nReversalType = max(REVERSAL_NORMAL, nReversalType - 1);
                        else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                            nReversalType = min(nReversalType + 1, REVERSAL_REPEAT);

                        if (nOldEnemyDefenseIndex == -1)
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                        else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                            nReversalIndex1 = max(0, nReversalIndex1 - 1);
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                            nReversalIndex1 = min(nReversalIndex1 + 1, vPatternNames.size() - 1);

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                            nReversalIndex2 = max(0, nReversalIndex2 - 1);
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                            nReversalIndex2 = min(nReversalIndex2 + 1, vPatternNames.size() - 1);

                        if (nOldAirRecoveryIndex == -1)
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                        else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                            nReversalIndex3 = max(0, nReversalIndex3 - 1);
                        else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                            nReversalIndex3 = min(nReversalIndex3 + 1, vPatternNames.size() - 1);

                        if (nOldDownRecoveryIndex == -1)
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
                        else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                            nReversalIndex4 = max(0, nReversalIndex4 - 1);
                        else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                            nReversalIndex4 = min(nReversalIndex4 + 1, vPatternNames.size() - 1);

                        if (nOldThrowRecoveryIndex == -1)
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                        else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                            nReversalDelayFrames = max(0, nReversalDelayFrames - 1);
                        else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                            nReversalDelayFrames = min(nReversalDelayFrames + 1, MAX_REVERSAL_DELAY);
                    }
                    else if (nExtendedSettingsPage == STATS_PAGE)
                    {
                        /*if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                            bRandomReversal = false;
                        else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                            bRandomReversal = true;*/

                        if (nOldEnemyDefenseIndex == -1)
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                        else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                            nExGuardSetting = max(0, nExGuardSetting - 1);
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                            nExGuardSetting = min(nExGuardSetting + 1, eEnemyOffOnRandom::RANDOM);

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                            bInfGuard = true;
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                            bInfGuard = false;

                        if (nOldAirRecoveryIndex == -1)
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                        else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                        {
                            nCustomMeter = max(0, nCustomMeter - (bAPressed ? 10 : 1000));
                            SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                        }
                        else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                        {
                            nCustomMeter = min(nCustomMeter + (bAPressed ? 10 : 1000), MAX_METER);
                            SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                        }

                        if (nOldDownRecoveryIndex == -1)
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
                        else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                        {
                            nCustomHealth = max(0, nCustomHealth - (bAPressed ? 1 : 100));
                            SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth);
                        }
                        else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                        {
                            nCustomHealth = min(nCustomHealth + (bAPressed ? 1 : 100), MAX_HEALTH);
                            SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth);
                        }

                        if (nOldThrowRecoveryIndex == -1)
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                        else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                        {
                            nHitsTillBurst = max(0, nHitsTillBurst - 1);
                            if (nHitsTillBurst == 0)
                                nHitsTillBurst = TOO_HIGH_TO_BURST;
                        }
                        else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                        {
                            if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                                nHitsTillBurst = 1;
                            else
                                nHitsTillBurst = min(MAX_BURST, nHitsTillBurst + 1);
                        }
                    }
                    else if (nExtendedSettingsPage == POSITIONS_PAGE)
                    {
                        if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                            bPositionsLocked = false;
                        else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                            bPositionsLocked = true;

                        if (nOldEnemyDefenseIndex == -1)
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                        else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                        {
                            nP1X = max(MIN_X, nP1X - (bAPressed ? 1 : 1000));
                            SetP1X(hMBAAHandle, dwBaseAddress, nP1X);
                        }
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                        {
                            nP1X = min(MAX_X, nP1X + (bAPressed ? 1 : 1000));
                            SetP1X(hMBAAHandle, dwBaseAddress, nP1X);
                        }

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                        {
                            nP3X = max(MIN_X, nP3X - (bAPressed ? 1 : 1000));
                            SetP3X(hMBAAHandle, dwBaseAddress, nP3X);
                        }
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                        {
                            nP3X = min(MAX_X, nP3X + (bAPressed ? 1 : 1000));
                            SetP3X(hMBAAHandle, dwBaseAddress, nP3X);
                        }

                        if (nOldAirRecoveryIndex == -1)
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                        else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                        {
                            nP2X = max(MIN_X, nP2X - (bAPressed ? 1 : 1000));
                            SetP2X(hMBAAHandle, dwBaseAddress, nP2X);
                        }
                        else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                        {
                            nP2X = min(MAX_X, nP2X + (bAPressed ? 1 : 1000));
                            SetP2X(hMBAAHandle, dwBaseAddress, nP2X);
                        }

                        if (nOldDownRecoveryIndex == -1)
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
                        else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                        {
                            nP4X = max(MIN_X, nP4X - (bAPressed ? 1 : 1000));
                            SetP4X(hMBAAHandle, dwBaseAddress, nP4X);
                        }
                        else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                        {
                            nP4X = min(MAX_X, nP4X + (bAPressed ? 1 : 1000));
                            SetP4X(hMBAAHandle, dwBaseAddress, nP4X);
                        }

                        if (nOldThrowRecoveryIndex == -1)
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                        else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                        {
                        }
                        else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                        {
                            nP1X *= -1;
                            nP2X *= -1;
                            nP3X *= -1;
                            nP4X *= -1;
                            SetP1X(hMBAAHandle, dwBaseAddress, nP1X);
                            SetP2X(hMBAAHandle, dwBaseAddress, nP2X);
                            SetP3X(hMBAAHandle, dwBaseAddress, nP3X);
                            SetP4X(hMBAAHandle, dwBaseAddress, nP4X);
                        }
                    }
                    else if (nExtendedSettingsPage == CHARACTER_SPECIFICS)
                    {
                        if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                        {
                            nSionBullets = min(nSionBullets + 1, MAX_BULLETS + 2);
                            SetSionBullets(hMBAAHandle, dwBaseAddress, nSionBullets);
                        }
                        else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                        {
                            nSionBullets = max(1, nSionBullets - 1);
                            SetSionBullets(hMBAAHandle, dwBaseAddress, nSionBullets);
                        }

                        if (nOldEnemyDefenseIndex == -1)
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                        else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                        {
                            nRoaVisibleCharge = max(-2, nRoaVisibleCharge - 1);
                            SetRoaVisibleCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge);
                        }
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                        {
                            nRoaVisibleCharge = min(nRoaVisibleCharge + 1, MAX_CHARGE);
                            SetRoaVisibleCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge);
                        }

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                        {
                            nRoaHiddenCharge = max(-2, nRoaHiddenCharge - 1);
                            SetRoaHiddenCharge(hMBAAHandle, dwBaseAddress, nRoaHiddenCharge);
                        }
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                        {
                            nRoaHiddenCharge = min(nRoaHiddenCharge + 1, MAX_CHARGE);
                            SetRoaHiddenCharge(hMBAAHandle, dwBaseAddress, nRoaHiddenCharge);
                        }

                        /*if (nOldThrowRecoveryIndex == -1)
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                        else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                        {
                            nHitsTillBurst = max(0, nHitsTillBurst - 1);
                            if (nHitsTillBurst == 0)
                                nHitsTillBurst = TOO_HIGH_TO_BURST;
                        }
                        else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                        {
                            if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                                nHitsTillBurst = 1;
                            else
                                nHitsTillBurst = min(MAX_BURST, nHitsTillBurst + 1);
                        }*/
                    }

                    // PAGE number handler
                    if (nOldReduceDamageIndex == -1)
                        nOldReduceDamageIndex = nReduceDamageIndex;
                    else if (nOldReduceDamageIndex > nReduceDamageIndex)// left
                    {
                        nExtendedSettingsPage = max(1, nExtendedSettingsPage - 1);

                        nOldEnemyActionIndex = -1;
                        nOldEnemyDefenseIndex = -1;
                        nOldEnemyDefenseTypeIndex = -1;
                        nOldAirRecoveryIndex = -1;
                        nOldDownRecoveryIndex = -1;
                        nOldThrowRecoveryIndex = -1;
                        nOldReduceDamageIndex = -1;
                    }
                    else if (nOldReduceDamageIndex < nReduceDamageIndex)// right
                    {
                        nExtendedSettingsPage = min(nExtendedSettingsPage + 1, MAX_PAGES);

                        nOldEnemyActionIndex = -1;
                        nOldEnemyDefenseIndex = -1;
                        nOldEnemyDefenseTypeIndex = -1;
                        nOldAirRecoveryIndex = -1;
                        nOldDownRecoveryIndex = -1;
                        nOldThrowRecoveryIndex = -1;
                        nOldReduceDamageIndex = -1;
                    }

                    // Replacing dynamic menu fields
                    if (nExtendedSettingsPage == REVERSALS_PAGE)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (nReversalType == REVERSAL_NORMAL)
                        {
                            char pcTemp[7] = "NORMAL";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else if (nReversalType == REVERSAL_REPEAT)
                        {
                            char pcTemp[7] = "REPEAT";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 7, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }
                        else if (nReversalType == REVERSAL_RANDOM)
                        {
                            char pcTemp[7] = "RANDOM";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 2;
                        }

                        if (nReversalIndex1 == 0)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nReversalIndex1 == vPatternNames.size() - 1)
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex1].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 19, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex1].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nReversalIndex2 == 0)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nReversalIndex2 == vPatternNames.size() - 1)
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex2].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex2].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }

                        if (nReversalIndex3 == 0)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 0;
                        }
                        else if (nReversalIndex3 == vPatternNames.size() - 1)
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex3].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 19, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex3].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 2;
                        }

                        if (nReversalIndex4 == 0)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 0;
                        }
                        else if (nReversalIndex4 == vPatternNames.size() - 1)
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex4].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 19, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[19];
                            strcpy_s(pcTemp, vPatternNames[nReversalIndex4].c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 2;
                        }

                        if (nReversalDelayFrames == 0)
                        {
                            char pcTemp[2] = "0";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryNormalString), &pcTemp, 2, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcTemp, 2, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 0;
                        }
                        else if (nReversalDelayFrames == MAX_REVERSAL_DELAY)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nReversalDelayFrames).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcTemp, 3, 0);

                            nWriteBuffer = 6;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 6;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nReversalDelayFrames).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllRandomString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 2;
                        }
                    }
                    else if (nExtendedSettingsPage == STATS_PAGE)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        /*if (!bRandomReversal)
                        {
                            char pcTemp[7] = "NORMAL";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else if (bRandomReversal)
                        {
                            char pcTemp[7] = "RANDOM";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 7, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }*/

                        if (nExGuardSetting == eEnemyOffOnRandom::OFF)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nExGuardSetting == eEnemyOffOnRandom::RANDOM)
                        {
                            char pcTemp[7] = "RANDOM";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[3] = "ON";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (bInfGuard)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcInfinite_10, 10, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcInfinite_10, 10, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcNormal_7, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }

                        if (nCustomMeter == 0)
                        {
                            char pcTemp[5] = "0.0%";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 5, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 5, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 0;
                        }
                        else if (nCustomMeter == MAX_METER)
                        {
                            char pcTemp[7] = "300.0%";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, (std::to_string(nCustomMeter / 100) + "." + std::to_string(nCustomMeter % 100 / 10) + "%").c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 2;
                        }

                        if (nCustomHealth == 0)
                        {
                            char pcTemp[7] = "0 (0%)";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 0;
                        }
                        else if (nCustomHealth == MAX_HEALTH)
                        {
                            char pcTemp[13] = "11400 (100%)";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 13, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 13, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[19];
                            int nPercent = (int)((float)nCustomHealth / 11400.0f * 100.0f);
                            strcpy_s(pcTemp, (std::to_string(nCustomHealth) + " (" + std::to_string(nPercent) + ")%").c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 2;
                        }

                        if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryNormalString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 0;

                            nWriteBuffer = eEnemyStatus::STAND;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                        }
                        else if (nHitsTillBurst == MAX_BURST)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcTemp, 3, 0);

                            nWriteBuffer = 6;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 6;

                            nWriteBuffer = eEnemyStatus::MANUAL;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                            nWriteBuffer = eMagicCircuit::UNLIMITED;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nHitsTillBurst).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllRandomString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomFastString), &pcTemp, 3, 0);

                            nWriteBuffer = 3;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 3;

                            nWriteBuffer = eEnemyStatus::MANUAL;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                            nWriteBuffer = eMagicCircuit::UNLIMITED;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                        }
                    }
                    else if (nExtendedSettingsPage == POSITIONS_PAGE)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                        if (bP3Exists)
                            nWriteBuffer = ONSCREEN_LOCATION;
                        else
                            nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                        if (bP4Exists)
                            nWriteBuffer = ONSCREEN_LOCATION;
                        else
                            nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (!bPositionsLocked)
                        {
                            char pcTemp[9] = "UNLOCKED";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcTemp, 9, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 9, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else
                        {
                            char pcTemp[7] = "LOCKED";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 7, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }

                        if (nP1X == MIN_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 8, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nP1X == MAX_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 8, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(nP1X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nP2X == MIN_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 8, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nP2X == MAX_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(nP3X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 8, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }

                        if (nP3X == MIN_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 8, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 0;
                        }
                        else if (nP3X == MAX_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 8, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(nP2X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 2;
                        }
                            
                        if (nP4X == MIN_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 8, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 0;
                        }
                        else if (nP4X == MAX_X)
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 8, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, std::to_string(nP4X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 2;
                        }

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                        nThrowRecoveryIndex = 0;
                    }
                    else if (nExtendedSettingsPage == CHARACTER_SPECIFICS)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);

                        nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (nSionBullets == MAX_BULLETS + 2)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcInfinite_10, 10, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcInfinite_10, 10, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else if (nSionBullets == MAX_BULLETS + 1)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcNormal_7, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 2;
                        }
                        else if (nSionBullets == 1)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcOne_2, 2, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcOne_2, 2, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nSionBullets).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 2;
                        }
                            
                        if (nRoaVisibleCharge == -2)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcInfinite_10, 10, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcInfinite_10, 10, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nRoaVisibleCharge == -1)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcNormal_7, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }
                        else if (nRoaVisibleCharge == MAX_CHARGE)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaVisibleCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nRoaHiddenCharge == -2)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcInfinite_10, 10, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcInfinite_10, 10, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nRoaHiddenCharge == -1)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcNormal_7, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcNormal_7, 7, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }
                        else if (nRoaHiddenCharge == MAX_CHARGE)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }
                    }

                    // Replace PAGE text
                    if (nExtendedSettingsPage == 1)
                    {
                        char pcTemp[7] = "PAGE 1";
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                        nReduceDamageIndex = 0;
                    }
                    else if (nExtendedSettingsPage == MAX_PAGES)
                    {
                        char pcTemp[7];
                        strcpy_s(pcTemp, ("PAGE " + std::to_string(MAX_PAGES)).c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                        nReduceDamageIndex = 2;
                    }
                    else
                    {
                        char pcTemp[7];
                        strcpy_s(pcTemp, ("PAGE " + std::to_string(nExtendedSettingsPage)).c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                        nWriteBuffer = 1;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                        nReduceDamageIndex = 1;
                    }

                    nOldEnemyActionIndex = nEnemyActionIndex;
                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                    nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                    nOldReduceDamageIndex = nReduceDamageIndex;
                }

                /// ENEMY SETTINGS
                if (!bOnExtendedSettingsMenu && nCurrentSubMenu == eMenu::ENEMY_SETTINGS)
                {
                    DWORD dwEnemyActionAString = GetEnemyActionActionAStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionBString = GetEnemyActionActionBStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionCString = GetEnemyActionActionCStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyRecoverCString = GetEnemyActionRecoverCStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyRecoverDString = GetEnemyActionRecoverDStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwEnemyActionString = GetEnemyActionStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwEnemyActionIndex;
                    DWORD dwEnemyDefenseIndex;
                    DWORD dwEnemyDefenseTypeIndex;
                    DWORD dwAirRecoveryIndex;
                    DWORD dwDownRecoveryIndex;
                    DWORD dwThrowRecoveryIndex;
                    DWORD dwReduceDamageIndex;
                    do
                    {
                        dwEnemyActionIndex = GetEnemyActionIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwThrowRecoveryIndex = GetThrowRecoveryIndexAddress(hMBAAHandle, dwBaseAddress);
                        dwReduceDamageIndex = GetReduceDamageIndexAddress(hMBAAHandle, dwBaseAddress);

                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                        nCurrentSubMenu = nReadResult;

                    } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58 || dwThrowRecoveryIndex == 0x58 || dwReduceDamageIndex == 0x58) && nCurrentSubMenu == eMenu::ENEMY_SETTINGS);

                    if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                        continue;

                    // enemy action.
                    char pcEnemyActionInfo[16] = "enemy reversal.";
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionInfoStringAddress), &pcEnemyActionInfo, 16, 0);

                    // ENEMY ACTION
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcEnemyReversal_15, 15, 0);

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nReadResult, 4, 0);
                    int nEnemyActionIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseTypeIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nReadResult, 4, 0);
                    int nAirRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nReadResult, 4, 0);
                    int nDownRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nReadResult, 4, 0);
                    int nThrowRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nReadResult, 4, 0);
                    int nReduceDamageIndex = nReadResult;

                    if (nOldEnemyActionIndex == -1)
                        nOldEnemyActionIndex = nEnemyActionIndex;
                    else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                        nReversalIndex1 = max(0, nReversalIndex1 - 1);
                    else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                        nReversalIndex1 = min(nReversalIndex1 + 1, vPatternNames.size() - 1);

                    if (nReversalIndex1 == 0)
                    {
                        char pcTemp[4] = "OFF";
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 4, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                        nEnemyActionIndex = 0;
                    }
                    else if (nReversalIndex1 == vPatternNames.size() - 1)
                    {
                        char pcTemp[19];
                        strcpy_s(pcTemp, vPatternNames[nReversalIndex1].c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 19, 0);

                        nWriteBuffer = 8;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                        nEnemyActionIndex = 8;
                    }
                    else
                    {
                        char pcTemp[19];
                        strcpy_s(pcTemp, vPatternNames[nReversalIndex1].c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                        nEnemyActionIndex = 2;
                    }

                    nOldEnemyActionIndex = nEnemyActionIndex;

                    nStoredEnemyDefense = nEnemyDefenseIndex;
                    nStoredEnemyAction = nEnemyActionIndex;
                    nStoredEnemyDefenseType = nEnemyDefenseTypeIndex;
                    nStoredAirRecovery = (nAirRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                    nStoredDownRecovery = (nDownRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                    nStoredThrowRecovery = nThrowRecoveryIndex;
                    nStoredReduceDamage = nReduceDamageIndex;
                }

                // BATTLE SETTINGS
                if (nCurrentSubMenu == eMenu::BATTLE_SETTINGS)
                {
                    DWORD dwNoRecoverString = GetNoRecoverStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwRecover25String = GetRecover25StringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwRecover50String = GetRecover50StringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwRecover75String = GetRecover75StringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwRecover100String = GetRecover100StringAddress(hMBAAHandle, dwBaseAddress);

                    DWORD dwLifeIndex;
                    do
                    {
                        dwLifeIndex = GetLifeIndexAddress(hMBAAHandle, dwBaseAddress);

                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                        nCurrentSubMenu = nReadResult;

                    } while (dwLifeIndex == 0x58 && nCurrentSubMenu == eMenu::BATTLE_SETTINGS);

                    if (nCurrentSubMenu != eMenu::BATTLE_SETTINGS)
                        continue;

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwLifeIndex), &nReadResult, 4, 0);
                    int nLifeIndex = nReadResult;

                    if (nOldLifeIndex == -1)
                        nOldLifeIndex = nLifeIndex;

                    // left
                    else if (nOldLifeIndex > nLifeIndex)
                        bLifeRecover = true;
                    // right
                    else if (nOldLifeIndex < nLifeIndex)
                        bLifeRecover = false;

                    if (bLifeRecover)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwNoRecoverString), &pcRecover_8, 8, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwRecover25String), &pcRecover_8, 8, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwLifeIndex), &nWriteBuffer, 4, 0);
                        nLifeIndex = 0;
                    }
                    else
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwRecover75String), &pcRecover_11, 11, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwRecover100String), &pcRecover_11, 11, 0);

                        nWriteBuffer = 4;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwLifeIndex), &nWriteBuffer, 4, 0);
                        nLifeIndex = 4;
                    }

                    nOldLifeIndex = nLifeIndex;
                }
            }
            else // not paused
            {
                // want to reset these for a clean setup next time the game is paused
                bOnExtendedSettingsMenu = false;
                nOldCurrentSubMenu = -1;
                nCurrentSubMenu = eMenu::MAIN;

                // Enable Ex Guard.  randomly if applicable
                nWriteBuffer = 1;
                if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                    nWriteBuffer = 10;
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwExGuard + dwP2Offset), &nWriteBuffer, 4, 0);

                // Disable built-in health recovery
                nWriteBuffer = 4;
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwLifeRecover), &nWriteBuffer, 4, 0);

                // Disable built-in reversal action
                nWriteBuffer = 0;
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);

                // This locks all the code that follows to the framerate of the game
                // put code that needs to run faster above this
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
                nFrameCounter = nReadResult;
                if (nFrameCounter == nOldFrameCounter)
                    continue;
                nOldFrameCounter = nFrameCounter;

                // populate the reversal patterns list and character data
                // may be able to remove the timer condition, since CSS clears patternname list
                if (nFrameCounter == 0 || vPatternNames.size() == 1)
                {
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1CharNumber), &nReadResult, 4, 0);
                    nP1CharacterNumber = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2CharNumber), &nReadResult, 4, 0);
                    nP2CharacterNumber = nReadResult;

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1CharMoon), &nReadResult, 4, 0);
                    nP1Moon = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2CharMoon), &nReadResult, 4, 0);
                    nP2Moon = nReadResult;

                    nP1CharacterID = 10 * nP1CharacterNumber + nP1Moon;
                    nP2CharacterID = 10 * nP2CharacterNumber + nP2Moon;

                    vPatternNames = GetPatternList(nP2CharacterID);
                }

                // this works but only by disabling hit particles
                /*ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstopRemaining + dwP2Offset), &nReadResult, 4, 0);
                int nHitstopRemaining_temp = nReadResult;
                if (nReadResult > 10000)
                {
                    nWriteBuffer = 0;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstopRemaining + dwP2Offset), &nWriteBuffer, 4, 0);
                }*/

                // burst a combo
                if (nHitsTillBurst != TOO_HIGH_TO_BURST)
                {
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwComboCount), &nReadResult, 4, 0);
                    int nComboCount = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstopRemaining + dwP2Offset), &nReadResult, 1, 0);
                    int nHitstopRemaining = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstunRemaining + dwP2Offset), &nReadResult, 4, 0);
                    int nHitstunRemaining = nReadResult;
                    if (nComboCount >= nHitsTillBurst && nHitstopRemaining == 0 && nHitstunRemaining != 0)
                    {
                        while (true)
                        {
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstunRemaining + dwP2Offset), &nReadResult, 4, 0);
                            nHitstunRemaining = nReadResult;
                            if (nHitstunRemaining == 0)
                            {
                                Sleep(16);  //need to wait 1 frame
                                nWriteBuffer = 0;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwComboCount), &nWriteBuffer, 4, 0);
                                break;
                            }

                            // mash the hell out of E
                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2E), &nWriteBuffer, 1, 0);

                        }
                    }
                }

                // reset resources to the preset when training mode is reset
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGuardSetting), &nReadResult, 4, 0);
                nCustomGuard = nReadResult;
                if (nFrameCounter == 1)
                {
                    SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                    SetGuard(hMBAAHandle, dwBaseAddress, nCustomGuard, nP1Moon, nP2Moon);
                    SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon);

                    if (bPositionsLocked)
                    {
                        nWriteBuffer = nP1X;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nWriteBuffer, 4, 0);
                        nWriteBuffer = nP2X;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset), &nWriteBuffer, 4, 0);
                        nWriteBuffer = nP3X;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 2), &nWriteBuffer, 4, 0);
                        nWriteBuffer = nP4X;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 3), &nWriteBuffer, 4, 0);
                    }
                }

                // infinite guard bar
                if (bInfGuard)
                    SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon);

                // increase the counter every frame p2 is standing idle to delay regenerating health and char specifics
                // taking an extra step to cap these at 20 to avoid any unexpected behavior if tmode is left running forever
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                if (nReadResult == 0)
                {
                    nHealthRefillTimer = min(nHealthRefillTimer + 1, 20);
                    nSionBulletsRefillTimer = min(nSionBulletsRefillTimer + 1, 20);
                    
                    // roa needs to be reset a little differently
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge), &nReadResult, 4, 0);
                    if (nReadResult < nRoaVisibleCharge)
                        nRoaVisibleChargeRefillTimer = min(nRoaVisibleChargeRefillTimer + 1, 20);
                    else
                        nRoaVisibleChargeRefillTimer = 0;

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge), &nReadResult, 4, 0);
                    if (nReadResult < nRoaVisibleCharge)
                        nRoaHiddenChargeRefillTimer = min(nRoaHiddenChargeRefillTimer + 1, 20);
                    else
                        nRoaHiddenChargeRefillTimer = 0;
                }
                else
                {
                    nHealthRefillTimer = 0;
                    nSionBulletsRefillTimer = 0;
                }

                

                // refill health if training mode is reset or long enough time has passed
                if (nFrameCounter == 1 || (nHealthRefillTimer == 20 && bLifeRecover))
                {
                    SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth);
                    nHealthRefillTimer = 0;
                }

                // refill character specifics
                if ((nFrameCounter == 1 || nSionBulletsRefillTimer == 20 || nSionBullets == 15) && nSionBullets != 14)
                {
                    SetSionBullets(hMBAAHandle, dwBaseAddress, nSionBullets);
                    nSionBulletsRefillTimer = 0;
                }

                // refill character specifics
                if (nFrameCounter == 1 || nRoaHiddenChargeRefillTimer == 20 || nRoaHiddenCharge == -2 && nRoaHiddenCharge != -1)
                {
                    SetRoaHiddenCharge(hMBAAHandle, dwBaseAddress, nRoaHiddenCharge);
                    nRoaHiddenChargeRefillTimer = 0;
                }

                // refill character specifics
                if ((nFrameCounter == 1 || nRoaVisibleChargeRefillTimer == 20 || nRoaVisibleCharge == -2) && nRoaVisibleCharge != -1)
                {
                    SetRoaVisibleCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge);
                    nRoaVisibleChargeRefillTimer = 0;
                }

                // easy access debug screen clear
                if (GetKeyState(VK_RETURN) < 0)
                {
                    //system("cls");
                }

                // convoluted reversal pattern logic
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwBurstCooldown), &nReadResult, 1, 0);
                nBurstCooldown = nReadResult;

                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Y + dwP2Offset), &nReadResult, 4, 0);
                nP2Y = nReadResult;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
                nMot = nReadResult;
                if (nFrameCounter == 0)
                    bReversaled = true;
                if (nFrameCounter == 2)
                    bReversaled = false;

                // extra check for current pattern == reversal pattern

                if (nFrameCounter != 0 && (GetPattern(nP2CharacterID, vPatternNames[nReversalIndex1]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex2]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex3]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex4]) != 0))
                {
                    if (!bDelayingReversal && nMot == 0 && (nMot != nOldMot || nReversalType == REVERSAL_REPEAT) && nP2Y == 0)
                    {
                        if (!bReversaled)
                        {
                            bDelayingReversal = true;
                            nTempReversalDelayFrames = nReversalDelayFrames;
                        }
                        else
                            bReversaled = false;

                        if (nReversalType == REVERSAL_REPEAT)
                            bDelayingReversal = true;
                    }
                }
                nOldMot = nMot;

                if (nMot != 0)
                {
                    bDelayingReversal = false;
                }

                if (bDelayingReversal)
                {
                    if (nTempReversalDelayFrames == 0)
                    {
                        
                        bDelayingReversal = false;
                        bReversaled = true;
                        if (nReversalType != REVERSAL_RANDOM || rand() % 2 == 0)
                        {
                            int pnReversals[4] = { GetPattern(nP2CharacterID, vPatternNames[nReversalIndex1]), GetPattern(nP2CharacterID, vPatternNames[nReversalIndex2]), GetPattern(nP2CharacterID, vPatternNames[nReversalIndex3]), GetPattern(nP2CharacterID, vPatternNames[nReversalIndex4]) };
                            int nRandomReversalIndex = 0;
                            while (1)
                            {
                                nRandomReversalIndex = rand() % 4;
                                if (pnReversals[nRandomReversalIndex] != 0)
                                    break;
                            }
                            nWriteBuffer = pnReversals[nRandomReversalIndex];
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                        }
                        else
                            bReversaled = false;
                    }
                    else
                        nTempReversalDelayFrames--;
                }

                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
                if (bSwitchToCrouch && nReadResult == eEnemyStance::STANDGUARDING)
                {
                    // TODO: Fuzzy Overhead
                }
            }
        }
    }

    CloseLogger();
    CloseHandle(hMBAAHandle);
    return 0;
}
