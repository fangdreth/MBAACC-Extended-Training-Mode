#include "MBAACC-Extended-Training-Mode.h"

int main(int argc, char* argv[])
{
    try
    {
        DWORD dwExitCode = 0;

        int nReadResult = 0;
        int nWriteBuffer = 0;

        bool bPaused = false;
        bool bAPressed = false;
        bool bBPressed = false;
        bool bOldAPressed = false;
        bool bOldBPressed = false;
        bool bF1Pressed = false;
        bool bOldF1Pressed = false;
        bool bF2Pressed = false;
        bool bOldF2Pressed = false;

        bool bOnCSS = false;
        bool bOnSettingsMenu = false;
        bool bOnHotkeySettingsMenu = false;

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
        int nReversalType = REVERSAL_NORMAL;
        int nReversalIndex1 = 0;
        int nReversalIndex2 = 0;
        int nReversalIndex3 = 0;
        int nReversalIndex4 = 0;
        std::vector<std::string> vPatternNames = GetEmptyPatternList();
        std::vector<int> vAirReversals;
        std::vector<int> vGroundReversals;
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
        int nP1ControlledCharacter = 0;
        int nP2ControlledCharacter = 1;
        int nFMaidsHearts = 5;
        bool bRyougiInfiniteKnife = false;
        int nP1TagFlag = 0;
        int nP2TagFlag = 0;
        int nP3TagFlag = 1;
        int nP4TagFlag = 1;

        uint8_t nRNGMode = RNG_OFF;
        uint8_t nRNGRate = RNG_EVERY_FRAME;
        uint32_t nCustomSeed = 0;
        uint32_t nCustomRN = 0;

    

        int nSettingsPage = 1;
        int nHotkeyPage = 1;

        int nGameMode = 0;

        bool bPositionsLocked = false;
        int nP1X = P1_DEFAULT_X;
        int nP2X = P2_DEFAULT_X;
        int nP3X = P3_DEFAULT_X;
        int nP4X = P4_DEFAULT_X;
        bool bP3Exists = false;
        bool bP4Exists = false;

        bool bJustUnpaused = false;

        uint8_t nFrameData = FRAMEDISPLAY_NORMAL;
        //bool bSaveStates = false;

        std::unordered_set<DWORD> setBlockingAnimationPointers;
        std::unordered_set<DWORD> setIdleAnimationPointers;
        int nIdleHighlightSetting = NO_HIGHLIGHT;
        int nBlockingHighlightSetting = NO_HIGHLIGHT;
        int nHitHighlightSetting = NO_HIGHLIGHT;
        int nArmorHighlightSetting = NO_HIGHLIGHT;
        int nThrowProtectionHighlightSetting = NO_HIGHLIGHT;

        Player P1{ 1, dwBaseAddress + adP1Base };
        Player P2{ 2, dwBaseAddress + adP2Base };
        Player P3{ 3, dwBaseAddress + adP3Base };
        Player P4{ 4, dwBaseAddress + adP4Base };

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
        SetConsoleMode(hConsoleHandle, 7);

        std::cout << "\x1b]0;Extended Training " << VERSION << "\x07";
  
        try
        {
            InitializeLogger(hConsoleHandle);
        }
        catch (...)
        { 
        
        }
    
        InitializeCharacterMaps();

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

        CreateRegistryKey();
        ReadFromRegistry(L"FreezeKey", &nFreezeKey);
        if (MapVirtualKeyW(nFreezeKey, MAPVK_VK_TO_VSC) == 0)
            nFreezeKey = nDefaultFreezeKey;
        else
            bFreezeKeySet = true;
        ReadFromRegistry(L"FrameStepKey", &nFrameStepKey);
        if (MapVirtualKeyW(nFrameStepKey, MAPVK_VK_TO_VSC) == 0)
            nFrameStepKey = nDefaultFrameDataDisplayKey;
        else
            bFrameStepKeySet = true;
        ReadFromRegistry(L"HitboxesDisplayKey", &nHitboxDisplayKey);
        if (MapVirtualKeyW(nHitboxDisplayKey, MAPVK_VK_TO_VSC) == 0)
            nHitboxDisplayKey = nDefaultHitboxDisplayKey;
        else
            bHitboxDisplayKeySet = true;
        ReadFromRegistry(L"FrameDataDisplayKey", &nFrameDataDisplayKey);
        if (MapVirtualKeyW(nFrameDataDisplayKey, MAPVK_VK_TO_VSC) == 0)
            nFrameDataDisplayKey = nDefaultFrameDataDisplayKey;
        else
            bFrameDataDisplayKeySet = true;
        ReadFromRegistry(L"HighlightsOnKey", &nHighlightsOnKey);
        if (MapVirtualKeyW(nHighlightsOnKey, MAPVK_VK_TO_VSC) == 0)
            nHighlightsOnKey = nDefaultHighlightsOnKey;
        else
            bHighlightsOnKeySet = true;
        ReadFromRegistry(L"SaveStateKey", &nSaveStateKey);
        if (MapVirtualKeyW(nSaveStateKey, MAPVK_VK_TO_VSC) == 0)
            nSaveStateKey = nDefaultSaveStateKey;
        else
            bSaveStateKeySet = true;
        ReadFromRegistry(L"PrevSaveSlotKey", &nPrevSaveSlotKey);
        if (MapVirtualKeyW(nPrevSaveSlotKey, MAPVK_VK_TO_VSC) == 0)
            nPrevSaveSlotKey = nDefaultPrevSaveSlotKey;
        else
            bPrevSaveSlotKey = true;
        ReadFromRegistry(L"NextSaveSlotKey", &nNextSaveSlotKey);
        if (MapVirtualKeyW(nNextSaveSlotKey, MAPVK_VK_TO_VSC) == 0)
            nNextSaveSlotKey = nDefaultNextSaveSlotKey;
        else
            bNextSaveSlotKey = true;
        ReadFromRegistry(L"FrameBarScrollLeftKey", &nFrameBarScrollLeftKey);
        if (MapVirtualKeyW(nFrameBarScrollLeftKey, MAPVK_VK_TO_VSC) == 0)
            nFrameBarScrollLeftKey = nDefaultFrameBarScrollLeftKey;
        else
            bFrameBarScrollLeftKey = true;
        ReadFromRegistry(L"FrameBarScrollRightKey", &nFrameBarScrollRightKey);
        if (MapVirtualKeyW(nFrameBarScrollRightKey, MAPVK_VK_TO_VSC) == 0)
            nFrameBarScrollRightKey = nDefaultFrameBarScrollRightKey;
        else
            bFrameBarScrollRightKey = true;
        ReadFromRegistry(L"RNGIncKey", &nRNGIncKey);
        if (MapVirtualKeyW(nRNGIncKey, MAPVK_VK_TO_VSC) == 0)
            nRNGIncKey = nDefaultRNGIncKey;
        else
            bRNGIncKey = true;
        ReadFromRegistry(L"RNGDecKey", &nRNGDecKey);
        if (MapVirtualKeyW(nRNGDecKey, MAPVK_VK_TO_VSC) == 0)
            nRNGDecKey = nDefaultRNGDecKey;
        else
            bRNGIncKey = true;
        ReadFromRegistry(L"FrameDisplay", &nFrameData);
        if (nFrameData == FRAMEDISPLAY_NORMAL)
        {
            bSimpleFrameInfo = true;
        }
        else if (nFrameData == FRAMEDISPLAY_ADVANCED)
        {
            bSimpleFrameInfo = false;
        }
        ReadFromRegistry(L"DisplayFreeze", &bDisplayFreeze);
        ReadFromRegistry(L"DisplayInputs", &bDisplayInputs);

        char pcModPath[MAX_PATH];
        GetModuleFileNameA(NULL, pcModPath, sizeof(pcModPath));
        std::string sDLLPath = std::string(pcModPath).substr(0, std::string(pcModPath).length() - 33) + "Extended-Training-Mode-DLL.dll";
        while (!std::ifstream(sDLLPath).good())
        {
            std::wstring wsErrorString(sDLLPath.begin(), sDLLPath.end());
            wsErrorString = L"UNABLE TO FIND " + wsErrorString;
            int nReturnVal = MessageBoxW(NULL, wsErrorString.c_str(), L"", MB_ICONERROR | MB_RETRYCANCEL);
            switch (nReturnVal)
            {
            case IDRETRY:
                continue;
            default:
                return 0;
                break;
            }
        }

        while (1)
        {
            nCurrentTime = std::time(nullptr);
        
            SetConsoleCursorPosition(hConsoleHandle, { 0, 0 });
            std::cout << "===========================================================================\x1b[K" << std::endl;
            std::cout << "|   Fang, gonp, and meepster99(Inana)'s Extended Training Mode Mod " << VERSION << "   |\x1b[K" << std::endl;
            std::cout << "|                                                                         |\x1b[K" << std::endl;
            std::cout << "|   " << GITHUB_RELEASE << "   |\x1b[K" << std::endl;
            if (bNeedToAnnounceNewVersion && nCurrentTime % 3 != 0)
                std::cout << "|   NEW VERSION " << sOnlineVersion << " AVAILABLE ON GITHUB                                 |\x1b[K" << std::endl;
            else
                std::cout << "|                                                                         |\x1b[K" << std::endl;
            std::cout << "===========================================================================\x1b[K" << std::endl;

            SetConsoleCursorPosition(hConsoleHandle, { 0, 6 });
            std::cout << "\x1b[K";

            GetExitCodeProcess(hMBAAHandle, &dwExitCode);
            if (hMBAAHandle == 0x0 || dwExitCode != 259)    //259 is the not-closed return code
            {
                // I'm not sure why, but when MBAA closes, for a really short
                // window, GetProcessByName will return a different handle
                // to something else.  This small wait helps with that,
                // though it's for sure a kludge.
                Sleep(100);

                hMBAAHandle = 0x0;

                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::string sLookingForMelty = "Looking for MBAA.exe        \x1b[K";
                for (int i = 0; i < nCurrentTime % 8; i++)
                    sLookingForMelty[20 + i] = '.';
                std::cout << sLookingForMelty;
                SetConsoleCursorPosition(hConsoleHandle, { 0, 8 });
                std::cout << "\x1b[J";

                hMBAAHandle = GetProcessByName(L"MBAA.exe");

                // don't do anything until we re-attach to mbaa
                if (hMBAAHandle == 0x0)
                    continue;

                LogInfo("Attached to MBAA");

                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "                                              ";

                Sleep(100);
                dwBaseAddress = GetBaseAddressByName(hMBAAHandle, L"MBAA.exe");
                LogInfo("Got BaseAddressByName");

                DWORD dwPID = GetProcessPID(L"MBAA.exe");
                InjectIntoMBAA(dwPID, sDLLPath);
                // it is ok if it fails.  write it to the log and move on.
                // it will be obvious in-game if it didn't inject correctly.

                // Set some initial stuff
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeKey), &nFreezeKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameStepKey), &nFrameStepKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitboxesDisplayKey), &nHitboxDisplayKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameDataDisplayKey), &nFrameDataDisplayKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHighlightsOnKey), &nHighlightsOnKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveStateKey), &nSaveStateKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedPrevSaveSlotKey), &nPrevSaveSlotKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedNextSaveSlotKey), &nNextSaveSlotKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameBarScrollLeftKey), &nFrameBarScrollLeftKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameBarScrollRightKey), &nFrameBarScrollRightKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGIncKey), &nRNGIncKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGDecKey), &nRNGDecKey, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nSaveSlot, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayFreeze), &bDisplayFreeze, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayInputs), &bDisplayInputs, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGMode), &nRNGMode, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGRate), &nRNGRate, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 1, 0);

                std::array<uint8_t, 4> arrTemp = CreateColorArray2(NO_HIGHLIGHT);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedIdleHighlight), &arrTemp, 4, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedBlockingHighlight), &arrTemp, 4, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitHighlight), &arrTemp, 4, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedArmorHighlight), &arrTemp, 4, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedThrowProtectionHighlight), &arrTemp, 4, 0);

                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedOnExtendedSettings), &bOnSettingsMenu, 1, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcBlank_64, 64, 0);
                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcBlank_64, 64, 0);
            }

            // check this to prevent attaching to netplay
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGameMode), &nReadResult, 4, 0);
            nGameMode = nReadResult;
            if (nGameMode != 4112)
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "Cannot attach to versus mode....\x1b[K";
                //LogInfo("MBAA is in versus mode");
                continue;
            }
            else
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "Attached to MBAA.exe\x1b[K" << "\n\x1b[K\n";
                //LogInfo("MBAA is in training mode");
            }

            FrameDisplay(hMBAAHandle, dwBaseAddress, P1, P2, P3, P4);

            //nWriteBuffer = nPlayerAdvantage;
            //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwFPS), &nWriteBuffer, 4, 0);

            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
            if (nReadResult == 0)
            {
                nReversalIndex1 = 0;
                nReversalIndex2 = 0;
                nReversalIndex3= 0;
                vPatternNames = GetEmptyPatternList();

                // TODO: add fun CSS features here
                setBlockingAnimationPointers = {};
                setIdleAnimationPointers = {};
            }
            else
            {
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nSaveSlot, 1, 0);
                bEnableFN2Load = nSaveSlot > 0 ? true : false;

                nReadResult = 0;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1APressed), &nReadResult, 1, 0);
                bOldAPressed = bAPressed;
                bAPressed = (nReadResult == 1 ? true : false);
                nReadResult = 0;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1BPressed), &nReadResult, 1, 0);
                bOldBPressed = bBPressed;
                bBPressed = (nReadResult == 1 ? true : false);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1F1Pressed), &nReadResult, 1, 0);
                bOldF1Pressed = bF1Pressed;
                bF1Pressed = (nReadResult == 1 ? true : false);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1F2Pressed), &nReadResult, 1, 0);
                bOldF2Pressed = bF2Pressed;
                bF2Pressed = (nReadResult == 1 ? true : false);

                // these flags are used to determine if assist chars exist
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP3Exists), &nReadResult, 4, 0);
                bP3Exists = (nReadResult == 1 ? true : false);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4Exists), &nReadResult, 4, 0);
                bP4Exists = (nReadResult == 1 ? true : false);

                // update the location variables if not locked
                if (!bPositionsLocked)
                {
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nReadResult, 4, 0);
                    nP1X = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2X), &nReadResult, 4, 0);
                    nP2X = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP3X), &nReadResult, 4, 0);
                    nP3X = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4X), &nReadResult, 4, 0);
                    nP4X = nReadResult;
                }

                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
                bPaused = (nReadResult == 1 ? true : false);

                // This is the big if-else
                if (bPaused)
                {
                    // this is used way later to trigger unpausing events
                    bJustUnpaused = true;

                    DWORD dwTrainingMenuString = GetTrainingMenuStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwCommandListStringAddress = GetCommandListStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hMBAAHandle, dwBaseAddress);

                    // Replace the menu title and COMMAND LIST
                    char pcTrainingMenu[19];
                    strcpy_s(pcTrainingMenu, ("EXTENDED MOD " + VERSION).c_str());
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwCommandListStringAddress), &pcExtendedSettings_18, 18, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwTrainingMenuString), &pcTrainingMenu, 19, 0);

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
    #define sadly_obsolete
    #ifdef sadly_obsolete
                        // Replace the RETURN TO MAIN MENU option with fancy scrolling text
                        // this is 100% unnecessary but I did it for fun
                        try
                        {
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
                        catch (...)
                        {
                            LogError("Error from scrolling text");
                        }
    #endif
                    }

                    // Turn COMMAND LIST into EXTENDED SETTINGS
                    DWORD dwTrainingMenuCursor = GetTrainingMenuCursorAddress(hMBAAHandle, dwBaseAddress);
                    int nTrainingMenuCursor;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwTrainingMenuCursor), &nTrainingMenuCursor, 4, 0);
                    if (nCurrentSubMenu == eMenu::COMMAND_LIST)
                    {
                        nWriteBuffer = eMenu::ENEMY_SETTINGS;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nWriteBuffer, 4, 0);

                        bOldAPressed = bAPressed = true;

                        bOnSettingsMenu = true;
                        bInExtendedSettings = true;

                        // wait for two frames to prevent a bug with the A press reader
                        Sleep(32);
                    }
                    /*else if (!bOnHotkeySettingsMenu && bAPressed && !bOldAPressed && nTrainingMenuCursor == 16)
                    {
                        nWriteBuffer = eMenu::ENEMY_SETTINGS;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nWriteBuffer, 4, 0);

                        bOldAPressed = bAPressed = true;

                        bOnHotkeySettingsMenu = true;
                        bInExtendedSettings = true;
                    }*/
                    else if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                    {
                        bOnSettingsMenu = false;
                        bInExtendedSettings = false;
                        bOnHotkeySettingsMenu = false;
                    }

                    if (nCurrentSubMenu == eMenu::ENEMY_SETTINGS)
                    {
                        // create a gray box over the helper text if on settings or hotkeys
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedOnExtendedSettings), &bOnSettingsMenu, 1, 0);

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

                        if (bOnSettingsMenu && !bOnHotkeySettingsMenu)
                        {
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Reset "hits till burst" if status is not manual
                            if (nEnemyStatusIndex != eEnemyStatus::MANUAL)
                                nHitsTillBurst = TOO_HIGH_TO_BURST;

                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // A Press handler
                            if (bAPressed && !bOldAPressed)
                            {
                                switch (nSettingsPage)
                                {
                                case POSITIONS_PAGE:
                                {
                                    if (nEnemySettingsCursor == 8)
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
                                    break;
                                }
                                case FRAME_TOOL:
                                {
                                    if (nEnemySettingsCursor == 6)
                                    {
                                        bPrintColorGuide = !bPrintColorGuide;
                                        system("cls");
                                    }
                                    break;
                                }
                                case SAVE_STATE_PAGE:
                                {
                                    if (nEnemySettingsCursor == 0)
                                    {
                                        ClearSave(nSaveSlot);

                                        char pcMessageBuffer[32] = "CLEARED SAVE";
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                    }
                                    else if (nEnemySettingsCursor == 2)
                                    {
                                        if (nSaveSlot == 0)
                                        {
                                            char pcMessageBuffer[32] = "NO SLOT SELECTED";
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                        }
                                        else
                                        {
                                            SaveState(hMBAAHandle, dwBaseAddress, nSaveSlot);

                                            char pcMessageBuffer[32] = "CREATED SAVE";
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                        }
                                    }
                                    else if (nEnemySettingsCursor == 3)
                                    {
                                        ClearAllSaves();

                                        char pcMessageBuffer[32] = "CLEARED ALL SAVES";
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                    }
                                    if (nEnemySettingsCursor == 5)
                                    {
                                        if (nSaveSlot == 0)
                                        {
                                            char pcTemp[32] = "NO SLOT SELECTED";
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcTemp, 32, 0);
                                        }
                                        else
                                            LoadStateFromFile(hMBAAHandle, dwBaseAddress, nSaveSlot);
                                    }
                                    else if (nEnemySettingsCursor == 6)
                                    {
                                        if (nSaveSlot == 0)
                                        {
                                            char pcMessageBuffer[32] = "NO SLOT SELECTED";
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                        }
                                        else if (!CheckSave(nSaveSlot))
                                        {
                                            char pcMessageBuffer[32] = "NO SAVE DATA";
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
                                        }
                                        else
                                            SaveStateToFile(hMBAAHandle, dwBaseAddress, nSaveSlot);
                                    }
                                    break;
                                }
                                case RNG_PAGE:
                                {
                                    if (nEnemySettingsCursor == 3)
                                    {
                                        int nOut = PromptForNumber(hMBAAHandle, dwBaseAddress);
                                        if (nOut != -1)
                                        {
                                            if (nRNGMode == RNG_SEED)
                                            {
                                                nCustomSeed = nOut;
                                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                            }
                                            if (nRNGMode == RNG_RN)
                                            {
                                                nCustomRN = PromptForNumber(hMBAAHandle, dwBaseAddress);
                                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                            }
                                        }
                                    }
                                    break;
                                }
                                case HOTKEYS_PAGE:
                                {
                                    if (nEnemySettingsCursor == 0)
                                    {
                                        bOldAPressed = bAPressed = true;
                                        bOnHotkeySettingsMenu = true;
                                        ResetKeysHeld();
                                        Sleep(32);
                                    }
                                    break;
                                }
                                default:
                                    break;
                                }
                            }

                            bool bIsHoveringScroll = false;
                            if (nSettingsPage == FRAME_TOOL && nEnemySettingsCursor == 8)
                            {
                                bIsHoveringScroll = true;
                            }
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHoveringScroll), &bIsHoveringScroll, 1, 0);

                            // Replace static menu fields
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset_17, 17, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings_18, 18, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageString), &pcBlank_1, 1, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDefaultString), &pcBlank_1, 1, 0);
                            switch (nSettingsPage)
                            {
                            case REVERSALS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcReversalType_14, 14, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcReversalSlot1_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcReversalSlot2_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcReversalSlot3_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcReversalSlot4_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcReversalDelay_15, 15, 0);

                                break;
                            }
                            case STATS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcExGuard_9, 9, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcGuardBar_10, 10, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcMeter_6, 6, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcHealth_7, 7, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcHitsUntilBurst_17, 17, 0);

                                break;
                            }
                            case POSITIONS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcP1XLoc_11, 11, 0);
                                if (bP3Exists)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcAssistLoc_13, 13, 0);
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), pcBlank_1, 1, 0);
                                }
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcP2XLoc_11, 11, 0);
                                if (bP4Exists)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcAssistLoc_13, 13, 0);
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), pcBlank_1, 1, 0);
                                }
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcInvert_7, 7, 0);

                                break;
                            }
                            case FRAME_TOOL:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcFrameData_11, 11, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcDisplayFreeze_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcDisplayInputs_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcScrollDisplay_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcColorGuide_12, 12, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);

                                break;
                            }
                            case SAVE_STATE_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcSaveStateSlot_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcSaveState_11, 11, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcClearAllSaves_16, 16, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcImportSave_12, 12, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcExportSave_12, 12, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcLoadRNG_9, 9, 0);

                                break;
                            }
                            case CHARACTER_SPECIFICS:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcSionBullets_13, 13, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcRoaVisibleCharge_19, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcRoaHiddenCharge_19, 19, 0);
                                if (bP3Exists)
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcP1ControlledChar_19, 19, 0);
                                else
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcBlank_1, 1, 0);
                                if (bP4Exists)
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcP2ControlledChar_19, 19, 0);
                                else
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcFMaidsHearts_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcRyougiKnife_13, 13, 0);

                                break;
                            }
                            case HIGHLIGHT_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcIdle_5, 5, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcBlock_6, 6, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcHit_4, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcArmor_6, 6, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcThrowProtection_17, 17, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);

                                break;
                            }
                            case RNG_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcCustomRNG_11, 11, 0);
                                if (nRNGMode == RNG_OFF)
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcBlank_1, 1, 0);
                                else
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcRate_5, 5, 0);
                                if (nRNGMode == RNG_OFF)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcBlank_1, 1, 0);
                                }
                                else if (nRNGMode == RNG_SEED)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcSeed_5, 5, 0);
                                }
                                else if (nRNGMode == RNG_RN)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcValue_6, 6, 0);
                                }
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);

                                break;
                            }
                            case HOTKEYS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcHotkeys_8, 8, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);

                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Replace Main Info text
                            switch (nSettingsPage)
                            {
                            case REVERSALS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Set the {enemy reversal pattern.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (nReversalType == REVERSAL_NORMAL)
                                        strcpy_s(pcTemp64, ">Reversal {once} after {blocking or being hit.}");
                                    if (nReversalType == REVERSAL_RANDOM)
                                        strcpy_s(pcTemp64, ">Reversal {sometimes} after {blocking or being hit.}");
                                    if (nReversalType == REVERSAL_REPEAT)
                                        strcpy_s(pcTemp64, ">Reversal {constantly.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Enemy reversal {slot 1}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, std::string(">Reversal {" + vPatternNames[nReversalIndex1] + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Enemy reversal {slot 2}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, std::string(">Reversal {" + vPatternNames[nReversalIndex2] + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Enemy reversal {slot 3}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, std::string(">Reversal {" + vPatternNames[nReversalIndex3] + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Enemy reversal {slot 4}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, std::string(">Reversal {" + vPatternNames[nReversalIndex4] + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    char pcTemp64[64] = "Number of {frames to delay} the enemy reversal.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nReversalDelayFrames == 1)
                                        strcpy_s(pcTemp64, ">Delay by {1 frame.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Delay by {" + std::to_string(nReversalDelayFrames) + " frames.").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case STATS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Enable the dummy to {EX Guard.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nExGuardSetting == eEnemyOffOnRandom::OFF)
                                        strcpy_s(pcTemp64, ">Dummy will {not} EX Guard.");
                                    else if (nExGuardSetting == eEnemyOffOnRandom::ON)
                                        strcpy_s(pcTemp64, ">Dummy will {always} EX Guard.");
                                    else if (nExGuardSetting == eEnemyOffOnRandom::RANDOM)
                                        strcpy_s(pcTemp64, ">Dummy will {sometimes} EX Guard.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Guard bar behavior.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bInfGuard)
                                        strcpy_s(pcTemp64, ">Guard Bar will {always be full.");
                                    else
                                        strcpy_s(pcTemp64, ">Guard Bar will {deplete like normal.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Set a specific meter amount.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Hold A} for precise numbers.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Set a specific health amount.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    int nPercent = (int)((float)nCustomHealth / 11400.0f * 100.0f);
                                    strcpy_s(pcTemp64, (">" + std::to_string(nPercent) + "%%. {Hold A} for precise numbers.").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    char pcTemp64[64] = "Number of hits into a combo before the will burst.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                                        strcpy_s(pcTemp64, ">Dummy will {not} burst.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Dummy will burst after {" + std::to_string(nHitsTillBurst) + " hits.").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case HIGHLIGHT_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Highlight when a character {is not performing actions.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nIdleHighlightSetting == NO_HIGHLIGHT)
                                        strcpy_s(pcTemp64, ">Do not highlight.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Highlight in " + std::string(vHighlightNamesWithFormatting[nIdleHighlightSetting]) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Highlight when a character {is blocking.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nBlockingHighlightSetting == NO_HIGHLIGHT)
                                        strcpy_s(pcTemp64, ">Do not highlight.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Highlight in " + std::string(vHighlightNamesWithFormatting[nBlockingHighlightSetting]) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Highlight when a character {is being hit.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nHitHighlightSetting == NO_HIGHLIGHT)
                                        strcpy_s(pcTemp64, ">Do not highlight.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Highlight in " + std::string(vHighlightNamesWithFormatting[nHitHighlightSetting]) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Highlight when a character {has armor.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nArmorHighlightSetting == NO_HIGHLIGHT)
                                        strcpy_s(pcTemp64, ">Do not highlight.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Highlight in " + std::string(vHighlightNamesWithFormatting[nArmorHighlightSetting]) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Highlight when a character {has throw protection.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nThrowProtectionHighlightSetting == NO_HIGHLIGHT)
                                        strcpy_s(pcTemp64, ">Do not highlight.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Highlight in " + std::string(vHighlightNamesWithFormatting[nThrowProtectionHighlightSetting]) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case POSITIONS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Lock the characters' positions during a training reset.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bPositionsLocked)
                                        strcpy_s(pcTemp64, ">Positions are {locked.");
                                    else
                                        strcpy_s(pcTemp64, ">Positions are {unlocked.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Player 1's {X-axis position.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Hold A} for precise numbers.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Player 1's assist character's {X-axis position.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Hold A} for precise numbers.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Player 2's {X-axis position.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Hold A} for precise numbers.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Player 2's assist character's {X-axis position.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Hold A} for precise numbers.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    char pcTemp64[64] = "{Invert} player 1 and player 2's positions.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press A} to invert.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case CHARACTER_SPECIFICS:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Number of bullets Sion's gun has.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nSionBullets == MAX_BULLETS + 2)
                                        strcpy_s(pcTemp64, ">Bullets are {infinite.");
                                    else if (nSionBullets == MAX_BULLETS + 1)
                                        strcpy_s(pcTemp64, ">Bullets {behave like normal.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Bullets are set to {" + std::to_string(nSionBullets) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Number of visible charges C-Roa has stocked.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nRoaVisibleCharge == -2)
                                        strcpy_s(pcTemp64, ">Visible charges are {infinite.");
                                    else if (nRoaVisibleCharge == -1)
                                        strcpy_s(pcTemp64, ">Visible charges {behave like normal.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Visible charges are set to {" + std::to_string(nRoaVisibleCharge) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Number of hidden charges C-Roa has stocked.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nRoaHiddenCharge == -2)
                                        strcpy_s(pcTemp64, ">Hidden charges are {infinite.");
                                    else if (nRoaHiddenCharge == -1)
                                        strcpy_s(pcTemp64, ">Hidden charges {behave like normal.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Hidden charges are set to {" + std::to_string(nRoaHiddenCharge) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "{Switch control} between the main character and the assist.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nP1ControlledCharacter)
                                        strcpy_s(pcTemp64, ">Control the {assist character.");
                                    else
                                        strcpy_s(pcTemp64, ">Control the {main character.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Number of hearts F-Maids has.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nFMaidsHearts == -1)
                                        strcpy_s(pcTemp64, ">Hearts are {infinite.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">Hearts are set to {" + std::to_string(nFMaidsHearts) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    char pcTemp64[64] = "Number of knives Ryougi has.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bRyougiInfiniteKnife)
                                        strcpy_s(pcTemp64, ">Knives are {infinite.");
                                    else
                                        strcpy_s(pcTemp64, ">Knives {behave like normal.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case SAVE_STATE_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Active {save state slot.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press A} to delete a save.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Save a state";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nSaveSlot == 0)
                                        strcpy_s(pcTemp64, ">No save slot selected.");
                                    else
                                        strcpy_s(pcTemp64, std::string(">{Press A} to save a state to {slot " + std::to_string(nSaveSlot) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "{Delete} all save slots.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press A} to ~delete all saves.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Import a save state file.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nSaveSlot == 0)
                                        strcpy_s(pcTemp64, ">No save slot selected.");
                                    else if (CheckSave(nSaveSlot))
                                        strcpy_s(pcTemp64, std::string(">{Press A} to import a state to {slot " + std::to_string(nSaveSlot) + " (slot " + std::to_string(nSaveSlot) + " has data).").c_str());
                                    else
                                        strcpy_s(pcTemp64, std::string(">{Press A} to import a state to {slot " + std::to_string(nSaveSlot) + ".").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Export a save state file.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nSaveSlot == 0)
                                        strcpy_s(pcTemp64, ">No save slot selected.");
                                    else if (CheckSave(nSaveSlot))
                                        strcpy_s(pcTemp64, std::string(">{Press A} to export the save state in {slot " + std::to_string(nSaveSlot) + ".").c_str());
                                    else
                                        strcpy_s(pcTemp64, std::string(">{Slot " + std::to_string(nSaveSlot) + " is empty.").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    char pcTemp64[64] = "Include RNG when loading a save state.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bLoadRNG)
                                        strcpy_s(pcTemp64, ">RNG {is loaded.");
                                    else
                                        strcpy_s(pcTemp64, ">RNG {is not loaded.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case FRAME_TOOL:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Level of detail to show in the {frame data display.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nFrameData == FRAMEDISPLAY_NORMAL)
                                        strcpy_s(pcTemp64, ">Display {the basics.");
                                    else
                                        strcpy_s(pcTemp64, ">Display {everything.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Display {freeze frames} in the frame data display.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bDisplayFreeze)
                                        strcpy_s(pcTemp64, ">Freeze frames {will be displayed.");
                                    else
                                        strcpy_s(pcTemp64, ">Freeze frames {will not be displayed.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Display {controller inputs} in the frame data display.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (bDisplayInputs)
                                        strcpy_s(pcTemp64, ">Controller inputs {will be displayed.");
                                    else
                                        strcpy_s(pcTemp64, ">Controller inputs {will not be displayed.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Control the scroll on the frame data display.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press Left} and {Right} to move the frame data display.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Display a color guide for the data in the frame display.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press A} to show the color guide.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case RNG_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Enable specific {RNG controls";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nRNGMode == RNG_OFF)
                                        strcpy_s(pcTemp64, ">RNG {behaves like normal.");
                                    else if (nRNGMode == RNG_SEED)
                                        strcpy_s(pcTemp64, ">RNG uses a {fixed seed.");
                                    if (nRNGMode == RNG_RN)
                                        strcpy_s(pcTemp64, ">RNG uses a {fixed value.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "{How often} the RNG will be {overwritten.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    if (nRNGRate == RNG_EVERY_FRAME)
                                        strcpy_s(pcTemp64, ">RNG is overwritten {every frame.");
                                    else if (nRNGRate == RNG_EVERY_RESET)
                                        strcpy_s(pcTemp64, ">RNG is overwritten {every training mode reset.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64];
                                    if (nRNGMode == RNG_SEED)
                                    {
                                        strcpy_s(pcTemp64, "Which specific {seed} to use.");
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    }
                                    if (nRNGMode == RNG_RN)
                                    {
                                        strcpy_s(pcTemp64, "Which specific {value} to use.");
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    }

                                    strcpy_s(pcTemp64, ">{Press A} to chose a value. {0x00000000-0xFFFFFFFF.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    break;
                                }
                                case 6:
                                {
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case HOTKEYS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "Customize {hotkeys} for various settings.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);
                                    strcpy_s(pcTemp64, ">{Press A} to open the {hotkeys menu.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    break;
                                }
                                case 3:
                                {
                                    break;
                                }
                                case 5:
                                {
                                    break;
                                }
                                case 6:
                                {
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

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

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Skipping blank menu items
                            switch (nSettingsPage)
                            {
                            case REVERSALS_PAGE:
                                break;
                            case STATS_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 14 && nEnemySettingsCursor == 0)
                                {
                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 2;
                                    nOldEnemySettingsCursor = 2;
                                }
                                if (nEnemySettingsCursor == 0)
                                {
                                    nWriteBuffer = 14;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 14;
                                    nOldEnemySettingsCursor = 14;
                                }
                                break;
                            }
                            case POSITIONS_PAGE:
                            {
                                if (!bP3Exists && nOldEnemySettingsCursor == 2 && nEnemySettingsCursor == 3)
                                {
                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 5;
                                    nOldEnemySettingsCursor = 5;
                                }
                                else if (!bP3Exists && nEnemySettingsCursor == 3)
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
                                else if (!bP4Exists && nEnemySettingsCursor == 6)
                                {
                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 5;
                                    nOldEnemySettingsCursor = 5;
                                }
                                break;
                            }
                            case FRAME_TOOL:
                            {
                                if (nOldEnemySettingsCursor == 6 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor > 6 && nEnemySettingsCursor < 10)
                                {
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 6;
                                    nOldEnemySettingsCursor = 6;
                                }
                                break;
                            }
                            case SAVE_STATE_PAGE:
                            {
                                /*
                                if (nOldEnemySettingsCursor == 6 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 6;
                                    nOldEnemySettingsCursor = 6;
                                }
                                */
                                break;
                            }
                            case CHARACTER_SPECIFICS:
                            {
                                if (!bP3Exists)
                                {
                                    if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                                    {
                                        nWriteBuffer = 6;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 6;
                                        nOldEnemySettingsCursor = 6;
                                    }
                                    else if (nEnemySettingsCursor == 5)
                                    {
                                        nWriteBuffer = 3;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 3;
                                        nOldEnemySettingsCursor = 3;
                                    }
                                }
                                break;
                            }
                            case HIGHLIGHT_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 6 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 6;
                                    nOldEnemySettingsCursor = 6;
                                }
                                break;
                            }
                            case RNG_PAGE:
                            {
                                if (nRNGMode == RNG_OFF)
                                {
                                    if (nOldEnemySettingsCursor == 0 && nEnemySettingsCursor == 2)
                                    {
                                        nWriteBuffer = 10;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 10;
                                        nOldEnemySettingsCursor = 10;
                                    }
                                    else if (nEnemySettingsCursor > 0 && nEnemySettingsCursor < 10)
                                    {
                                        nWriteBuffer = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 0;
                                        nOldEnemySettingsCursor = 0;
                                    }
                                }
                                else
                                {
                                    if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                                    {
                                        nWriteBuffer = 10;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 10;
                                        nOldEnemySettingsCursor = 10;
                                    }
                                    else if (nEnemySettingsCursor > 3 && nEnemySettingsCursor < 10)
                                    {
                                        nWriteBuffer = 3;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                        nEnemySettingsCursor = 3;
                                        nOldEnemySettingsCursor = 3;
                                    }
                                }
                                break;
                            }
                            case HOTKEYS_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 0 && nEnemySettingsCursor == 2)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor > 0 && nEnemySettingsCursor < 10)
                                {
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 0;
                                    nOldEnemySettingsCursor = 0;
                                }
                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // left-right handlers
                            switch (nSettingsPage)
                            {
                            case REVERSALS_PAGE:
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                {
                                    nReversalType = max(REVERSAL_NORMAL, nReversalType - 1);
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                {
                                    nReversalType = min(nReversalType + 1, REVERSAL_REPEAT);
                                }

                                if (nOldEnemyDefenseIndex == -1)
                                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                {
                                    nReversalIndex1 = max(0, nReversalIndex1 - 1);
                                }
                                else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                {
                                    nReversalIndex1 = min(nReversalIndex1 + 1, vPatternNames.size() - 1);
                                }

                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                {
                                    nReversalIndex2 = max(0, nReversalIndex2 - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, &nReversalIndex2);*/
                                }
                                else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                {
                                    nReversalIndex2 = min(nReversalIndex2 + 1, vPatternNames.size() - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, &nReversalIndex2);*/
                                }

                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                                {
                                    nReversalIndex3 = max(0, nReversalIndex3 - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, &nReversalIndex3);*/
                                }
                                else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                                {
                                    nReversalIndex3 = min(nReversalIndex3 + 1, vPatternNames.size() - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, &nReversalIndex3);*/
                                }

                                if (nOldDownRecoveryIndex == -1)
                                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                                else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                                {
                                    nReversalIndex4 = max(0, nReversalIndex4 - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, &nReversalIndex4);*/
                                }
                                else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                                {
                                    nReversalIndex4 = min(nReversalIndex4 + 1, vPatternNames.size() - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, &nReversalIndex4);*/
                                }

                                if (nOldThrowRecoveryIndex == -1)
                                    nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                                else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                                {
                                    nReversalDelayFrames = max(0, nReversalDelayFrames - 1);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, nullptr, &nReversalDelayFrames);*/
                                }
                                else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                                {
                                    nReversalDelayFrames = min(nReversalDelayFrames + 1, MAX_REVERSAL_DELAY);
                                    /*SetSharedMemory(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                        nullptr, nullptr, nullptr, nullptr, &nReversalDelayFrames);*/
                                }

                                PopulateAirAndGroundReversals(&vAirReversals, &vGroundReversals, nP2CharacterID, &vPatternNames, nReversalIndex1, nReversalIndex2, nReversalIndex3, nReversalIndex4);
                                break;
                            }
                            case STATS_PAGE:
                            {
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
                                break;
                            }
                            case POSITIONS_PAGE:
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
                                break;
                            }
                            case FRAME_TOOL:
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                {
                                    nFrameData = FRAMEDISPLAY_NORMAL;
                                    SetRegistryValue(L"FrameDisplay", nFrameData);
                                    bSimpleFrameInfo = true;
                                    system("cls");
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                {
                                    nFrameData = FRAMEDISPLAY_ADVANCED;
                                    SetRegistryValue(L"FrameDisplay", nFrameData);
                                    bSimpleFrameInfo = false;
                                }

                                if (nOldEnemyDefenseIndex == -1)
                                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                {
                                    bDisplayFreeze = false;
                                    SetRegistryValue(L"DisplayFreeze", bDisplayFreeze);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayFreeze), &bDisplayFreeze, 1, 0);
                                    bDisplayInputs = false;
                                    SetRegistryValue(L"DisplayInputs", bDisplayInputs);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayInputs), &bDisplayInputs, 1, 0);
                                    system("cls");
                                }
                                else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                {
                                    bDisplayFreeze = true;
                                    SetRegistryValue(L"DisplayFreeze", bDisplayFreeze);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayFreeze), &bDisplayFreeze, 1, 0);
                                }

                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                {
                                    bDisplayInputs = false;
                                    SetRegistryValue(L"DisplayInputs", bDisplayInputs);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayInputs), &bDisplayInputs, 1, 0);
                                    system("cls");
                                }
                                else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                {
                                    bDisplayInputs = true;
                                    SetRegistryValue(L"DisplayInputs", bDisplayInputs);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayInputs), &bDisplayInputs, 1, 0);
                                    bDisplayFreeze = true;
                                    SetRegistryValue(L"DisplayFreeze", bDisplayFreeze);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayFreeze), &bDisplayFreeze, 1, 0);
                                }

                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                                {
                                    if (nBarScrolling < min(nBarCounter - min(nBarDisplayRange, DISPLAY_RANGE), BAR_MEMORY_SIZE - min(nBarDisplayRange, DISPLAY_RANGE)))
                                    {
                                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
                                        nBarScrolling++;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
                                    }
                                }
                                else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                                {
                                    if (nBarScrolling > 0)
                                    {
                                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
                                        nBarScrolling--;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
                                    }
                                }

                                break;
                            }
                            case SAVE_STATE_PAGE:
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                if (nOldEnemyDefenseIndex == -1)
                                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                if (nOldDownRecoveryIndex == -1)
                                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex || // left
                                    nOldEnemyDefenseIndex > nEnemyDefenseIndex ||
                                    nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex ||
                                    nOldAirRecoveryIndex > nAirRecoveryIndex ||
                                    nOldDownRecoveryIndex > nDownRecoveryIndex)
                                {
                                    nSaveSlot = max(0, nSaveSlot - 1);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nSaveSlot, 1, 0);
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex || // right
                                    nOldEnemyDefenseIndex < nEnemyDefenseIndex ||
                                    nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex ||
                                    nOldAirRecoveryIndex < nAirRecoveryIndex ||
                                    nOldDownRecoveryIndex < nDownRecoveryIndex)
                                {
                                    nSaveSlot = min(nSaveSlot + 1, MAX_SAVES);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nSaveSlot, 1, 0);
                                }
                                bEnableFN2Load = nSaveSlot > 0 ? true : false;

                                if (nOldThrowRecoveryIndex == -1)
                                    nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                                else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                                {
                                    if (bLoadRNG)
                                        bLoadRNG = false;
                                }
                                else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                                {
                                    if (!bLoadRNG)
                                        bLoadRNG = true;
                                }

                                break;
                            }
                            case CHARACTER_SPECIFICS:
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

                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                                {
                                    DWORD dw0 = 0x0;
                                    DWORD dw1 = 0x1;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &dw0, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1TagFlag), &dw0, 1, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3TagFlag), &dw1, 1, 0);
                                    nP1ControlledCharacter = 0;
                                    nP1TagFlag = 0;
                                    nP3TagFlag = 1;
                                }
                                else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                                {
                                    DWORD dw0 = 0x0;
                                    DWORD dw1 = 0x1;
                                    DWORD dw2 = 0x2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &dw2, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1TagFlag), &dw1, 1, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3TagFlag), &dw0, 1, 0);
                                    nP1ControlledCharacter = 2;
                                    nP1TagFlag = 1;
                                    nP3TagFlag = 0;
                                }

                                if (nOldDownRecoveryIndex == -1)
                                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                                else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                                {
                                    nFMaidsHearts = min(nFMaidsHearts + 1, MAX_HEARTS);
                                    SetFMaidsHearts(hMBAAHandle, dwBaseAddress, nFMaidsHearts);
                                }
                                else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                                {
                                    nFMaidsHearts = max(nFMaidsHearts - 1, -1);
                                    SetFMaidsHearts(hMBAAHandle, dwBaseAddress, nFMaidsHearts);
                                }

                                if (nOldThrowRecoveryIndex == -1)
                                    nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                                else if (nOldThrowRecoveryIndex > nThrowRecoveryIndex)// left
                                {
                                    if (bRyougiInfiniteKnife)
                                        bRyougiInfiniteKnife = false;
                                }
                                else if (nOldThrowRecoveryIndex < nThrowRecoveryIndex)// right
                                {
                                    if (!bRyougiInfiniteKnife)
                                        bRyougiInfiniteKnife = true;
                                }
                                break;
                            }
                            case HIGHLIGHT_PAGE:
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                {
                                    nIdleHighlightSetting = max(NO_HIGHLIGHT, nIdleHighlightSetting - 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nIdleHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedIdleHighlight), &arrTemp, 4, 0);
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                {
                                    nIdleHighlightSetting = min(BLACK_HIGHLIGHT, nIdleHighlightSetting + 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nIdleHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedIdleHighlight), &arrTemp, 4, 0);
                                }

                                if (nOldEnemyDefenseIndex == -1)
                                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                {
                                    nBlockingHighlightSetting = max(NO_HIGHLIGHT, nBlockingHighlightSetting - 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nBlockingHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedBlockingHighlight), &arrTemp, 4, 0);
                                }
                                else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                {
                                    nBlockingHighlightSetting = min(BLACK_HIGHLIGHT, nBlockingHighlightSetting + 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nBlockingHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedBlockingHighlight), &arrTemp, 4, 0);
                                }

                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                {
                                    nHitHighlightSetting = max(NO_HIGHLIGHT, nHitHighlightSetting - 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nHitHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitHighlight), &arrTemp, 4, 0);
                                }
                                else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                {
                                    nHitHighlightSetting = min(BLACK_HIGHLIGHT, nHitHighlightSetting + 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nHitHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitHighlight), &arrTemp, 4, 0);
                                }

                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                                {
                                    nArmorHighlightSetting = max(NO_HIGHLIGHT, nArmorHighlightSetting - 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nArmorHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedArmorHighlight), &arrTemp, 4, 0);
                                }
                                else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                                {
                                    nArmorHighlightSetting = min(BLACK_HIGHLIGHT, nArmorHighlightSetting + 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nArmorHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedArmorHighlight), &arrTemp, 4, 0);
                                }

                                if (nOldDownRecoveryIndex == -1)
                                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                                else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                                {
                                    nThrowProtectionHighlightSetting = max(NO_HIGHLIGHT, nThrowProtectionHighlightSetting - 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nThrowProtectionHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedThrowProtectionHighlight), &arrTemp, 4, 0);
                                }
                                else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                                {
                                    nThrowProtectionHighlightSetting = min(BLACK_HIGHLIGHT, nThrowProtectionHighlightSetting + 1);
                                    std::array<uint8_t, 4> arrTemp = CreateColorArray2(nThrowProtectionHighlightSetting);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedThrowProtectionHighlight), &arrTemp, 4, 0);
                                }
                                break;
                            }
                            case RNG_PAGE:  // rethinking how you save the rng settings.  don't gotta be THAT fancy
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                {
                                    nRNGMode = max(RNG_OFF, nRNGMode - 1);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGMode), &nRNGMode, 1, 0);
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                {
                                    nRNGMode = min(RNG_RN, nRNGMode + 1);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGMode), &nRNGMode, 1, 0);
                                }

                                if (nRNGMode != RNG_OFF)
                                {
                                    if (nOldEnemyDefenseIndex == -1)
                                        nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                    else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                    {
                                        nRNGRate = max(RNG_EVERY_FRAME, nRNGRate - 1);
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGRate), &nRNGRate, 1, 0);
                                    }
                                    else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                    {
                                        nRNGRate = min(RNG_EVERY_RESET, nRNGRate + 1);
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGRate), &nRNGRate, 1, 0);
                                    }

                                    if (nOldEnemyDefenseTypeIndex == -1)
                                        nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                    else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                    {
                                        if (nRNGMode == RNG_SEED)
                                        {
                                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                            nCustomSeed--;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                        }
                                        else if (nRNGMode == RNG_RN)
                                        {
                                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                            nCustomRN--;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                        }
                                    
                                    }
                                    else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                    {
                                        if (nRNGMode == RNG_SEED)
                                        {
                                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                            nCustomSeed++;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                        }
                                        else if (nRNGMode == RNG_RN)
                                        {
                                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                            nCustomRN++;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                        }
                                    }
                                }
                                break;
                            }
                            case HOTKEYS_PAGE:
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // PAGE number handler
                            if (nOldReduceDamageIndex == -1)
                                nOldReduceDamageIndex = nReduceDamageIndex;
                            else if (nOldReduceDamageIndex > nReduceDamageIndex || (bF1Pressed && !bOldF1Pressed))// left
                            {
                                nSettingsPage = max(0, nSettingsPage - 1);
                                if (nSettingsPage == 0)
                                    nSettingsPage = MAX_SETTINGS_PAGES;

                                nOldEnemyActionIndex = -1;
                                nOldEnemyDefenseIndex = -1;
                                nOldEnemyDefenseTypeIndex = -1;
                                nOldAirRecoveryIndex = -1;
                                nOldDownRecoveryIndex = -1;
                                nOldThrowRecoveryIndex = -1;
                                nOldReduceDamageIndex = -1;
                            }
                            else if (nOldReduceDamageIndex < nReduceDamageIndex || (bF2Pressed && !bOldF2Pressed))// right
                            {
                                nSettingsPage = min(nSettingsPage + 1, MAX_SETTINGS_PAGES + 1);
                                if (nSettingsPage == MAX_SETTINGS_PAGES + 1)
                                    nSettingsPage = 1;

                                nOldEnemyActionIndex = -1;
                                nOldEnemyDefenseIndex = -1;
                                nOldEnemyDefenseTypeIndex = -1;
                                nOldAirRecoveryIndex = -1;
                                nOldDownRecoveryIndex = -1;
                                nOldThrowRecoveryIndex = -1;
                                nOldReduceDamageIndex = -1;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // dynamic menu items
                            switch (nSettingsPage)
                            {
                            case REVERSALS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                switch (nReversalType)
                                {
                                case REVERSAL_NORMAL:
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcNormal_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcNormal_7, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                    break;
                                case REVERSAL_REPEAT:
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcRepeat_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcRepeat_7, 7, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                    break;
                                case REVERSAL_RANDOM:
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcRandom_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcRandom_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcRandom_7, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 2;
                                    break;
                                default:
                                    break;
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
                                break;
                            }
                            case STATS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);

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
                                    char pcTemp[7] = "0";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 0;
                                }
                                else if (nCustomHealth == MAX_HEALTH)
                                {
                                    char pcTemp[13] = "11400";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 13, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 13, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, std::to_string(nCustomHealth).c_str());
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

                                    //nWriteBuffer = eEnemyStatus::STAND;
                                    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
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
                                break;
                            }
                            case POSITIONS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
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

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

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
                                break;
                            }
                            case FRAME_TOOL:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (nFrameData == FRAMEDISPLAY_NORMAL)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcNormal_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcNormal_7, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcAdvanced_9, 9, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcAdvanced_9, 9, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }

                                if (!bDisplayFreeze)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcOff_4, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcOff_4, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 0;
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcOn_3, 3, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcOn_3, 3, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 5;
                                }

                                if (!bDisplayInputs)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcOff_4, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcOff_4, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 0;
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcOn_3, 3, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcOn_3, 3, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 2;
                                }

                                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
                                if (nBarScrolling == 0)
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nBarScrolling).c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 19, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 5;
                                }
                                else if (nBarScrolling >= min(nBarCounter - nBarDisplayRange, BAR_MEMORY_SIZE - nBarDisplayRange))
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, ("-" + std::to_string(nBarScrolling)).c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 0;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, ("-" + std::to_string(nBarScrolling)).c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 2;
                                }
                                break;
                            }
                            case SAVE_STATE_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);

                                if (nSaveSlot == 0)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcNone_5, 5, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcNone_5, 5, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else if (nSaveSlot < MAX_SAVES)
                                {
                                    std::string sData = "NO DATA";
                                    if (CheckSave(nSaveSlot))
                                    {
                                        sData = "SAVED";
                                    }

                                    char pcTemp[19];
                                    strcpy_s(pcTemp, ("SLOT 0" + std::to_string(nSaveSlot) + " (" + sData + ")").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 2;
                                }
                                else
                                {
                                    std::string sData = "NO DATA";
                                    if (CheckSave(nSaveSlot))
                                    {
                                        sData = "SAVED";
                                    }
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, ("SLOT 0" + std::to_string(nSaveSlot) + " (" + sData + ")").c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 19, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }


                                char pcTempSave[19];
                                if (nSaveSlot == 0)
                                    strcpy_s(pcTempSave, pcNoSlotSelected_17);
                                else
                                    strcpy_s(pcTempSave, pcPressA_8);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTempSave, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTempSave, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTempSave, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 2;


                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcPressA_8, 8, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcPressA_8, 8, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcPressA_8, 8, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 1;


                                char pcTempImport[19];
                                if (nSaveSlot == 0)
                                    strcpy_s(pcTempImport, pcNoSlotSelected_17);
                                else
                                    strcpy_s(pcTempImport, std::string("SLOT 0" + std::to_string(nSaveSlot) + " (PRESS A)").c_str());
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTempImport, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTempImport, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTempImport, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 2;

                                if (bLoadRNG)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomLateString), &pcOn_3, 3, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcOn_3, 3, 0);
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 6;
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryNormalString), &pcOff_4, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcOff_4, 4, 0);
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 0;
                                }

                                break;
                            }
                            case CHARACTER_SPECIFICS:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (bP3Exists)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &ONSCREEN_LOCATION, 4, 0);
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &OFFSCREEN_LOCATION, 4, 0);
                                }

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

                                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &nP1ControlledCharacter, 4, 0);
                                if (bP3Exists)
                                {
                                    if (nP1ControlledCharacter == 0)
                                    {
                                        if (nP1CharacterNumber == 19)
                                        {
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcHisui_6, 6, 0);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcHisui_6, 6, 0);

                                            nWriteBuffer = 0;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                            nAirRecoveryIndex = 0;
                                        }
                                        else
                                        {
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcMain_5, 5, 0);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcMain_5, 5, 0);

                                            nWriteBuffer = 0;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                            nAirRecoveryIndex = 0;
                                        }
                                    }
                                    else
                                    {
                                        if (nP1CharacterNumber == 19)
                                        {
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcKohaku_8, 8, 0);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcKohaku_8, 8, 0);

                                            nWriteBuffer = 5;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                            nAirRecoveryIndex = 5;
                                        }
                                        else
                                        {
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcAssist_7, 7, 0);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcAssist_7, 7, 0);

                                            nWriteBuffer = 5;
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                            nAirRecoveryIndex = 5;
                                        }
                                    }
                                }

                                if (nFMaidsHearts == MAX_HEARTS)
                                {
                                    char pcFive_2[2] = "5";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcFive_2, 2, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcFive_2, 2, 0);
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 0;
                                }
                                else if (nFMaidsHearts == -1)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcInfinite_10, 10, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcInfinite_10, 10, 0);
                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[8];
                                    strcpy_s(pcTemp, std::to_string(nFMaidsHearts).c_str());
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 8, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 8, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 8, 0);
                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 2;
                                }

                                if (bRyougiInfiniteKnife)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomLateString), &pcInfinite_10, 10, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcInfinite_10, 10, 0);
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 6;
                                }
                                else
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryNormalString), &pcNormal_7, 7, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryAllFastString), &pcNormal_7, 7, 0);
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 0;
                                }

                                break;
                            }
                            case HIGHLIGHT_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (nIdleHighlightSetting == NO_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nIdleHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else if (nIdleHighlightSetting == BLACK_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nIdleHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 19, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nIdleHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 2;
                                }

                                if (nBlockingHighlightSetting == NO_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nBlockingHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 0;
                                }
                                else if (nBlockingHighlightSetting == BLACK_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nBlockingHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 19, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nBlockingHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 2;
                                }

                                if (nHitHighlightSetting == NO_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nHitHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 0;
                                }
                                else if (nHitHighlightSetting == BLACK_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nHitHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 2;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nHitHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                    nWriteBuffer = 1;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 1;
                                }

                                if (nArmorHighlightSetting == NO_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nArmorHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 0;
                                }
                                else if (nArmorHighlightSetting == BLACK_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nArmorHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 19, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nArmorHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 2;
                                }

                                if (nThrowProtectionHighlightSetting == NO_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nThrowProtectionHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 0;
                                }
                                else if (nThrowProtectionHighlightSetting == BLACK_HIGHLIGHT)
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nThrowProtectionHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 19, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[19];
                                    strcpy_s(pcTemp, vHighlightNames[nThrowProtectionHighlightSetting]);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 2;
                                }
                                break;
                            }
                            case RNG_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);

                                if (nRNGMode == RNG_OFF)
                                    nWriteBuffer = OFFSCREEN_LOCATION;
                                else
                                    nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (nRNGMode == RNG_OFF)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOffString), &pcOff_4, 4, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcOff_4, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else if (nRNGMode == RNG_RN)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverCString), &pcValue_6, 6, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyRecoverDString), &pcValue_6, 6, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }
                                else if (nRNGMode == RNG_SEED)
                                {
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcSeed_5, 5, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcSeed_5, 5, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcSeed_5, 5, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 2;
                                }
                                if (nRNGMode != RNG_OFF)
                                {
                                    if (nRNGRate == RNG_EVERY_FRAME)
                                    {
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOffString), &pcEveryFrame_13, 13, 0);
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcEveryFrame_13, 13, 0);

                                        nWriteBuffer = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                        nEnemyDefenseIndex = 0;
                                    }
                                    else if (nRNGRate == RNG_EVERY_RESET)
                                    {
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseDodgeString), &pcEveryReset_13, 13, 0);
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcEveryReset_13, 13, 0);

                                        nWriteBuffer = 5;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                        nEnemyDefenseIndex = 5;
                                    }

                                    char pcTemp[19];
                                    if (nRNGMode == RNG_SEED)
                                    {
                                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 4, 0);
                                        std::string sSeedString = std::format("{:x}", nCustomSeed);
                                        std::transform(sSeedString.begin(), sSeedString.end(), sSeedString.begin(), ::toupper);
                                        strcpy_s(pcTemp, std::string("0x" + sSeedString).c_str());
                                    }
                                    if (nRNGMode == RNG_RN)
                                    {
                                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 4, 0);
                                        std::string sRNString = std::format("{:x}", nCustomRN);
                                        std::transform(sRNString.begin(), sRNString.end(), sRNString.begin(), ::toupper);
                                        strcpy_s(pcTemp, std::string("0x" + sRNString).c_str());
                                    }

                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                    nWriteBuffer = 1;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 1;
                                }
                                break;
                            }
                            case HOTKEYS_PAGE:
                            {
                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Replace PAGE text
                            char pcTemp[7];
                            strcpy_s(pcTemp, ("PAGE " + std::to_string(nSettingsPage)).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                            nReduceDamageIndex = 1;

                            nOldEnemyActionIndex = nEnemyActionIndex;
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                            nOldReduceDamageIndex = nReduceDamageIndex;
                        }

                        if (bOnHotkeySettingsMenu)
                        {
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // A Press handler
                            if (bAPressed && !bOldAPressed)
                            {
                                switch (nHotkeyPage)
                                {
                                case GENERIC_HOTKEYS_PAGE:
                                {
                                    if (nEnemySettingsCursor == 0)
                                    {
                                        ResetKeysHeld();
                                        bFreezeKeySet = false;
                                        nFreezeKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeKey), &nFreezeKey, 1, 0);
                                        SetRegistryValue(L"FreezeKey", nFreezeKey);
                                    }
                                    else if (nEnemySettingsCursor == 2)
                                    {
                                        ResetKeysHeld();
                                        bFrameStepKeySet = false;
                                        nFrameStepKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameStepKey), &nFrameStepKey, 1, 0);
                                        SetRegistryValue(L"FrameStepKey", nFrameStepKey);
                                    }
                                    else if (nEnemySettingsCursor == 3)
                                    {
                                        ResetKeysHeld();
                                        bHitboxDisplayKeySet = false;
                                        nHitboxDisplayKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitboxesDisplayKey), &nHitboxDisplayKey, 1, 0);
                                        SetRegistryValue(L"HitboxesDisplayKey", nHitboxDisplayKey);
                                    }
                                    else if (nEnemySettingsCursor == 5)
                                    {
                                        ResetKeysHeld();
                                        bFrameDataDisplayKeySet = false;
                                        nFrameDataDisplayKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameDataDisplayKey), &nFrameDataDisplayKey, 1, 0);
                                        SetRegistryValue(L"FrameDataDisplayKey", nFrameDataDisplayKey);
                                    }
                                    else if (nEnemySettingsCursor == 6)
                                    {
                                        ResetKeysHeld();
                                        bHighlightsOnKeySet = false;
                                        nHighlightsOnKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHighlightsOnKey), &nHighlightsOnKey, 1, 0);
                                        SetRegistryValue(L"HighlightsOnKey", nHighlightsOnKey);
                                    }
                                    break;
                                }
                                case FRAME_TOOL_HOTKEYS_PAGE:
                                {
                                    if (nEnemySettingsCursor == 0)
                                    {
                                        ResetKeysHeld();
                                        bSaveStateKeySet = false;
                                        nSaveStateKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveStateKey), &nSaveStateKey, 1, 0);
                                        SetRegistryValue(L"SaveStateKey", nSaveStateKey);
                                    }
                                    else if (nEnemySettingsCursor == 2)
                                    {
                                        ResetKeysHeld();
                                        bPrevSaveSlotKey = false;
                                        nPrevSaveSlotKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedPrevSaveSlotKey), &nPrevSaveSlotKey, 1, 0);
                                        SetRegistryValue(L"PrevSaveSlotKey", nPrevSaveSlotKey);
                                    }
                                    else if (nEnemySettingsCursor == 3)
                                    {
                                        ResetKeysHeld();
                                        bNextSaveSlotKey = false;
                                        nNextSaveSlotKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedNextSaveSlotKey), &nNextSaveSlotKey, 1, 0);
                                        SetRegistryValue(L"NextSaveSlotKey", nNextSaveSlotKey);
                                    }
                                    else if (nEnemySettingsCursor == 5)
                                    {
                                        ResetKeysHeld();
                                        bFrameBarScrollLeftKey = false;
                                        nFrameBarScrollLeftKey = 0;
                                        SetRegistryValue(L"FrameBarScrollLeftKey", nFrameBarScrollLeftKey);
                                    }
                                    else if (nEnemySettingsCursor == 6)
                                    {
                                        ResetKeysHeld();
                                        bFrameBarScrollRightKey = false;
                                        nFrameBarScrollRightKey = 0;
                                        SetRegistryValue(L"FrameBarScrollRightKey", nFrameBarScrollRightKey);
                                    }
                                    else if (nEnemySettingsCursor == 8)
                                    {

                                    }
                                    break;
                                }
                                case RNG_HOTKEYS_PAGE:
                                {
                                    if (nEnemySettingsCursor == 2)
                                    {
                                        ResetKeysHeld();
                                        bRNGDecKey = false;
                                        nRNGDecKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGDecKey), &nRNGDecKey, 1, 0);
                                        SetRegistryValue(L"RNGDecKey", nRNGDecKey);
                                    }
                                    else if (nEnemySettingsCursor == 3)
                                    {
                                        ResetKeysHeld();
                                        bRNGIncKey = false;
                                        nRNGIncKey = 0;
                                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGIncKey), &nRNGIncKey, 1, 0);
                                        SetRegistryValue(L"RNGIncKey", nRNGIncKey);
                                    }
                                    break;
                                }
                                default:
                                    break;
                                }
                            }

                            // Replace static menu fields
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset_17, 17, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings_18, 18, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageString), &pcBlank_1, 1, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDefaultString), &pcBlank_1, 1, 0);
                            switch (nHotkeyPage)
                            {
                            case GENERIC_HOTKEYS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcFreeze_11, 11, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcNextStep_14, 14, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcHitboxes_13, 13, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcFrameDisplay_18, 18, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcHighlights_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);
                                break;
                            }
                            case FRAME_TOOL_HOTKEYS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcSaveState_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcPrevSaveSlot_19, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcNextSaveSlot_19, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcFrameBarLeft_15, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcFrameBarRight_17, 17, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);
                                break;
                            }
                            case RNG_HOTKEYS_PAGE:
                            {
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcBlank_1, 1, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseString), &pcPrevRNG_13, 13, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcPrevRNG_13, 13, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryString), &pcBlank_1, 15, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryString), &pcBlank_1, 17, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryString), &pcBlank_1, 1, 0);
                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Replace Main Info text
                            switch (nHotkeyPage)
                            {
                            case GENERIC_HOTKEYS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "{Freeze} the game in place.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bFreezeKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Advance the game by one frame while frozen.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bFrameStepKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Turn {hitboxes} on or off.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bHitboxDisplayKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Turn {in-game frame data} on or off";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bFrameDataDisplayKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Turn {character highlighting} on or off.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bHighlightsOnKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case FRAME_TOOL_HOTKEYS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    char pcTemp64[64] = "{Save a state} in the current slot.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bSaveStateKeySet)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Switch to a {previous save slot}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bPrevSaveSlotKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Switch to the {next save slot}.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bNextSaveSlotKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    char pcTemp64[64] = "Scroll the in-game frame data {left}";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bFrameBarScrollLeftKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 6:
                                {
                                    char pcTemp64[64] = "Scroll the in-game frame data {right}";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bFrameBarScrollRightKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            case RNG_HOTKEYS_PAGE:
                            {
                                switch (nEnemySettingsCursor)
                                {
                                case 0:
                                {
                                    break;
                                }
                                case 2:
                                {
                                    char pcTemp64[64] = "Increment the RNG value.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bRNGIncKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 3:
                                {
                                    char pcTemp64[64] = "Deccrement the RNG value.";
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcTemp64, 64, 0);

                                    if (bRNGDecKey)
                                        strcpy_s(pcTemp64, ">{Press A} to change the key.");
                                    else
                                        strcpy_s(pcTemp64, ">{Press any key} to set the key.");
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcTemp64, 64, 0);
                                    break;
                                }
                                case 5:
                                {
                                    break;
                                }
                                case 6:
                                {
                                    break;
                                }
                                case 8:
                                {
                                    break;
                                }
                                default:
                                    break;
                                }
                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

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

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Skipping blank menu items
                            switch (nHotkeyPage)
                            {
                            case GENERIC_HOTKEYS_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 6 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 6;
                                    nOldEnemySettingsCursor = 6;
                                }
                                break;
                            }
                            case FRAME_TOOL_HOTKEYS_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 6 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 6;
                                    nOldEnemySettingsCursor = 6;
                                }
                                break;
                            }
                            case RNG_HOTKEYS_PAGE:
                            {
                                if (nOldEnemySettingsCursor == 2 && nEnemySettingsCursor == 0)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                                {
                                    nWriteBuffer = 10;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                else if (nEnemySettingsCursor > 3 && nEnemySettingsCursor < 10)
                                {
                                    nWriteBuffer = 3;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 3;
                                    nOldEnemySettingsCursor = 3;
                                }
                                else if (nEnemySettingsCursor == 0)
                                {
                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 2;
                                    nOldEnemySettingsCursor = 2;
                                }

                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // left-right handlers
                            switch (nHotkeyPage)
                            {
                            case GENERIC_HOTKEYS_PAGE:
                            case FRAME_TOOL_HOTKEYS_PAGE:
                            case RNG_HOTKEYS_PAGE:
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // PAGE number handler
                            if (nOldReduceDamageIndex == -1)
                                nOldReduceDamageIndex = nReduceDamageIndex;
                            else if (nOldReduceDamageIndex > nReduceDamageIndex || (bF1Pressed && !bOldF1Pressed))// left
                            {
                                nHotkeyPage = max(0, nHotkeyPage - 1);
                                if (nHotkeyPage == 0)
                                    nHotkeyPage = MAX_HOTKEY_PAGES;

                                nOldEnemyActionIndex = -1;
                                nOldEnemyDefenseIndex = -1;
                                nOldEnemyDefenseTypeIndex = -1;
                                nOldAirRecoveryIndex = -1;
                                nOldDownRecoveryIndex = -1;
                                nOldThrowRecoveryIndex = -1;
                                nOldReduceDamageIndex = -1;
                            }
                            else if (nOldReduceDamageIndex < nReduceDamageIndex || (bF2Pressed && !bOldF2Pressed))// right
                            {
                                nHotkeyPage = min(nHotkeyPage + 1, MAX_HOTKEY_PAGES + 1);
                                if (nHotkeyPage == MAX_HOTKEY_PAGES + 1)
                                    nHotkeyPage = 1;

                                nOldEnemyActionIndex = -1;
                                nOldEnemyDefenseIndex = -1;
                                nOldEnemyDefenseTypeIndex = -1;
                                nOldAirRecoveryIndex = -1;
                                nOldDownRecoveryIndex = -1;
                                nOldThrowRecoveryIndex = -1;
                                nOldReduceDamageIndex = -1;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // dynamic menu items
                            switch (nHotkeyPage)
                            {
                            case GENERIC_HOTKEYS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                // FREEZE
                                char pcTemp[19];
                                if (!bFreezeKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 0)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bFreezeKeySet = true;
                                            nFreezeKey = nKeyJustPressed;
                                            SetRegistryValue(L"FreezeKey", nFreezeKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeKey), &nFreezeKey, 1, 0);
                                            ReplaceKey(nFreezeKey, KEY_FREEZE);
                                        }
                                    }
                                }
                                if (bFreezeKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nFreezeKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                nEnemyActionIndex = 2;

                                // FRAME STEP
                                if (!bFrameStepKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 2)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bFrameStepKeySet = true;
                                            nFrameStepKey = nKeyJustPressed;
                                            SetRegistryValue(L"FrameStepKey", nFrameStepKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameStepKey), &nFrameStepKey, 1, 0);
                                            ReplaceKey(nFrameStepKey, KEY_FRAMESTEP);
                                        }
                                    }
                                }
                                if (bFrameStepKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nFrameStepKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 2;

                                // HITBOX
                                if (!bHitboxDisplayKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 3)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bHitboxDisplayKeySet = true;
                                            nHitboxDisplayKey = nKeyJustPressed;
                                            SetRegistryValue(L"HitboxesDisplayKey", nHitboxDisplayKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitboxesDisplayKey), &nHitboxDisplayKey, 1, 0);
                                            ReplaceKey(nHitboxDisplayKey, KEY_HITBOX);
                                        }
                                    }
                                }
                                if (bHitboxDisplayKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nHitboxDisplayKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 1;

                                // FRAME BAR
                                if (!bFrameDataDisplayKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 5)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bFrameDataDisplayKeySet = true;
                                            nFrameDataDisplayKey = nKeyJustPressed;
                                            SetRegistryValue(L"FrameDataDisplayKey", nFrameDataDisplayKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameDataDisplayKey), &nFrameDataDisplayKey, 1, 0);
                                            ReplaceKey(nFrameDataDisplayKey, KEY_FRAMEDATA);
                                        }
                                    }
                                }
                                if (bFrameDataDisplayKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nFrameDataDisplayKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 2;

                                // HIGHLIGHT
                                if (!bHighlightsOnKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 6)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bHighlightsOnKeySet = true;
                                            nHighlightsOnKey = nKeyJustPressed;
                                            SetRegistryValue(L"HighlightsOnKey", nHighlightsOnKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHighlightsOnKey), &nHighlightsOnKey, 1, 0);
                                            ReplaceKey(nHighlightsOnKey, KEY_HIGHLIGHT);
                                        }
                                    }
                                }
                                if (bHighlightsOnKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nHighlightsOnKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                nDownRecoveryIndex = 2;

                                break;
                            }
                            case FRAME_TOOL_HOTKEYS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                char pcTemp[19];

                                // SAVE STATE
                                if (!bSaveStateKeySet)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 0)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bSaveStateKeySet = true;
                                            nSaveStateKey = nKeyJustPressed;
                                            SetRegistryValue(L"SaveStateKey", nSaveStateKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveStateKey), &nSaveStateKey, 1, 0);
                                            ReplaceKey(nSaveStateKey, KEY_SAVESTATE);
                                        }
                                    }
                                }
                                if (bSaveStateKeySet)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nSaveStateKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                nEnemyActionIndex = 2;

                                // PREV SAVE SLOT
                                if (!bPrevSaveSlotKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 2)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bPrevSaveSlotKey = true;
                                            nPrevSaveSlotKey = nKeyJustPressed;
                                            SetRegistryValue(L"PrevSaveSlotKey", nPrevSaveSlotKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedPrevSaveSlotKey), &nPrevSaveSlotKey, 1, 0);
                                            ReplaceKey(nPrevSaveSlotKey, KEY_PREVSAVE);
                                        }
                                    }
                                }
                                if (bPrevSaveSlotKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nPrevSaveSlotKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 2;

                                // NEXT SAVE SLOT
                                if (!bNextSaveSlotKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 3)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bNextSaveSlotKey = true;
                                            nNextSaveSlotKey = nKeyJustPressed;
                                            SetRegistryValue(L"NextSaveSlotKey", nNextSaveSlotKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedNextSaveSlotKey), &nNextSaveSlotKey, 1, 0);
                                            ReplaceKey(nNextSaveSlotKey, KEY_NEXTSAVE);
                                        }
                                    }
                                }
                                if (bNextSaveSlotKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nNextSaveSlotKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 1;

                                // FRAME BAR SCROLL LEFT SLOT
                                if (!bFrameBarScrollLeftKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 5)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bFrameBarScrollLeftKey = true;
                                            nFrameBarScrollLeftKey = nKeyJustPressed;
                                            SetRegistryValue(L"FrameBarScrollLeftKey", nFrameBarScrollLeftKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameBarScrollLeftKey), &nFrameBarScrollLeftKey, 1, 0);
                                            ReplaceKey(nFrameBarScrollLeftKey, KEY_FRAMEBARLEFT);
                                        }
                                    }
                                }
                                if (bFrameBarScrollLeftKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nFrameBarScrollLeftKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 2;

                                // FRAME BAR SCROLL RIGHT SLOT
                                if (!bFrameBarScrollRightKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 6)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bFrameBarScrollRightKey = true;
                                            nFrameBarScrollRightKey = nKeyJustPressed;
                                            SetRegistryValue(L"FrameBarScrollRightKey", nFrameBarScrollRightKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameBarScrollRightKey), &nFrameBarScrollRightKey, 1, 0);
                                            ReplaceKey(nFrameBarScrollRightKey, KEY_FRAMEBARRIGHT);
                                        }
                                    }
                                }
                                if (bFrameBarScrollRightKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nFrameBarScrollRightKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                nDownRecoveryIndex = 2;

                                break;
                            }
                            case RNG_HOTKEYS_PAGE:
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);

                                char pcTemp[19];
                                // DEC RNG
                                if (!bRNGDecKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 2)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bRNGDecKey = true;
                                            nRNGDecKey = nKeyJustPressed;
                                            SetRegistryValue(L"RNGDecKey", nRNGDecKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGDecKey), &nRNGDecKey, 1, 0);
                                            ReplaceKey(nRNGDecKey, KEY_RNGDEC);
                                        }
                                    }
                                }
                                if (bRNGDecKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nRNGDecKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 2;

                                // Inc RNG
                                if (!bRNGIncKey)
                                {
                                    uint8_t nKeyJustPressed = 0;
                                    if (nEnemySettingsCursor == 3)
                                        nKeyJustPressed = KeyJustPressed();

                                    strcpy_s(pcTemp, "PRESS ANY KEY");
                                    if (nKeyJustPressed != 0)
                                    {
                                        if (IsControllerNeutral())
                                        {
                                            bRNGIncKey = true;
                                            nRNGIncKey = nKeyJustPressed;
                                            SetRegistryValue(L"RNGIncKey", nRNGIncKey);
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGIncKey), &nRNGIncKey, 1, 0);
                                            ReplaceKey(nRNGIncKey, KEY_RNGINC);
                                        }
                                    }
                                }
                                if (bRNGIncKey)
                                {
                                    WCHAR wcName[19];
                                    UINT scanCode = MapVirtualKeyW(nRNGIncKey, MAPVK_VK_TO_VSC);
                                    LONG lParamValue = (scanCode << 16);
                                    int result = GetKeyNameTextW(lParamValue, wcName, 19);
                                    std::wstring wsKeyName = wcName;
                                    std::string sKeyName = std::string(wsKeyName.begin(), wsKeyName.end());
                                    strcpy_s(pcTemp, sKeyName.c_str());
                                }

                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 19, 0);
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 19, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 1;

                                break;
                            }
                            default:
                                break;
                            }

                            // kludge
                            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                            nCurrentSubMenu = nReadResult;
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // Replace PAGE text
                            char pcTemp[7];
                            strcpy_s(pcTemp, ("PAGE " + std::to_string(nHotkeyPage)).c_str());
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                            nWriteBuffer = 1;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                            nReduceDamageIndex = 1;

                            nOldEnemyActionIndex = nEnemyActionIndex;
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
                            nOldThrowRecoveryIndex = nThrowRecoveryIndex;
                            nOldReduceDamageIndex = nReduceDamageIndex;
                        }

                        /// ENEMY SETTINGS
                        if (!bOnSettingsMenu && !bOnHotkeySettingsMenu && nCurrentSubMenu == eMenu::ENEMY_SETTINGS)
                        {
                            if (nCurrentSubMenu != eMenu::ENEMY_SETTINGS)
                                continue;

                            // enemy action.
                            char pcEnemyActionInfo[16] = "enemy reversal.";
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionInfoStringAddress), &pcEnemyActionInfo, 16, 0);

                            // ENEMY ACTION
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionString), &pcBlank_1, 1, 0);

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

                            if (nEnemySettingsCursor == 0)
                            {
                                nWriteBuffer = 2;
                                WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                nEnemySettingsCursor = 2;
                                nOldEnemySettingsCursor = 2;
                            }

                            nWriteBuffer = OFFSCREEN_LOCATION;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwEnemyActionOptionX), &nWriteBuffer, 4, 0);

                            /*if (nOldEnemyActionIndex == -1)
                                nOldEnemyActionIndex = nEnemyActionIndex;
                            else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                nReversalIndex1 = max(0, nReversalIndex1 - 1);
                            else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                nReversalIndex1 = min(nReversalIndex1 + 1, vPatternNames.size() - 1);*/

                                /*if (nReversalIndex1 == 0)
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

                                nOldEnemyActionIndex = nEnemyActionIndex;*/

                            nStoredEnemyDefense = nEnemyDefenseIndex;
                            nStoredEnemyAction = nEnemyActionIndex;
                            nStoredEnemyDefenseType = nEnemyDefenseTypeIndex;
                            nStoredAirRecovery = (nAirRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                            nStoredDownRecovery = (nDownRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                            nStoredThrowRecovery = nThrowRecoveryIndex;
                            nStoredReduceDamage = nReduceDamageIndex;
                        }
                    }
                    else
                    {
                        // don't want to write anything on the main menu
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedOnExtendedSettings), &bOnSettingsMenu, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcBlank_64, 64, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcBlank_64, 64, 0);
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
                    if (bJustUnpaused)
                    {
                        bJustUnpaused = false;

                        // put stuff here
                    }

                    // want to reset these for a clean setup next time the game is paused
                    bOnSettingsMenu = false;
                    bInExtendedSettings = false;
                    nOldCurrentSubMenu = -1;
                    nCurrentSubMenu = eMenu::MAIN;

                    // Enable Ex Guard.  randomly if applicable
                    nWriteBuffer = 1;
                    if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                        nWriteBuffer = 10;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2ExGuard), &nWriteBuffer, 4, 0);

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
                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstopRemaining), &nReadResult, 1, 0);
                        int nHitstopRemaining = nReadResult;
                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 4, 0);
                        int nHitstunRemaining = nReadResult;
                        if (nComboCount >= nHitsTillBurst && nHitstopRemaining == 0 && nHitstunRemaining != 0)
                        {
                            while (true)
                            {
                                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 4, 0);
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
                    if (nFrameCounter == 1 && !CheckSave(nSaveSlot))
                    {
                        SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                        SetGuard(hMBAAHandle, dwBaseAddress, nCustomGuard, nP1Moon, nP2Moon);
                        SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon);

                        if (bPositionsLocked)
                        {
                            nWriteBuffer = nP1X;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1X), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP2X;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2X), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP3X;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP3X), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP4X;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4X), &nWriteBuffer, 4, 0);
                        }
                    }

                    // infinite guard bar
                    if (bInfGuard)
                        SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon);

                    // increase the counter every frame p2 is standing idle to delay regenerating health and char specifics
                    // taking an extra step to cap these at 20 to avoid any unexpected behavior if tmode is left running forever
                    //ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 4, 0);
                    int nHitstunRemaining = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    int nP2Pattern = nReadResult;
                    //if (nReadResult == 0 || nReadResult == 13)
                    if (nHitstunRemaining == 0 && nP2Pattern != 350)
                    {
                        nHealthRefillTimer = 1;
                        nSionBulletsRefillTimer = 1;
                    
                        // roa needs to be reset a little differently
                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaVisibleCharge), &nReadResult, 4, 0);
                        if (nReadResult < nRoaVisibleCharge)
                            nRoaVisibleChargeRefillTimer = 1;
                        else
                            nRoaVisibleChargeRefillTimer = 0;

                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1RoaHiddenCharge), &nReadResult, 4, 0);
                        if (nReadResult < nRoaVisibleCharge)
                            nRoaHiddenChargeRefillTimer = 1;
                        else
                            nRoaHiddenChargeRefillTimer = 0;
                    }
                    else
                    {
                        nHealthRefillTimer = 0;
                        nSionBulletsRefillTimer = 0;
                    }

                

                    // refill health if training mode is reset or long enough time has passed
                    if ((nFrameCounter == 1 && !CheckSave(nSaveSlot)) || (nHealthRefillTimer == 1 && bLifeRecover))
                    {
                        SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth);
                        nHealthRefillTimer = 0;
                    }

                    // refill character specifics
                    if (!CheckSave(nSaveSlot))
                    {
                        if ((nFrameCounter == 1 || nSionBulletsRefillTimer == 1 || nSionBullets == 15) && nSionBullets != 14)
                        {
                            SetSionBullets(hMBAAHandle, dwBaseAddress, nSionBullets);
                            nSionBulletsRefillTimer = 0;
                        }

                        // refill character specifics
                        if (nFrameCounter == 1 || nRoaHiddenChargeRefillTimer == 1 || nRoaHiddenCharge == -2 && nRoaHiddenCharge != -1)
                        {
                            SetRoaHiddenCharge(hMBAAHandle, dwBaseAddress, nRoaHiddenCharge);
                            nRoaHiddenChargeRefillTimer = 0;
                        }

                        // refill character specifics
                        if ((nFrameCounter == 1 || nRoaVisibleChargeRefillTimer == 1 || nRoaVisibleCharge == -2) && nRoaVisibleCharge != -1)
                        {
                            SetRoaVisibleCharge(hMBAAHandle, dwBaseAddress, nRoaVisibleCharge);
                            nRoaVisibleChargeRefillTimer = 0;
                        }

                        if (nFrameCount == 1 || nFMaidsHearts == -1)
                        {
                            SetFMaidsHearts(hMBAAHandle, dwBaseAddress, nFMaidsHearts);
                        }

                        if (bRyougiInfiniteKnife)
                            SetRyougiKnife(hMBAAHandle, dwBaseAddress, true);

                        //reset character specifics
                        if (nFrameCounter == 1 && bP3Exists)
                        {
                            WriteProcessMemory(hMBAAHandle, (LPVOID) (dwBaseAddress + adP1ControlledCharacter), &nP1ControlledCharacter, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1TagFlag), &nP1TagFlag, 1, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3TagFlag), &nP3TagFlag, 1, 0);
                        }
                    }
                
                    // easy access debug screen clear
                    if (GetKeyState(VK_RETURN) < 0)
                    {
                        //system("cls");
                    }

    //#ifdef Obsolete
                    // convoluted reversal pattern logic
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwBurstCooldown), &nReadResult, 1, 0);
                    nBurstCooldown = nReadResult;

                    nReadResult = 0;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 1, 0);
                    int nHitstun = nReadResult;

                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Y), &nReadResult, 4, 0);
                    nP2Y = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
                    nOldMot = nMot;
                    nMot = nReadResult;
                    if (nFrameCounter == 0)
                        bReversaled = true;
                    if (nFrameCounter == 2)
                    {
                        bReversaled = false;
                        bDelayingReversal = false;
                    }

                    // if training was not just reset and if at least one move was selected
                    if (nFrameCounter != 0 && (GetPattern(nP2CharacterID, vPatternNames[nReversalIndex1]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex2]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex3]) != 0 || GetPattern(nP2CharacterID, vPatternNames[nReversalIndex4]) != 0))
                    {
                        if (!bDelayingReversal && (nMot == 0 || nHitstun != 0) && (nMot != nOldMot || nReversalType == REVERSAL_REPEAT) /* && nP2Y == 0*/)
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
                                std::vector<int> vValidReversals = (nP2Y == 0 ? vGroundReversals : vAirReversals);
                                if (vValidReversals.size() != 0)
                                {
                                    int nTempReversalIndex = 0;
                                    while (1)
                                    {
                                        nTempReversalIndex = rand() % vValidReversals.size();
                                        if (vValidReversals[nTempReversalIndex] != 0)
                                            break;
                                    }
                                    nWriteBuffer = vValidReversals[nTempReversalIndex];
                                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                                    nTempReversalDelayFrames = nReversalDelayFrames;
                                }
                            }
                            else
                                bReversaled = false;
                        }
                        else
                            nTempReversalDelayFrames--;
                    }
    //#endif
                    /*bSwitchToCrouch = false;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    if (bSwitchToCrouch && nReadResult == eEnemyStance::STANDGUARDING)
                    {
                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyStatusCopy), &nWriteBuffer, 4, 0);
                    }*/
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        DeleteRegistry();
        LogError("Exception: " + std::string(ex.what()));
    }

    CloseLogger();
    CloseHandle(hMBAAHandle);
    return 0;
}
