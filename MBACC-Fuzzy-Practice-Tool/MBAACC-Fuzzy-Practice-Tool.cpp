#include "MBAACC-Fuzzy-Practice-Tool.h"
#include "CharacterData.h"

int main(int argc, char* argv[])
{
    HANDLE hProcess = 0x0;
    std::cout << "Looking for MBAA.exe...";
    while (hProcess == 0x0)
        hProcess = GetProcessByName(L"MBAA.exe");
    DWORD dwBaseAddress = GetBaseAddressByName(hProcess, L"MBAA.exe");

    InitializeCharacterMaps();

    const DWORD dwP2Offset = 0xAFC;
    const DWORD dwRoundTime = 0x162A40; //0-inf
    const DWORD dwMot = 0x1581CC;   // this one is mysterious.  I think it's an animation counter
    const DWORD dwPlayerState = 0x155140;  //0:STAND 13:CROUCH 17:STANDGUARDING
    const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
    const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE
    const DWORD dwGuardLevel = 0x1551F4; //0-1174011904 aka 0.0f-8000.0f
    const DWORD dwGuardSetting = 0x37C200; //0:100 1:75 2:50 3:25 4:0
    const DWORD dwExGuard = 0x1551E0; //10:ExGuard
    const DWORD dwY = 0x155248;
    const DWORD dwP2PatternSet = 0x155F38;
    const DWORD dwP2PatternRead = 0x155C3C;
    const DWORD dwCSSFlag = 0x155130;
    const DWORD dwP2CharNumber = 0x34D91C;
    const DWORD dwP2CharMoon = 0x34D924;

    const int nCArc22B = 323682455;

    int nReadResult;
    int nWriteBuffer;
    
    bool bUpPressed = false;
    bool bDownPressed = false;
    bool bRightPressed = false;
    bool bLeftPressed = false;
    bool bSpacePressed = false;

    bool bSwitchToCrouch = false;
    bool bRandomBlock = false;
    int nSwitchBlockDelayFrames = 0;
    int nPresetSetting = ePresetSettings::DEFAULT;
    int nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
    int nEnemyDefenseBiasSetting = eEnemyDefenseBiasSetting::OFF;
    int nEnemyStatusSetting = eEnemyStatus::STAND;
    bool bExGuard = false;
    int nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::ONEHUNDRED;
    int nReversalPattern = 0;
    int nReversalDelayFrames = 0;
    int nTempReversalDelayFrames = 0;
    bool bDelayingReversal = false;
    bool bOnCSS = false;
    int nReversalIndex = 0;
    int nCharacterID = 0;
    std::vector<std::string> vPresetSettings = { "Default", "Fuzzy Overhead", "Blockstring", "Heat OS", "Defensive Fuzzy Mash", "Defensive Fuzzy Jump", "Custom" };
    std::vector<std::string> vEnemyDefenseSettings = { "No Guard", "All Guard", "Status Guard", "All Shield", "Status Shield"};
    std::vector<std::string> vEnemyDefenseBiasSettings = { "Unlikely", "Even", "Likely", "Off" };
    std::vector<std::string> vEnemyStatusSettings = { "Stand", "Jump", "Crouch" };
    std::vector<std::string> vEnemyGuardLevelSettings = { "Infinite", "100%", "75%", "50%", "25%", "0%" };
    std::vector<std::string> vPatternNames = GetEmptyPatternList();
    std::vector<int> vGuardLevelLookupTable = { 1174011904, 1174011904, 1169915904, 1165623296, 1157234688, 0 };

    bool bReversaled = false;
    int nTimer = 0;
    int nOldTimer = 0;
    bool bFramePassed = false;
    int nMot = 0;
    int nOldMot = 0;
    int nP2Y = 0;

    int nCursorIndex = 1;

    int nDebugBias = 0;
    int nDebugFrameCount = 0;

    std::srand(std::time(nullptr));

    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    system("cls");
    std::cout << "Fang's Extended Training Mode v0.1" << std::endl;
    std::cout << "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases";

    while (1)
    {
        // Loop till the dummy is blocking
        while (1)
        {
            std::cout.flush();

            SetConsoleCursorPosition(hOut, { 0, 4 });
            std::cout << (nCursorIndex == 0 ? "=>  " : "    ") << "Preset:\t\t\t\t<- " << vPresetSettings[nPresetSetting] << " ->                                        ";

            SetConsoleCursorPosition(hOut, { 0, 5 });
            std::cout << (nCursorIndex == 1 ? "=>  " : "    ") << "Switch to crouching after blocking:\t<- " << (bSwitchToCrouch ? "On " : "Off") << " ->      ";

            SetConsoleCursorPosition(hOut, { 0, 6 });
            std::cout << (nCursorIndex == 2 ? "=>  " : "    ") << "Delay:\t\t\t\t<- " << nSwitchBlockDelayFrames << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 8 });
            std::cout << (nCursorIndex == 3 ? "=>  " : "    ") << "Enemy Status:\t\t\t<- " << vEnemyStatusSettings[nEnemyStatusSetting] << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 9 });
            std::cout << (nCursorIndex == 4 ? "=>  " : "    ") << "Enemy Defense:\t\t\t<- " << vEnemyDefenseSettings[nEnemyDefenseSetting] << " ->              ";

            SetConsoleCursorPosition(hOut, { 0, 11 });
            //std::cout << (nCursorIndex == 5 ? "=>  " : "    ") << "Random Bias:\t\t\t<- " << vEnemyDefenseBiasSettings[nEnemyDefenseBiasSetting] << " ->              ";
            std::cout << (nCursorIndex == 5 ? "=>  " : "    ") << "howdy pardner";

            SetConsoleCursorPosition(hOut, { 0, 13 });
            std::cout << (nCursorIndex == 6 ? "=>  " : "    ") << "Guard Bar:\t\t\t\t<- " << vEnemyGuardLevelSettings[nEnemyGuardLevelSetting] << " ->      ";

            SetConsoleCursorPosition(hOut, { 0, 14 });
            std::cout << (nCursorIndex == 7 ? "=>  " : "    ") << "Ex Guard:\t\t\t\t<- " << (bExGuard ? "On " : "Off") << " ->      ";

            SetConsoleCursorPosition(hOut, { 0, 16 });
            std::cout << (nCursorIndex == 8 ? "=>  " : "    ") << "Reversal:\t\t\t\t" << vPatternNames[nReversalIndex] << "                                    ";
            
            SetConsoleCursorPosition(hOut, { 0, 17 });
            std::cout << (nCursorIndex == 9 ? "=>  " : "    ") << "Delay:\t\t\t\t<- " << nReversalDelayFrames << " ->              ";

#define RELEASE
#ifdef DEBUG
            SetConsoleCursorPosition(hOut, { 60, 0 });
            std::cout << "Bias:" << nDebugBias << " FrameCount:" << nTimer << " Reversaled:" << (bReversaled ? "true" : "false") << " nMot:" << nMot << "                ";
#endif
#ifdef RELEASE
            SetConsoleCursorPosition(hOut, { 0, 20 });
            std::cout << "This is an early release, so please let me know if you find any problems or have any suggestions '-'b";
#endif // RELEASE


            if (GetAsyncKeyState(VK_ESCAPE))
                goto CLEANUP;

            if (GetAsyncKeyState(VK_UP))
            {
                if (!bUpPressed)
                {
                    nCursorIndex = max(0, nCursorIndex - 1);
                    bUpPressed = true;
                }
            }
            else
                bUpPressed = false;

            if (GetAsyncKeyState(VK_DOWN))
            {
                if (!bDownPressed)
                {
                    nCursorIndex++;
                    bDownPressed = true;
                }
            }
            else
                bDownPressed = false;

            

            
            if (GetAsyncKeyState(VK_RIGHT))
            {
                if (!bRightPressed)
                {
                    bRightPressed = true;
                    if (nCursorIndex != 0)
                        nPresetSetting = ePresetSettings::CUSTOM;

                    switch (nCursorIndex)
                    {
                    case 0:
                        nPresetSetting = min(nPresetSetting + 1, vPresetSettings.size() - 2);
                        ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nSwitchBlockDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyDefenseBiasSetting, &nEnemyGuardLevelSetting, &bExGuard, &nReversalPattern, &nReversalDelayFrames);
                        break;
                    case 1:
                        bSwitchToCrouch = !bSwitchToCrouch;
                        break;
                    case 2:
                        nSwitchBlockDelayFrames++;
                        break;
                    case 3:
                        nEnemyStatusSetting = min(nEnemyStatusSetting + 1, vEnemyStatusSettings.size() - 1);
                        break;
                    case 4:
                        nEnemyDefenseSetting = min(nEnemyDefenseSetting + 1, vEnemyDefenseSettings.size() - 1);
                        break;
                    case 5:
                        nEnemyDefenseBiasSetting = max(nEnemyDefenseBiasSetting - 1, 0);
                        break;
                    case 6:
                        nEnemyGuardLevelSetting = min(nEnemyGuardLevelSetting + 1, vEnemyGuardLevelSettings.size() - 1);
                        nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardLevel + dwP2Offset), &nWriteBuffer, 4, 0);
                        break;
                    case 7:
                        bExGuard = !bExGuard;
                        break;
                    case 8:
                        nReversalIndex = min(nReversalIndex + 1, vPatternNames.size() - 1);
                        break;
                    case 9:
                        nReversalDelayFrames++;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
                bRightPressed = false;

            if (GetAsyncKeyState(VK_LEFT))
            {
                if (!bLeftPressed)
                {
                    bLeftPressed = true;
                    if (nCursorIndex != 0)
                        nPresetSetting = ePresetSettings::CUSTOM;

                    switch (nCursorIndex)
                    {
                    case 0:
                        nPresetSetting = max(nPresetSetting - 1, 0);
                        ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nSwitchBlockDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyDefenseBiasSetting, &nEnemyGuardLevelSetting, &bExGuard, &nReversalPattern, &nReversalDelayFrames);
                        break;
                    case 1:
                        bSwitchToCrouch = !bSwitchToCrouch;
                        break;
                    case 2:
                        nSwitchBlockDelayFrames = max(0, nSwitchBlockDelayFrames - 1);
                        break;
                    case 3:
                        nEnemyStatusSetting = max(nEnemyStatusSetting - 1, 0);
                        break;
                    case 4:
                        nEnemyDefenseSetting = max(nEnemyDefenseSetting - 1, 0);
                        break;
                    case 5:
                        nEnemyDefenseBiasSetting = min(nEnemyDefenseBiasSetting + 1, vEnemyDefenseBiasSettings.size() - 1);
                        break;
                    case 6:
                        nEnemyGuardLevelSetting = max(nEnemyGuardLevelSetting - 1, 0);
                        nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardLevel + dwP2Offset), &nWriteBuffer, 4, 0);
                        break;
                    case 7:
                        bExGuard = !bExGuard;
                        break;
                    case 8:
                        nReversalIndex = max(0, nReversalIndex - 1);
                        break;
                    case 9:
                        nReversalDelayFrames = max(0, nReversalDelayFrames - 1);
                        break;
                    default:
                        break;
                    }
                }
            }
            else
                bLeftPressed = false;

            if (GetAsyncKeyState(VK_SPACE))
            {
                if (!bSpacePressed)
                {
                    switch (nCursorIndex)
                    {
                    case 8:
                        nPresetSetting = ePresetSettings::CUSTOM;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                        nReversalPattern = nReadResult;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
                bSpacePressed = false;

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
            if (nReadResult == 0)
            {
                nReversalIndex = 0;
                vPatternNames = GetEmptyPatternList();
                continue;
            }

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
            nTimer = nReadResult;
            if (nTimer == nOldTimer)
                continue;
            nOldTimer = nTimer;

            if (nTimer == 0 || vPatternNames.size() == 1)
            {
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharNumber), &nReadResult, 4, 0);
                int nCharacterNumber = nReadResult;

                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharMoon), &nReadResult, 4, 0);
                int nCharacterMoon = nReadResult;

                nCharacterID = 10 * nCharacterNumber + nCharacterMoon;

                vPatternNames = GetPatternList(nCharacterID);
            }

            nWriteBuffer = nEnemyStatusSetting;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

            nWriteBuffer = nEnemyDefenseSetting;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

            /*int nRandVal = std::rand() % 4;
            if (nTimer % 5 == 0)
            {
                if (nRandVal > nEnemyDefenseBiasSetting)
                {
                    nWriteBuffer = eEnemyDefense::NOGUARD;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
                }
            }*/

            if (nEnemyGuardLevelSetting == eEnemyGuardLevelSettings::INF || nTimer == 1)
            {
                nWriteBuffer = 1174011904; //8000.0f
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardLevel + dwP2Offset), &nWriteBuffer, 4, 0);
            }
            else
            {
                nWriteBuffer = nEnemyGuardLevelSetting - 1;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardSetting), &nWriteBuffer, 4, 0);
            }

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
            if (nReadResult == 0)
            {
                nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting];
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardLevel + dwP2Offset), &nWriteBuffer, 4, 0);
            }

            if (bExGuard)
            {
                nWriteBuffer = 10;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwExGuard + dwP2Offset), &nWriteBuffer, 4, 0);
            }

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwY + dwP2Offset), &nReadResult, 4, 0);
            nP2Y = nReadResult;
            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
            nMot = nReadResult;
            if (nTimer == 0)
                bReversaled = true;
            if (nTimer == 2)
                bReversaled = false;
            if (nTimer != 0 && GetPattern(nCharacterID, vPatternNames[nReversalIndex]) != 0)
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
                    bReversaled = true;
                    nWriteBuffer = GetPattern(nCharacterID, vPatternNames[nReversalIndex]);
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                }
                else
                {
                    nTempReversalDelayFrames--;
                }
            }

            ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPlayerState + dwP2Offset), &nReadResult, 4, 0);
            if (nReadResult == eEnemyStance::STANDGUARDING)
                break;
        }

        if (bSwitchToCrouch)
        {
            Sleep(16 * nSwitchBlockDelayFrames);

            nWriteBuffer = eEnemyStatus::CROUCH;
            //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

            nWriteBuffer = eEnemyDefense::STATUSGUARD;
            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

            

            // Loop until the dummy returns to neutral
            while (1)
            {
                //nWriteBuffer = 33685504;
                nWriteBuffer = 50528256;
                nWriteBuffer = 50397184;
                int dwP1MagicAddress = 0x155f47;
                int dwP2MagicAddress = 0x155F14;
                int dwP2Control = 0x37144C;
                nWriteBuffer = 1;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);
                dwP2Control = 0x3714FC;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);
                dwP2Control = 0x154638;
                nWriteBuffer = 126;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);

                int dwController = 0x371398;
                int nOld1 = 0;
                int nOld2 = 0;
                int nOld3 = 0;
                nWriteBuffer = 2;
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nReadResult, 4, 0);
                nOld1 = nReadResult;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nWriteBuffer, 4, 0);
                dwController = 0x371448;
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nReadResult, 4, 0);
                nOld2 = nReadResult;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nWriteBuffer, 4, 0);
                dwController = 0x3714F8;
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nReadResult, 4, 0);
                nOld3 = nReadResult;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nWriteBuffer, 4, 0);

                dwP2Control = 0x37144C;
                nWriteBuffer = 0;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);
                dwP2Control = 0x3714FC;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);
                dwP2Control = 0x154638;
                nWriteBuffer = 0;
                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Control), &nWriteBuffer, 4, 0);

                dwController = 0x371398;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nOld1, 4, 0);
                dwController = 0x371448;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nOld2, 4, 0);
                dwController = 0x3714F8;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nOld3, 4, 0);
                
                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                if (nReadResult == 13)
                {
                    

                    //nWriteBuffer = 13;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                    //break;
                }
            }
        }
    }

CLEANUP:
    CloseHandle(hProcess);
    return 0;
}
