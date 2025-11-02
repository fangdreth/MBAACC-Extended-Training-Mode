#include <cstddef>
#include "..\Common\Common.h"
#include "TrainingMenu.h"

//wrapper for call to ReadDataFile (misleading name? pulled straight from ghidra)
//subtract 4 bytes from actual destination to get dest input
void ReadDataFile(void* dest, const char* name, int nameLength) {
	//dest should be ecx
	//name should be stack[4]
	//nameLength should be stack[8]
	__asm {
		mov ecx, dest;
		push nameLength;
		push name;
		call[MBAA_ReadDataFile];
	}
}

//wrapper for call to CopyStringFromIndex
//subtract 4 bytes from actual destination to get dest input
void CopyStringFromIndex(void* dest, void* source, int startIndex, int copyLength) {
	//dest should be ecx
	//source should be stack[4]
	//startIndex should be stack[8]
	//copyLength should be stack[0xc]
	__asm {
		mov ecx, dest;
		push copyLength;
		push startIndex;
		push source;
		call[MBAA_StringCopyFromIndex];
	}
}

void SetInfoWindowText(const char* text, void* dest) {
	//text should be ecx
	//dest should be stack[4]
	__asm {
		mov ecx, text;
		push dest;
		call[MBAA_SetInfoWindowText];
	}
}

bool CompareSSOString(const char* compareTo, MenuString* str) {
	//compareTo should be ecx
	//str should be edi
	__asm {
		mov ecx, compareTo;
		mov edi, str;
		call[MBAA_CompareSSOString];
	}
}

int GetHotkeyPressed()
{
	//http://www.kbdedit.com/manual/low_level_vk_list.html

	// there're some weird collisions between keys
	// like numpad-/ and the arrow keys,
	// but all of the normal keys work as expected.
	// I'm fine with it, personally.
	for (int i = 0; i < 256; i++) {
		// key pressed AND key isn't garbage
		if (GetAsyncKeyState(i) & 0x8000 && MapVirtualKeyW(i, MAPVK_VK_TO_VSC) != 0)
		{
			return i;
		}
	}

	return -1;
}

void CheckNewHotkey(bool bAPress, KeyState& oHotkey, LPCTSTR sRegKey) {
	if (bAPress && nHOTKEY_CD_TIMER == 0) {
		SetRegistryValue(sRegKey, 0);
		nHOTKEY_CD_TIMER = 10;
		if (oHotkey.getKey() == 0) {
			oHotkey.setKey(-1); // using -1 to indicate awaiting input
		}
		else if (oHotkey.getKey() > 0) {
			oHotkey.setKey(0);
		}
	}
	else if (oHotkey.getKey() == -1 && nHOTKEY_CD_TIMER == 0) {
		int temp;
		if (temp = GetHotkeyPressed()) {
			oHotkey.setKey(temp);
			SetRegistryValue(sRegKey, temp);
			nHOTKEY_CD_TIMER = 10;
		}
	}
}

void GetKeyStateMenuLabel(char* buffer, KeyState oHotkey) {
	if (oHotkey.getKey() > 0) {
		UINT scanCode = MapVirtualKeyA(oHotkey.getKey(), MAPVK_VK_TO_VSC);
		LONG lParamValue = (scanCode << 16);
		GetKeyNameTextA(lParamValue, buffer, 19);
	}
	else if (oHotkey.getKey() < 0) {
		snprintf(buffer, 32, "...");
	}
	else if (oHotkey.getKey() == 0) {
		snprintf(buffer, 32, "PRESS A TO SET KEY");
	}
}

void Element::SetItemLabel(const char* newLabel, int itemIndex) {
	if (itemList.listStart == 0x0 || itemList.listEnd - itemList.listStart <= itemIndex) {
		return;
	}
	Item* targetItem = itemList.listStart[itemIndex];
	ReadDataFile((void*)(targetItem), newLabel, strlen(newLabel));
}

void Element::SetCurItemLabel(const char* newLabel) {
	int itemIndex = selectedItem;
	if (itemList.listStart == 0x0 || itemList.listEnd - itemList.listStart <= itemIndex) {
		return;
	}
	Item* targetItem = itemList.listStart[itemIndex];
	ReadDataFile((void*)(targetItem), newLabel, strlen(newLabel));
}

//vftable[6]
int Element::GetItemValue(int itemIndex) {
	if (itemList.listStart == 0x0 || itemList.listEnd - itemList.listStart <= itemIndex) {
		return 0;
	}
	return itemList.listStart[itemIndex]->value;
}

//vftable[11]
int Element::GetItemListSize() {
	if (itemList.listStart == 0x0) {
		return 0;
	}
	return itemList.listEnd - itemList.listStart;
}

void MenuWindow::SetLabel(const char* newLabel) {
	ReadDataFile((void*)(&label), newLabel, strlen(newLabel));
}

// --- Vectors guide ---
//Empty vectors are blank spaces (only need the one defined)
//Vectors with one value are elements with only a label (like the vanilla "default" and "return" elements)
//Vectors with more than one value are elements with selectable options

int nPAGE = 1;

//Page 1
int nREVERSAL_TYPE = defREVERSAL_TYPE;
int nREVERSAL_SLOT_1 = defREVERSAL_SLOT_1;
int nREV_SLOT_1_WEIGHT = defREV_SLOT_1_WEIGHT;
int nREVERSAL_SLOT_2 = defREVERSAL_SLOT_2;
int nREV_SLOT_2_WEIGHT = defREV_SLOT_2_WEIGHT;
int nREVERSAL_SLOT_3 = defREVERSAL_SLOT_3;
int nREV_SLOT_3_WEIGHT = defREV_SLOT_3_WEIGHT;
int nREVERSAL_SLOT_4 = defREVERSAL_SLOT_4;
int nREV_SLOT_4_WEIGHT = defREV_SLOT_4_WEIGHT;
int nNO_REV_WEIGHT = defNO_REV_WEIGHT;
int nREVERSAL_DELAY = defREVERSAL_DELAY;

int nREV_ID_1 = defREV_ID;
int nREV_ID_2 = defREV_ID;
int nREV_ID_3 = defREV_ID;
int nREV_ID_4 = defREV_ID;

void DefaultP1(MenuInfo* menuInfo) {
	nREVERSAL_TYPE = defREVERSAL_TYPE;
	nREVERSAL_SLOT_1 = defREVERSAL_SLOT_1;
	nREV_SLOT_1_WEIGHT = defREV_SLOT_1_WEIGHT;
	nREVERSAL_SLOT_2 = defREVERSAL_SLOT_2;
	nREV_SLOT_2_WEIGHT = defREV_SLOT_2_WEIGHT;
	nREVERSAL_SLOT_3 = defREVERSAL_SLOT_3;
	nREV_SLOT_3_WEIGHT = defREV_SLOT_3_WEIGHT;
	nREVERSAL_SLOT_4 = defREVERSAL_SLOT_4;
	nREV_SLOT_4_WEIGHT = defREV_SLOT_4_WEIGHT;
	nNO_REV_WEIGHT = defNO_REV_WEIGHT;
	nREVERSAL_DELAY = defREVERSAL_DELAY;

	nREV_ID_1 = defREV_ID;
	nREV_ID_2 = defREV_ID;
	nREV_ID_3 = defREV_ID;
	nREV_ID_4 = defREV_ID;

	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_TYPE]->selectedItem = defREVERSAL_TYPE;
	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_1]->selectedItem = defREVERSAL_SLOT_1;
	(menuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_1]->selectedItem = defREV_SLOT_1_WEIGHT;
	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_2]->selectedItem = defREVERSAL_SLOT_2;
	(menuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_2]->selectedItem = defREV_SLOT_2_WEIGHT;
	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_3]->selectedItem = defREVERSAL_SLOT_3;
	(menuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_3]->selectedItem = defREV_SLOT_3_WEIGHT;
	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_4]->selectedItem = defREVERSAL_SLOT_4;
	(menuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_4]->selectedItem = defREV_SLOT_4_WEIGHT;
	(menuInfo->elementList).listStart[(int)eREVERSALS::NO_REVERSAL_WEIGHT]->selectedItem = defNO_REV_WEIGHT;
	(menuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_DELAY]->selectedItem = defREVERSAL_DELAY;
}

//Page 2
int nPENALTY_RESET = defPEN_RESET;
int nGUARD_BAR_RESET = defGUARD_RESET;
int nEX_GUARD = defEX_GUARD;
int nP1_METER = defP1_METER;
int nP2_METER = defP2_METER;
int nP1_HEALTH = defP1_HEALTH;
int nP2_HEALTH = defP2_HEALTH;
int nHITS_UNTIL_BURST = defHITS_BURST;
int nHITS_UNTIL_BUNKER = defHITS_BUNKER;
int nHITS_UNTIL_FORCE_GUARD = defHITS_FORCE_GUARD;
int nFORCE_GUARD_STANCE = defFORCE_GUARD_STANCE;

int nTRUE_P1_METER = defTRUE_P1_METER;
int nTRUE_P2_METER = defTRUE_P2_METER;
int nTRUE_P1_HEALTH = defTRUE_P1_HEALTH;
int nTRUE_P2_HEALTH = defTRUE_P2_HEALTH;
int nTRUE_HITS_UNTIL_BURST = defTRUE_HITS_BURST;
int nTRUE_HITS_UNTIL_BUNKER = defTRUE_HITS_BUNKER;
int nTRUE_HITS_UNTIL_FORCE_GUARD = defTRUE_HITS_FORCE_GUARD;

void DefaultP2(MenuInfo* menuInfo) {
	nPENALTY_RESET = defPEN_RESET;
	nGUARD_BAR_RESET = defGUARD_RESET;
	nEX_GUARD = defEX_GUARD;
	nP1_METER = defP1_METER;
	nP2_METER = defP2_METER;
	nP1_HEALTH = defP1_HEALTH;
	nP2_HEALTH = defP2_HEALTH;
	nHITS_UNTIL_BURST = defHITS_BURST;
	nHITS_UNTIL_BUNKER = defHITS_BUNKER;
	nHITS_UNTIL_FORCE_GUARD = defHITS_FORCE_GUARD;
	nFORCE_GUARD_STANCE = defFORCE_GUARD_STANCE;

	nTRUE_P1_METER = defTRUE_P1_METER;
	nTRUE_P2_METER = defTRUE_P2_METER;
	nTRUE_P1_HEALTH = defTRUE_P1_HEALTH;
	nTRUE_P2_HEALTH = defTRUE_P2_HEALTH;
	nTRUE_HITS_UNTIL_BURST = defTRUE_HITS_BURST;
	nTRUE_HITS_UNTIL_BUNKER = defTRUE_HITS_BUNKER;
	nTRUE_HITS_UNTIL_FORCE_GUARD = defTRUE_HITS_FORCE_GUARD;

	(menuInfo->elementList).listStart[(int)eTRAINING::PENALTY_RESET]->selectedItem = defPEN_RESET;
	(menuInfo->elementList).listStart[(int)eTRAINING::GUARD_BAR_RESET]->selectedItem = defGUARD_RESET;
	(menuInfo->elementList).listStart[(int)eTRAINING::EX_GUARD]->selectedItem = defEX_GUARD;
	(menuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->selectedItem = defP1_METER;
	(menuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->selectedItem = defP2_METER;
	(menuInfo->elementList).listStart[(int)eTRAINING::P1_HEALTH]->selectedItem = defP1_HEALTH;
	(menuInfo->elementList).listStart[(int)eTRAINING::P2_HEALTH]->selectedItem = defP2_HEALTH;
	(menuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_BURST]->selectedItem = defHITS_BURST;
	(menuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_BUNKER]->selectedItem = defHITS_BUNKER;
	(menuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_FORCE_GUARD]->selectedItem = defHITS_FORCE_GUARD;
	(menuInfo->elementList).listStart[(int)eTRAINING::FORCE_GUARD_STANCE]->selectedItem = defFORCE_GUARD_STANCE;
}

//Page 3
int nHIGHLIGHTS = defHIGHLIGHTS;
int nGUARD_HIGHLIGHT = defGUARD_HIGHLIGHT;
int nHIT_HIGHLIGHT = defHIT_HIGHLIGHT;
int nARMOR_HIGHLIGHT = defARMOR_HIGHLIGHT;
int nTHROW_PROTECTION_HIGHLIGHT = defTHROW_PROT_HIGHLIGHT;
int nIDLE_HIGHLIGHT = defIDLE_HIGHLIGHT;

void DefaultP3(MenuInfo* menuInfo) {
	nHIGHLIGHTS = defHIGHLIGHTS;
	nGUARD_HIGHLIGHT = defGUARD_HIGHLIGHT;
	nHIT_HIGHLIGHT = defHIT_HIGHLIGHT;
	nARMOR_HIGHLIGHT = defARMOR_HIGHLIGHT;
	nTHROW_PROTECTION_HIGHLIGHT = defTHROW_PROT_HIGHLIGHT;
	nIDLE_HIGHLIGHT = defIDLE_HIGHLIGHT;

	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::HIGHLIGHTS]->selectedItem = defHIGHLIGHTS;
	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::GUARD]->selectedItem = defGUARD_HIGHLIGHT;
	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::HIT]->selectedItem = defHIT_HIGHLIGHT;
	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::ARMOR]->selectedItem = defARMOR_HIGHLIGHT;
	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::THROW_PROTECTION]->selectedItem = defTHROW_PROT_HIGHLIGHT;
	(menuInfo->elementList).listStart[(int)eHIGHLIGHTS::IDLE]->selectedItem = defIDLE_HIGHLIGHT;
}

//Page 4
int nRESET_TO_POSITIONS = defRESET_POS;
int nP1_X_LOC = defP1_X;
int nP1_ASSIST_X_LOC = defP1_ASSIST_X;
int nP2_X_LOC = defP2_X;
int nP2_ASSIST_X_LOC = defP2_ASSIST_X;

int nTRUE_P1_X_LOC = defTRUE_P1_X;
int nTRUE_P1_ASSIST_X_LOC = defTRUE_P1_ASSIST_X;
int nTRUE_P2_X_LOC = defTRUE_P2_X;
int nTRUE_P2_ASSIST_X_LOC = defTRUE_P2_ASSIST_X;

void DefaultP4(MenuInfo* menuInfo) {
	nRESET_TO_POSITIONS = defRESET_POS;
	nP1_X_LOC = defP1_X;
	nP1_ASSIST_X_LOC = defP1_ASSIST_X;
	nP2_X_LOC = defP2_X;
	nP2_ASSIST_X_LOC = defP2_ASSIST_X;

	nTRUE_P1_X_LOC = defTRUE_P1_X;
	nTRUE_P1_ASSIST_X_LOC = defTRUE_P1_ASSIST_X;
	nTRUE_P2_X_LOC = defTRUE_P2_X;
	nTRUE_P2_ASSIST_X_LOC = defTRUE_P2_ASSIST_X;

	(menuInfo->elementList).listStart[(int)ePOSITIONS::RESET_TO_POSITIONS]->selectedItem = defRESET_POS;
	(menuInfo->elementList).listStart[(int)ePOSITIONS::P1_POSITION]->selectedItem = defP1_X;
	(menuInfo->elementList).listStart[(int)ePOSITIONS::P1_ASSIST_POSITION]->selectedItem = defP1_ASSIST_X;
	(menuInfo->elementList).listStart[(int)ePOSITIONS::P2_POSITION]->selectedItem = defP2_X;
	(menuInfo->elementList).listStart[(int)ePOSITIONS::P2_ASSIST_POSITION]->selectedItem = defP2_ASSIST_X;
}

//Page 5
int nSION_BULLETS = defSION_BULLETS;
int nROA_VISIBLE_CHARGE = defROA_VISIBLE;
int nROA_HIDDEN_CHARGE = defROA_HIDDEN;
int nF_MAIDS_HEARTS = defMAIDS_HEARTS;
int nRYOUGI_KNIFE = defRYOUGI_KNIFE;

void DefaultP5(MenuInfo* menuInfo) {
	nSION_BULLETS = defSION_BULLETS;
	nROA_VISIBLE_CHARGE = defROA_VISIBLE;
	nROA_HIDDEN_CHARGE = defROA_HIDDEN;
	nF_MAIDS_HEARTS = defMAIDS_HEARTS;
	nRYOUGI_KNIFE = defRYOUGI_KNIFE;

	(menuInfo->elementList).listStart[(int)eCHARACTER::SION_BULLETS]->selectedItem = defSION_BULLETS;
	(menuInfo->elementList).listStart[(int)eCHARACTER::ROA_VISIBLE_CHARGES]->selectedItem = defROA_VISIBLE;
	(menuInfo->elementList).listStart[(int)eCHARACTER::ROA_HIDDEN_CHARGES]->selectedItem = defROA_HIDDEN;
	(menuInfo->elementList).listStart[(int)eCHARACTER::F_MAIDS_HEARTS]->selectedItem = defMAIDS_HEARTS;
	(menuInfo->elementList).listStart[(int)eCHARACTER::RYOUGI_KNIFE]->selectedItem = defRYOUGI_KNIFE;
}

//Page 6
int nDISPLAY_HITBOXES = defDISPLAY_HITBOXES;
int nHITBOX_STYLE = defHITBOX_STYLE;
int nCOLOR_BLIND_MODE = defCOLOR_BLIND;
int nORIGIN_STYLE = defORIGIN_STYLE;
int nDRAW_GROUND = defDRAW_GROUND;

void DefaultP6(MenuInfo* menuInfo) {
	nDISPLAY_HITBOXES = defDISPLAY_HITBOXES;
	nHITBOX_STYLE = defHITBOX_STYLE;
	nCOLOR_BLIND_MODE = defCOLOR_BLIND;
	nORIGIN_STYLE = defORIGIN_STYLE;
	nDRAW_GROUND = defDRAW_GROUND;

	(menuInfo->elementList).listStart[(int)eHITBOXES::DISPLAY_HITBOXES]->selectedItem = defDISPLAY_HITBOXES;
	(menuInfo->elementList).listStart[(int)eHITBOXES::HITBOX_STYLE]->selectedItem = defHITBOX_STYLE;
	(menuInfo->elementList).listStart[(int)eHITBOXES::COLOR_BLIND_MODE]->selectedItem = defCOLOR_BLIND;
	(menuInfo->elementList).listStart[(int)eHITBOXES::ORIGIN_STYLE]->selectedItem = defORIGIN_STYLE;
	(menuInfo->elementList).listStart[(int)eHITBOXES::DRAW_GROUND]->selectedItem = defDRAW_GROUND;
}

//Page 7
int nSAVE_STATE_SLOT = defSAVE_SLOT;
int nLOAD_RNG = defLOAD_RNG;

void DefaultP7(MenuInfo* menuInfo) {
	nSAVE_STATE_SLOT = defSAVE_SLOT;
	nLOAD_RNG = defLOAD_RNG;

	(menuInfo->elementList).listStart[(int)eSAVE_STATES::SAVE_STATE_SLOT]->selectedItem = defSAVE_SLOT;
	(menuInfo->elementList).listStart[(int)eSAVE_STATES::LOAD_RNG]->selectedItem = defLOAD_RNG;
}

//Page 8
int nFRAME_DATA = defFRAME_DATA;
int nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
int nSHOW_HITSTOP_AND_FREEZE = defSHOW_HITSTOP_AND_FREEZE;
int nSHOW_INPUTS = defSHOW_INPUTS;
int nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
int nSCROLL_DISPLAY = defSCROLL_DISPLAY;

bool bCOLOR_GUIDE = defCOLOR_GUIDE;

int nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

void DefaultP8(MenuInfo* menuInfo) {
	nFRAME_DATA = defFRAME_DATA;
	nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
	nSHOW_HITSTOP_AND_FREEZE = defSHOW_HITSTOP_AND_FREEZE;
	nSHOW_INPUTS = defSHOW_INPUTS;
	nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
	nSCROLL_DISPLAY = defSCROLL_DISPLAY;

	bCOLOR_GUIDE = defCOLOR_GUIDE;

	nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

	(menuInfo->elementList).listStart[(int)eFRAME_DATA::FRAME_DATA]->selectedItem = defFRAME_DATA;
	(menuInfo->elementList).listStart[(int)eFRAME_DATA::IN_GAME_FRAME_DISPLAY]->selectedItem = defIN_GAME_FRAME_DISPLAY;
	(menuInfo->elementList).listStart[(int)eFRAME_DATA::SHOW_HITSTOP_AND_FREEZE]->selectedItem = defSHOW_HITSTOP_AND_FREEZE;
	(menuInfo->elementList).listStart[(int)eFRAME_DATA::SHOW_INPUTS]->selectedItem = defSHOW_INPUTS;
	(menuInfo->elementList).listStart[(int)eFRAME_DATA::SHOW_CANCEL_WINDOWS]->selectedItem = defSHOW_CANCEL;
	(menuInfo->elementList).listStart[(int)eFRAME_DATA::SCROLL_DISPLAY]->selectedItem = defSCROLL_DISPLAY;
}

//Page 9
int nCUSTOM_RNG = defCUSTOM_RNG;
int nRATE = defRATE;
int nSEED = defSEED;

int nTRUE_SEED = defTRUE_SEED;

void DefaultP9(MenuInfo* menuInfo) {
	nCUSTOM_RNG = defCUSTOM_RNG;
	nRATE = defRATE;
	nSEED = defSEED;

	nTRUE_SEED = defTRUE_SEED;

	(menuInfo->elementList).listStart[(int)eRNG::CUSTOM_RNG]->selectedItem = defCUSTOM_RNG;
	(menuInfo->elementList).listStart[(int)eRNG::RATE]->selectedItem = defRATE;
	(menuInfo->elementList).listStart[(int)eRNG::SEED]->selectedItem = defSEED;
}

//Page 10
int nSHOW_STATS = defSHOW_STATS;
int nACCURATE_COMBO_DAMAGE = defACCURATE_COMBO_DAMAGE;
int nP1_INPUT_DISPLAY = defP1_INPUT;
int nP2_INPUT_DISPLAY = defP2_INPUT;
int nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

int nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

extern float fP1_LIST_INPUT_X = defP1_LIST_INPUT_X;
extern float fP1_LIST_INPUT_Y = defP1_LIST_INPUT_Y;
extern float fP2_LIST_INPUT_X = defP2_LIST_INPUT_X;
extern float fP2_LIST_INPUT_Y = defP2_LIST_INPUT_Y;

extern float fP1_ARCADE_INPUT_X = defP1_ARCADE_INPUT_X;
extern float fP1_ARCADE_INPUT_Y = defP1_ARCADE_INPUT_Y;
extern float fP2_ARCADE_INPUT_X = defP2_ARCADE_INPUT_X;
extern float fP2_ARCADE_INPUT_Y = defP2_ARCADE_INPUT_Y;

void DefaultP10(MenuInfo* menuInfo) {
	nSHOW_STATS = defSHOW_STATS;
	nACCURATE_COMBO_DAMAGE = defACCURATE_COMBO_DAMAGE;
	nP1_INPUT_DISPLAY = defP1_INPUT;
	nP2_INPUT_DISPLAY = defP2_INPUT;
	nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

	nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

	(menuInfo->elementList).listStart[(int)eUI::SHOW_STATS]->selectedItem = defSHOW_STATS;
	(menuInfo->elementList).listStart[(int)eUI::ACCURATE_COMBO_DAMAGE]->selectedItem = defACCURATE_COMBO_DAMAGE;
	(menuInfo->elementList).listStart[(int)eUI::P1_INPUT_DISPLAY]->selectedItem = defP1_INPUT;
	(menuInfo->elementList).listStart[(int)eUI::P2_INPUT_DISPLAY]->selectedItem = defP2_INPUT;
	(menuInfo->elementList).listStart[(int)eUI::FRAME_DISPLAY_Y]->selectedItem = defFRAME_DISPLAY_Y;
}

//Page 11
int nGAME_SPEED = defGAME_SPEED;
int nHIDE_HUD = defHIDE_HUD;
int nHIDE_SHADOWS = defHIDE_SHADOWS;
int nHIDE_EXTRAS = defHIDE_EXTRAS;
int nBACKGROUND = defBACKGROUND;

void DefaultP11(MenuInfo* menuInfo) {
	nGAME_SPEED = defGAME_SPEED;
	nHIDE_HUD = defHIDE_HUD;
	nHIDE_SHADOWS = defHIDE_SHADOWS;
	nHIDE_EXTRAS = defHIDE_EXTRAS;
	nBACKGROUND = defBACKGROUND;

	(menuInfo->elementList).listStart[(int)eSYSTEM::GAME_SPEED]->selectedItem = defGAME_SPEED;
	(menuInfo->elementList).listStart[(int)eSYSTEM::HIDE_HUD]->selectedItem = defHIDE_HUD;
	(menuInfo->elementList).listStart[(int)eSYSTEM::HIDE_SHADOWS]->selectedItem = defHIDE_SHADOWS;
	(menuInfo->elementList).listStart[(int)eSYSTEM::HIDE_EXTRAS]->selectedItem = defHIDE_EXTRAS;
	(menuInfo->elementList).listStart[(int)eSYSTEM::BACKGROUND]->selectedItem = defBACKGROUND;
}

//All pages
uint8_t nEXTENDED_SETTINGS_PAGE = 0;
uint8_t nEXTENDED_SETTINGS_CURSOR[XS_NUM_PAGES + 1] = { 0 };

bool bOldFN1Input = 0;
bool bOldFN2Input = 0;

//Hotkey settings

//Page 1
int nHOTKEYS = 0; //The hotkey menu elements dont need to save their index, but they still need a location to save to

KeyState oFreezeHotkey(0);
KeyState oNextFrameHotkey(0);
KeyState oPrevFrameHotkey(0);
KeyState oToggleHitboxesHotkey(0);
KeyState oToggleFrameBarHotkey(0);
KeyState oToggleHighlightsHotkey(0);
KeyState oQueueReversalHotkey(0);
KeyState oIncrementRNGHotkey(0);
KeyState oDecrementRNGHotkey(0);

//Page 2

KeyState oSaveStateHotkey(0);
KeyState oPrevSaveSlotHotkey(0);
KeyState oNextSaveSlotHotkey(0);
KeyState oFrameBarLeftHotkey(0);
KeyState oFrameBarRightHotkey(0);

//All pages
uint8_t nHOTKEY_SETTINGS_PAGE = 0;
uint8_t nHOTKEY_SETTINGS_CURSOR[HK_NUM_PAGES + 1] = { 0 };

int nHOTKEY_CD_TIMER = 0;

//Information
const std::map<std::string, const char*> MAIN_INFORMATION_MAP = {
	{"EXTENDED_SETTING", "Set \\@COLOR@<015,183,255,255>extended details."},
	{"HOTKEY_SETTING", "Set \\@COLOR@<015,183,255,255>hotkeys."},

	//REVERSALS
	{"XS_0_0", "Set \\@COLOR@<015, 183, 255, 255>dummy reversal mode."},
	{"XS_0_2", "Set \\@COLOR@<015, 183, 255, 255>reversal slot 1."},
	{"XS_0_3", "Set weight of \\@COLOR@<015, 183, 255, 255>reversal slot 1."},
	{"XS_0_4", "Set \\@COLOR@<015, 183, 255, 255>reversal slot 2."},
	{"XS_0_5", "Set weight of \\@COLOR@<015, 183, 255, 255>reversal slot 2."},
	{"XS_0_6", "Set \\@COLOR@<015, 183, 255, 255>reversal slot 3."},
	{"XS_0_7", "Set weight of \\@COLOR@<015, 183, 255, 255>reversal slot 3."},
	{"XS_0_8", "Set \\@COLOR@<015, 183, 255, 255>reversal slot 4."},
	{"XS_0_9", "Set weight of \\@COLOR@<015, 183, 255, 255>reversal slot 4."},
	{"XS_0_11", "Set weight of \\@COLOR@<015, 183, 255, 255>no reversal."},
	{"XS_0_13", "Set \\@COLOR@<015, 183, 255, 255>delay before a reversal is performed."},
	{"XS_0_15_n", DEFAULT_INFO},
	{"XS_0_17_n", RETURN_INFO},
	{"XS_0_19", PAGE_INFO},

	//TRAINING
	{"XS_1_0", "Set the \\@COLOR@<015, 183, 255, 255>speed at which Reverse Penalty resets."},
	{"XS_1_1", "Set the \\@COLOR@<015, 183, 255, 255>speed at which the Guard Bar resets."},
	{"XS_1_2", "Set dummy \\@COLOR@<015, 183, 255, 255>ex guard."},
	{"XS_1_4", "Set \\@COLOR@<015, 183, 255, 255>P1 meter on reset."},
	{"XS_1_5", "Set \\@COLOR@<015, 183, 255, 255>P2 meter on reset."},
	{"XS_1_7", "Set \\@COLOR@<015, 183, 255, 255>P1 health on reset."},
	{"XS_1_8", "Set \\@COLOR@<015, 183, 255, 255>P2 health on reset."},
	{"XS_1_10", "Set \\@COLOR@<015, 183, 255, 255>hits until the dummy will burst."},
	{"XS_1_11", "Set \\@COLOR@<015, 183, 255, 255>hits until the dummy will bunker."},
	{"XS_1_12", "Set \\@COLOR@<015, 183, 255, 255>hits until the dummy will guard with forced stance."},
	{"XS_1_13", "Set \\@COLOR@<015, 183, 255, 255>stance to force the dummy to guard with."},
	{"XS_1_15_n", DEFAULT_INFO},
	{"XS_1_17_n", RETURN_INFO},
	{"XS_1_19", PAGE_INFO},

	//HIGHLIGHTS
	{"XS_2_0", "Set \\@COLOR@<015, 183, 255, 255>highlights display."},
	{"XS_2_2", "Set color to highlight \\@COLOR@<015, 183, 255, 255>Guard state."},
	{"XS_2_3", "Set color to highlight \\@COLOR@<015, 183, 255, 255>Hit state."},
	{"XS_2_4", "Set color to highlight \\@COLOR@<015, 183, 255, 255>Armor state."},
	{"XS_2_5", "Set color to highlight \\@COLOR@<015, 183, 255, 255>Throw Protection state."},
	{"XS_2_6", "Set color to highlight \\@COLOR@<015, 183, 255, 255>Idle state."},
	{"XS_2_8_n", DEFAULT_INFO},
	{"XS_2_10_n", RETURN_INFO},
	{"XS_2_12", PAGE_INFO},

	//POSITIONS
	{"XS_3_0", "Set whether to \\@COLOR@<015, 183, 255, 255>reset to these positions."},
	{"XS_3_2", "Set \\@COLOR@<015, 183, 255, 255>P1 position on reset."},
	{"XS_3_3", "Set \\@COLOR@<015, 183, 255, 255>P1 assist position on reset."},
	{"XS_3_5", "Set \\@COLOR@<015, 183, 255, 255>P2 position on reset."},
	{"XS_3_6", "Set \\@COLOR@<015, 183, 255, 255>P2 assist position on reset."},
	{"XS_3_8_n", "Move \\@COLOR@<015, 183, 255, 255>players to the set positions."},
	{"XS_3_9_n", "Swap \\@COLOR@<015, 183, 255, 255>P1 and P2 positions."},
	{"XS_3_11_n", DEFAULT_INFO},
	{"XS_3_13_n", RETURN_INFO},
	{"XS_3_15", PAGE_INFO},

	//CHARACTER
	{"XS_4_0", "Set \\@COLOR@<015, 183, 255, 255>Sion bullets on reset."},
	{"XS_4_2", "Set \\@COLOR@<015, 183, 255, 255>C-Roa visible charges on reset."},
	{"XS_4_3", "Set \\@COLOR@<015, 183, 255, 255>C-Roa hidden charges on reset."},
	{"XS_4_5", "Set \\@COLOR@<015, 183, 255, 255>F-Maids hearts on reset."},
	{"XS_4_7", "Set \\@COLOR@<015, 183, 255, 255>Ryougi knives."},
	{"XS_4_9_n", DEFAULT_INFO},
	{"XS_4_11_n", RETURN_INFO},
	{"XS_4_13", PAGE_INFO},

	//HITBOXES
	{"XS_5_0", "Set \\@COLOR@<015, 183, 255, 255>hitbox display."},
	{"XS_5_1", "Set \\@COLOR@<015, 183, 255, 255>hitbox style."},
	{"XS_5_2", "Set \\@COLOR@<015, 183, 255, 255>color-blind mode."},
	{"XS_5_4", "Set \\@COLOR@<015, 183, 255, 255>draw style of player origins."},
	{"XS_5_6", "Set \\@COLOR@<015, 183, 255, 255>ground draw."},
	{"XS_5_8_n", DEFAULT_INFO},
	{"XS_5_10_n", RETURN_INFO},
	{"XS_5_12", PAGE_INFO},

	//SAVE STATES
	{"XS_6_0", "Set \\@COLOR@<015, 183, 255, 255>save slot."},
	{"XS_6_2_n", "Save \\@COLOR@<015, 183, 255, 255>current state."},
	{"XS_6_3_n", "Clear \\@COLOR@<015, 183, 255, 255>all saved states."},
	{"XS_6_5_n", "Import \\@COLOR@<015, 183, 255, 255>save from file."},
	{"XS_6_6_n", "Export \\@COLOR@<015, 183, 255, 255>save to file."},
	{"XS_6_8", "Set \\@COLOR@<015, 183, 255, 255>RNG loading."},
	{"XS_6_10_n", DEFAULT_INFO},
	{"XS_6_12_n", RETURN_INFO},
	{"XS_6_14", PAGE_INFO},

	//FRAME DATA
	{"XS_7_0",  "Set \\@COLOR@<015, 183, 255, 255>frame data display level (console only)."},
	{"XS_7_1", "Set \\@COLOR@<015, 183, 255, 255>in-game frame display."},
	{"XS_7_3", "Set \\@COLOR@<015, 183, 255, 255>hitstop and freeze display."},
	{"XS_7_4", "Set \\@COLOR@<015, 183, 255, 255>input display (console only)."},
	{"XS_7_5", "Set \\@COLOR@<015, 183, 255, 255>cancel window display (console only)."},
	{"XS_7_7", "Scroll \\@COLOR@<015, 183, 255, 255>frame display."},
	{"XS_7_9_n", "Show \\@COLOR@<015, 183, 255, 255>color guide."},
	{"XS_7_11_n", DEFAULT_INFO},
	{"XS_7_13_n", RETURN_INFO},
	{"XS_7_15", PAGE_INFO},

	//RNG
	{"XS_8_0", "Set \\@COLOR@<015, 183, 255, 255>RNG mode."},
	{"XS_8_2", "Set \\@COLOR@<015, 183, 255, 255>rate of RNG setting."},
	{"XS_8_3", "Set \\@COLOR@<015, 183, 255, 255>seed / value of RNG to be set."},
	{"XS_8_5_n", DEFAULT_INFO},
	{"XS_8_7_n", RETURN_INFO},
	{"XS_8_9", PAGE_INFO},

	//UI
	{"XS_9_0", "Set \\@COLOR@<015, 183, 255, 255>stats display."},
	{"XS_9_1", "Set \\@COLOR@<015, 183, 255, 255>accurate combo damage display."},
	{"XS_9_3", "Set \\@COLOR@<015, 183, 255, 255>P1 input display."},
	{"XS_9_4", "Set \\@COLOR@<015, 183, 255, 255>P2 input display."},
	{"XS_9_6", "Set \\@COLOR@<015, 183, 255, 255>Y-Position of frame display."},
	{"XS_9_7_n", DEFAULT_INFO},
	{"XS_9_10_n", RETURN_INFO},
	{"XS_9_12", PAGE_INFO},

	//SYSTEM
	{"XS_10_0", "Set \\@COLOR@<015, 183, 255, 255>game speed."},
	{"XS_10_2", "Set \\@COLOR@<015, 183, 255, 255>HUD display."},
	{"XS_10_3", "Set \\@COLOR@<015, 183, 255, 255>shadow display."},
	{"XS_10_4", "Set \\@COLOR@<015, 183, 255, 255>extras display."},
	{"XS_10_6", "Set \\@COLOR@<015, 183, 255, 255>background color."},
	{"XS_10_8_n", DEFAULT_INFO},
	{"XS_10_10_n", RETURN_INFO},
	{"XS_10_12", PAGE_INFO},

	//HOTKEYS 1
	{"HK_0_0", "Set hotkey to \\@COLOR@<015, 183, 255, 255>freeze the game."},
	{"HK_0_1", "Set hotkey to \\@COLOR@<015, 183, 255, 255>advance to the next frame."},
	{"HK_0_2", "Set hotkey to \\@COLOR@<015, 183, 255, 255>rewind to the previous frame."},
	{"HK_0_3", "Set hotkey to \\@COLOR@<015, 183, 255, 255>toggle hitbox display."},
	{"HK_0_4", "Set hotkey to \\@COLOR@<015, 183, 255, 255>toggle the in-game frame bar."},
	{"HK_0_5", "Set hotkey to \\@COLOR@<015, 183, 255, 255>toggle highlights."},
	{"HK_0_6", "Set hotkey to \\@COLOR@<015, 183, 255, 255>queue up a reversal regardless of settings."},
	{"HK_0_7", "Set hotkey to \\@COLOR@<015, 183, 255, 255>increment the RNG Seed / Value."},
	{"HK_0_8", "Set hotkey to \\@COLOR@<015, 183, 255, 255>decrement the RNG Seed / Value."},
	{"HK_0_10_n", RETURN_INFO},
	{"HK_0_12", PAGE_INFO},

	//HOTKEYS 2
	{"HK_1_0", "Set hotkey to \\@COLOR@<015, 183, 255, 255>save the current game state."},
	{"HK_1_1", "Set hotkey to \\@COLOR@<015, 183, 255, 255>select the previous save slot."},
	{"HK_1_2", "Set hotkey to \\@COLOR@<015, 183, 255, 255>select the next save slot."},
	{"HK_1_3", "Set hotkey to \\@COLOR@<015, 183, 255, 255>scroll the frame bar to the left."},
	{"HK_1_4", "Set hotkey to \\@COLOR@<015, 183, 255, 255>scroll the frame bar to the right."},
	{"HK_1_6_n", RETURN_INFO},
	{"HK_1_8", PAGE_INFO},
};

const std::map<std::string, const char*> SUB_INFORMATION_MAP = {
	{"XS_0_0_0", "No reversal."}, {"XS_0_0_1", "Reversal after recovery."}, {"XS_0_0_2", "Reversal after guarding."}, {"XS_0_0_3", "Reversal after being hit."}, {"XS_0_0_4", "Reversal after wakeup."},

	{"XS_0_2_1", "_REV"},
	{"XS_0_3_0", "0."}, {"XS_0_3_1", "1."}, {"XS_0_3_2", "2."}, {"XS_0_3_3", "3."}, {"XS_0_3_4", "4."}, {"XS_0_3_5", "5."},
	{"XS_0_3_6", "6."}, {"XS_0_3_7", "7."}, {"XS_0_3_8", "8."}, {"XS_0_3_9", "9."}, {"XS_0_3_10", "10."},
	{"XS_0_4_1", "_REV"},
	{"XS_0_5_0", "0."}, {"XS_0_5_1", "1."}, {"XS_0_5_2", "2."}, {"XS_0_5_3", "3."}, {"XS_0_5_4", "4."}, {"XS_0_5_5", "5."},
	{"XS_0_5_6", "6."}, {"XS_0_5_7", "7."}, {"XS_0_5_8", "8."}, {"XS_0_5_9", "9."}, {"XS_0_5_10", "10."},
	{"XS_0_6_1", "_REV"},
	{"XS_0_7_0", "0."}, {"XS_0_7_1", "1."}, {"XS_0_7_2", "2."}, {"XS_0_7_3", "3."}, {"XS_0_7_4", "4."}, {"XS_0_7_5", "5."},
	{"XS_0_7_6", "6."}, {"XS_0_7_7", "7."}, {"XS_0_7_8", "8."}, {"XS_0_7_9", "9."}, {"XS_0_7_10", "10."},
	{"XS_0_8_1", "_REV"},
	{"XS_0_9_0", "0."}, {"XS_0_9_1", "1."}, {"XS_0_9_2", "2."}, {"XS_0_9_3", "3."}, {"XS_0_9_4", "4."}, {"XS_0_9_5", "5."},
	{"XS_0_9_6", "6."}, {"XS_0_9_7", "7."}, {"XS_0_9_8", "8."}, {"XS_0_9_9", "9."}, {"XS_0_9_10", "10."},

	{"XS_0_11_0", "0."}, {"XS_0_11_1", "1."}, {"XS_0_11_2", "2."}, {"XS_0_11_3", "3."}, {"XS_0_11_4", "4."}, {"XS_0_11_5", "5."},
	{"XS_0_11_6", "6."}, {"XS_0_11_7", "7."}, {"XS_0_11_8", "8."}, {"XS_0_11_9", "9."}, {"XS_0_11_10", "10."},

	{"XS_0_13_0", "0 frames."}, {"XS_0_13_1", "1 frames."}, {"XS_0_13_2", "2 frames."}, {"XS_0_13_3", "3 frames."}, {"XS_0_13_4", "4 frames."}, {"XS_0_13_5", "5 frames."},
	{"XS_0_13_6", "6 frames."}, {"XS_0_13_7", "7 frames."}, {"XS_0_13_8", "8 frames."}, {"XS_0_13_9", "9 frames."}, {"XS_0_13_10", "10 frames."},
	{"XS_0_13_11", "11 frames."}, {"XS_0_13_12", "12 frames."}, {"XS_0_13_13", "13 frames."}, {"XS_0_13_14", "14 frames."}, {"XS_0_13_15", "15 frames."},
	{"XS_0_13_16", "16 frames."}, {"XS_0_13_17", "17 frames."}, {"XS_0_13_18", "18 frames."}, {"XS_0_13_19", "19 frames."}, {"XS_0_13_20", "20 frames."},

	{"XS_1_0_0", "Normal."}, {"XS_1_0_1", "Instant."},
	{"XS_1_1_0", "Normal."}, {"XS_1_1_1", "Instant."},
	{"XS_1_2_0", "No EX Guard."}, {"XS_1_2_1", "EX Guard all hits."}, {"XS_1_2_2", "EX Guard at random (50% chance)."},

	{"XS_1_4_0", "_METER1"}, {"XS_1_4_1", "_METER1"}, {"XS_1_4_2", "_METER1"},
	{"XS_1_5_0", "_METER2"}, {"XS_1_5_1", "_METER2"}, {"XS_1_5_2", "_METER2"},

	{"XS_1_7_0", "_HEALTH1"}, {"XS_1_7_1", "_HEALTH1"}, {"XS_1_7_2", "_HEALTH1"},
	{"XS_1_8_0", "_HEALTH2"}, {"XS_1_8_1", "_HEALTH2"}, {"XS_1_8_2", "_HEALTH2"},

	{"XS_1_10_0", "_BURST"}, {"XS_1_10_1", "_BURST"}, {"XS_1_10_2", "_BURST"},
	{"XS_1_11_0", "_BUNKER"}, {"XS_1_11_1", "_BUNKER"}, {"XS_1_11_2", "_BUNKER"},
	{"XS_1_12_0", "_FORCEGUARD"}, {"XS_1_12_1", "_FORCEGUARD"}, {"XS_1_12_2", "_FORCEGUARD"},
	{"XS_1_13_0", "Force standing guard."}, {"XS_1_13_1", "Force crouching guard."},

	{"XS_2_0_0", "No highlights."}, {"XS_2_0_1", "Enable highlights."},

	{"XS_2_2_0", "Do not highlight guard state."}, {"XS_2_2_1", "Highlight guard state in red."}, {"XS_2_2_2", "Highlight guard state in yellow."}, {"XS_2_2_3", "Highlight guard state in green."},
	{"XS_2_2_4", "Highlight guard state in blue."}, {"XS_2_2_5", "Highlight guard state in purple."}, {"XS_2_2_6", "Highlight guard state in black."},

	{"XS_2_3_0", "Do not highlight hit state."}, {"XS_2_3_1", "Highlight hit state in red."}, {"XS_2_3_2", "Highlight hit state in yellow."}, {"XS_2_3_3", "Highlight hit state in green."},
	{"XS_2_3_4", "Highlight hit state in blue."}, {"XS_2_3_5", "Highlight hit state in purple."}, {"XS_2_3_6", "Highlight hit state in black."},

	{"XS_2_4_0", "Do not highlight armor state."}, {"XS_2_4_1", "Highlight armor state in red."}, {"XS_2_4_2", "Highlight armor state in yellow."}, {"XS_2_4_3", "Highlight armor state in green."},
	{"XS_2_4_4", "Highlight armor state in blue."}, {"XS_2_4_5", "Highlight armor state in purple."}, {"XS_2_4_6", "Highlight armor state in black."},

	{"XS_2_5_0", "Do not highlight throw protection state."}, {"XS_2_5_1", "Highlight throw protection state in red."}, {"XS_2_5_2", "Highlight throw protection state in yellow."}, {"XS_2_5_3", "Highlight throw protection state in green."},
	{"XS_2_5_4", "Highlight throw protection state in blue."}, {"XS_2_5_5", "Highlight throw protection state in purple."}, {"XS_2_5_6", "Highlight throw protection state in black."},

	{"XS_2_6_0", "Do not highlight idle state."}, {"XS_2_6_1", "Highlight idle state in red."}, {"XS_2_6_2", "Highlight idle state in yellow."}, {"XS_2_6_3", "Highlight idle state in green."},
	{"XS_2_6_4", "Highlight idle state in blue."}, {"XS_2_6_5", "Highlight idle state in purple."}, {"XS_2_6_6", "Highlight idle state in black."},

	{"XS_3_0_0", "Do not reset to custom positions."}, {"XS_3_0_1", "Reset to custom positions."},

	{"XS_3_2_0", "_POS1"}, {"XS_3_2_1", "_POS1"}, {"XS_3_2_2", "_POS1"},
	{"XS_3_3_0", "_POS1A"}, {"XS_3_3_1", "_POS1A"}, {"XS_3_3_2", "_POS1A"},

	{"XS_3_5_0", "_POS2"}, {"XS_3_5_1", "_POS2"}, {"XS_3_5_2", "_POS2"},
	{"XS_3_6_0", "_POS2A"}, {"XS_3_6_1", "_POS2A"}, {"XS_3_6_2", "_POS2A"},

	{"XS_4_0_0", "Refill bullets on recovery."}, {"XS_4_0_1", "Reset to 13 bullets."}, {"XS_4_0_2", "Reset to 1 bullet."}, {"XS_4_0_3", "Reset to 2 bullets."}, {"XS_4_0_4", "Reset to 3 bullets."},
	{"XS_4_0_5", "Reset to 4 bullets."}, {"XS_4_0_6", "Reset to 5 bullets."}, {"XS_4_0_7", "Reset to 6 bullets."}, {"XS_4_0_8", "Reset to 7 bullets."}, {"XS_4_0_9", "Reset to 8 bullets."},
	{"XS_4_0_10", "Reset to 9 bullets."}, {"XS_4_0_11", "Reset to 10 bullets."}, {"XS_4_0_12", "Reset to 11 bullets."}, {"XS_4_0_13", "Reset to 12 bullets."},

	{"XS_4_2_0", "Refill visible charges on recovery."}, {"XS_4_2_1", "Reset to 0 visible charges."}, {"XS_4_2_2", "Reset to 1 visible charges."}, {"XS_4_2_3", "Reset to 2 visible charges."}, {"XS_4_2_4", "Reset to 3 visible charges."},
	{"XS_4_2_5", "Reset to 4 visible charges."}, {"XS_4_2_6", "Reset to 5 visible charges."}, {"XS_4_2_7", "Reset to 6 visible charges."}, {"XS_4_2_8", "Reset to 7 visible charges."}, {"XS_4_2_9", "Reset to 8 visible charges."},
	{"XS_4_2_10", "Reset to 9 visible charges."},

	{"XS_4_3_0", "Refill hidden charges on recovery."}, {"XS_4_3_1", "Reset to 0 hidden charges."}, {"XS_4_3_2", "Reset to 1 hidden charges."}, {"XS_4_3_3", "Reset to 2 hidden charges."}, {"XS_4_3_4", "Reset to 3 hidden charges."},
	{"XS_4_3_5", "Reset to 4 hidden charges."}, {"XS_4_3_6", "Reset to 5 hidden charges."}, {"XS_4_3_7", "Reset to 6 hidden charges."}, {"XS_4_3_8", "Reset to 7 hidden charges."}, {"XS_4_3_9", "Reset to 8 hidden charges."},
	{"XS_4_3_10", "Reset to 9 hidden charges."},

	{"XS_4_5_0", "Refill hearts on recovery."}, {"XS_4_5_1", "Reset to 5 hearts."}, {"XS_4_5_2", "Reset to 4 hearts."}, {"XS_4_5_3", "Reset to 3 hearts."}, {"XS_4_5_4", "Reset to 2 hearts."},
	{"XS_4_5_5", "Reset to 1 hearts."}, {"XS_4_6_6", "Reset to 0 hearts."},

	{"XS_4_7_0", "Restore knife on recovery."}, {"XS_4_7_1", "Reset to 1 knife."},

	{"XS_5_0_0", "Do not show hitboxes."}, {"XS_5_0_1", "Show hitboxes."},
	{"XS_5_1_0", "Show indiviudal box borders."}, {"XS_5_1_1", "Hide box borders within overlap."},
	{"XS_5_2_0", "Use default box colors."}, {"XS_5_2_1", "Use color-blind friendly box colors."},

	{"XS_5_4_0", "Draw player origins as a small blue cross."}, {"XS_5_4_1", "Draw player origins extending to the edge of the screen."},

	{"XS_5_6_0", "Do not draw the ground."}, {"XS_5_6_1", "Draw a blue line at ground-level."},

	{"XS_6_0_0", "No slot selected."}, {"XS_6_0_1", "Slot 1."}, {"XS_6_0_2", "Slot 2."}, {"XS_6_0_3", "Slot 3."},

	{"XS_6_8_0", "Do not load RNG saved with state."}, {"XS_6_8_1", "Load RNG saved with state."},

	{"XS_7_0_0", "Show the basic level of information."}, {"XS_7_0_1", "Show all information."},
	{"XS_7_1_0", "Do not show the framebar in-game."}, {"XS_7_1_1", "Show the framebar in-game."},

	{"XS_7_3_0", "Do not show frames where both players are frozen."}, {"XS_7_3_1", "Show all frames."},
	{"XS_7_4_0", "Do not show inputs."}, {"XS_7_4_1", "Show inputs."},
	{"XS_7_5_0", "Do not show cancel windows."}, {"XS_7_5_1", "Show cancel windows."},

	{"XS_7_7_0", "_SCROLL"}, {"XS_7_7_1", "_SCROLL"}, {"XS_7_7_2", "_SCROLL"},

	{"XS_8_0_0", "No custom RNG."}, {"XS_8_0_1", "Use a seed for custom RNG."}, {"XS_8_0_2", "Use a value for custom RNG."},

	{"XS_8_2_0", "Set RNG every frame."}, {"XS_8_2_1", "Set RNG every reset."},
	{"XS_8_3_1", "_RNG"},

	{"XS_9_0_0", "Do not show health and guard values."}, {"XS_9_0_1", "Show health and guard values."},
	{"XS_9_1_0", "Show default combo damage values."}, {"XS_9_1_1", "Show accurate combo damage values."},

	{"XS_9_3_0", "No P1 input display."}, {"XS_9_3_1", "Display P1 inputs as a list."}, {"XS_9_3_2", "Display P1 inputs on an arcade layout."}, {"XS_9_3_3", "Display P1 inputs both ways."},
	{"XS_9_4_0", "No P2 input display."}, {"XS_9_4_1", "Display P2 inputs as a list."}, {"XS_9_4_2", "Display P2 inputs on an arcade layout."}, {"XS_9_4_3", "Display P2 inputs both ways."},

	{"XS_9_6_0", "_BARY"}, {"XS_9_6_1", "_BARY"}, {"XS_9_6_2", "_BARY"},

	{"XS_10_0_0", "Play the game at 100% speed."}, {"XS_10_0_1", "Play the game at 75% speed."}, {"XS_10_0_2", "Play the game at 50% speed."}, {"XS_10_0_3", "Play the game at 25% speed."},

	{"XS_10_2_0", "Do not hide the HUD."}, {"XS_10_2_1", "Hide the HUD."},
	{"XS_10_3_0", "Do not hide shadows."}, {"XS_10_3_1", "Hide shadows."},
	{"XS_10_4_0", "Do not hide extras (FPS, Hash)."}, {"XS_10_4_1", "Hide extras (FPS, Hash)."},

	{"XS_10_6_0", "Show the stage background."}, {"XS_10_6_1", "Show a white background."}, {"XS_10_6_2", "Show a gray background."}, {"XS_10_6_3", "Show a black background."},
	{"XS_10_6_4", "Show a red background."}, {"XS_10_6_5", "Show a yellow background."}, {"XS_10_6_6", "Show a green background."}, {"XS_10_6_7", "Show a blue background."}, {"XS_10_6_8", "Show a purple background."},
};