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
	if (ItemList == 0x0 || ItemListEnd - ItemList <= itemIndex) {
		return;
	}
	Item* targetItem = ItemList[itemIndex];
	ReadDataFile((void*)(targetItem), newLabel, strlen(newLabel));
}

void Element::SetCurItemLabel(const char* newLabel) {
	int itemIndex = selectedItem;
	if (ItemList == 0x0 || ItemListEnd - ItemList <= itemIndex) {
		return;
	}
	Item* targetItem = ItemList[itemIndex];
	ReadDataFile((void*)(targetItem), newLabel, strlen(newLabel));
}

//vftable[6]
int Element::GetItemValue(int itemIndex) {
	if (ItemList == 0x0 || ItemListEnd - ItemList <= itemIndex) {
		return 0;
	}
	return ItemList[itemIndex]->value;
}

//vftable[11]
int Element::GetItemListSize() {
	if (ItemList == 0x0) {
		return 0;
	}
	return ItemListEnd - ItemList;
}

void MenuWindow::SetLabel(const char* newLabel) {
	ReadDataFile((void*)(&labelBase), newLabel, strlen(newLabel));
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

	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_TYPE]->selectedItem = defREVERSAL_TYPE;
	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_SLOT_1]->selectedItem = defREVERSAL_SLOT_1;
	menuInfo->ElementList[(int)eREVERSALS::WEIGHT_1]->selectedItem = defREV_SLOT_1_WEIGHT;
	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_SLOT_2]->selectedItem = defREVERSAL_SLOT_2;
	menuInfo->ElementList[(int)eREVERSALS::WEIGHT_2]->selectedItem = defREV_SLOT_2_WEIGHT;
	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_SLOT_3]->selectedItem = defREVERSAL_SLOT_3;
	menuInfo->ElementList[(int)eREVERSALS::WEIGHT_3]->selectedItem = defREV_SLOT_3_WEIGHT;
	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_SLOT_4]->selectedItem = defREVERSAL_SLOT_4;
	menuInfo->ElementList[(int)eREVERSALS::WEIGHT_4]->selectedItem = defREV_SLOT_4_WEIGHT;
	menuInfo->ElementList[(int)eREVERSALS::NO_REVERSAL_WEIGHT]->selectedItem = defNO_REV_WEIGHT;
	menuInfo->ElementList[(int)eREVERSALS::REVERSAL_DELAY]->selectedItem = defREVERSAL_DELAY;
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

	menuInfo->ElementList[(int)eTRAINING::PENALTY_RESET]->selectedItem = defPEN_RESET;
	menuInfo->ElementList[(int)eTRAINING::GUARD_BAR_RESET]->selectedItem = defGUARD_RESET;
	menuInfo->ElementList[(int)eTRAINING::EX_GUARD]->selectedItem = defEX_GUARD;
	menuInfo->ElementList[(int)eTRAINING::P1_METER]->selectedItem = defP1_METER;
	menuInfo->ElementList[(int)eTRAINING::P2_METER]->selectedItem = defP2_METER;
	menuInfo->ElementList[(int)eTRAINING::P1_HEALTH]->selectedItem = defP1_HEALTH;
	menuInfo->ElementList[(int)eTRAINING::P2_HEALTH]->selectedItem = defP2_HEALTH;
	menuInfo->ElementList[(int)eTRAINING::HITS_UNTIL_BURST]->selectedItem = defHITS_BURST;
	menuInfo->ElementList[(int)eTRAINING::HITS_UNTIL_BUNKER]->selectedItem = defHITS_BUNKER;
	menuInfo->ElementList[(int)eTRAINING::HITS_UNTIL_FORCE_GUARD]->selectedItem = defHITS_FORCE_GUARD;
	menuInfo->ElementList[(int)eTRAINING::FORCE_GUARD_STANCE]->selectedItem = defFORCE_GUARD_STANCE;
}

//Page 3
int nHIGHLIGHTS = defHIGHLIGHTS;
int nGUARD = defGUARD;
int nHIT = defHIT;
int nARMOR = defARMOR;
int nTHROW_PROTECTION = defTHROW_PROT;
int nIDLE = defIDLE;

void DefaultP3(MenuInfo* menuInfo) {
	nHIGHLIGHTS = defHIGHLIGHTS;
	nGUARD = defGUARD;
	nHIT = defHIT;
	nARMOR = defARMOR;
	nTHROW_PROTECTION = defTHROW_PROT;
	nIDLE = defIDLE;

	menuInfo->ElementList[(int)eHIGHLIGHTS::HIGHLIGHTS]->selectedItem = defHIGHLIGHTS;
	menuInfo->ElementList[(int)eHIGHLIGHTS::GUARD]->selectedItem = defGUARD;
	menuInfo->ElementList[(int)eHIGHLIGHTS::HIT]->selectedItem = defHIT;
	menuInfo->ElementList[(int)eHIGHLIGHTS::ARMOR]->selectedItem = defARMOR;
	menuInfo->ElementList[(int)eHIGHLIGHTS::THROW_PROTECTION]->selectedItem = defTHROW_PROT;
	menuInfo->ElementList[(int)eHIGHLIGHTS::IDLE]->selectedItem = defIDLE;
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

	menuInfo->ElementList[(int)ePOSITIONS::RESET_TO_POSITIONS]->selectedItem = defRESET_POS;
	menuInfo->ElementList[(int)ePOSITIONS::P1_POSITION]->selectedItem = defP1_X;
	menuInfo->ElementList[(int)ePOSITIONS::P1_ASSIST_POSITION]->selectedItem = defP1_ASSIST_X;
	menuInfo->ElementList[(int)ePOSITIONS::P2_POSITION]->selectedItem = defP2_X;
	menuInfo->ElementList[(int)ePOSITIONS::P2_ASSIST_POSITION]->selectedItem = defP2_ASSIST_X;
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

	menuInfo->ElementList[(int)eCHARACTER::SION_BULLETS]->selectedItem = defSION_BULLETS;
	menuInfo->ElementList[(int)eCHARACTER::ROA_VISIBLE_CHARGES]->selectedItem = defROA_VISIBLE;
	menuInfo->ElementList[(int)eCHARACTER::ROA_HIDDEN_CHARGES]->selectedItem = defROA_HIDDEN;
	menuInfo->ElementList[(int)eCHARACTER::F_MAIDS_HEARTS]->selectedItem = defMAIDS_HEARTS;
	menuInfo->ElementList[(int)eCHARACTER::RYOUGI_KNIFE]->selectedItem = defRYOUGI_KNIFE;
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

	menuInfo->ElementList[(int)eHITBOXES::DISPLAY_HITBOXES]->selectedItem = defDISPLAY_HITBOXES;
	menuInfo->ElementList[(int)eHITBOXES::HITBOX_STYLE]->selectedItem = defHITBOX_STYLE;
	menuInfo->ElementList[(int)eHITBOXES::COLOR_BLIND_MODE]->selectedItem = defCOLOR_BLIND;
	menuInfo->ElementList[(int)eHITBOXES::ORIGIN_STYLE]->selectedItem = defORIGIN_STYLE;
	menuInfo->ElementList[(int)eHITBOXES::DRAW_GROUND]->selectedItem = defDRAW_GROUND;
}

//Page 7
int nSAVE_STATE_SLOT = defSAVE_SLOT;
int nLOAD_RNG = defLOAD_RNG;

void DefaultP7(MenuInfo* menuInfo) {
	nSAVE_STATE_SLOT = defSAVE_SLOT;
	nLOAD_RNG = defLOAD_RNG;

	menuInfo->ElementList[(int)eSAVE_STATES::SAVE_STATE_SLOT]->selectedItem = defSAVE_SLOT;
	menuInfo->ElementList[(int)eSAVE_STATES::LOAD_RNG]->selectedItem = defLOAD_RNG;
}

//Page 8
int nFRAME_DATA = defFRAME_DATA;
int nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
int nSHOW_FREEZE_AND_INPUTS = defSHOW_FREEZE_INPUTS;
int nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
int nSCROLL_DISPLAY = defSCROLL_DISPLAY;

bool bCOLOR_GUIDE = defCOLOR_GUIDE;

int nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

void DefaultP8(MenuInfo* menuInfo) {
	nFRAME_DATA = defFRAME_DATA;
	nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
	nSHOW_FREEZE_AND_INPUTS = defSHOW_FREEZE_INPUTS;
	nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
	nSCROLL_DISPLAY = defSCROLL_DISPLAY;

	bCOLOR_GUIDE = defCOLOR_GUIDE;

	nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

	menuInfo->ElementList[(int)eFRAME_DATA::FRAME_DATA]->selectedItem = defFRAME_DATA;
	menuInfo->ElementList[(int)eFRAME_DATA::IN_GAME_FRAME_DISPLAY]->selectedItem = defIN_GAME_FRAME_DISPLAY;
	menuInfo->ElementList[(int)eFRAME_DATA::SHOW_FREEZE_AND_INPUTS]->selectedItem = defSHOW_FREEZE_INPUTS;
	menuInfo->ElementList[(int)eFRAME_DATA::SHOW_CANCEL_WINDOWS]->selectedItem = defSHOW_CANCEL;
	menuInfo->ElementList[(int)eFRAME_DATA::SCROLL_DISPLAY]->selectedItem = defSCROLL_DISPLAY;
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

	menuInfo->ElementList[(int)eRNG::CUSTOM_RNG]->selectedItem = defCUSTOM_RNG;
	menuInfo->ElementList[(int)eRNG::RATE]->selectedItem = defRATE;
	menuInfo->ElementList[(int)eRNG::SEED]->selectedItem = defSEED;
}

//Page 10
int nSHOW_STATS = defSHOW_STATS;
int nP1_INPUT_DISPLAY = defP1_INPUT;
int nP2_INPUT_DISPLAY = defP2_INPUT;
int nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

int nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

void DefaultP10(MenuInfo* menuInfo) {
	nSHOW_STATS = defSHOW_STATS;
	nP1_INPUT_DISPLAY = defP1_INPUT;
	nP2_INPUT_DISPLAY = defP2_INPUT;
	nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

	nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

	menuInfo->ElementList[(int)eUI::SHOW_STATS]->selectedItem = defSHOW_STATS;
	menuInfo->ElementList[(int)eUI::P1_INPUT_DISPLAY]->selectedItem = defP1_INPUT;
	menuInfo->ElementList[(int)eUI::P2_INPUT_DISPLAY]->selectedItem = defP2_INPUT;
	menuInfo->ElementList[(int)eUI::FRAME_DISPLAY_Y]->selectedItem = defFRAME_DISPLAY_Y;
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

	menuInfo->ElementList[(int)eSYSTEM::GAME_SPEED]->selectedItem = defGAME_SPEED;
	menuInfo->ElementList[(int)eSYSTEM::HIDE_HUD]->selectedItem = defHIDE_HUD;
	menuInfo->ElementList[(int)eSYSTEM::HIDE_SHADOWS]->selectedItem = defHIDE_SHADOWS;
	menuInfo->ElementList[(int)eSYSTEM::HIDE_EXTRAS]->selectedItem = defHIDE_EXTRAS;
	menuInfo->ElementList[(int)eSYSTEM::BACKGROUND]->selectedItem = defBACKGROUND;
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