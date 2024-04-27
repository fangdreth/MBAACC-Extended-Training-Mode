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
    bool bAPressed = false;
    /*int nButtonsPressed = 0;
    int nOldButtonsPressed = 0;
    int nDirectionPressed = 0;
    int nOldDirectionPressed = 0;*/

    int nP1CharacterID = 0;
    int nP2CharacterID = 0;
    int nP1Moon = 0;
    int nP2Moon = 0;
    int nP1CharacterNumber = 0;
    int nP2CharacterNumber = 0;
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
    bool bOnExtraMenu = false;
    int nOldEnemyActionIndex = -1;
    int nOldPresetIndex = -1;
    int nOldEnemyDefenseIndex = -1;
    int nOldEnemyDefenseTypeIndex = -1;
    int nOldAirRecoveryIndex = -1;
    int nOldDownRecoveryIndex = -1;
    int nOldLifeIndex = -1;
    std::vector<std::string> vPatternNames = GetEmptyPatternList();
    bool bReversaled = false;
    int nTimer = 0;
    int nOldTimer = 0;
    bool bFramePassed = false;
    int nMot = 0;
    int nOldMot = 0;
    int nP2Y = 0;
    int nStoredEnemyAction = 0;
    int nStoredEnemyDefense = 0;
    int nStoredEnemyDefenseType = 0;
    int nStoredAirRecovery = 0;
    int nStoredDownRecovery = 0;
    int nGameCursorIndex = 0;
    int nOldGameCursorIndex = 0;
    int nCustomMeter = 10000;
    int nCustomHealth = 11400;
    int nHealthRefillTimer = 0;
    int nLifeRecover = 0; //0:ON 1:OFF

    int nDebugBias = 0;
    int nDebugFrameCount = 0;


    Menu oMenu = Menu();
    std::srand((unsigned int)std::time(nullptr));

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
                    //nOldButtonsPressed = nButtonsPressed;
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1APressed), &nReadResult, 4, 0);
                    bAPressed = (nReadResult == 1 ? true : false);

                    /*nOldDirectionPressed = nDirectionPressed;
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1DirectionPress), &nReadResult, 4, 0);
                    nDirectionPressed = nReadResult;*/


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

                        if (nCurrentSubMenu == 2)
                        {
                            // these are used in the menus.
                            nOldEnemyActionIndex = -1;
                            nOldEnemyDefenseIndex = -1;
                            nOldEnemyDefenseTypeIndex = -1;
                            nOldAirRecoveryIndex = -1;
                            nOldDownRecoveryIndex = -1;
                            nOldLifeIndex = -1;

                            nWriteBuffer = nStoredEnemyDefense;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nStoredEnemyDefenseType;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefenseType), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nStoredEnemyAction;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nStoredAirRecovery;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwAirRecovery), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nStoredDownRecovery;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwDownRecovery), &nWriteBuffer, 4, 0);
                        }

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

                            DWORD dwEnemyDefenseTypeStringAddress = GetEnemyDefenseTypeStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseTypeNormalStringAddress = GetEnemyDefenseTypeNormalStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseTypeComboStringAddress = GetEnemyDefenseTypeComboStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseTypeRandomStringAddress = GetEnemyDefenseTypeRandomStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionInfoStringAddress = GetEnemyActionInfoStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyDefenseOffString = GetEnemyDefenseOffStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseAllGuardString = GetEnemyDefenseAllGuardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseStatusGuardString = GetEnemyDefenseStatusGuardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseAllShieldString = GetEnemyDefenseAllShieldStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseStatusShieldString = GetEnemyDefenseStatusShieldStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseDodgeString = GetEnemyDefenseDodgeStringAddress(hProcess, dwBaseAddress);

                            DWORD dwAirRecoveryString = GetAirRecoveryStringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryOffString = GetAirRecoveryOffStringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryNeutralString = GetAirRecoveryNeutralStringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryBackString = GetAirRecoveryBackStringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryForwardString = GetAirRecoveryForwardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryRandom1String = GetAirRecoveryRandom1StringAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryRandom2String = GetAirRecoveryRandom2StringAddress(hProcess, dwBaseAddress);

                            DWORD dwDownRecoveryString = GetDownRecoveryStringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryOffString = GetDownRecoveryOffStringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryNeutralString = GetDownRecoveryNeutralStringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryBackString = GetDownRecoveryBackStringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryForwardString = GetDownRecoveryForwardStringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryRandom1String = GetDownRecoveryRandom1StringAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryRandom2String = GetDownRecoveryRandom2StringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionString = GetEnemyActionStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseString = GetEnemyDefenseStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionIndex;
                            DWORD dwEnemyDefenseIndex;
                            DWORD dwEnemyDefenseTypeIndex;
                            DWORD dwAirRecoveryIndex;
                            DWORD dwDownRecoveryIndex;
                            do
                            {
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hProcess, dwBaseAddress);
                                dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58) && nCurrentSubMenu == 7);

                            if (nCurrentSubMenu != 7)
                                continue;


                            char pcTrainingPreset[17] = "training preset.";
                            char pcExtendedSettings[18] = "EXTENDED SETTINGS";
                            char pcPreset[7] = "PRESET";
                            char pcExGuard[9] = "EX GUARD";
                            char pcReversalDelay[15] = "REVERSAL DELAY";
                            char pcMeter[18] = "METER [A]->SLOWER";
                            char pcHealth[19] = "HEALTH [A]->SLOWER";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset, 18, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings, 18, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcPreset, 7, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseString), &pcExGuard, 9, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcReversalDelay, 15, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryString), &pcMeter, 18, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryString), &pcHealth, 19, 0);

                            // Index values for menu items
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nReadResult, 4, 0);
                            int nEnemyActionIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nReadResult, 4, 0);
                            int nEnemyDefenseIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                            int nEnemyDefenseTypeIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nReadResult, 4, 0);
                            int nAirRecoveryIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nReadResult, 4, 0);
                            int nDownRecoveryIndex = nReadResult;

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
                                SetMeter(hProcess, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                            }
                            else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                            {
                                nCustomMeter = min(nCustomMeter + (bAPressed ? 10 : 1000), MAX_METER /*yes I know this lets hmoons go over 200*/);
                                SetMeter(hProcess, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                            }

                            if (nOldDownRecoveryIndex == -1)
                                nOldDownRecoveryIndex = nDownRecoveryIndex;
                            else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                            {
                                nCustomHealth = max(0, nCustomHealth - (bAPressed ? 1 : 100));
                                SetHealth(hProcess, dwBaseAddress, nCustomHealth);
                            }
                            else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                            {
                                nCustomHealth = min(nCustomHealth + (bAPressed ? 1 : 100), MAX_HEALTH);
                                SetHealth(hProcess, dwBaseAddress, nCustomHealth);
                            }

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

                            if (nReversalDelayFrames == 0)
                            {
                                char pcTemp[2] = "0";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 2, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 2, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 0;
                            }
                            else if (nReversalDelayFrames == MAX_DELAY)
                            {
                                char pcTemp[3];
                                strcpy_s(pcTemp, std::to_string(MAX_DELAY).c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 2;
                            }
                            else
                            {
                                char pcTemp[3];
                                strcpy_s(pcTemp, std::to_string(nReversalDelayFrames).c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 3, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                nEnemyDefenseTypeIndex = 1;
                            }

                            if (nCustomMeter == 0)
                            {
                                char pcTemp[5] = "0.0%";
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 5, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 5, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 0;
                            }
                            else if (nCustomMeter == MAX_METER)
                            {
                                char pcTemp[7] = "300.0%";
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 7, 0);

                                nWriteBuffer = 5;
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 5;
                            }
                            else
                            {
                                char pcTemp[8];
                                strcpy_s(pcTemp, (std::to_string(nCustomMeter / 100) + "." + std::to_string(nCustomMeter % 100 / 10) + "%").c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 8, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 8, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 8, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                nAirRecoveryIndex = 2;
                            }

                            if (nCustomHealth == 0)
                            {
                                char pcTemp[7] = "0 (0%)";
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                nDownRecoveryIndex = 0;
                            }
                            else if (nCustomHealth == MAX_HEALTH)
                            {
                                char pcTemp[13] = "11400 (100%)";
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 13, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 13, 0);

                                nWriteBuffer = 5;
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                nDownRecoveryIndex = 5;
                            }
                            else
                            {
                                char pcTemp[19];
                                int nPercent = (int)((float)nCustomHealth / 11400.0f * 100.0f);
                                strcpy_s(pcTemp, (std::to_string(nCustomHealth) + " (" + std::to_string(nPercent) + ")%").c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 19, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                nDownRecoveryIndex = 2;
                            }

                            nOldEnemyActionIndex = nEnemyActionIndex;
                            nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                            nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                            nOldAirRecoveryIndex = nAirRecoveryIndex;
                            nOldDownRecoveryIndex = nDownRecoveryIndex;
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
                            DWORD dwEnemyDefenseIndex;
                            DWORD dwEnemyDefenseTypeIndex;
                            DWORD dwAirRecoveryIndex;
                            DWORD dwDownRecoveryIndex;
                            do
                            {
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hProcess, dwBaseAddress);
                                dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58) && nCurrentSubMenu == 7);

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
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nReadResult, 4, 0);
                            int nEnemyDefenseIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nReadResult, 4, 0);
                            int nEnemyDefenseTypeIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nReadResult, 4, 0);
                            int nAirRecoveryIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nReadResult, 4, 0);
                            int nDownRecoveryIndex = nReadResult;

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

                            nWriteBuffer = nEnemyDefenseIndex;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nEnemyDefenseTypeIndex;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefenseType), &nWriteBuffer, 4, 0);

                            nStoredEnemyDefense = nEnemyDefenseIndex;
                            nStoredEnemyAction = nEnemyActionIndex;
                            nStoredEnemyDefenseType = nEnemyDefenseTypeIndex;
                            nStoredAirRecovery = (nAirRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                            nStoredDownRecovery = (nDownRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                        }

                        // BATTLE SETTINGS
                        if (nCurrentSubMenu == 6)
                        {
                            DWORD dwNoRecoverString = GetNoRecoverStringAddress(hProcess, dwBaseAddress);
                            DWORD dwRecover25String = GetRecover25StringAddress(hProcess, dwBaseAddress);
                            DWORD dwRecover50String = GetRecover50StringAddress(hProcess, dwBaseAddress);
                            DWORD dwRecover75String = GetRecover75StringAddress(hProcess, dwBaseAddress);
                            DWORD dwRecover100String = GetRecover100StringAddress(hProcess, dwBaseAddress);

                            DWORD dwLifeIndex;
                            do
                            {
                                dwLifeIndex = GetLifeIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwLifeIndex == 0x58) && nCurrentSubMenu == 6);

                            if (nCurrentSubMenu != 6)
                                continue;

                            ReadProcessMemory(hProcess, (LPVOID)(dwLifeIndex), &nReadResult, 4, 0);
                            int nLifeIndex = nReadResult;

                            if (nOldLifeIndex == -1)
                                nOldLifeIndex = nLifeIndex;

                            // left
                            else if (nOldLifeIndex > nLifeIndex)
                                nLifeRecover = max(0, nLifeRecover - 1);
                            // right
                            else if (nOldLifeIndex < nLifeIndex)
                                nLifeRecover = min(nLifeRecover + 1, 1);

                            if (nLifeRecover == 0)
                            {
                                char pcTemp[8] = "RECOVER";
                                WriteProcessMemory(hProcess, (LPVOID)(dwNoRecoverString), &pcTemp, 8, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwRecover25String), &pcTemp, 8, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwLifeIndex), &nWriteBuffer, 4, 0);
                                nLifeIndex = 0;
                            }
                            else
                            {
                                char pcTemp[11] = "NO RECOVER";
                                WriteProcessMemory(hProcess, (LPVOID)(dwRecover75String), &pcTemp, 11, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwRecover100String), &pcTemp, 11, 0);

                                nWriteBuffer = 4;
                                WriteProcessMemory(hProcess, (LPVOID)(dwLifeIndex), &nWriteBuffer, 4, 0);
                                nLifeIndex = 4;
                            }

                            nOldLifeIndex = nLifeIndex;
                        }

                        oMenu.UpdateProcessInfo(hProcess, dwBaseAddress);
                        //oMenu.DrawMenu();
                    }

                    else // not paused
                        bOnExtraMenu = false;

                    
                    nWriteBuffer = 1;
                    if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                        nWriteBuffer = 10;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwExGuard + dwP2Offset), &nWriteBuffer, 4, 0);

                    nWriteBuffer = 4;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwLifeRecover), &nWriteBuffer, 4, 0);

                    
                    //nWriteBuffer = 0;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                    //nWriteBuffer = 1;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x155da8/*Will block*/), &nWriteBuffer, 1, 0);
                    //nWriteBuffer = 6;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x3713C4/*p2 direction*/), &nWriteBuffer, 1, 0);
                    //nWriteBuffer = 0x6;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x371474/*edx*/), &nWriteBuffer, 4, 0);
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x370f5c/*p2 direction*/), &nWriteBuffer, 1, 0);
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x371524/*p2 direction*/), &nWriteBuffer, 1, 0);
                    //nWriteBuffer = 1; // sorta works but makes them invincible...
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x1553F4 + dwP2Offset /*dash disable flag*/), &nWriteBuffer, 1, 0);
                    //continue;
                    //nWriteBuffer = 0x9090;
                    //WriteProcessMemory(hProcess, (LPVOID)(0x41f2fa/*nop slide*/), &nWriteBuffer, 2, 0);
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x41f308/*nop slide*/), &nWriteBuffer, 2, 0);
                    //nWriteBuffer = 0;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x15524c + dwP2Offset/*x-vel*/), &nWriteBuffer, 4, 0);
                    


                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
                    nTimer = nReadResult;
                    if (nTimer == nOldTimer)
                        continue;
                    nOldTimer = nTimer;

                    if (nTimer == 0 || vPatternNames.size() == 1)
                    {
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1CharNumber), &nReadResult, 4, 0);
                        nP1CharacterNumber = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharNumber), &nReadResult, 4, 0);
                        nP2CharacterNumber = nReadResult;

                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1CharMoon), &nReadResult, 4, 0);
                        nP1Moon = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2CharMoon), &nReadResult, 4, 0);
                        nP2Moon = nReadResult;

                        nP1CharacterID = 10 * nP1CharacterNumber + nP1Moon;
                        nP2CharacterID = 10 * nP2CharacterNumber + nP2Moon;

                        vPatternNames = GetPatternList(nP2CharacterID);
                    }

                    if (nTimer == 1)
                    {
                        SetMeter(hProcess, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);

                        nWriteBuffer = vGuardLevelLookupTable[nEnemyGuardLevelSetting + nP2Moon * 6];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2), &nWriteBuffer, 4, 0);
                    }


                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    if (nReadResult == 0)
                        nHealthRefillTimer++;
                    else
                        nHealthRefillTimer = 0;

                    if (nTimer == 1 || (nHealthRefillTimer == 20 && nLifeRecover == 0))
                    {
                        SetHealth(hProcess, dwBaseAddress, nCustomHealth);
                        nHealthRefillTimer = 0;
                    }

                    // Disable vanilla reversal action
                    nWriteBuffer = 0;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);

                    //nWriteBuffer = nEnemyStatusSetting;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

                    //nWriteBuffer = eEnemyDefense::ALLGUARD;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);



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
                        nWriteBuffer = vGuardLevelLookupTable[nP2Moon * 6];
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardAmount2), &nWriteBuffer, 4, 0);
                    }
                    else
                    {
                        nWriteBuffer = nEnemyGuardLevelSetting - 1;
                        WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardSetting), &nWriteBuffer, 4, 0);
                    }

                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwY + dwP2Offset), &nReadResult, 4, 0);
                    nP2Y = nReadResult;
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwMot), &nReadResult, 4, 0);
                    nMot = nReadResult;
                    if (nTimer == 0)
                        bReversaled = true;
                    if (nTimer == 2)
                        bReversaled = false;
                    if (nTimer != 0 && GetPattern(nP2CharacterID, vPatternNames[nReversalIndex]) != 0)
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
                            nWriteBuffer = GetPattern(nP2CharacterID, vPatternNames[nReversalIndex]);
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

            //bSwitchToCrouch = true;
            if (bSwitchToCrouch)
            {
                //Sleep(16 * nSwitchBlockDelayFrames);

                //nWriteBuffer = eEnemyStatus::CROUCH;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyStatus), &nWriteBuffer, 4, 0);

                //nWriteBuffer = eEnemyDefense::STATUSGUARD;
                //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);

                

                // Loop until the dummy returns to neutral
                while (1)
                {
                    //nWriteBuffer = 0;
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x155da8/*Will block*/), &nWriteBuffer, 1, 0);

                    //nWriteBuffer = 33685504;
                    /*nWriteBuffer = 50528256;
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
                    //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwController), &nOld3, 4, 0);*/

                    nWriteBuffer = 2;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + 0x3713C4/*p2 direction*/), &nWriteBuffer, 4, 0);

                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    if (nReadResult == 13)
                    {


                        //nWriteBuffer = 13;
                        //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                        break;
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
