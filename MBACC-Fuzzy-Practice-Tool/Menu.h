#pragma once
#include <Windows.h>
#include <iostream>
#include "Constants.h"
#include "CharacterData.h"

class Menu
{
public:
	Menu();
	~Menu();
	void UpdateProcessInfo(HANDLE hProcess, DWORD dwBaseAddress);
	void DrawMenu();
	
private:
	int GetMainCursor();
	int GetBattleSettingsCursor();
	int GetEnemySettingsCursor();
	int GetSubMenu();
	void DrawMainMenu();
	void DrawBattleSettingsMenu();
	void DrawEnemySettingsMenu();
	void ClearScreen();
};