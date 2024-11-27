#include "MBAACC-Extended-Training-Mode.h"
#include "Logger.h"
#include "Injector.h"

int main(int argc, char* argv[])
{
    HANDLE hMBAAHandle = 0x0;
    DWORD dwExitCode = 0;
    DWORD dwBaseAddress = 0;

    int nReadResult = 0;
    int nWriteBuffer = 0;

    bool bPaused = false;
    bool bAPressed = false;
    bool bOldAPressed = false;
    bool bF1Pressed = false;
    bool bOldF1Pressed = false;
    bool bF2Pressed = false;
    bool bOldF2Pressed = false;

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

    int nExtendedSettingsPage = 1;

    int nGameMode = 0;

    bool bPositionsLocked = false;
    int nP1X = P1_DEFAULT_X;
    int nP2X = P2_DEFAULT_X;
    int nP3X = P3_DEFAULT_X;
    int nP4X = P4_DEFAULT_X;
    bool bP3Exists = false;
    bool bP4Exists = false;

    int nFrameData = FRAMEDISPLAY_NORMAL;
    //bool bSaveStates = false;
    bool bDisplayInputs = false;

    std::unordered_set<DWORD> setBlockingAnimationPointers;
    std::unordered_set<DWORD> setIdleAnimationPointers;
    int nBlockingHighlightSetting = NO_HIGHLIGHT;
    int nIdleHighlightSetting = NO_HIGHLIGHT;

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

    while (1)
    {
        nCurrentTime = std::time(nullptr);
        
        
        SetConsoleCursorPosition(hConsoleHandle, { 0, 0 });
        std::cout << "===========================================================================\x1b[K" << std::endl;
        std::cout << "| meepster99(Inana)'s,,,, thing?                                          |\x1b[K" << std::endl;
        std::cout << "| I'm pretty much only using the training mode for the injector           |\x1b[K" << std::endl;
        //std::cout << "|   " << GITHUB_RELEASE << "   |\x1b[K" << std::endl;
        std::cout << "| Put a video named \"EffectsVideo.mp4\" in the same directory as MBAA.exe  |\x1b[K" << std::endl;
        /*if (bNeedToAnnounceNewVersion && nCurrentTime % 3 != 0)
            std::cout << "|   NEW VERSION " << sOnlineVersion << " AVAILABLE ON GITHUB                                 |\x1b[K" << std::endl;
        else
            std::cout << "|                                                                         |\x1b[K" << std::endl;*/
        std::cout << "| for the love of all, make sure this exe and dll are both with MBAA.exe  |\x1b[K" << std::endl;
        std::cout << "===========================================================================\x1b[K" << std::endl;

        GetExitCodeProcess(hMBAAHandle, &dwExitCode);

        SetConsoleCursorPosition(hConsoleHandle, { 0, 6 });
        std::cout << "\x1b[K";

        if (hMBAAHandle == 0x0 || dwExitCode != 259)
        {
            // Print the variable values when MBAA closes, possibly from a crash
            //if (hMBAAHandle != 0x0)
                //CloseLog(bPaused, bAPressed, bOnCSS, bOnExtendedSettingsMenu, nP1CharacterID, nP2CharacterID, nP1Moon, nP2Moon, nP1CharacterNumber, nP2CharacterNumber, bSwitchToCrouch, nExGuardSetting, nCustomGuard, nReversalPattern, nReversalDelayFrames, nTempReversalDelayFrames, bDelayingReversal, bReversaled, nReversalType, nReversalIndex1, nReversalIndex2, nReversalIndex3, nReversalIndex4, vPatternNames, nMot, nOldMot, nP2Y, nBurstCooldown ,nOldEnemyActionIndex, nOldPresetIndex, nOldEnemyDefenseIndex, nOldEnemyDefenseTypeIndex, nOldAirRecoveryIndex, nOldDownRecoveryIndex, nOldThrowRecoveryIndex, nOldReduceDamageIndex, nOldLifeIndex, nCurrentSubMenu, nOldCurrentSubMenu, nFrameCounter, nOldFrameCounter, nStoredEnemyAction, nStoredEnemyDefense, nStoredEnemyDefenseType, nStoredAirRecovery, nStoredDownRecovery, nStoredThrowRecovery, nStoredReduceDamage, nGameCursorIndex, nOldGameCursorIndex, nEnemySettingsCursor, nOldEnemySettingsCursor, nCustomMeter, nCustomHealth, nHealthRefillTimer, nCharSpecificsRefillTimer, bLifeRecover, nSionBullets, nRoaVisibleCharge, nRoaHiddenCharge, nSionBulletsRefillTimer, nRoaHiddenChargeRefillTimer, nRoaVisibleChargeRefillTimer, nExtendedSettingsPage, bPositionsLocked, nP1X, nP2X, nP3X, nP4X, bP3Exists, bP4Exists, nHitsTillBurst, bInfGuard, nGameMode);

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

            auto PID = GetProcessPID(L"MBAA.exe");
            std::string sProcessPath = GetProcessPath(hMBAAHandle);
            sProcessPath += "Extended-Training-Mode-DLL.dll";

            // this is just for my convenience
            if (std::filesystem::exists("C:\\Users\\willf\\WH\\Repos\\MBAACC-Extended-Training-Mode\\MBAACC-Extended-Training-Mode\\Debug\\Extended-Training-Mode-DLL.dll"))
                sProcessPath = "C:\\Users\\willf\\WH\\Repos\\MBAACC-Extended-Training-Mode\\MBAACC-Extended-Training-Mode\\Debug\\Extended-Training-Mode-DLL.dll";


            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwGameMode), &nReadResult, 4, 0);
            nGameMode = nReadResult;
            if (nGameMode != 4112)
            {
                hMBAAHandle = 0;
                //continue;
            }


            bool bInjectStatus = WH_Inject(PID, sProcessPath);
            if (bInjectStatus)
            {
                getchar();
                break;
            }
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

        Sleep(16);

    }

    CloseLogger();
    CloseHandle(hMBAAHandle);
    return 0;
}
