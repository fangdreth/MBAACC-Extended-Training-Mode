#include "Menu.h"

#ifndef MENU_H
#define MENU_H

enum eSubMenu { MAINMENU = 2, BATTLEMENU = 6, ENEMYMENU = 7, DUMMYMENU = 9, DISPLAYMENU = 8, COMMANDLISTMENU = 13, VIEWSCREENMENU = 12 };

HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;

const DWORD dwMenuBasePointer = 0x34D7FC;

HANDLE m_hProcess;
DWORD m_dwBaseAddress;
DWORD m_dwMainCursorAddress;
DWORD m_dwBattleSettingsCursorAddress;
DWORD m_dwEnemySettingsCursorAddress;
DWORD m_dwSubMenuAddress;

int m_nCurrentSubMenu = 0;
int m_nOldMainCursor = 999;
int m_nOldBattleSettingsCursor = 999;
int m_nOldEnemySettingsCursor = 999;

std::vector<std::string> vPatternNames =  GetEmptyPatternList();

Menu::Menu()
{
    GetConsoleCursorInfo(m_hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(m_hConsole, &cursorInfo);
}
Menu::~Menu()
{

}

void Menu::UpdateProcessInfo(HANDLE hProcess, DWORD dwBaseAddress)
{
    m_hProcess = hProcess;
    m_dwBaseAddress = dwBaseAddress;

    DWORD dwTempAddress = dwMenuBasePointer;
    int nReadResult;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x10;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    m_dwMainCursorAddress = nReadResult + 0x40;

    dwTempAddress = dwMenuBasePointer;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0xC8;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x10;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    m_dwBattleSettingsCursorAddress = nReadResult + 0x40;

    dwTempAddress = dwMenuBasePointer;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0xCC;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x10;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    dwTempAddress = nReadResult + 0x0;
    ReadProcessMemory(m_hProcess, (LPVOID)(dwTempAddress), &nReadResult, 4, 0);
    m_dwEnemySettingsCursorAddress = nReadResult + 0x40;

    dwTempAddress = dwMenuBasePointer;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBaseAddress + dwTempAddress), &nReadResult, 4, 0);
    m_dwSubMenuAddress = nReadResult + 0x84;

    //m_nCurrentSubMenu = GetSubMenu();
}

void Menu::DrawMenu()
{
    SetConsoleCursorPosition(m_hConsole, { 0, 0 });
    std::cout << "Fang's Extended Training Mode v0.1" << std::endl;
    std::cout << "https://github.com/fangdreth/MBAACC-Extended-Training-Mode";

    int nCursor = 0;
    int nSubMenu = GetSubMenu();

    if (nSubMenu != m_nCurrentSubMenu)
        ClearScreen();
    m_nCurrentSubMenu = nSubMenu;

    switch (nSubMenu)
    {
    case eSubMenu::MAINMENU:
        nCursor = GetMainCursor();
        /*if (nCursor - m_nOldMainCursor == 2)
        {
            m_nOldMainCursor = nCursor - 1;
            WriteProcessMemory(m_hProcess, (LPVOID)(m_dwMainCursorAddress), &m_nOldMainCursor, 4, 0);
        }
        else if (nCursor - m_nOldMainCursor == -2)
        {
            m_nOldMainCursor = nCursor + 1;
            WriteProcessMemory(m_hProcess, (LPVOID)(m_dwMainCursorAddress), &m_nOldMainCursor, 4, 0);
        }
        else*/
        m_nOldMainCursor = nCursor;
        DrawMainMenu();
        break;
    case eSubMenu::BATTLEMENU:
        DrawBattleSettingsMenu();
        break;
    case eSubMenu::ENEMYMENU:
        DrawEnemySettingsMenu();
        break;
    case eSubMenu::VIEWSCREENMENU:

        break;

    case eSubMenu::DUMMYMENU:
    case eSubMenu::COMMANDLISTMENU:
    case eSubMenu::DISPLAYMENU:
        
        break;
    default:
        break;
    }
}

int Menu::GetMainCursor()
{
    int nReadResult;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwMainCursorAddress), &nReadResult, 4, 0);
    return nReadResult;
}

int Menu::GetBattleSettingsCursor()
{
    int nReadResult;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBattleSettingsCursorAddress), &nReadResult, 4, 0);
    return nReadResult;
}

int Menu::GetEnemySettingsCursor()
{
    int nReadResult;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwEnemySettingsCursorAddress), &nReadResult, 4, 0);
    return nReadResult;
}

int Menu::GetSubMenu()
{
    int nReadResult;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwSubMenuAddress), &nReadResult, 4, 0);
    return nReadResult;
}

void Menu::ClearScreen()
{
    for (short i = 4; i < 30; i++)
    {
        SetConsoleCursorPosition(m_hConsole, { 0, i });
        std::cout << "                                                                                                                ";
    }
}

void Menu::DrawBattleSettingsMenu()
{
    int nCursor = GetBattleSettingsCursor();

    SetConsoleCursorPosition(m_hConsole, { 0, 4 });
    std::cout << (nCursor == 0 ? "=>  " : "    ") << "Life";
    SetConsoleCursorPosition(m_hConsole, { 0, 5 });
    std::cout << (nCursor == 1 ? "=>  " : "    ") << "Magic Circuit";

    SetConsoleCursorPosition(m_hConsole, { 0, 7 });
    std::cout << (nCursor == 3 ? "=>  " : "    ") << "Guard Guage";
    SetConsoleCursorPosition(m_hConsole, { 0, 8 });
    std::cout << (nCursor == 4 ? "=>  " : "    ") << "Guard Quality";

    SetConsoleCursorPosition(m_hConsole, { 0, 10 });
    std::cout << (nCursor == 6 ? "=>  " : "    ") << "Counter Hit";
    SetConsoleCursorPosition(m_hConsole, { 0, 11 });
    std::cout << (nCursor == 7 ? "=>  " : "    ") << "Critical Hit";

    SetConsoleCursorPosition(m_hConsole, { 0, 13 });
    std::cout << (nCursor == 9 ? "=>  " : "    ") << "Default";
    SetConsoleCursorPosition(m_hConsole, { 0, 14 });
    std::cout << (nCursor == 11 ? "=>  " : "    ") << "Return";
}

void Menu::DrawEnemySettingsMenu()
{
    int nCursor = GetEnemySettingsCursor();

    SetConsoleCursorPosition(m_hConsole, { 0, 4 });
    std::cout << (nCursor == 0 ? "=>  " : "    ") << "Enemy Action\t\t\t" << "TODO";

    SetConsoleCursorPosition(m_hConsole, { 0, 6 });
    std::cout << (nCursor == 2 ? "=>  " : "    ") << "Enemy Defense";
    SetConsoleCursorPosition(m_hConsole, { 0, 7 });
    std::cout << (nCursor == 3 ? "=>  " : "    ") << "Enemy Defense Type";

    SetConsoleCursorPosition(m_hConsole, { 0, 9 });
    std::cout << (nCursor == 5 ? "=>  " : "    ") << "Air Recovery";
    SetConsoleCursorPosition(m_hConsole, { 0, 10 });
    std::cout << (nCursor == 6 ? "=>  " : "    ") << "Down Recovery";

    SetConsoleCursorPosition(m_hConsole, { 0, 12 });
    std::cout << (nCursor == 8 ? "=>  " : "    ") << "Throw Recovery";

    SetConsoleCursorPosition(m_hConsole, { 0, 14 });
    std::cout << (nCursor == 10 ? "=>  " : "    ") << "Reduce Damage";

    SetConsoleCursorPosition(m_hConsole, { 0, 16 });
    std::cout << (nCursor == 12 ? "=>  " : "    ") << "Default";
    SetConsoleCursorPosition(m_hConsole, { 0, 17 });
    std::cout << (nCursor == 14 ? "=>  " : "    ") << "Return";
}

void Menu::DrawMainMenu()
{
    int nEnemyStatusSetting;
    ReadProcessMemory(m_hProcess, (LPVOID)(m_dwBaseAddress + dwEnemyStatus), &nEnemyStatusSetting, 4, 0);
    int nPresetSetting = 0;

    SetConsoleCursorPosition(m_hConsole, { 0, 4 });
    std::cout << (m_nOldMainCursor == 0 ? "=>  " : "    ") << "Battle Settings >";

    SetConsoleCursorPosition(m_hConsole, { 0, 6 });
    std::cout << (m_nOldMainCursor == 1 ? "=>  " : "    ") << "Preset:\t\t\t\t<- " << vPresetSettings[nPresetSetting] << " ->                                        ";

    SetConsoleCursorPosition(m_hConsole, { 0, 8 });
    std::cout << (m_nOldMainCursor == 2 ? "=>  " : "    ") << "Enemy Status:\t\t\t<- " << vEnemyStatusSettings[nEnemyStatusSetting] << " ->              ";
    SetConsoleCursorPosition(m_hConsole, { 0, 9 });
    std::cout << (m_nOldMainCursor == 3 ? "=>  " : "    ") << "Enemy Settings >";

    SetConsoleCursorPosition(m_hConsole, { 0, 11 });
    std::cout << (m_nOldMainCursor == 5 ? "=>  " : "    ") << "---";
    SetConsoleCursorPosition(m_hConsole, { 0, 12 });
    std::cout << (m_nOldMainCursor == 6 ? "=>  " : "    ") << "---";
    SetConsoleCursorPosition(m_hConsole, { 0, 13 });
    std::cout << (m_nOldMainCursor == 7 ? "=>  " : "    ") << "---";

    SetConsoleCursorPosition(m_hConsole, { 0, 15 });
    std::cout << (m_nOldMainCursor == 9 ? "=>  " : "    ") << "Training Display >";

    SetConsoleCursorPosition(m_hConsole, { 0, 17 });
    std::cout << (m_nOldMainCursor == 11 ? "=>  " : "    ") << "Command List";

    SetConsoleCursorPosition(m_hConsole, { 0, 19 });
    std::cout << (m_nOldMainCursor == 13 ? "=>  " : "    ") << "Fuzzy Settings >";

    SetConsoleCursorPosition(m_hConsole, { 0, 21 });
    std::cout << (m_nOldMainCursor == 15 ? "=>  " : "    ") << "Character Select";
    SetConsoleCursorPosition(m_hConsole, { 0, 22 });
    std::cout << (m_nOldMainCursor == 16 ? "=>  " : "    ") << "this button don't work";

    SetConsoleCursorPosition(m_hConsole, { 0, 24 });
    std::cout << (m_nOldMainCursor == 18 ? "=>  " : "    ") << "Restart";
    SetConsoleCursorPosition(m_hConsole, { 0, 25 });
    std::cout << (m_nOldMainCursor == 19 ? "=>  " : "    ") << "Continue";

    /*SetConsoleCursorPosition(m_hConsole, {0, 7});
    std::cout << (m_nOldMainCursor == 1 ? "=>  " : "    ") << "Switch to crouching after blocking:\t<- " << (bSwitchToCrouch ? "On " : "Off") << " ->      ";

    SetConsoleCursorPosition(m_hConsole, { 0, 8 });
    std::cout << (m_nOldMainCursor == 2 ? "=>  " : "    ") << "Delay:\t\t\t\t<- " << nSwitchBlockDelayFrames << " ->              ";

    SetConsoleCursorPosition(m_hConsole, { 0, 11 });
    std::cout << (m_nOldMainCursor == 4 ? "=>  " : "    ") << "Enemy Defense:\t\t\t<- " << vEnemyDefenseSettings[nEnemyDefenseSetting] << " ->              ";

    SetConsoleCursorPosition(m_hConsole, { 0, 13 });
    //std::cout << (m_nOldMainCursor == 5 ? "=>  " : "    ") << "Random Bias:\t\t\t<- " << vEnemyDefenseBiasSettings[nEnemyDefenseBiasSetting] << " ->              ";
    std::cout << (m_nOldMainCursor == 5 ? "=>  " : "    ") << "howdy pardner";

    SetConsoleCursorPosition(m_hConsole, { 0, 15 });
    std::cout << (m_nOldMainCursor == 6 ? "=>  " : "    ") << "Guard Bar:\t\t\t\t<- " << vEnemyGuardLevelSettings[nEnemyGuardLevelSetting] << " ->      ";

    SetConsoleCursorPosition(m_hConsole, { 0, 16 });
    std::cout << (m_nOldMainCursor == 7 ? "=>  " : "    ") << "Ex Guard:\t\t\t\t<- " << (bExGuard ? "On " : "Off") << " ->      ";

    SetConsoleCursorPosition(m_hConsole, { 0, 18 });
    std::cout << (m_nOldMainCursor == 8 ? "=>  " : "    ") << "Reversal:\t\t\t\t" << vPatternNames[nReversalIndex] << "                                    ";

    SetConsoleCursorPosition(m_hConsole, { 0, 19 });
    std::cout << (m_nOldMainCursor == 9 ? "=>  " : "    ") << "Delay:\t\t\t\t<- " << nReversalDelayFrames << " ->              ";*/
}

#endif