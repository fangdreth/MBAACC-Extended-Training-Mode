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
    bool bDPressed = false;
    bool bOldDPressed = false;
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
    int nCustomGuard = eEnemyGuardLevelSettings::ONEHUNDRED;
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
    int nOldThrowRecoveryIndex = -1;
    int nOldReduceDamageIndex = -1;
    int nOldLifeIndex = -1;
    int nCurrentSubMenu = 2;
    int nOldCurrentSubMenu = -1;
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
    int nStoredThrowRecovery = 0;
    int nStoredReduceDamage = 0;
    int nGameCursorIndex = 0;
    int nOldGameCursorIndex = 0;
    int nCustomMeter = 10000;
    int nCustomHealth = 11400;
    int nHealthRefillTimer = 0;
    int nCharSpecificsRefillTimer = 0;
    bool bLifeRecover = true;
    int nExtendedSettingsPage = 1;
    int nSionBullets = 13;
    int nRoaVisibleCharge = 0;
    int nRoaHiddenCharge = 0;
    bool bPositionsLocked = false;
    int nP1X = -16384;
    int nP2X = 16384;
    int nP3X = -29184;
    int nP4X = 29184;
    bool bP3Exists = false;
    bool bP4Exists = false;
    int nHitsTillBurst = TOO_HIGH_TO_BURST;
    bool bInfGuard = true;
    bool bRandomReversal = false;
    int nEnemySettingsCursor = 0;
    int nOldEnemySettingsCursor = 0;
    bool bNeedToAnnounceNewVersion = false;
    int nMovingMessageIndex = 0;
    int nStartingTime = 0;

    int nDebugBias = 0;
    int nDebugFrameCount = 0;

    std::srand((unsigned int)std::time(nullptr));

    std::string sOnlineVersion = GetLatestVersion();
    if (sOnlineVersion != "" && sOnlineVersion != VERSION)
        bNeedToAnnounceNewVersion = true;

    std::cout << "Fang's Extended Training Mode Mod" << std::endl;
    std::cout << GITHUB_RELEASE << std::endl;

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

                    InitializeCharacterMaps();
                }

                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwCSSFlag), &nReadResult, 4, 0);
                if (nReadResult == 0)
                {
                    nReversalIndex = 0;
                    vPatternNames = GetEmptyPatternList();
                }
                else
                {
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1APressed), &nReadResult, 4, 0);
                    bAPressed = (nReadResult == 1 ? true : false);
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1DPressed), &nReadResult, 4, 0);
                    bOldDPressed = bDPressed;
                    bDPressed = (nReadResult == 1 ? true : false);

                    // these flags are used to determine if assist chars exist
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Exists + dwP2Offset * 2), &nReadResult, 4, 0);
                    bP3Exists = (nReadResult == 1 ? true : false);
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1Exists + dwP2Offset * 3), &nReadResult, 4, 0);
                    bP4Exists = (nReadResult == 1 ? true : false);

                    // get the locations if not locked
                    if (!bPositionsLocked)
                    {
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X), &nReadResult, 4, 0);
                        nP1X = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset), &nReadResult, 4, 0);
                        nP2X = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 2), &nReadResult, 4, 0);
                        nP3X = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 3), &nReadResult, 4, 0);
                        nP4X = nReadResult;
                    }

                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwPausedFlag), &nReadResult, 4, 0);
                    bPaused = (nReadResult == 1 ? true : false);
                    if (bPaused)
                    {
                        DWORD dwMainViewScreenStringAddress = GetMainViewScreenStringAddress(hProcess, dwBaseAddress);
                        DWORD dwTrainingMenuString = GetTrainingMenuStringAddress(hProcess, dwBaseAddress);

                        char pcExtendedSettings[18] = "EXTENDED SETTINGS";
                        char pcTrainingMenu[19];
                        strcpy_s(pcTrainingMenu, ("EXTENDED MOD  " + VERSION).c_str());

                        WriteProcessMemory(hProcess, (LPVOID)(dwMainViewScreenStringAddress), &pcExtendedSettings, 18, 0);
                        WriteProcessMemory(hProcess, (LPVOID)(dwTrainingMenuString), &pcTrainingMenu, 19, 0);

                        DWORD dwSubMenuAddress = GetSubMenuEnumAddress(hProcess, dwBaseAddress);
                        ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                        nOldCurrentSubMenu = nCurrentSubMenu;
                        nCurrentSubMenu = nReadResult;

                        if (nCurrentSubMenu == 2)
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
                            nWriteBuffer = nStoredThrowRecovery;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwThrowRecovery), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nStoredReduceDamage;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwReduceRecovery), &nWriteBuffer, 4, 0);
                        
                            DWORD dwReturnToMainMenuString = GetReturnToMainMenuStringAddress(hProcess, dwBaseAddress);
                            if (bNeedToAnnounceNewVersion)
                            {
                                // assemble the string for the message
                                std::string sNewVersionMessage = "    VERSION " + sOnlineVersion + " AVAILABLE NOW ON GITHUB";
                                int nMessageLength = sNewVersionMessage.size();
                                sNewVersionMessage += "     " + sNewVersionMessage;

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
                                WriteProcessMemory(hProcess, (LPVOID)(dwReturnToMainMenuString), &pcNewVersionMessage, 19, 0);
                            }
                            else
                            {
                                char pcBlank[19] = "LATEST VERSION";
                                WriteProcessMemory(hProcess, (LPVOID)(dwReturnToMainMenuString), &pcBlank, 19, 0);
                            }
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

                            DWORD dwEnemyActionOffString = GetEnemyActionOffStringAddress(hProcess, dwBaseAddress);
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

                            DWORD dwThrowRecoveryString = GetThrowRecoveryStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryNormalString = GetThrowRecoveryNormalStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryAllFastString = GetThrowRecoveryAllFastStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryAllLateString = GetThrowRecoveryAllLateStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryAllRandomString = GetThrowRecoveryAllRandomStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryRandomFastString = GetThrowRecoveryRandomFastStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryRandomLateString = GetThrowRecoveryRandomLateStringAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryRandomRandomString = GetThrowRecoveryRandomRandomStringAddress(hProcess, dwBaseAddress);

                            DWORD dwReduceDamageString = GetReduceDamageStringAddress(hProcess, dwBaseAddress);
                            DWORD dwReduceDamageNormalString = GetReduceDamageNormalStringAddress(hProcess, dwBaseAddress);
                            DWORD dwReduceDamageAllString = GetReduceDamageAllStringAddress(hProcess, dwBaseAddress);
                            DWORD dwReduceDamageRandomString = GetReduceDamageRandomStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionString = GetEnemyActionStringAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseString = GetEnemyDefenseStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemyActionOptionX = GetEnemyActionOptionXAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseOptionX = GetEnemyDefenseOptionXAddress(hProcess, dwBaseAddress);
                            DWORD dwEnemyDefenseTypeOptionX = GetEnemyDefenseTypeOptionXAddress(hProcess, dwBaseAddress);
                            DWORD dwAirRecoveryOptionX = GetAirRecoveryOptionXAddress(hProcess, dwBaseAddress);
                            DWORD dwDownRecoveryOptionX = GetDownRecoveryOptionXAddress(hProcess, dwBaseAddress);
                            DWORD dwThrowRecoveryOptionX = GetThrowRecoveryOptionXAddress(hProcess, dwBaseAddress);

                            DWORD dwDefaultString = GetDefaultStringAddress(hProcess, dwBaseAddress);

                            DWORD dwEnemySettingsCursor = GetEnemySettingsCursorAddress(hProcess, dwBaseAddress);

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
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hProcess, dwBaseAddress);
                                dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwThrowRecoveryIndex = GetThrowRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwReduceDamageIndex = GetReduceDamageIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyStatusIndex = GetEnemyStatusAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58 || dwThrowRecoveryIndex == 0x58 || dwReduceDamageIndex == 0x58 || dwEnemyStatusIndex == 0x58) && nCurrentSubMenu == 7);

                            if (nCurrentSubMenu != 7)
                                continue;

                            // Index values for menu items
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nReadResult, 4, 0);
                            nOldEnemySettingsCursor = nEnemySettingsCursor;
                            nEnemySettingsCursor = nReadResult;
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
                            ReadProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nReadResult, 4, 0);
                            int nThrowRecoveryIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwReduceDamageIndex), &nReadResult, 4, 0);
                            int nReduceDamageIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwEnemyStatusIndex), &nReadResult, 4, 0);
                            int nEnemyStatusIndex = nReadResult;

                            // Reset hits till burst if status is not manual
                            if (nEnemyStatusIndex != eEnemyStatus::MANUAL)
                                nHitsTillBurst = TOO_HIGH_TO_BURST;

                            // Replace static menu fields
                            char pcTrainingPreset[17] = "training preset.";
                            char pcExtendedSettings[18] = "EXTENDED SETTINGS";
                            char pcBlank[1] = "";
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionInfoStringAddress), &pcTrainingPreset, 18, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsMenuTitle), &pcExtendedSettings, 18, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageString), &pcBlank, 1, 0);
                            WriteProcessMemory(hProcess, (LPVOID)(dwDefaultString), &pcBlank, 1, 0);
                            if (nExtendedSettingsPage == 1)
                            {
                                char pcReversalType[14] = "REVERSAL TYPE";
                                char pcExGuard[9] = "EX GUARD";
                                char pcReversalDelay[15] = "REVERSAL DELAY";
                                char pcMeter[18] = "METER [A]->SLOWER";
                                char pcHealth[19] = "HEALTH [A]->SLOWER";
                                char pcGuardBar[10] = "GUARD BAR";
                                char pcBlank[1] = "";
                                
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcReversalType, 14, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseString), &pcExGuard, 9, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcReversalDelay, 15, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryString), &pcMeter, 18, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryString), &pcHealth, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryString), &pcGuardBar, 10, 0);
                            }
                            else if (nExtendedSettingsPage == 2)
                            {
                                char pcASlow[12] = "[A]->SLOWER";
                                char pcP1XLoc[11] = "P1 X-LOC";
                                char pcP2XLoc[11] = "P2 X-LOC";
                                char pcAssistLoc[13] = "ASSIST X-LOC";
                                char pcInvert[7] = "INVERT";
                                char pcUnstable[11] = "(UNSTABLE)";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcASlow, 12, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseString), &pcP1XLoc, 11, 0);
                                if (bP3Exists)
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcAssistLoc, 13, 0);
                                else
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeStringAddress), pcBlank, 2, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryString), &pcP2XLoc, 11, 0);
                                if (bP4Exists)
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryString), &pcAssistLoc, 13, 0);
                                else
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryString), pcBlank, 2, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryString), &pcUnstable, 11, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryNormalString), &pcInvert, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryAllFastString), &pcInvert, 7, 0);
                            }
                            else if (nExtendedSettingsPage == 3)
                            {
                                char pcSionBullets[13] = "SION BULLETS";
                                char pcRoaVisibleCharge[19] = "ROA VISIBLE CHARGE";
                                char pcRoaHiddenCharge[19] = "ROA HIDDEN CHARGE";
                                char pcHitsUntilBurst[17] = "HITS UNTIL BURST";
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionString), &pcSionBullets, 13, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseString), &pcRoaVisibleCharge, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeStringAddress), &pcRoaHiddenCharge, 19, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryString), &pcBlank, 2, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryString), &pcBlank, 2, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryString), &pcHitsUntilBurst, 17, 0);
                            }

                            // Skip DEFAULT
                            if (nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 12)
                            {
                                nWriteBuffer = 14;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                nEnemySettingsCursor = 14;
                                nOldEnemySettingsCursor = 14;
                            }
                            else if (nOldEnemySettingsCursor == 14 && nEnemySettingsCursor == 12)
                            {
                                nWriteBuffer = 10;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
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
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 10;
                                    nOldEnemySettingsCursor = 10;
                                }
                                if (bPositionsLocked && nOldEnemySettingsCursor == 10 && nEnemySettingsCursor == 8)
                                {
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 0;
                                    nOldEnemySettingsCursor = 0;
                                }
                                if (!bP3Exists && nOldEnemySettingsCursor == 2 && nEnemySettingsCursor == 3)
                                {
                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 5;
                                    nOldEnemySettingsCursor = 5;
                                }
                                else if (!bP3Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 3)
                                {
                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 2;
                                    nOldEnemySettingsCursor = 2;
                                }
                                if (!bP4Exists && nOldEnemySettingsCursor == 5 && nEnemySettingsCursor == 6)
                                {
                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 8;
                                    nOldEnemySettingsCursor = 8;
                                }
                                else if (!bP4Exists && nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                                {
                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 5;
                                    nOldEnemySettingsCursor = 5;
                                }
                            }
                            else if (nExtendedSettingsPage == 3)
                            {
                                if (nOldEnemySettingsCursor == 3 && nEnemySettingsCursor == 5)
                                {
                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
                                    nEnemySettingsCursor = 8;
                                    nOldEnemySettingsCursor = 8;
                                }
                                else if (nOldEnemySettingsCursor == 8 && nEnemySettingsCursor == 6)
                                {
                                    nWriteBuffer = 3;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemySettingsCursor), &nWriteBuffer, 4, 0);
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
                                    SetP1X(hProcess, dwBaseAddress, nP1X);
                                }
                                else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                {
                                    nP1X = min(MAX_X, nP1X + (bAPressed ? 1 : 1000));
                                    SetP1X(hProcess, dwBaseAddress, nP1X);
                                }

                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                {
                                    nP3X = max(MIN_X, nP3X - (bAPressed ? 1 : 1000));
                                    SetP3X(hProcess, dwBaseAddress, nP3X);
                                }
                                else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                {
                                    nP3X = min(MAX_X, nP3X + (bAPressed ? 1 : 1000));
                                    SetP3X(hProcess, dwBaseAddress, nP3X);
                                }

                                if (nOldAirRecoveryIndex == -1)
                                    nOldAirRecoveryIndex = nAirRecoveryIndex;
                                else if (nOldAirRecoveryIndex > nAirRecoveryIndex)// left
                                {
                                    nP2X = max(MIN_X, nP2X - (bAPressed ? 1 : 1000));
                                    SetP2X(hProcess, dwBaseAddress, nP2X);
                                }
                                else if (nOldAirRecoveryIndex < nAirRecoveryIndex)// right
                                {
                                    nP2X = min(MAX_X, nP2X + (bAPressed ? 1 : 1000));
                                    SetP2X(hProcess, dwBaseAddress, nP2X);
                                }

                                if (nOldDownRecoveryIndex == -1)
                                    nOldDownRecoveryIndex = nDownRecoveryIndex;
                                else if (nOldDownRecoveryIndex > nDownRecoveryIndex)// left
                                {
                                    nP4X = max(MIN_X, nP4X - (bAPressed ? 1 : 1000));
                                    SetP4X(hProcess, dwBaseAddress, nP4X);
                                }
                                else if (nOldDownRecoveryIndex < nDownRecoveryIndex)// right
                                {
                                    nP4X = min(MAX_X, nP4X + (bAPressed ? 1 : 1000));
                                    SetP4X(hProcess, dwBaseAddress, nP4X);
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
                                    SetP1X(hProcess, dwBaseAddress, nP1X);
                                    SetP2X(hProcess, dwBaseAddress, nP2X);
                                    SetP3X(hProcess, dwBaseAddress, nP3X);
                                    SetP4X(hProcess, dwBaseAddress, nP4X);
                                }
                            }
                            else if (nExtendedSettingsPage == 3)
                            {
                                if (nOldEnemyActionIndex == -1)
                                    nOldEnemyActionIndex = nEnemyActionIndex;
                                else if (nOldEnemyActionIndex > nEnemyActionIndex)// left
                                {
                                    nSionBullets = min(nSionBullets + 1, MAX_BULLETS);
                                    SetSionBullets(hProcess, dwBaseAddress, nSionBullets);
                                }
                                else if (nOldEnemyActionIndex < nEnemyActionIndex)// right
                                {
                                    nSionBullets = max(1, nSionBullets - 1);
                                    SetSionBullets(hProcess, dwBaseAddress, nSionBullets);
                                }

                                if (nOldEnemyDefenseIndex == -1)
                                    nOldEnemyDefenseIndex = nEnemyDefenseIndex;
                                else if (nOldEnemyDefenseIndex > nEnemyDefenseIndex)// left
                                {
                                    nRoaVisibleCharge = max(0, nRoaVisibleCharge - 1);
                                    SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                                }
                                else if (nOldEnemyDefenseIndex < nEnemyDefenseIndex)// right
                                {
                                    nRoaVisibleCharge = min(nRoaVisibleCharge + 1, MAX_CHARGE);
                                    SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                                }

                                if (nOldEnemyDefenseTypeIndex == -1)
                                    nOldEnemyDefenseTypeIndex = nEnemyDefenseTypeIndex;
                                else if (nOldEnemyDefenseTypeIndex > nEnemyDefenseTypeIndex)// left
                                {
                                    nRoaHiddenCharge = max(0, nRoaHiddenCharge - 1);
                                    SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                                }
                                else if (nOldEnemyDefenseTypeIndex < nEnemyDefenseTypeIndex)// right
                                {
                                    nRoaHiddenCharge = min(nRoaHiddenCharge + 1, MAX_CHARGE);
                                    SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
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
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (!bRandomReversal)
                                {
                                    char pcTemp[7] = "NORMAL";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionOffString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else if (bRandomReversal)
                                {
                                    char pcTemp[7] = "RANDOM";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 7, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
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

                                if (bInfGuard)
                                {
                                    char pcTemp[9] = "INFINITE";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryNormalString), &pcTemp, 9, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryAllFastString), &pcTemp, 9, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 0;
                                }
                                else
                                {
                                    char pcTemp[7] = "NORMAL";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryRandomLateString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcTemp, 7, 0);

                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 6;
                                }
                            }
                            else if (nExtendedSettingsPage == 2)
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                if (bP3Exists)
                                    nWriteBuffer = ONSCREEN_LOCATION;
                                else
                                    nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);
                                if (bP4Exists)
                                    nWriteBuffer = ONSCREEN_LOCATION;
                                else
                                    nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (!bPositionsLocked)
                                {
                                    char pcTemp[9] = "UNLOCKED";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionOffString), &pcTemp, 9, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 9, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else
                                {
                                    char pcTemp[7] = "LOCKED";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 7, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }

                                if (nP1X == MIN_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 0;
                                }
                                else if (nP1X == MAX_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(nP1X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 2;
                                }

                                if (nP2X == MIN_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 0;
                                }
                                else if (nP2X == MAX_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 2;
                                }
                                else
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(nP3X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                                    nWriteBuffer = 1;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 1;
                                }

                                if (nP3X == MIN_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryOffString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 0;
                                }
                                else if (nP3X == MAX_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryRandom1String), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryRandom2String), &pcTemp, 7, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(nP2X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryNeutralString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryBackString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryForwardString), &pcTemp, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nAirRecoveryIndex = 2;
                                }
                            
                                if (nP4X == MIN_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MIN_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryOffString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 7, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 0;
                                }
                                else if (nP4X == MAX_X)
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(MAX_X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryRandom1String), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryRandom2String), &pcTemp, 7, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[7];
                                    strcpy_s(pcTemp, std::to_string(nP4X).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryNeutralString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryBackString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryForwardString), &pcTemp, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nDownRecoveryIndex = 2;
                                }

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                nThrowRecoveryIndex = 0;
                            }
                            else if (nExtendedSettingsPage == 3)
                            {
                                nWriteBuffer = ONSCREEN_LOCATION;
                                WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeOptionX), &nWriteBuffer, 4, 0);

                                nWriteBuffer = OFFSCREEN_LOCATION;
                                WriteProcessMemory(hProcess, (LPVOID)(dwAirRecoveryOptionX), &nWriteBuffer, 4, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwDownRecoveryOptionX), &nWriteBuffer, 4, 0);

                                if (nSionBullets == MAX_BULLETS)
                                {
                                    char pcTemp[3] = "13";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionOffString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 3, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 0;
                                }
                                else if (nSionBullets == 1)
                                {
                                    char pcTemp[2] = "1";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverCString), &pcTemp, 2, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyRecoverDString), &pcTemp, 2, 0);

                                    nWriteBuffer = 8;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 8;
                                }
                                else
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nSionBullets).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionAString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionBString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionCString), &pcTemp, 3, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyActionIndex), &nWriteBuffer, 4, 0);
                                    nEnemyActionIndex = 2;
                                }
                            
                                if (nRoaVisibleCharge == 0)
                                {
                                    char pcTemp[2] = "0";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseOffString), &pcTemp, 4, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 0;
                                }
                                else if (nRoaVisibleCharge == MAX_CHARGE)
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseDodgeString), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusShieldString), &pcTemp, 7, 0);

                                    nWriteBuffer = 5;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 5;
                                }
                                else
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nRoaVisibleCharge).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllGuardString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseStatusGuardString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseAllShieldString), &pcTemp, 3, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseIndex = 2;
                                }

                                if (nRoaHiddenCharge == 0)
                                {
                                    char pcTemp[2] = "0";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 4, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 0;
                                }
                                else if (nRoaHiddenCharge == MAX_CHARGE)
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(MAX_CHARGE).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 7, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 7, 0);

                                    nWriteBuffer = 2;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 2;
                                }
                                else
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeNormalStringAddress), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeComboStringAddress), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeRandomStringAddress), &pcTemp, 3, 0);

                                    nWriteBuffer = 1;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyDefenseTypeIndex), &nWriteBuffer, 4, 0);
                                    nEnemyDefenseTypeIndex = 1;
                                }
                            
                                if (nHitsTillBurst == TOO_HIGH_TO_BURST)
                                {
                                    char pcTemp[4] = "OFF";
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryNormalString), &pcTemp, 4, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryAllFastString), &pcTemp, 4, 0);

                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 0;

                                    nWriteBuffer = eEnemyStatus::STAND;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                                }
                                else if (nHitsTillBurst == MAX_BURST)
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nRoaHiddenCharge).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryRandomLateString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryRandomRandomString), &pcTemp, 3, 0);

                                    nWriteBuffer = 6;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 6;

                                    nWriteBuffer = eEnemyStatus::MANUAL;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                                    nWriteBuffer = eMagicCircuit::UNLIMITED;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                                }
                                else
                                {
                                    char pcTemp[3];
                                    strcpy_s(pcTemp, std::to_string(nHitsTillBurst).c_str());
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryAllLateString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryAllRandomString), &pcTemp, 3, 0);
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryRandomFastString), &pcTemp, 3, 0);

                                    nWriteBuffer = 3;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nWriteBuffer, 4, 0);
                                    nThrowRecoveryIndex = 3;

                                    nWriteBuffer = eEnemyStatus::MANUAL;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwEnemyStatusIndex), &nWriteBuffer, 4, 0);
                                    nWriteBuffer = eMagicCircuit::UNLIMITED;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwMagicCircuitSetting), &nWriteBuffer, 4, 0);
                                }
                            }

                            // Replace PAGE text
                            if (nExtendedSettingsPage == 1)
                            {
                                char pcTemp[7] = "PAGE 1";
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);

                                nWriteBuffer = 0;
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                                nReduceDamageIndex = 0;
                            }
                            else if (nExtendedSettingsPage == MAX_PAGES)
                            {
                                char pcTemp[7];
                                strcpy_s(pcTemp, ("PAGE " + std::to_string(MAX_PAGES)).c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                                nWriteBuffer = 2;
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
                                nReduceDamageIndex = 2;
                            }
                            else
                            {
                                char pcTemp[7];
                                strcpy_s(pcTemp, ("PAGE " + std::to_string(nExtendedSettingsPage)).c_str());
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageNormalString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageAllString), &pcTemp, 7, 0);
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageRandomString), &pcTemp, 7, 0);

                                nWriteBuffer = 1;
                                WriteProcessMemory(hProcess, (LPVOID)(dwReduceDamageIndex), &nWriteBuffer, 4, 0);
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
                            DWORD dwThrowRecoveryIndex;
                            DWORD dwReduceDamageIndex;
                            do
                            {
                                dwEnemyActionIndex = GetEnemyActionIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseIndex = GetEnemyDefenseIndexAddress(hProcess, dwBaseAddress);
                                dwEnemyDefenseTypeIndex = GetEnemyDefenseTypeIndexAddress(hProcess, dwBaseAddress);
                                dwAirRecoveryIndex = GetAirRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwDownRecoveryIndex = GetDownRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwThrowRecoveryIndex = GetThrowRecoveryIndexAddress(hProcess, dwBaseAddress);
                                dwReduceDamageIndex = GetReduceDamageIndexAddress(hProcess, dwBaseAddress);

                                ReadProcessMemory(hProcess, (LPVOID)(dwSubMenuAddress), &nReadResult, 4, 0);
                                nCurrentSubMenu = nReadResult;

                            } while ((dwEnemyActionIndex == 0x58 || dwEnemyDefenseIndex == 0x58 || dwEnemyDefenseTypeIndex == 0x58 || dwAirRecoveryIndex == 0x58 || dwDownRecoveryIndex == 0x58 || dwThrowRecoveryIndex == 0x58 || dwReduceDamageIndex == 0x58) && nCurrentSubMenu == 7);

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
                            ReadProcessMemory(hProcess, (LPVOID)(dwThrowRecoveryIndex), &nReadResult, 4, 0);
                            int nThrowRecoveryIndex = nReadResult;
                            ReadProcessMemory(hProcess, (LPVOID)(dwReduceDamageIndex), &nReadResult, 4, 0);
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

                            //nWriteBuffer = nEnemyDefenseIndex;
                            //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefense), &nWriteBuffer, 4, 0);
                            //nWriteBuffer = nEnemyDefenseTypeIndex;
                            //WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyDefenseType), &nWriteBuffer, 4, 0);

                            nStoredEnemyDefense = nEnemyDefenseIndex;
                            nStoredEnemyAction = nEnemyActionIndex;
                            nStoredEnemyDefenseType = nEnemyDefenseTypeIndex;
                            nStoredAirRecovery = (nAirRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                            nStoredDownRecovery = (nDownRecoveryIndex + 5) % 6; // different indices in-game vs in-menu
                            nStoredThrowRecovery = nThrowRecoveryIndex;
                            nStoredReduceDamage = nReduceDamageIndex;
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
                                bLifeRecover = true;
                            // right
                            else if (nOldLifeIndex < nLifeIndex)
                                bLifeRecover = false;

                            if (bLifeRecover)
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
                    }
                    else // not paused
                    {
                        bOnExtraMenu = false;
                        nOldCurrentSubMenu = -1;
                        nCurrentSubMenu = 2;
                    }

                    // Enable Ex Guard.  randomly if applicable
                    nWriteBuffer = 1;
                    if (nExGuardSetting == eEnemyOffOnRandom::ON || (rand() % 2 == 0 && nExGuardSetting == eEnemyOffOnRandom::RANDOM))
                        nWriteBuffer = 10;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwExGuard + dwP2Offset), &nWriteBuffer, 4, 0);

                    // Disable built-in health recovery
                    nWriteBuffer = 4;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwLifeRecover), &nWriteBuffer, 4, 0);

                    // Disable built-in reversal action
                    nWriteBuffer = 0;
                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwEnemyAction), &nWriteBuffer, 4, 0);

                    // This locks all the code that follows to the framerate of the game
                    // put code that should run faster above this
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwRoundTime), &nReadResult, 4, 0);
                    nTimer = nReadResult;
                    if (nTimer == nOldTimer)
                        continue;
                    nOldTimer = nTimer;

                    // populate the reversal patterns list and character data
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

                    // burst a combo
                    if (nHitsTillBurst != TOO_HIGH_TO_BURST)
                    {
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwComboCount), &nReadResult, 4, 0);
                        int nComboCount = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1HitstopRemaining + dwP2Offset), &nReadResult, 4, 0);
                        int nHitstopRemaining = nReadResult;
                        ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1HitstunRemaining + dwP2Offset), &nReadResult, 4, 0);
                        int nHitstunRemaining = nReadResult;
                        if (nComboCount >= nHitsTillBurst && nHitstopRemaining == 0 && nHitstunRemaining != 0)
                        {
                            while (true)
                            {
                                ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1HitstunRemaining + dwP2Offset), &nReadResult, 4, 0);
                                nHitstunRemaining = nReadResult;
                                if (nHitstunRemaining == 0)
                                {
                                    Sleep(16);  //need to wait 1 frame
                                    nWriteBuffer = 0;
                                    WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwComboCount), &nWriteBuffer, 4, 0);
                                    break;
                                }

                                nWriteBuffer = 1;
                                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2E), &nWriteBuffer, 1, 0);

                            }
                        }
                    }

                    // reset resources to the preset when training mode is reset
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwGuardSetting), &nReadResult, 4, 0);
                    nCustomGuard = nReadResult;
                    if (nTimer == 1)
                    {
                        SetMeter(hProcess, dwBaseAddress, nCustomMeter, nP1Moon, nP2Moon);
                        SetGuard(hProcess, dwBaseAddress, nCustomGuard, nP1Moon, nP2Moon);
                        SetGuard(hProcess, dwBaseAddress, 0, nP1Moon, nP2Moon);

                        if (bPositionsLocked)
                        {
                            nWriteBuffer = nP1X;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP2X;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP3X;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 2), &nWriteBuffer, 4, 0);
                            nWriteBuffer = nP4X;
                            WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP1X + dwP2Offset * 3), &nWriteBuffer, 4, 0);
                        }
                    }

                    // infinite guard bar
                    if (bInfGuard)
                        SetGuard(hProcess, dwBaseAddress, 0, nP1Moon, nP2Moon);

                    // increase the counter every frame p2 is standing idle to delay regenerating health
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternRead), &nReadResult, 4, 0);
                    if (nReadResult == 0 && nHealthRefillTimer < 20)
                    {
                        nHealthRefillTimer++;
                        nCharSpecificsRefillTimer++;
                    }
                    else
                    {
                        nHealthRefillTimer = 0;
                        nCharSpecificsRefillTimer = 0;
                    }

                    // refill health if training mode is reset or long enough time has passed
                    if (nTimer == 1 || (nHealthRefillTimer == 20 && bLifeRecover))
                    {
                        SetHealth(hProcess, dwBaseAddress, nCustomHealth);
                        SetSionBullets(hProcess, dwBaseAddress, nSionBullets);
                        SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                        nHealthRefillTimer = 0;
                    }

                    // refill character specifics
                    if (nTimer == 1 || nCharSpecificsRefillTimer == 20)
                    {
                        SetSionBullets(hProcess, dwBaseAddress, nSionBullets);
                        SetRoaCharge(hProcess, dwBaseAddress, nRoaVisibleCharge, nRoaHiddenCharge);
                        nCharSpecificsRefillTimer = 0;
                    }

                    // might need this in other places
                    /*if (GetKeyState(VK_RETURN) < 0)
                    {
                        system("cls");
                    }*/

                    // convoluted reversal pattern logic
                    ReadProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2Y + dwP2Offset), &nReadResult, 4, 0);
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
                    {
                        //std::cout << "bDelayingReversal = false;1377" << std::endl;
                        bDelayingReversal = false;
                    }

                    if (bDelayingReversal)
                    {
                        if (nTempReversalDelayFrames == 0)
                        {
                            //std::cout << "bDelayingReversal = false;1385" << std::endl;
                            bDelayingReversal = false;
                            nWriteBuffer = GetPattern(nP2CharacterID, vPatternNames[nReversalIndex]);
                            bReversaled = true;
                            if (!bRandomReversal || nTimer % 2 == 0)
                                WriteProcessMemory(hProcess, (LPVOID)(dwBaseAddress + dwP2PatternSet), &nWriteBuffer, 4, 0);
                            else
                                bReversaled = false;
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

    CloseHandle(hProcess);
    return 0;
}
