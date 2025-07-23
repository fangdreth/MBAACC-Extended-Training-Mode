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

        int nGameMode = 0;
        int nVersusCheck = 0;
        int nTrainingMenuPtr = 0;

        bool bJustUnpaused = false;
        bool bInjected = false;
    
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
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwTrainingMenu), &nTrainingMenuPtr, 4, 0);
            if(nGameMode == 1 && nVersusCheck == 1) //Versus
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "Cannot attach to versus mode....\x1b[K";
                //LogInfo("MBAA is in versus mode");
                continue;
            }
            else if (nTrainingMenuPtr != 0) //Training menu is open
            {
                SetConsoleCursorPosition(hConsoleHandle, { 0, 7 });
                std::cout << "Cannot attach while training menu is open....\x1b[K";
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
                // TODO: add fun CSS features here
            }
            else
            {
                ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
                bPaused = (nReadResult == 1 ? true : false);

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
                    }

                    long long testStartTime = getMicroSec();
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
