#include "MBAACC-Extended-Training-Mode.h"

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
    bool bRandomReversal = false;
    int nReversalIndex = 0;
    std::vector<std::string> vPatternNames = GetEmptyPatternList();
    int nMot = 0;
    int nOldMot = 0;
    int nP2Y = 0;

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
    int nSionBullets = 13;
    int nRoaVisibleCharge = 0;
    int nRoaHiddenCharge = 0;

    int nExtendedSettingsPage = 1;
    
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

    PointerManager oPointerManager = PointerManager();

    std::srand((unsigned int)std::time(nullptr));

    sOnlineVersion = GetLatestVersion();
    if (sOnlineVersion != "" && sOnlineVersion != VERSION)
        bNeedToAnnounceNewVersion = true;

    hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsoleHandle, &cursorInfo);

  
    while (1)
    {
        std::cout.flush();

        nCurrentTime = std::time(nullptr);

        SetConsoleCursorPosition(hConsoleHandle, { 0, 0 });
        std::cout << "===========================================================================" << std::endl;
        std::cout << "|   Fang's Extended Training Mode Mod " << VERSION << "                                " << std::endl;
        std::cout << "|                                                                         |" << std::endl;
        std::cout << "|   " << GITHUB_RELEASE << "   |" << std::endl;
        if (bNeedToAnnounceNewVersion && nCurrentTime % 3 != 0)
            std::cout << "|   NEW VERSION " << sOnlineVersion << " AVAILABLE ON GITHUB                                  |" << std::endl;
        else
            std::cout << "|                                                                         |" << std::endl;
        std::cout << "===========================================================================" << std::endl;

        //MBAA.exe is not open || has closed.  259 exit code means still open
        GetExitCodeProcess(hMBAAHandle, &dwExitCode);
        if (hMBAAHandle == 0x0 || dwExitCode != 259)
        {
            hMBAAHandle = 0x0;

            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::string sLookingForMelty = "Looking for MBAA.exe                 ";
            for (int i = 0; i < nCurrentTime % 8; i++)
                sLookingForMelty[20 + i] = '.';
            std::cout << sLookingForMelty;

            hMBAAHandle = GetProcessByName(L"MBAA.exe");

            // don't do anything until we re-attach to mbaa
            if (hMBAAHandle == 0x0)
                continue;

            SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
            std::cout << "                                              ";

            Sleep(100);
            dwBaseAddress = GetBaseAddressByName(hMBAAHandle, L"MBAA.exe");

            InitializeCharacterMaps();
        }

        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
        if (nReadResult == 0)
        {
            nReversalIndex = 0;
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
                if (nCurrentSubMenu != nOldCurrentSubMenu)
                {
                    Sleep(32); // it takes a moment for a couple pointers to initialize between menus.  wait 2 frames for safety
                    oPointerManager.InitializePointers(hMBAAHandle, dwBaseAddress);
                }

                // Replace the menu title and VIEW SCREEN
                char pcTrainingMenu[19];
                strcpy_s(pcTrainingMenu, ("EXTENDED MOD " + VERSION).c_str());
                WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwViewScreenStringAddress), &pcExtendedSettings_18, 18, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwTrainingMenuString), &pcTrainingMenu, 19, 0);

                //DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hMBAAHandle, dwBaseAddress);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwSubMenuAddress), &nReadResult, 4, 0);
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
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReturnToMainMenuString), &pcNewVersionMessage, 19, 0);
                    }
                    else
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReturnToMainMenuString), &pcLatestVersion_19, 19, 0);
                    }
                }

                // Turn VIEW SCREEN into EXTENDED SETTINGS
                if (nCurrentSubMenu == eMenu::VIEW_SCREEN)
                {
                    nWriteBuffer = eMenu::ENEMY_SETTINGS;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwSubMenuAddress), &nWriteBuffer, 4, 0);

                    bOnExtendedSettingsMenu = true;
                }
                else if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                    bOnExtendedSettingsMenu = false;

                if (bOnExtendedSettingsMenu)
                {
                    // Index values for menu items
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nReadResult, 4, 0);
                    nOldEnemySettingsCursor = nEnemySettingsCursor;
                    nEnemySettingsCursor = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nReadResult, 4, 0);
                    int nEnemyActionIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseTypeIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nReadResult, 4, 0);
                    int nAirRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nReadResult, 4, 0);
                    int nDownRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nReadResult, 4, 0);
                    int nThrowRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageIndex), &nReadResult, 4, 0);
                    int nReduceDamageIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyStatusIndex), &nReadResult, 4, 0);
                    int nEnemyStatusIndex = nReadResult;

                    // Reset hits till burst if status is not manual
                    if (nEnemyStatusIndex != eEnemyStatus::MANUAL)
                        nHitsTillBurst = TOO_HIGH_TO_BURST;

                    // Replace static menu fields
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionInfoStringAddress), &pcTrainingPreset_17, 17, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsMenuTitle), &pcExtendedSettings_18, 18, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageString), &pcBlank_1, 1, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDefaultString), &pcBlank_1, 1, 0);
                    if (nExtendedSettingsPage == 1)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionString), &pcReversalType_14, 14, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseString), &pcExGuard_9, 9, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeStringAddress), &pcReversalDelay_15, 15, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryString), &pcMeter_18, 18, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryString), &pcHealth_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryString), &pcGuardBar_10, 10, 0);
                    }
                    else if (nExtendedSettingsPage == 2)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionString), &pcASlow_12, 12, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseString), &pcP1XLoc_11, 11, 0);
                        if (bP3Exists)
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeStringAddress), &pcAssistLoc_13, 13, 0);
                        else
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeStringAddress), pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryString), &pcP2XLoc_11, 11, 0);
                        if (bP4Exists)
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryString), &pcAssistLoc_13, 13, 0);
                        else
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryString), pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryString), &pcUnstable_11, 11, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryNormalString), &pcInvert_7, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryAllFastString), &pcInvert_7, 7, 0);
                    }
                    else if (nExtendedSettingsPage == 3)
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionString), &pcSionBullets_13, 13, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseString), &pcRoaVisibleCharge_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeStringAddress), &pcRoaHiddenCharge_19, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryString), &pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryString), &pcBlank_1, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryString), &pcHitsUntilBurst_17, 17, 0);
                    }

                    // Skip DEFAULT
                    if (nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 12)
                    {
                        nWriteBuffer = 14;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                        nEnemySettingsCursor = 14;
                        nOldEnemySettingsCursor = 14;
                    }
                    else if (nOldEnemySettingsCursor == 14 && nEnemySettingsCursor == 12)
                    {
                        nWriteBuffer = 10;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                        nEnemySettingsCursor = 10;
                        nOldEnemySettingsCursor = 10;
                    }

                    // Skipping blank menu items
                    if (nExtendedSettingsPage == 1)
                    {
                        // don't skip anything
                    }
                    else if (nExtendedSettingsPage == 2)
                    {
                        if (bPositionsLocked && nOldEnemySettingsCursor == 0 && nEnemySettingsCursor == 2)
                        {
                            nWriteBuffer = 10;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 10;
                            nOldEnemySettingsCursor = 10;
                        }
                        if (bPositionsLocked && nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 8)
                        {
                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 0;
                            nOldEnemySettingsCursor = 0;
                        }
                        if (!bP3Exists && nOldEnemySettingsCursor == 2 && nEnemySettingsCursor == 3)
                        {
                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 5;
                            nOldEnemySettingsCursor = 5;
                        }
                        else if (!bP3Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 3)
                        {
                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 2;
                            nOldEnemySettingsCursor = 2;
                        }
                        if (!bP4Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 8;
                            nOldEnemySettingsCursor = 8;
                        }
                        else if (!bP4Exists && nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 5;
                            nOldEnemySettingsCursor = 5;
                        }
                    }
                    else if (nExtendedSettingsPage == 3)
                    {
                        if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                        {
                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 8;
                            nOldEnemySettingsCursor = 8;
                        }
                        else if (nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                        {
                            nWriteBuffer = 3;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                            nEnemySettingsCursor = 3;
                            nOldEnemySettingsCursor = 3;
                        }
                    }

                    // Left/Right on menu handlers
                    if (nExtendedSettingsPage == 1)
                    {
                        if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                            bRandomReversal = false;
                        else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                            bRandomReversal = true;

                        if (nOldEnemyDefenseIndex == -1)
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                        else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                            nExGuardSetting = max(0, nExGuardSetting - 1);
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                            nExGuardSetting = min(nExGuardSetting + 1, eEnemyOffOnRandom::RANDOM);

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                            nReversalDelayFrames = max(0, nReversalDelayFrames - 1);
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                            nReversalDelayFrames = min(nReversalDelayFrames + 1, MAX_DELAY /*I chose this number arbitrarily*/);

                        if (nOldAirRecoveryIndex == -1)
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                        else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                        {
                            nCustomMeter = max(0, nCustomMeter - (bAPressed ? 10 : 1000));
                            SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                        }
                        else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                        {
                            nCustomMeter = min(nCustomMeter + (bAPressed ? 10 : 1000), MAX_METER /*yes I know this lets hmoons go over 200*/);
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
                            bInfGuard = true;
                        else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                            bInfGuard = false;
                    }
                    else if (nExtendedSettingsPage == 2)
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
                    else if (nExtendedSettingsPage == 3)
                    {
                        if (nOldEnemyActionIndex == -1)
                            nOldEnemyActionIndex = nEnemyActionIndex;
                        else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                        {
                            nSionBullets = min(nSionBullets + 1, MAX_BULLETS);
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
                            nRoaVisibleCharge = max(0, nRoaVisibleCharge - 1);
                            SetRoaCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                        }
                        else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                        {
                            nRoaVisibleCharge = min(nRoaVisibleCharge + 1, MAX_CHARGE);
                            SetRoaCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                        }

                        if (nOldEnemyDefenseTypeIndex == -1)
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                        else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                        {
                            nRoaHiddenCharge = max(0, nRoaHiddenCharge - 1);
                            SetRoaCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                        }
                        else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                        {
                            nRoaHiddenCharge = min(nRoaHiddenCharge + 1, MAX_CHARGE);
                            SetRoaCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
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
                    if (nExtendedSettingsPage == 1)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (!bRandomReversal)
                        {
                            char pcTemp[7] = "NORMAL";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else if (bRandomReversal)
                        {
                            char pcTemp[7] = "RANDOM";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverCString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverDString), &pcTemp, 7, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }
                                
                        if (nExGuardSetting == eEnemyOffOnRandom::OFF)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nExGuardSetting == eEnemyOffOnRandom::RANDOM)
                        {
                            char pcTemp[7] = "RANDOM";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[3] = "ON";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nReversalDelayFrames == 0)
                        {
                            char pcTemp[2] = "0";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 2, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 2, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nReversalDelayFrames == MAX_DELAY)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(MAX_DELAY).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nReversalDelayFrames).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }

                        if (nCustomMeter == 0)
                        {
                            char pcTemp[5] = "0.0%";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryOffString), &pcTemp, 5, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryNeutralString), &pcTemp, 5, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 0;
                        }
                        else if (nCustomMeter == MAX_METER)
                        {
                            char pcTemp[7] = "300.0%";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryRandom1String), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryRandom2String), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[8];
                            strcpy_s(pcTemp, (std::to_string(nCustomMeter / 100) + "." + std::to_string(nCustomMeter % 100 / 10) + "%").c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryNeutralString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryBackString), &pcTemp, 8, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryForwardString), &pcTemp, 8, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 2;
                        }

                        if (nCustomHealth == 0)
                        {
                            char pcTemp[7] = "0 (0%)";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 0;
                        }
                        else if (nCustomHealth == MAX_HEALTH)
                        {
                            char pcTemp[13] = "11400 (100%)";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryRandom1String), &pcTemp, 13, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryRandom2String), &pcTemp, 13, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[19];
                            int nPercent = (int)((float)nCustomHealth / 11400.0f * 100.0f);
                            strcpy_s(pcTemp, (std::to_string(nCustomHealth) + " (" + std::to_string(nPercent) + ")%").c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryBackString), &pcTemp, 19, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryForwardString), &pcTemp, 19, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 2;
                        }

                        if (bInfGuard)
                        {
                            char pcTemp[9] = "INFINITE";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryNormalString), &pcTemp, 9, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryAllFastString), &pcTemp, 9, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 0;
                        }
                        else
                        {
                            char pcTemp[7] = "NORMAL";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryRandomLateString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryRandomRandomString), &pcTemp, 7, 0);

                            nWriteBuffer = 6;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 6;
                        }
                    }
                    else if (nExtendedSettingsPage == 2)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                        if (bP3Exists)
                            nWriteBuffer = ONSCREEN_LOCATION;
                        else
                            nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                        if (bP4Exists)
                            nWriteBuffer = ONSCREEN_LOCATION;
                        else
                            nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (!bPositionsLocked)
                        {
                            char pcTemp[9] = "UNLOCKED";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionOffString), &pcTemp, 9, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 9, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else
                        {
                            char pcTemp[7] = "LOCKED";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverCString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverDString), &pcTemp, 7, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }

                        if (nP1X == MIN_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nP1X == MAX_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(nP1X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusGuardString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nP2X == MIN_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nP2X == MAX_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(nP3X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }

                        if (nP3X == MIN_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryNeutralString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 0;
                        }
                        else if (nP3X == MAX_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryRandom1String), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryRandom2String), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(nP2X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryNeutralString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryBackString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryForwardString), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                            nAirRecoveryIndex = 2;
                        }
                            
                        if (nP4X == MIN_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryOffString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 0;
                        }
                        else if (nP4X == MAX_X)
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryRandom1String), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryRandom2String), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 5;
                        }
                        else
                        {
                            char pcTemp[7];
                            strcpy_s(pcTemp, std::to_string(nP4X).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryNeutralString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryBackString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryForwardString), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                            nDownRecoveryIndex = 2;
                        }

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                        nThrowRecoveryIndex = 0;
                    }
                    else if (nExtendedSettingsPage == 3)
                    {
                        nWriteBuffer = ONSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);

                        nWriteBuffer = OFFSCREEN_LOCATION;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                        if (nSionBullets == MAX_BULLETS)
                        {
                            char pcTemp[3] = "13";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionOffString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 3, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 0;
                        }
                        else if (nSionBullets == 1)
                        {
                            char pcTemp[2] = "1";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverCString), &pcTemp, 2, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverDString), &pcTemp, 2, 0);

                            nWriteBuffer = 8;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 8;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nSionBullets).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionBString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionCString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                            nEnemyActionIndex = 2;
                        }
                            
                        if (nRoaVisibleCharge == 0)
                        {
                            char pcTemp[2] = "0";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseOffString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 0;
                        }
                        else if (nRoaVisibleCharge == MAX_CHARGE)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                            nWriteBuffer = 5;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 5;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaVisibleCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseIndex = 2;
                        }

                        if (nRoaHiddenCharge == 0)
                        {
                            char pcTemp[2] = "0";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 0;
                        }
                        else if (nRoaHiddenCharge == MAX_CHARGE)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                            nWriteBuffer = 2;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 2;
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                            nEnemyDefenseTypeIndex = 1;
                        }
                            
                        if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                        {
                            char pcTemp[4] = "OFF";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryNormalString), &pcTemp, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryAllFastString), &pcTemp, 4, 0);

                            nWriteBuffer = 0;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 0;

                            nWriteBuffer = eEnemyStatus::STAND;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                        }
                        else if (nHitsTillBurst == MAX_BURST)
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryRandomLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryRandomRandomString), &pcTemp, 3, 0);

                            nWriteBuffer = 6;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 6;

                            nWriteBuffer = eEnemyStatus::MANUAL;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                            nWriteBuffer = eMagicCircuit::UNLIMITED;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                        }
                        else
                        {
                            char pcTemp[3];
                            strcpy_s(pcTemp, std::to_string(nHitsTillBurst).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryAllLateString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryAllRandomString), &pcTemp, 3, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryRandomFastString), &pcTemp, 3, 0);

                            nWriteBuffer = 3;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                            nThrowRecoveryIndex = 3;

                            nWriteBuffer = eEnemyStatus::MANUAL;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                            nWriteBuffer = eMagicCircuit::UNLIMITED;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                        }
                    }

                    // Replace PAGE text
                    if (nExtendedSettingsPage == 1)
                    {
                        char pcTemp[7] = "PAGE 1";
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageNormalString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageAllString), &pcTemp, 7, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                        nReduceDamageIndex = 0;
                    }
                    else if (nExtendedSettingsPage == MAX_PAGES)
                    {
                        char pcTemp[7];
                        strcpy_s(pcTemp, ("PAGE " + std::to_string(MAX_PAGES)).c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageAllString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageRandomString), &pcTemp, 7, 0);

                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                        nReduceDamageIndex = 2;
                    }
                    else
                    {
                        char pcTemp[7];
                        strcpy_s(pcTemp, ("PAGE " + std::to_string(nExtendedSettingsPage)).c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageNormalString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageAllString), &pcTemp, 7, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageRandomString), &pcTemp, 7, 0);

                        nWriteBuffer = 1;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageIndex), &nWriteBuffer, 4, 0);
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
                    // enemy action.
                    char pcEnemyActionInfo[16] = "enemy reversal.";
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionInfoStringAddress), &pcEnemyActionInfo, 16, 0);

                    // ENEMY ACTION
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionString), &pcEnemyReversal_15, 15, 0);

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nReadResult, 4, 0);
                    int nEnemyActionIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                    int nEnemyDefenseTypeIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwAirRecoveryIndex), &nReadResult, 4, 0);
                    int nAirRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwDownRecoveryIndex), &nReadResult, 4, 0);
                    int nDownRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwThrowRecoveryIndex), &nReadResult, 4, 0);
                    int nThrowRecoveryIndex = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwReduceDamageIndex), &nReadResult, 4, 0);
                    int nReduceDamageIndex = nReadResult;

                    if (nOldEnemyActionIndex == -1)
                        nOldEnemyActionIndex = nEnemyActionIndex;
                    else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                        nReversalIndex = max(0, nReversalIndex - 1);
                    else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                        nReversalIndex = min(nReversalIndex + 1, vPatternNames.size() - 1);

                    if (nReversalIndex == 0)
                    {
                        char pcTemp[4] = "OFF";
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 4, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                        nEnemyActionIndex = 0;
                    }
                    else if (nReversalIndex == vPatternNames.size() - 1)
                    {
                        char pcTemp[19];
                        strcpy_s(pcTemp, vPatternNames[nReversalIndex].c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverCString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyRecoverDString), &pcTemp, 19, 0);

                        nWriteBuffer = 8;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                        nEnemyActionIndex = 8;
                    }
                    else
                    {
                        char pcTemp[19];
                        strcpy_s(pcTemp, vPatternNames[nReversalIndex].c_str());
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionAString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionBString), &pcTemp, 19, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionCString), &pcTemp, 19, 0);

                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwEnemyActionIndex), &nWriteBuffer, 4, 0);
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
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwLifeIndex), &nReadResult, 4, 0);
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
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwNoRecoverString), &pcRecover_8, 8, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwRecover25String), &pcRecover_8, 8, 0);

                        nWriteBuffer = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwLifeIndex), &nWriteBuffer, 4, 0);
                        nLifeIndex = 0;
                    }
                    else
                    {
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwRecover75String), &pcRecover_11, 11, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwRecover100String), &pcRecover_11, 11, 0);

                        nWriteBuffer = 4;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(oPointerManager.m_dwLifeIndex), &nWriteBuffer, 4, 0);
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

                // burst a combo
                if (nHitsTillBurst != TOO_HIGH_TO_BURST)
                {
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwComboCount), &nReadResult, 4, 0);
                    int nComboCount = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1HitstopRemaining + dwP2Offset), &nReadResult, 4, 0);
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
                    nCharSpecificsRefillTimer = min(nCharSpecificsRefillTimer + 1, 20);
                }
                else
                {
                    nHealthRefillTimer = 0;
                    nCharSpecificsRefillTimer = 0;
                }

                // refill health if training mode is reset or long enough time has passed
                if (nFrameCounter == 1 || (nHealthRefillTimer == 20 && bLifeRecover))
                {
                    SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth);
                    nHealthRefillTimer = 0;
                }

                // refill character specifics
                if (nFrameCounter == 1 || nCharSpecificsRefillTimer == 20)
                {
                    SetSionBullets(hMBAAHandle, dwBaseAddress, nSionBullets);
                    SetRoaCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                    nCharSpecificsRefillTimer = 0;
                }

                // easy access debug screen clear
                /*if (GetKeyState(VK_RETURN) < 0)
                {
                    system("cls");
                }*/

                // convoluted reversal pattern logic
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Y + dwP2Offset), &nReadResult, 4, 0);
                nP2Y = nReadResult;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
                nMot = nReadResult;
                if (nFrameCounter == 0)
                    bReversaled = true;
                if (nFrameCounter == 2)
                    bReversaled = false;
                if (nFrameCounter != 0 && GetPattern(nP2CharacterID, vPatternNames[nReversalIndex]) != 0)
                {
                    if (!bDelayingReversal && nMot == 0 && nMot != nOldMot && nP2Y == 0)
                    {
                        if (!bReversaled)
                        {
                            bDelayingReversal = true;
                            nTempReversalDelayFrames = nReversalDelayFrames;
                        }
                        else
                            bReversaled = false;
                    }
                }
                nOldMot = nMot;

                if (nMot != 0)
                    bDelayingReversal = false;

                if (bDelayingReversal)
                {
                    if (nTempReversalDelayFrames == 0)
                    {
                        bDelayingReversal = false;
                        nWriteBuffer = GetPattern(nP2CharacterID, vPatternNames[nReversalIndex]);
                        bReversaled = true;
                        if (!bRandomReversal || rand() % 2 == 0)
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
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

    CloseHandle(hMBAAHandle);
    return 0;
}
