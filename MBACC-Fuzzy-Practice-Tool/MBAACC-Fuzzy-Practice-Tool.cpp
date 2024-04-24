#pragma warning(suppress : 6387)

#include "MBAACC-Fuzzy-Practice-Tool.h"


/*
TODO
manual meter levels for both players
permanent heat
permanent blood heat
fix up fuzzy overhead
random ex guard
random block
random shield
random dodge
hold to scroll
cap bottom of pause menu
add dummy status
see what's up with manual AD and AADs
proof all the moves recorded x_x
write readme
new trailer
lock or disable training mode pause
change window size and fullscreen
draw on game itself
create controller class
*/



int main(int argc, char* argv[])
{
    

    HANDLE hProcess = 0x0;
    DWORD dwExitCode = 0;
    DWORD dwBaseAddress = 0;

    int nReadResult = 0;
    int nWriteBuffer;
    
    bool bUpPressed = false;
    bool bDownPressed = false;
    bool bRightPressed = false;
    bool bLeftPressed = false;
    bool bSpacePressed = false;

    bool bPaused = false;
    int nButtonsPressed = 0;
    int nOldButtonsPressed = 0;
    int nDirectionPressed = 0;
    int nOldDirectionPressed = 0;

    int nCharacterMoon = 0;
    int nCharacterNumber = 0;
    bool bSwitchToCrouch = false;
    bool bRandomBlock = false;
    int nSwitchBlockDelayFrames = 0;
    int nPresetSetting = ePresetSettings::DEFAULT;
    int nEnemyDefenseSetting = eEnemyDefense::NOGUARD;
    int nEnemyStatusSetting = eEnemyStatus::STAND;
    bool bExGuard = false;
    int nExGuardSetting = eEnemyOffOnRandom::OFF;
    int nEnemyGuardLevelSetting = eEnemyGuardLevelSettings::ONEHUNDRED;
    int nReversalPattern = 0;
    int nReversalDelayFrames = 0;
    int nTempReversalDelayFrames = 0;
    bool bDelayingReversal = false;
    bool bOnCSS = false;
    int nReversalIndex = 0;
    int nCharacterID = 0;
    bool bOnExtraMenu = false;
    int nOldEnemyActionIndex = -1;
    int nOldPresetIndex = -1;
    int nOldEnemyDefenseIndex = -1;
    const std::vector<std::string> vPresetSettings = { "Default", "Fuzzy Overhead", "Blockstring", "Heat OS", "Defensive Fuzzy Mash", "Defensive Fuzzy Jump", "Custom" };
    const std::vector<std::string> vEnemyDefenseSettings = { "No Guard", "All Guard", "Status Guard", "All Shield", "Status Shield"};
    const std::vector<std::string> vEnemyDefenseBiasSettings = { "Unlikely", "Even", "Likely", "Off" };
    const std::vector<std::string> vEnemyStatusSettings = { "Stand", "Jump", "Crouch" };
    const std::vector<std::string> vEnemyGuardLevelSettings = { "Infinite", "100%", "75%", "50%", "25%", "0%" };
    std::vector<std::string> vPatternNames = GetEmptyPatternList();
    const std::vector<int> vGuardLevelLookupTable =
    {
        1174011904/*C Infinite*/, 1174011904/*C 100*/, 1169915904/*C 75*/, 1165623296/*C 50*/, 1157234688/*C 25*/, 0/*C 0*/,
        1171963904/*F Infinite*/, 1171963904/*F 100*/, 1168379904/*F 75*/, 1163575296/*F 50*/, 1155186688/*F 25*/, 0/*F 0*/,
        1176768512/*H Infinite*/, 1176768512/*H 100*/, 1173755904/*H 75*/, 1168379904/*H 50*/, 1159991296/*H 25*/, 0/*H 0*/
    };

    bool bReversaled = false;
    int nTimer = 0;
    int nOldTimer = 0;
    bool bFramePassed = false;
    int nMot = 0;
    int nOldMot = 0;
    int nP2Y = 0;

    int nGameCursorIndex = 0;
    int nOldGameCursorIndex = 0;

    int nDebugBias = 0;
    int nDebugFrameCount = 0;

    Menu oMenu = Menu();
    std::srand(std::time(nullptr));

    try
    {
        while (1)
        {
            while (1)
            {
                std::cout.flush();

                GetExitCodeProcess(hProcess, &dwExitCode);
                if (hProcess == 0x0 || dwExitCode != 259)
                {
                    system("cls");

                    hProcess = 0x0;
                    std::cout << "Looking for MBAA.exe...";
                    while (hProcess == 0x0)
                        hProcess = GetProcessByName(L"MBAA.exe");
                    Sleep(100);
                    dwBaseAddress = GetBaseAddressByName(hProcess, L"MBAA.exe");

                    oMenu.UpdateProcessInfo(hProcess, dwBaseAddress);
                    InitializeCharacterMaps();

                    system("cls");
                }

                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
                if (nReadResult == 0)
                {
                    system("cls");
                    std::cout << "On CSS";
                    nReversalIndex = 0;
                    vPatternNames = GetEmptyPatternList();
                }
                else
                {
                    nOldButtonsPressed = nButtonsPressed;
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1ButtonPress), &nReadResult, 4, 0);
                    nButtonsPressed = nReadResult;

                    nOldDirectionPressed = nDirectionPressed;
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1DirectionPress), &nReadResult, 4, 0);
                    nDirectionPressed = nReadResult;


                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
                    bPaused = (nReadResult == 1 ? true : false);
                    if (bPaused)
                    {
                        DWORD dwMainViewScreenStringAddress = GetMainViewScreenStringAddress(hProcess, dwBaseAddress);
                        char pcTemp[18] = "EXTENDED SETTINGS";
                        WriteProcessMemory(hProcess, (LPVOID)(dwMainViewScreenStringAddress), &pcTemp, 18, 0);

                        DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hProcess, dwBaseAddress);
                        ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                        int nCurrentSubMenu = nReadResult;

                        // Turn "View Screen" into "Extended Settings" menu
                        if (nCurrentSubMenu == 12)
                        {
                            nWriteBuffer = 7;
                            WriteProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nWriteBuffer, 4, 0);

                            bOnExtraMenu = true;
                        }
                        else if (nCurrentSubMenu != 7)
                            bOnExtraMenu = false;

                        if (bOnExtraMenu)
                        {
                            DWORD dwEnemySettingsMenuTitle = GetEnemySettingsMenuTitleStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionAString = GetEnemyActionActionAStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyActionBString = GetEnemyActionActionBStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyActionCString = GetEnemyActionActionCStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyRecoverCString = GetEnemyActionRecoverCStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyRecoverDString = GetEnemyActionRecoverDStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyDefenseOffString = GetEnemyDefenseOffStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseAllGuardString = GetEnemyDefenseAllGuardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseStatusGuardString = GetEnemyDefenseStatusGuardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseAllShieldString = GetEnemyDefenseAllShieldStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseStatusShieldString = GetEnemyDefenseStatusShieldStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseDodgeString = GetEnemyDefenseDodgeStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionString = GetEnemyActionStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseString = GetEnemyDefenseStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionIndex;
                            DWORD dwEnemyDefenseIndex;
                            do
                            {
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58) && nCurrentSubMenu == 7);

                            if (nCurrentSubMenu != 7)
                                continue;

                            // enemy action.
                            char pcTrainingPreset[17] = "training preset.";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset, 18, 0);

                            // ENEMY SETTINGS
                            char pcExtendedSettings[18] = "EXTENDED SETTINGS";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings, 18, 0);

                            // ENEMY ACTION
                            char pcPreset[7] = "PRESET";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcPreset, 7, 0);

                            // ENEMY DEFENSE
                            char pcExGuard[9] = "EX GUARD";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseString), &pcExGuard, 9, 0);

                            // Index values for menu items
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nReadResult, 4, 0);
                            int nEnemyActionIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nReadResult, 4, 0);
                            int nEnemyDefenseIndex = nReadResult;


                            if (nOldEnemyActionIndex == -1)
                                nOldEnemyActionIndex = nEnemyActionIndex;
                            else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                nReversalIndex = max(0, nReversalIndex - 1);
                            else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                nReversalIndex = min(nReversalIndex + 1, vPatternNames.size() - 1);

                            if (nOldEnemyDefenseIndex == -1)
                                nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                            else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                nExGuardSetting = max(0, nExGuardSetting - 1);
                            else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                nExGuardSetting = min(nExGuardSetting + 1, eEnemyOffOnRandom::RANDOM);

                            // TODO Enemy Defense Index
                            if (nExGuardSetting == eEnemyOffOnRandom::OFF)
                            {
                                char pcTemp[4] = "OFF";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 4, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 0;
                            }
                            else if (nExGuardSetting == eEnemyOffOnRandom::RANDOM)
                            {
                                char pcTemp[7] = "RANDOM";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                                nWriteBuffer = 5;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 5;
                            }
                            else
                            {
                                char pcTemp[3] = "ON";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseIndex = 2;
                            }

                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                            nOldEnemyActionIndex = nEnemyActionIndex;;
                        }

                        // ENEMEY SETTINGS
                        if (!bOnExtraMenu && nCurrentSubMenu == 7)
                        {
                            DWORD dwEnemyActionAString = GetEnemyActionActionAStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyActionBString = GetEnemyActionActionBStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyActionCString = GetEnemyActionActionCStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyRecoverCString = GetEnemyActionRecoverCStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyRecoverDString = GetEnemyActionRecoverDStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyActionString = GetEnemyActionStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionIndex;
                            do
                            {
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while (dwEnemyActionIndex == 0x58 && nCurrentSubMenu == 7);

                            if (nCurrentSubMenu != 7)
                                continue;

                            // enemy action.
                            char pcEnemyActionInfo[16] = "enemy reversal.";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionInfoStringAddress), &pcEnemyActionInfo, 16, 0);

                            // ENEMY ACTION
                            char pcEnemyReversal[15] = "ENEMY REVERSAL";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcEnemyReversal, 15, 0);

                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nReadResult, 4, 0);
                            int nEnemyActionIndex = nReadResult;

                            if (nOldEnemyActionIndex == -1)
                                nOldEnemyActionIndex = nEnemyActionIndex;

                            // left
                            else if (nOldEnemyActionIndex > nEnemyActionIndex)
                                nReversalIndex = max(0, nReversalIndex - 1);

                            // right
                            else if (nOldEnemyActionIndex < nEnemyActionIndex)
                            {
                                nReversalIndex = min(nReversalIndex + 1, vPatternNames.size() - 1);
                            }

                            if (nReversalIndex == 0)
                            {
                                char pcTemp[4] = "OFF";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 4, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                nEnemyActionIndex = 0;
                            }
                            else if (nReversalIndex == vPatternNames.size() - 1)
                            {
                                char pcTemp[19];
                                strcpy_s(pcTemp, vPatternNames[nReversalIndex].c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 19, 0);

                                nWriteBuffer = 8;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                nEnemyActionIndex = 8;
                            }
                            else
                            {
                                char pcTemp[19];
                                strcpy_s(pcTemp, vPatternNames[nReversalIndex].c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionBString), &pcTemp, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionCString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                nEnemyActionIndex = 2;
                            }

                            nOldEnemyActionIndex = nEnemyActionIndex;
                        }

                        oMenu.UpdateProcessInfo(hProcess, dwBaseAddress);
                        //oMenu.DrawMenu();
                    }

                    else // not paused
                        bOnExtraMenu = false;

                    if (GetAsyncKeyState(VK_ESCAPE))
                        goto CLEANUP;

                    if (nDirectionPressed == 6 && nOldDirectionPressed != 6 && false)
                    {
                        if (nGameCursorIndex != 0)
                            nPresetSetting = ePresetSettings::CUSTOM;

                        switch (nGameCursorIndex)
                        {
                        case 0:
                            nPresetSetting = min(nPresetSetting + 1, vPresetSettings.size() - 2);
                            ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nSwitchBlockDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyGuardLevelSetting, &bExGuard, &nReversalPattern, &nReversalDelayFrames);
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
                            
                            break;
                        case 6:
                            nEnemyGuardLevelSetting = min(nEnemyGuardLevelSetting + 1, vEnemyGuardLevelSettings.size() - 1);
                            nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting + nCharacterMoon * 6];
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2 + dwP2Offset), &nWriteBuffer, 4, 0);
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

                    if (nDirectionPressed == 4 && nOldDirectionPressed != 4 && false)
                    {
                        if (nGameCursorIndex != 0)
                            nPresetSetting = ePresetSettings::CUSTOM;

                        switch (nGameCursorIndex)
                        {
                        case 0:
                            nPresetSetting = max(nPresetSetting - 1, 0);
                            ApplyPreset(nPresetSetting, &bSwitchToCrouch, &nSwitchBlockDelayFrames, &nEnemyStatusSetting, &nEnemyDefenseSetting, &nEnemyGuardLevelSetting, &bExGuard, &nReversalPattern, &nReversalDelayFrames);
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
                            
                            break;
                        case 6:
                            nEnemyGuardLevelSetting = max(nEnemyGuardLevelSetting - 1, 0);
                            nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting + nCharacterMoon * 6];
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2 + dwP2Offset), &nWriteBuffer, 4, 0);
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


                    nWriteBuffer = 1;
                    if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                        nWriteBuffer = 10;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwExGuard + dwP2Offset), &nWriteBuffer, 4, 0);


                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
                    nTimer = nReadResult;
                    if (nTimer == nOldTimer)
                        continue;
                    nOldTimer = nTimer;

                    if (nTimer == 0 || vPatternNames.size() == 1)
                    {
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharNumber), &nReadResult, 4, 0);
                        nCharacterNumber = nReadResult;

                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharMoon), &nReadResult, 4, 0);
                        nCharacterMoon = nReadResult;

                        nCharacterID = 10 * nCharacterNumber + nCharacterMoon;

                        vPatternNames = GetPatternList(nCharacterID);
                    }

                    nOldEnemyActionIndex = -1;
                    nWriteBuffer = 0;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);

                    nWriteBuffer = nEnemyStatusSetting;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

                    nWriteBuffer = eEnemyDefense::ALLGUARD;
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
                        nWriteBuffer = vGuardLevelLookupTable[nCharacterMoon * 6];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2), &nWriteBuffer, 4, 0);
                    }
                    else
                    {
                        nWriteBuffer = nEnemyGuardLevelSetting - 1;
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardSetting), &nWriteBuffer, 4, 0);
                    }

                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
                    if (nReadResult == 0)
                    {
                        nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting + nCharacterMoon * 6];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2), &nWriteBuffer, 4, 0);
                    }

                    if (nExGuardSetting == eEnemyOffOnRandom::ON)// || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
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
    }
    catch(...)
    {
        std:: cout << "erm..." << std::endl;
    }
CLEANUP:
    CloseHandle(hProcess);
    return 0;
}
