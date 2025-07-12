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

int nREV_SHIELD_1 = defREV_SHIELD; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_2 = defREV_SHIELD; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_3 = defREV_SHIELD; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_4 = defREV_SHIELD; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]

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

	nREV_SHIELD_1 = defREV_SHIELD;
	nREV_SHIELD_2 = defREV_SHIELD;
	nREV_SHIELD_3 = defREV_SHIELD;
	nREV_SHIELD_4 = defREV_SHIELD;

	menuInfo->ElementList[0]->selectedItem = defREVERSAL_TYPE;
	menuInfo->ElementList[2]->selectedItem = defREVERSAL_SLOT_1;
	menuInfo->ElementList[3]->selectedItem = defREV_SLOT_1_WEIGHT;
	menuInfo->ElementList[4]->selectedItem = defREVERSAL_SLOT_2;
	menuInfo->ElementList[5]->selectedItem = defREV_SLOT_2_WEIGHT;
	menuInfo->ElementList[6]->selectedItem = defREVERSAL_SLOT_3;
	menuInfo->ElementList[7]->selectedItem = defREV_SLOT_3_WEIGHT;
	menuInfo->ElementList[8]->selectedItem = defREVERSAL_SLOT_4;
	menuInfo->ElementList[9]->selectedItem = defREV_SLOT_4_WEIGHT;
	menuInfo->ElementList[11]->selectedItem = defNO_REV_WEIGHT;
	menuInfo->ElementList[13]->selectedItem = defREVERSAL_DELAY;
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

	menuInfo->ElementList[0]->selectedItem = defPEN_RESET;
	menuInfo->ElementList[1]->selectedItem = defGUARD_RESET;
	menuInfo->ElementList[3]->selectedItem = defEX_GUARD;
	menuInfo->ElementList[5]->selectedItem = defP1_METER;
	menuInfo->ElementList[6]->selectedItem = defP2_METER;
	menuInfo->ElementList[8]->selectedItem = defP1_HEALTH;
	menuInfo->ElementList[9]->selectedItem = defP2_HEALTH;
	menuInfo->ElementList[11]->selectedItem = defHITS_BURST;
	menuInfo->ElementList[12]->selectedItem = defHITS_BUNKER;
	menuInfo->ElementList[13]->selectedItem = defHITS_FORCE_GUARD;
	menuInfo->ElementList[14]->selectedItem = defFORCE_GUARD_STANCE;
}

//Page 3
int nHIGHLIGHTS = defHIGHLIGHTS;
int nGUARD = defGUARD;
int nHIT = defHIT;
int nARMOR = defARMOR;
int nTHROW_PROTECTION = defTHROW_PROT;
int nIDLE = defIDLE;

void DefaultP3() {
	nHIGHLIGHTS = defHIGHLIGHTS;
	nGUARD = defGUARD;
	nHIT = defHIT;
	nARMOR = defARMOR;
	nTHROW_PROTECTION = defTHROW_PROT;
	nIDLE = defIDLE;
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

void DefaultP4() {
	nRESET_TO_POSITIONS = defRESET_POS;
	nP1_X_LOC = defP1_X;
	nP1_ASSIST_X_LOC = defP1_ASSIST_X;
	nP2_X_LOC = defP2_X;
	nP2_ASSIST_X_LOC = defP2_ASSIST_X;

	nTRUE_P1_X_LOC = defTRUE_P1_X;
	nTRUE_P1_ASSIST_X_LOC = defTRUE_P1_ASSIST_X;
	nTRUE_P2_X_LOC = defTRUE_P2_X;
	nTRUE_P2_ASSIST_X_LOC = defTRUE_P2_ASSIST_X;
}

//Page 5
int nSION_BULLETS = defSION_BULLETS;
int nROA_VISIBLE_CHARGE = defROA_VISIBLE;
int nROA_HIDDEN_CHARGE = defROA_HIDDEN;
int nF_MAIDS_HEARTS = defMAIDS_HEARTS;
int nRYOUGI_KNIFE = defRYOUGI_KNIFE;

void DefaultP5() {
	nSION_BULLETS = defSION_BULLETS;
	nROA_VISIBLE_CHARGE = defROA_VISIBLE;
	nROA_HIDDEN_CHARGE = defROA_HIDDEN;
	nF_MAIDS_HEARTS = defMAIDS_HEARTS;
	nRYOUGI_KNIFE = defRYOUGI_KNIFE;
}

//Page 6
int nDISPLAY_HITBOXES = defDISPLAY_HITBOXES;
int nHITBOX_STYLE = defHITBOX_STYLE;
int nCOLOR_BLIND_MODE = defCOLOR_BLIND;
int nORIGIN_STYLE = defORIGIN_STYLE;
int nDRAW_GROUND = defDRAW_GROUND;

void DefaultP6() {
	nDISPLAY_HITBOXES = defDISPLAY_HITBOXES;
	nHITBOX_STYLE = defHITBOX_STYLE;
	nCOLOR_BLIND_MODE = defCOLOR_BLIND;
	nORIGIN_STYLE = defORIGIN_STYLE;
	nDRAW_GROUND = defDRAW_GROUND;
}

//Page 7
int nSAVE_STATE_SLOT = defSAVE_SLOT;
int nLOAD_RNG = defLOAD_RNG;

void DefaultP7() {
	nSAVE_STATE_SLOT = defSAVE_SLOT;
	nLOAD_RNG = defLOAD_RNG;
}

//Page 8
int nFRAME_DATA = defFRAME_DATA;
int nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
int nSHOW_FREEZE_AND_INPUTS = defSHOW_FREEZE_INPUTS;
int nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
int nSCROLL_DISPLAY = defSCROLL_DISPLAY;

bool bCOLOR_GUIDE = defCOLOR_GUIDE;

int nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

void DefaultP8() {
	nFRAME_DATA = defFRAME_DATA;
	nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
	nSHOW_FREEZE_AND_INPUTS = defSHOW_FREEZE_INPUTS;
	nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
	nSCROLL_DISPLAY = defSCROLL_DISPLAY;

	bCOLOR_GUIDE = defCOLOR_GUIDE;

	nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;
}

//Page 9
int nCUSTOM_RNG = defCUSTOM_RNG;
int nRATE = defRATE;
int nSEED = defSEED;

int nTRUE_SEED = defTRUE_SEED;

void DefaultP9() {
	nCUSTOM_RNG = defCUSTOM_RNG;
	nRATE = defRATE;
	nSEED = defSEED;

	nTRUE_SEED = defTRUE_SEED;
}

//Page 10
int nSHOW_STATS = defSHOW_STATS;
int nP1_INPUT_DISPLAY = defP1_INPUT;
int nP2_INPUT_DISPLAY = defP2_INPUT;
int nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

int nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

void DefaultP10() {
	nSHOW_STATS = defSHOW_STATS;
	nP1_INPUT_DISPLAY = defP1_INPUT;
	nP2_INPUT_DISPLAY = defP2_INPUT;
	nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

	nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;
}

//Page 11
int nGAME_SPEED = defGAME_SPEED;
int nHIDE_HUD = defHIDE_HUD;
int nHIDE_SHADOWS = defHIDE_SHADOWS;
int nHIDE_EXTRAS = defHIDE_EXTRAS;
int nBACKGROUND = defBACKGROUND;

void DefaultP11() {
	nGAME_SPEED = defGAME_SPEED;
	nHIDE_HUD = defHIDE_HUD;
	nHIDE_SHADOWS = defHIDE_SHADOWS;
	nHIDE_EXTRAS = defHIDE_EXTRAS;
	nBACKGROUND = defBACKGROUND;
}

//All pages
uint8_t nEXTENDED_SETTINGS_PAGE = 0;
uint8_t nEXTENDED_SETTINGS_CURSOR[XS_NUM_PAGES] = { 0 };

bool bOldFN1Input = 0;
bool bOldFN2Input = 0;

//Hotkey settings

//Page 1
int nFREEZE = 0;
int nSTEP_FRAME = 0;
int nTOGGLE_HITBOXES = 0;
int nTOGGLE_FRAME_BAR = 0;
int nTOGGLE_HIGHLIGHTS = 0;
int nQUEUE_REVERSAL = 0;
int nINCREMENT_RNG = 0;
int nDECREMENT_RNG = 0;

//Page 2
int nSAVE_STATE = 0;
int nPREV_SAVE_SLOT = 0;
int nNEXT_SAVE_SLOT = 0;
int nFRAME_BAR_LEFT = 0;
int nFRAME_BAR_RIGHT = 0;

//All pages
uint8_t nHOTKEY_SETTINGS_PAGE = 0;
uint8_t nHOTKEY_SETTINGS_CURSOR[2] = { 0 };