#include "MBAACC-Extended-Training-Mode.h"

int main(int argc, char* argv[])
{

    if (!writeDLL()) {
        fprintf(stderr, "FAILED TO WRITE DLL\n");
        fprintf(stderr, "press any key to exit\n");
        getchar();
        return 1;
    }

    /*while (true) {
        KeyState::updateControllers();
        KeyState::showControllerState();
        Sleep(16);
    }*/

    timeBeginPeriod(1);
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
        bool bDoingHeldReversal = false;
        bool bRecoverD = false;

        float arrP1Guts[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        float arrP2Guts[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

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

        float fP1FancyInputX = DEFAULT_P1_FANCYINPUT_X;
        float fP1FancyInputY = DEFAULT_P1_FANCYINPUT_Y;
        float fP2FancyInputX = DEFAULT_P2_FANCYINPUT_X;
        float fP2FancyInputY = DEFAULT_P2_FANCYINPUT_Y;
        float fP1ListInputX = DEFAULT_P1_LISTINPUT_X;
        float fP1ListInputY = DEFAULT_P1_LISTINPUT_Y;
        float fP2ListInputX = DEFAULT_P2_LISTINPUT_X;
        float fP2ListInputY = DEFAULT_P2_LISTINPUT_Y;

        uint8_t nRNGMode = RNG_OFF;
        uint8_t nRNGRate = RNG_EVERY_FRAME;
        uint32_t nCustomSeed = 0;
        uint32_t nCustomRN = 0;

        bool bColorBlindMode = false;
        uint8_t nHitboxStyle = HITBOX_DRAW_ALL;
        bool bDisplayHitboxes = false;
        bool bExtendOrigins = false;

        bool bDisableHud = false;
        uint8_t nBackgroundStyle = false;
        bool bDrawGround = false;
        bool bDisableShadow = false;
        DWORD dwDisableExtras = 0x00000000;

        int nFrameBarY = 400;
        bool bShowStats = true;
        uint8_t nP1InputDisplay = INPUT_OFF;
        uint8_t nP2InputDisplay = INPUT_OFF;
        bool bAttackInfo = false;

        bool bFastReversePenalty = false;

        bool bSlow = false;
        uint8_t nSlow = SLOW_THREE_FOURTHS;

        int nSettingsPage = 1;
        int nHotkeyPage = 1;

        int nGameMode = 0;
        int nVersusCheck = 0;

        bool bPositionsLocked = false;
        int nP1X = P1_DEFAULT_X;
        int nP2X = P2_DEFAULT_X;
        int nP3X = P3_DEFAULT_X;
        int nP4X = P4_DEFAULT_X;
        bool bP3Exists = false;
        bool bP4Exists = false;

        bool bJustUnpaused = false;
        bool bInjected = false;

        uint8_t nFrameData = FRAMEDISPLAY_NORMAL;
        //bool bSaveStates = false;

        std::unordered_set<DWORD> setBlockingAnimationPointers;
        std::unordered_set<DWORD> setIdleAnimationPointers;
        int nIdleHighlightSetting = NO_HIGHLIGHT;
        int nBlockingHighlightSetting = NO_HIGHLIGHT;
        int nHitHighlightSetting = NO_HIGHLIGHT;
        int nArmorHighlightSetting = NO_HIGHLIGHT;
        int nThrowProtectionHighlightSetting = NO_HIGHLIGHT;
        bool bHighlight = false;

        bool bFrameDataDisplay = false;

        int nHitsTillBurst = TOO_HIGH_TO_BURST;
        int nHitsTillBunker = 0;
        bool bAlreadyBursted = false;
        bool bBunkerInsteadOfBurst = false;

        bool bInfGuard = true;
    
        bool bNeedToAnnounceNewVersion = false;
        int nMovingMessageIndex = 0;
        int nStartingTime = 0;
        std::string sOnlineVersion;
        int nCurrentTime;

        HANDLE hConsoleHandle;


        char pcModPath[MAX_PATH];
        GetModuleFileNameA(NULL, pcModPath, sizeof(pcModPath));
        std::vector<std::string> vPathTokens;
        std::istringstream f(pcModPath);
        std::string s;
        while (std::getline(f, s, '\\'))
            vPathTokens.push_back(s + "\\");
        vPathTokens.pop_back();
        std::string sInstallPath = std::accumulate(vPathTokens.begin(), vPathTokens.end(), std::string{});
        //std::string sDLLPath = sInstallPath + "Extended-Training-Mode-DLL.dll";
        std::wstring sDLLPath = getDLLPath();
        while (!std::ifstream(sDLLPath).good())
        {
            std::wstring sErrorString = L"UNABLE TO FIND " + sDLLPath;
            //int nReturnVal = MessageBoxA(NULL, sErrorString.c_str(), "", MB_ICONERROR | MB_RETRYCANCEL);
            switch (MessageBoxW(NULL, sErrorString.c_str(), L"", MB_ICONERROR | MB_RETRYCANCEL))
            {
            case IDRETRY:
                continue;
            default:
                return 0;
                break;
            }
        }
        
        // Not needed currently, but if we ever need it this can be uncommented
        //SetRegistryValue(L"InstallPath", sInstallPath);

        try
        {
            sOnlineVersion = GetLatestVersion();
            if (sOnlineVersion != "" && sOnlineVersion != VERSION && false)
            {
                std::string sUpdate = "A new version of the Extended Training Mode is available.\nWould you like to update?";
                switch (MessageBoxA(NULL, sUpdate.c_str(), "", MB_YESNO | MB_ICONASTERISK | MB_DEFBUTTON1))
                {
                case IDYES:
                {
                    std::string sUpdaterName = sInstallPath + UPDATER_NAME;

                    STARTUPINFOA si;
                    PROCESS_INFORMATION pi;

                    ZeroMemory(&si, sizeof(si));
                    si.cb = sizeof(si);
                    ZeroMemory(&pi, sizeof(pi));

                    std::string sCmdArgs =  UPDATER_NAME + " " +                        // Updater Name
                                            sInstallPath + " " +                        // Path to the intstallation
                                            GITHUB_DOWNLOAD + sOnlineVersion + "/ " +   // Path to the latest download
                                            "exampleArg1;exampleArg2 " +                // cmdArgs to pass back
                                            EXE_NAME + " " +                            // 1st download and what gets launched
                                            DLL_NAME;                                   // 2nd download
                                                                                        // append more files to download
                    
                    CreateProcessA(sUpdaterName.c_str(),    // the path
                        (LPSTR)sCmdArgs.c_str(),            // Command line
                        NULL,                               // Process handle not inheritable
                        NULL,                               // Thread handle not inheritable
                        FALSE,                              // Set handle inheritance to FALSE
                        0,                                  // No creation flags
                        NULL,                               // Use parent's environment block
                        NULL,                               // Use parent's starting directory 
                        &si,                                // Pointer to STARTUPINFO structure
                        &pi                                 // Pointer to PROCESS_INFORMATION structure
                    );
                    
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);

                    TerminateProcess(GetCurrentProcess(), 0);
                }
                default:
                    break;
                }

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
            bPrevSaveSlotKeySet = true;
        ReadFromRegistry(L"NextSaveSlotKey", &nNextSaveSlotKey);
        if (MapVirtualKeyW(nNextSaveSlotKey, MAPVK_VK_TO_VSC) == 0)
            nNextSaveSlotKey = nDefaultNextSaveSlotKey;
        else
            bNextSaveSlotKeySet = true;
        ReadFromRegistry(L"FrameBarScrollLeftKey", &nFrameBarScrollLeftKey);
        if (MapVirtualKeyW(nFrameBarScrollLeftKey, MAPVK_VK_TO_VSC) == 0)
            nFrameBarScrollLeftKey = nDefaultFrameBarScrollLeftKey;
        else
            bFrameBarScrollLeftKeySet = true;
        ReadFromRegistry(L"FrameBarScrollRightKey", &nFrameBarScrollRightKey);
        if (MapVirtualKeyW(nFrameBarScrollRightKey, MAPVK_VK_TO_VSC) == 0)
            nFrameBarScrollRightKey = nDefaultFrameBarScrollRightKey;
        else
            bFrameBarScrollRightKeySet = true;
        ReadFromRegistry(L"RNGIncKey", &nRNGIncKey);
        if (MapVirtualKeyW(nRNGIncKey, MAPVK_VK_TO_VSC) == 0)
            nRNGIncKey = nDefaultRNGIncKey;
        else
            bRNGIncKeySet = true;
        ReadFromRegistry(L"RNGDecKey", &nRNGDecKey);
        if (MapVirtualKeyW(nRNGDecKey, MAPVK_VK_TO_VSC) == 0)
            nRNGDecKey = nDefaultRNGDecKey;
        else
            bRNGIncKeySet = true;
        ReadFromRegistry(L"ReversalKey", &nReversalKey);
        if (MapVirtualKeyW(nReversalKey, MAPVK_VK_TO_VSC) == 0)
            nReversalKey = nDefaultReversalKey;
        else
            bReversalKeySet = true;
        ReadFromRegistry(L"SlowKey", &nSlowKey);
        if (MapVirtualKeyW(nSlowKey, MAPVK_VK_TO_VSC) == 0)
            nSlowKey = nDefaultSlowKey;
        else
            bSlowKeySet = true;
        ReadFromRegistry(L"FrameDisplay", &nFrameData);
        if (nFrameData == FRAMEDISPLAY_NORMAL)
            bSimpleFrameInfo = true;
        else if (nFrameData == FRAMEDISPLAY_ADVANCED)
            bSimpleFrameInfo = false;
        ReadFromRegistry(L"DisplayFreeze", &bDisplayFreeze);
        ReadFromRegistry(L"DisplayInputs", &bDisplayInputs);
        ReadFromRegistry(L"DisplayCancels", &bDisplayCancels);
        ReadFromRegistry(L"HitboxStyle", &nHitboxStyle);
        ReadFromRegistry(L"FrameBarY", &nFrameBarY);
        ReadFromRegistry(L"P1InputDisplay", &nP1InputDisplay);
        ReadFromRegistry(L"P2InputDisplay", &nP2InputDisplay);
        ReadFromRegistry(L"BlockingHighlight", &nBlockingHighlightSetting);
        ReadFromRegistry(L"ThrowProtectionHighlight", &nThrowProtectionHighlightSetting);
        ReadFromRegistry(L"HitHighlight", &nHitHighlightSetting);
        ReadFromRegistry(L"IdleHighlight", &nIdleHighlightSetting);
        ReadFromRegistry(L"ArmorHighlight", &nArmorHighlightSetting);
        ReadFromRegistry(L"HighlightToggle", &bHighlight);
        ReadFromRegistry(L"P1FancyInputX", &fP1FancyInputX);
        ReadFromRegistry(L"P1FancyInputY", &fP1FancyInputY);
        ReadFromRegistry(L"P2FancyInputX", &fP2FancyInputX);
        ReadFromRegistry(L"P2FancyInputY", &fP2FancyInputY);
        ReadFromRegistry(L"P1ListInputX", &fP1ListInputX);
        ReadFromRegistry(L"P1ListInputY", &fP1ListInputY);
        ReadFromRegistry(L"P2ListInputX", &fP2ListInputX);
        ReadFromRegistry(L"P2ListInputY", &fP2ListInputY);

        while (1)
        {
            nCurrentTime = std::time(nullptr);
        
            // calling std::endl flushes to the console. doing that multiple times a frame causes slowdown.

            
            SetConsoleCursorPosition(hConsoleHandle, { 0, 0 });
            std::cout << "===========================================================================\x1b[K" << "\n";
            std::cout << "|   Fang, gonp, and meepster99(Inana)'s Extended Training Mode Mod " << VERSION << "   |\x1b[K" << "\n";
            std::cout << "|                                                                         |\x1b[K" << "\n";
            std::cout << "|   " << GITHUB_RELEASE << "   |\x1b[K" << "\n";
            if (bNeedToAnnounceNewVersion && nCurrentTime % 3 != 0)
                std::cout << "|   NEW VERSION " << sOnlineVersion << " AVAILABLE ON GITHUB                                  |\x1b[K" << "\n";
            else
                std::cout << "|                                                                         |\x1b[K" << "\n";
            std::cout << "===========================================================================\x1b[K" << "\n";
            
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

                bInjected = false;
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
            }

            // check this to prevent attaching to netplay
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGameMode), &nGameMode, 4, 0);
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwVersusCheck), &nVersusCheck, 4, 0);
            if(nGameMode == 1 && nVersusCheck == 1) //Versus
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "Cannot attach to versus mode....\x1b[K";
                //LogInfo("MBAA is in versus mode");
                continue;
            }
            else if (nGameMode == 4112 || (nGameMode == 1 && nVersusCheck == 2)) //Training OR Replay
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });               
                std::cout << "Attached to MBAA.exe\x1b[K" << "\n\x1b[K\n";
                
                if (!bInjected)
                {
                    DWORD dwPID = GetProcessPID(L"MBAA.exe");
                    if (InjectIntoMBAA(dwPID, sDLLPath) != 0)
                    {
                        bInjected = true;

                        // Set some initial stuff in the DLL
                        bool bTrue = true;
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
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedReversalKey), &nReversalKey, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedReversalKeyHeld), &bTrue, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSlowKey), &nSlowKey, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSlowSpeed), &bSlow, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nSaveSlot, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayFreeze), &bDisplayFreeze, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayInputs), &bDisplayInputs, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGMode), &nRNGMode, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGRate), &nRNGRate, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomSeed), &nCustomSeed, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedRNGCustomRN), &nCustomRN, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitboxStyle), &nHitboxStyle, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedColorBlindMode), &bColorBlindMode, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisplayHitboxes), &bDisplayHitboxes, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedExtendOrigins), &bExtendOrigins, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adHealthRestore), &MAX_HEALTH, 4, 0);

                        //std::array<uint8_t, 4> arrTemp = CreateColorArray2(NO_HIGHLIGHT);
                        std::array<uint8_t, 4> arrTempBlockingHighlight = CreateColorArray2(nBlockingHighlightSetting);
                        std::array<uint8_t, 4> arrTempHitHighlight = CreateColorArray2(nHitHighlightSetting);
                        std::array<uint8_t, 4> arrTempThrowProtectionHighlight = CreateColorArray2(nThrowProtectionHighlightSetting);
                        std::array<uint8_t, 4> arrTempIdleHighlight = CreateColorArray2(nIdleHighlightSetting);
                        std::array<uint8_t, 4> arrTempArmorHighlight = CreateColorArray2(nArmorHighlightSetting);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedIdleHighlight), &arrTempIdleHighlight, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedBlockingHighlight), &arrTempBlockingHighlight, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHitHighlight), &arrTempHitHighlight, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedArmorHighlight), &arrTempArmorHighlight, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedThrowProtectionHighlight), &arrTempThrowProtectionHighlight, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHighlight), &bHighlight, 1, 0);

                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedOnExtendedSettings), &bOnSettingsMenu, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcBlank_64, 64, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcBlank_64, 64, 0);

                        uint8_t nZero = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwDrawFPS), &nZero, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisableHUD), &bDisableHud, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedBackgroundStyle), &nBackgroundStyle, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDrawGround), &bDrawGround, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDisableShadow), &bDisableShadow, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHideFPS), &dwDisableExtras, 3, 0);

                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameDataDisplay), &bFrameDataDisplay, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFastReversePenalty), &bFastReversePenalty, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFrameBarY), &nFrameBarY, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedShowStats), &bShowStats, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1InputDisplay), &nP1InputDisplay, 1, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2InputDisplay), &nP2InputDisplay, 1, 0);

                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1FancyInputX), &fP1FancyInputX, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1FancyInputY), &fP1FancyInputY, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2FancyInputX), &fP2FancyInputX, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2FancyInputY), &fP2FancyInputY, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1ListInputX), &fP1ListInputX, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1ListInputY), &fP1ListInputY, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2ListInputX), &fP2ListInputX, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2ListInputY), &fP2ListInputY, 4, 0);

                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1Guts), &arrP1Guts, 12, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2Guts), &arrP2Guts, 12, 0);
                    }
                }
            }

            long long start = getMicroSec();

            FrameDisplay(hMBAAHandle);
            
            long long totalTime = getMicroSec() - start;

            if (totalTime > 8000) {
                //netlog("console framedisplay took %3lld.%03lld ms", totalTime / 1000, totalTime % 1000);
            }

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
                bP3Exists = (nReadResult == 1 && nP1CharacterNumber != 4 ? true : false);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4Exists), &nReadResult, 4, 0);
                bP4Exists = (nReadResult == 1 && nP2CharacterNumber != 4 ? true : false);

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

                if (bP3Exists && nFrameCount > 1)
                {
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &nP1ControlledCharacter, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1TagFlag), &nP1TagFlag, 1, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3TagFlag), &nP3TagFlag, 1, 0);
                }

                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
                bPaused = (nReadResult == 1 ? true : false);

                uint8_t nP1Controlled;
                uint8_t nP2Controlled;
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &nP1Controlled, 1, 0);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2ControlledCharacter), &nP2Controlled, 1, 0);
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1SubBase + adDoTrainingAction), &nReadResult, 1, 0);
                uint8_t nPlayer = nReadResult == 0 ? nP1Controlled : nP2Controlled;
                uint8_t nDummy = nReadResult == 0 ? nP2Controlled : nP1Controlled;

                // This is the big if-else
                if (bPaused)
                {
                    // this is used way later to trigger unpausing events
                    bJustUnpaused = true;

                    DWORD dwTrainingMenuString = GetTrainingMenuStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwCommandListStringAddress = GetCommandListStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwTrainingDisplayStringAddress = GetTrainingDisplayStringAddress(hMBAAHandle, dwBaseAddress);
                    DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hMBAAHandle, dwBaseAddress);

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
                    }
                }
                else // not paused
                {
                    if (bJustUnpaused)
                    {
                        bJustUnpaused = false;

                        // put stuff here
                        if (bPrintColorGuide)
                        {
                            bPrintColorGuide = false;
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedColorGuide), &bPrintColorGuide, 1, 0);
                            system("cls");
                        }

                        uint8_t nIsHoveringScroll = 0;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedHoveringScroll), &nIsHoveringScroll, 1, 0);
                    }

                    long long testStartTime = getMicroSec();

                    // want to reset these for a clean setup next time the game is paused
                    bOnSettingsMenu = false;
                    bInExtendedSettings = false;
                    nOldCurrentSubMenu = -1;
                    nCurrentSubMenu = eMenu::MAIN;

                    // Enable Ex Guard.  randomly if applicable
                    nWriteBuffer = 1;
                    if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                        nWriteBuffer = 10;
                    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2ExGuard), &nWriteBuffer, 4, 0);
                    ReadCharacterMemory(hMBAAHandle, adMBAABase + adP1SubBase + adDoTrainingAction, &nReadResult, 1, nDummy);
                    if (nReadResult)
                        WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1ExGuard, &nWriteBuffer, 4, nDummy);

                    // Disable built-in health recovery
                    //nWriteBuffer = 4;
                    //WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwLifeRecover), &nWriteBuffer, 4, 0);

                    // Disable built-in reversal action
                    nWriteBuffer = 0;
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);


                    if (bDoingHeldReversal)
                    {
                        uint8_t nHoldButtons = 112;
                        WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1ButtonHeld, &nHoldButtons, 1, nP2Controlled);
                    }
                    //else
                    //{
                    //    uint8_t nHoldButtons = 0;
                    //    WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1ButtonHeld, &nHoldButtons, 1, nP2Controlled);
                    //}


                    float fTempP1FancyInputX;
                    float fTempP1FancyInputY;
                    float fTempP2FancyInputX;
                    float fTempP2FancyInputY;
                    float fTempP1ListInputX;
                    float fTempP1ListInputY;
                    float fTempP2ListInputX;
                    float fTempP2ListInputY;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1FancyInputX), &fTempP1FancyInputX, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1FancyInputY), &fTempP1FancyInputY, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2FancyInputX), &fTempP2FancyInputX, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2FancyInputY), &fTempP2FancyInputY, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1ListInputX), &fTempP1ListInputX, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1ListInputY), &fTempP1ListInputY, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2ListInputX), &fTempP2ListInputX, 4, 0);
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2ListInputY), &fTempP2ListInputY, 4, 0);
                    if (fTempP1FancyInputX != fP1FancyInputX)
                    {
                        SetRegistryValue(L"P1FancyInputX", fTempP1FancyInputX);
                        fP1FancyInputX = fTempP1FancyInputX;
                    }
                    if (fTempP1FancyInputY != fP1FancyInputY)
                    {
                        SetRegistryValue(L"P1FancyInputY", fTempP1FancyInputY);
                        fP1FancyInputY = fTempP1FancyInputY;
                    }
                    if (fTempP2FancyInputX != fP2FancyInputX)
                    {
                        SetRegistryValue(L"P2FancyInputX", fTempP2FancyInputX);
                        fP2FancyInputX = fTempP2FancyInputX;
                    }
                    if (fTempP2FancyInputY != fP2FancyInputY)
                    {
                        SetRegistryValue(L"P2FancyInputY", fTempP2FancyInputY);
                        fP2FancyInputY = fTempP2FancyInputY;
                    }
                    if (fTempP1ListInputX != fP1ListInputX)
                    {
                        SetRegistryValue(L"P1ListInputX", fTempP1ListInputX);
                        fP1ListInputX = fTempP1ListInputX;
                    }
                    if (fTempP1ListInputY != fP1ListInputY)
                    {
                        SetRegistryValue(L"P1ListInputY", fTempP1ListInputY);
                        fP1ListInputY = fTempP1ListInputY;
                    }
                    if (fTempP2ListInputX != fP2ListInputX)
                    {
                        SetRegistryValue(L"P2ListInputX", fTempP2ListInputX);
                        fP2ListInputX = fTempP2ListInputX;
                    }
                    if (fTempP2ListInputY != fP2ListInputY)
                    {
                        SetRegistryValue(L"P2ListInputY", fTempP2ListInputY);
                        fP2ListInputY = fTempP2ListInputY;
                    }


                    GetP1Guts(arrP1Guts, nP1Controlled);
                    GetP2Guts(arrP2Guts, nP2Controlled);

                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP1Guts), &arrP1Guts, 16, 0);
                    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedP2Guts), &arrP2Guts, 16, 0);


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
                    int nHitComboCount;
                    int nBlockComboCount;
                    ReadCharacterMemory(hMBAAHandle, adMBAABase + adP1SubBase + adOnHitCount, &nHitComboCount, 2, nDummy);
                    ReadCharacterMemory(hMBAAHandle, adMBAABase + adP1SubBase + adOnBlockCount, &nBlockComboCount, 2, nDummy);

                    if (nHitComboCount == 0 && nBlockComboCount == 0)
                        bAlreadyBursted = false;

                    // burst a combo
                    if (nHitsTillBurst != TOO_HIGH_TO_BURST && !bAlreadyBursted && !bBunkerInsteadOfBurst)
                    {
                        uint8_t nHitstunRemaining, nHitstopRemaining;
                        ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1HitstopRemaining, &nHitstopRemaining, 1, nDummy);
                        ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1HitstunRemaining, &nHitstunRemaining, 1, nDummy);
                        if ((nHitComboCount >= nHitsTillBurst || nBlockComboCount >= nHitsTillBurst) && nHitstopRemaining <= 2 && nHitstunRemaining != 0)
                        {
                            uint16_t nButtonInputs[3] = { 0, 1, 1 };
                            WriteCharacterMemory(hMBAAHandle, adMBAABase + adP1Base + adAInputBuffer, &nButtonInputs, 6, nDummy);
                            WriteCharacterMemory(hMBAAHandle, adMBAABase + adP1Base + adBInputBuffer, &nButtonInputs, 6, nDummy);
                            WriteCharacterMemory(hMBAAHandle, adMBAABase + adP1Base + adCInputBuffer, &nButtonInputs, 6, nDummy);
                            bAlreadyBursted = true;
                        }
                    }

                    //guard cancel bunker
                    if (nHitsTillBunker != 0 && bBunkerInsteadOfBurst)
                    {
                        uint8_t nHitstunRemaining;
                        uint8_t nStance;
                        ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1HitstunRemaining, &nHitstunRemaining, 1, nDummy);
                        ReadCharacterMemory(hMBAAHandle, adMBAABase + adP1SubBase + adStanceDelayed, &nStance, 1, nDummy);
                        if (nBlockComboCount >= nHitsTillBunker && nHitstunRemaining != 0 && nStance != 1)
                        {
                            uint16_t nDirInputs[7] = { 2, 2, 1, 1, 1, 4, 1 };
                            uint16_t nDInputs[5] = { 1, 0, 3, 1, 1 };
                            WriteCharacterMemory(hMBAAHandle, adMBAABase + adP1Base + adDirInputBuffer, &nDirInputs, 14, nDummy);
                            WriteCharacterMemory(hMBAAHandle, adMBAABase + adP1Base + adDInputBuffer, &nDInputs, 10, nDummy);
                        }
                    }

                    // reset resources to the preset when training mode is reset
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGuardSetting), &nReadResult, 4, 0);
                    nCustomGuard = nReadResult;
                    if (nFrameCounter == 1 && !CheckSave(nSaveSlot))
                    {
                        SetMeter(hMBAAHandle, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon, nP1Controlled, nP2Controlled);
                        SetGuard(hMBAAHandle, dwBaseAddress, nCustomGuard, nP1Moon, nP2Moon, nP1Controlled, nP2Controlled);
                        SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon, nP1Controlled, nP2Controlled);

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
                        SetGuard(hMBAAHandle, dwBaseAddress, 0, nP1Moon, nP2Moon, nP1Controlled, nP2Controlled);

                    // increase the counter every frame p2 is standing idle to delay regenerating health and char specifics
                    // taking an extra step to cap these at 20 to avoid any unexpected behavior if tmode is left running forever
                    //ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    /*ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 4, 0);
                    int nHitstunRemaining = nReadResult;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    int nP2Pattern = nReadResult;*/
                    BYTE nHitstunRemaining;
                    int nP2Pattern;
                    ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1HitstunRemaining, &nHitstunRemaining, 1, nP2Controlled);
                    ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1PatternRead, &nP2Pattern, 4, nP2Controlled);

                    //static int nHealthRefillTimerCount = 0;

                    if (nHitstunRemaining == 0 && nP2Pattern != 350)
                    {
                        //nHealthRefillTimerCount++;
                        //if (nHealthRefillTimerCount == 45) { // 6c into kouma AAD causes problems without this
                        //    nHealthRefillTimerCount = 0;
                        //    nHealthRefillTimer = 1;
                        //}
                        
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
                        //nHealthRefillTimer = 0;
                        nSionBulletsRefillTimer = 0;
                    }

                

                    // refill health if training mode is reset or long enough time has passed
                    //if ((nFrameCounter == 1 && !CheckSave(nSaveSlot)) || (nHealthRefillTimer == 1 && bLifeRecover))
                    //{
                    //    SetHealth(hMBAAHandle, dwBaseAddress, nCustomHealth, nP1Controlled, nP2Controlled);
                    //    nHealthRefillTimer = 0;
                    //}

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
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &nP1ControlledCharacter, 4, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1TagFlag), &nP1TagFlag, 1, 0);
                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3TagFlag), &nP3TagFlag, 1, 0);
                        }
                    }
                

                    nOldMot = nMot;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMot), &nMot, 4, 0);
                    if (!bDelayingReversal && nMot == 0 && nMot != nOldMot)
                    { 
                        bDoingHeldReversal = false;
                    }

                    bool bReversalKeyHeld;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedReversalKeyHeld), &bReversalKeyHeld, 1, 0);

                    nReadResult = 0;
                    //ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2ShieldType), &nReadResult, 1, 0);
                    ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1ShieldType, &nReadResult, 1, nP2Controlled);

                    if (nReversalType != REVERSAL_OFF && !bReversalKeyHeld && nReadResult != 0)
                    {
                        
                        if (nReadResult != 0)
                        {
                            std::vector<int> vValidReversals = (nP2Y == 0 ? vGroundReversals : vAirReversals);
                            RemoveShieldReversals(&vValidReversals, nP2CharacterID);
                            if (vValidReversals.size() != 0)
                            {
                                int nTempReversalIndex = 0;
                                while (1)
                                {
                                    nTempReversalIndex = rand() % vValidReversals.size();
                                    if (vValidReversals[nTempReversalIndex] != 0)
                                        break;
                                }
                                nWriteBuffer = vValidReversals[nTempReversalIndex] % 1000;
                                
                                if (vValidReversals[nTempReversalIndex] > 999)
                                    bDoingHeldReversal = true;
                                else
                                    bDoingHeldReversal = false;

                                WriteCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1InputEvent, &nWriteBuffer, 4, nP2Controlled);
                            }
                            
                        }
                    }
                    else if (nReversalType != REVERSAL_OFF || bReversalKeyHeld)
                    {
                        // convoluted reversal pattern logic
                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwBurstCooldown), &nReadResult, 1, 0);
                        //ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwBurstCooldown - 0xAFC, &nBurstCooldown, 1, nP2Controlled);
                        nBurstCooldown = nReadResult;

                        //nReadResult = 0;
                        //ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2HitstunRemaining), &nReadResult, 1, 0);
                        //int nHitstun = nReadResult;
                        uint8_t nHitstun;
                        ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1HitstunRemaining, &nHitstun, 1, nP2Controlled);

                        /*ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2Y), &nReadResult, 4, 0);
                        nP2Y = nReadResult;
                        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
                        nOldMot = nMot;
                        nMot = nReadResult;*/
                        ReadCharacterMemory(hMBAAHandle, dwBaseAddress + dwP1Y, &nP2Y, 4, nP2Controlled);

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
                            if (!bDelayingReversal && (nMot == 0 || nHitstun != 0) && (nMot != nOldMot || nReversalType == REVERSAL_REPEAT || bReversalKeyHeld) /* && nP2Y == 0*/)
                            {
                                bDoingHeldReversal = false;
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
                                    if (nReversalType == REVERSAL_SHIELD)
                                        RemoveNonShieldReversals(&vValidReversals, nP2CharacterID);
                                    if (vValidReversals.size() != 0)
                                    {
                                        int nTempReversalIndex = 0;
                                        while (1)
                                        {
                                            nTempReversalIndex = rand() % vValidReversals.size();
                                            if (vValidReversals[nTempReversalIndex] != 0)
                                                break;
                                        }
                                        nWriteBuffer = vValidReversals[nTempReversalIndex] % 1000;
                                        
                                        if (vValidReversals[nTempReversalIndex] > 999)
                                            bDoingHeldReversal = true;
                                        else
                                            bDoingHeldReversal = false;
                                        
                                        if (nP2Controlled == 1)
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                                        else
                                            WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP4PatternSet), &nWriteBuffer, 4, 0);
                                        nTempReversalDelayFrames = nReversalDelayFrames;
                                    }
                                }
                                else
                                    bReversaled = false;
                            }
                            else
                                nTempReversalDelayFrames--;
                        }
                    }
                    
                    short sOnHitCount = 0;
                    char cEnemyDefense = 0;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyDefense), &cEnemyDefense, 1, 0);
                    if (cEnemyDefense == 1 || cEnemyDefense == 2)
                    {
                        ReadCharacterMemory(hMBAAHandle, adMBAABase + adP1SubBase + adOnHitCount, &sOnHitCount, 2, nDummy);
                        if (sOnHitCount)
                        {
                            char c0 = 0;
                            WriteCharacterMemory(hMBAAHandle, dwBaseAddress + adP1SubBase + adWillBlock, &c0, 1, nDummy);
                        }
                    }
                  
                      
                    /*bSwitchToCrouch = false;
                    ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    if (bSwitchToCrouch && nReadResult == eEnemyStance::STANDGUARDING)
                    {
                        nWriteBuffer = 2;
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);
                        WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwEnemyStatusCopy), &nWriteBuffer, 4, 0);
                    }*/
                    //Sleep(1);

                    ///long long testSleepTime = getMicroSec();

                    //std::this_thread::sleep_for(std::chrono::milliseconds(1));

                    //long long endTestTime = getMicroSec();

                    //long long testTime = endTestTime - testStartTime;
                    //long long testSleepTimeEnd = endTestTime - testSleepTime;

                    //netlog("test time took %3lld.%03lld ms sleep: %3lld.%03lld ms", testTime / 1000, testTime % 1000, testSleepTimeEnd / 1000, testSleepTimeEnd % 1000);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    catch (const std::exception& ex)
    {
        DeleteRegistry();
        LogError("Exception: " + std::string(ex.what()));
    }

    CloseLogger();
    CloseHandle(hMBAAHandle);
    timeEndPeriod(1);
    return 0;
}
