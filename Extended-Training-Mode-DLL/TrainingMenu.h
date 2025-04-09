#pragma once
#include <cstddef>
#include <vector>
#include "..\Common\Common.h"

DWORD MBAA_ReadDataFile = 0x00407c10;
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

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define UNUSED(length) BYTE LINE_NAME[length]

#define CHECKOFFSET(s, v, n) static_assert(offsetof(s, v) == n, #s " offset incorrect for " #v ". Expected " #n);
#define CHECKSIZE(s, n) static_assert(sizeof(s) == n, #s " size must be " #n);

#pragma pack(push, 1)

struct Item //scrolling items on right
{
	void* vftable;
	char label[0x10]; // becomes pointer if size > 0xf
	int labelLength;
	int labelMaxLength;
	int nameBase;
	char tag[0x10]; // becomes pointer if size > 0xf
	int tagLength;
	int tagMaxLength;
	int value;
};

CHECKOFFSET(Item, tag, 0x20)
CHECKOFFSET(Item, value, 0x38)
CHECKSIZE(Item, 0x3c)

struct Element //listed elements on left
{
	void* vftable;
	int elementType;
	int isHovered;
	int canSelect;
	int timeHovered;
	int timeNotHovered;
	int bottomMargin;
	float textOpacity;
	int labelBase;
	char label[0x10]; // becomes pointer if size > 0xf
	int labelLength;
	int labelMaxLength;
	int tagBase;
	char tag[0x10]; // becomes pointer if size > 0xf
	int tagLength;
	int tagMaxLength;
	int selectedItem;
	int selectItemLabelXOffset;
	int ListInput;
	Item** ItemList;
	Item** ItemListEnd;
	UNUSED(0x04);

	void SetItemLabel(const char* newLabel, int itemIndex) {
		if (ItemList == 0x0 || ItemListEnd - ItemList <= itemIndex) {
			return;
		}
		Item* targetItem = ItemList[itemIndex];
		ReadDataFile((void*)(targetItem), newLabel, strlen(newLabel));
	}

	//vftable[6]
	int GetItemValue(int itemIndex) {
		if (ItemList == 0x0 || ItemListEnd - ItemList <= itemIndex) {
			return 0;
		}
		return ItemList[itemIndex]->value;
	}
	
	//vftable[9] sets selectedItem

	//vftable[10] gets selectedItem

	//vftable[11]
	int GetItemListSize() {
		if (ItemList == 0x0) {
			return 0;
		}
		return ItemListEnd - ItemList;
	}
};

CHECKOFFSET(Element, label, 0x24)
CHECKOFFSET(Element, tag, 0x40)
CHECKOFFSET(Element, ItemList, 0x64)
CHECKSIZE(Element, 0x70)

struct MenuWindow;

struct MenuInfo
{
	void* vftable;
	MenuWindow* parentWindow;
	int tagBase;
	char tag[0x10]; // becomes pointer if size > 0xf
	int tagLength;
	int tagMaxLength;
	int blankBase;
	char blank[0x10]; // becomes pointer if size > 0xf
	int blankLength;
	int blankMaxLength;
	int selectedElement;
	int prevSelectedElement;
	int ListInput;
	Element** ElementList;
	Element** ElementListEnd;
	int field_0x54;
	int field_0x58;
	int field_0x5c;
	int field_0x60;
	int field_0x64;
	int finishedDrawing;
	int timeDisplayed;
	int field_0x70;
	int close;
};

CHECKOFFSET(MenuInfo, tagMaxLength, 0x20)
CHECKOFFSET(MenuInfo, ElementList, 0x4c)
CHECKSIZE(MenuInfo, 0x78)

struct MenuWindow
{
	void* vftable;
	int menuInfoIndex;
	int field_0x8;
	int ListInput;
	MenuInfo** MenuInfoList;
	MenuInfo** MenuInfoListEnd;
	int field_0x18;
	int didPress;
	int hoveredTagBase;
	char hoveredTag[0x10]; // becomes pointer if size > 0xf
	int hoveredTagLength;
	int hoveredTagMaxLength;
	int field_0x3c;
	int field_0x40;
	int yOffset;
	int someYOffset;
	int xOffset;
	int field_0x50;
	int field_0x54;
	int field_0x58;
	int labelBase;
	char label[0x10]; // becomes pointer if size > 0xf
	int labelLength;
	int labelMaxLength;
	int playerInControl;
	int isRootMenu;
	int timeOpened;
	int openSubmenuIndex;
	int timeSubmenuOpened;
	float closeAnimProgress;
	float menuOpacity;
	float progressionRate;
	float degressionRate;
	int isBlurred;
	int field_0xa0;
	int isMenuBackgroundDisplayed;
	int u_layer;
	int paragraphMode;
	int field_0xb0;
	int textXWidth;
	int textYWidth;
	int isMenuLit;
	float dimScreenPercentage;
	MenuWindow* YesNoMenu;
	MenuWindow* BattleSettings;
	MenuWindow* EnemyStatus;
	MenuWindow* TrainingDisplay;
	MenuWindow* DummySettings;
	int u_hideMenu;
	void* InformationMenu;
	MenuWindow* ExtendedSettings;
	MenuWindow* HotkeySettings;
};

CHECKOFFSET(MenuWindow, MenuInfoList, 0x10)
CHECKOFFSET(MenuWindow, labelBase, 0x5c)
CHECKOFFSET(MenuWindow, isRootMenu, 0x7c)
CHECKOFFSET(MenuWindow, dimScreenPercentage, 0xc0)
CHECKSIZE(MenuWindow, 0xe8)

#pragma pack(pop, 1)
#undef CHECKOFFSET
#undef CHECKSIZE

//Vectors guide
//Empty vectors are blank spaces(only need the one defined)
//Vectors with one value are elements with only a label (like the vanilla "default" and "return" elements)
//Vectors with more than one value are elements with selectable options

std::vector<const char*> vSPACE_ELEMENT = {};

//Page 1
std::vector<const char*> vREVERSAL_TYPE = {
	"REVERSAL TYPE",
	"OFF", "NORMAL", "RANDOM", "SHIELD", "REPEAT"
};

std::vector<const char*> vREVERSAL_SLOT_1 = {
	"REVERSAL SLOT 1",
	"X1", "OFF", "X3"
};

std::vector<const char*> vREV_SLOT_1_WEIGHT = {
	"WEIGHT 1",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_2 = {
	"REVERSAL SLOT 2",
	"X1", "OFF", "X3"
};

std::vector<const char*> vREV_SLOT_2_WEIGHT = {
	"WEIGHT 2",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_3 = {
	"REVERSAL SLOT 3",
	"X1", "OFF", "X3"
};

std::vector<const char*> vREV_SLOT_3_WEIGHT = {
	"WEIGHT 3",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_4 = {
	"REVERSAL SLOT 4",
	"X1", "OFF", "X3"
};

std::vector<const char*> vREV_SLOT_4_WEIGHT = {
	"WEIGHT 4",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_DELAY = {
	"REVERSAL DELAY",
	"0", "X2", "X3"
};

std::vector<std::vector<const char*>> P1_Options = {
	vREVERSAL_TYPE, vSPACE_ELEMENT,
	vREVERSAL_SLOT_1, vREV_SLOT_1_WEIGHT,
	vREVERSAL_SLOT_2, vREV_SLOT_2_WEIGHT,
	vREVERSAL_SLOT_3, vREV_SLOT_3_WEIGHT,
	vREVERSAL_SLOT_4, vREV_SLOT_4_WEIGHT,
	vSPACE_ELEMENT, vREVERSAL_DELAY
};

int nREVERSAL_TYPE = 1;
int nREVERSAL_SLOT_1 = 1;
int nREV_SLOT_1_WEIGHT = 1;
int nREVERSAL_SLOT_2 = 1;
int nREV_SLOT_2_WEIGHT = 1;
int nREVERSAL_SLOT_3 = 1;
int nREV_SLOT_3_WEIGHT = 1;
int nREVERSAL_SLOT_4 = 1;
int nREV_SLOT_4_WEIGHT = 1;
int nREVERSAL_DELAY = 0;

int nTRUE_REVERSAL_SLOT_1 = 0;
int nTRUE_REVERSAL_SLOT_2 = 0;
int nTRUE_REVERSAL_SLOT_3 = 0;
int nTRUE_REVERSAL_SLOT_4 = 0;
int nTRUE_REVERSAL_DELAY = 0;

const int nNUM_REVERSALS = 4;
int* nREV_WEIGHTS[nNUM_REVERSALS] = { &nREV_SLOT_1_WEIGHT, &nREV_SLOT_2_WEIGHT , &nREV_SLOT_3_WEIGHT , &nREV_SLOT_4_WEIGHT };
int* nTRUE_REVS[nNUM_REVERSALS] = { &nTRUE_REVERSAL_SLOT_1, &nTRUE_REVERSAL_SLOT_2, &nTRUE_REVERSAL_SLOT_3, &nTRUE_REVERSAL_SLOT_4 };

std::vector<int*> P1_Settings = {
	&nREVERSAL_TYPE,
	&nREVERSAL_SLOT_1, &nREV_SLOT_1_WEIGHT,
	&nREVERSAL_SLOT_2, &nREV_SLOT_2_WEIGHT,
	&nREVERSAL_SLOT_3, &nREV_SLOT_3_WEIGHT,
	&nREVERSAL_SLOT_4, &nREV_SLOT_4_WEIGHT,
	&nREVERSAL_DELAY
};

//Page 2
std::vector<const char*> vPENALTY_RESET = {
	"PENALTY RESET",
	"NORMAL", "INSTANT",
};

std::vector<const char*> vEX_GUARD = {
	"EX GUARD",
	"OFF", "ON", "RANDOM",
};

std::vector<const char*> vGUARD_BAR = {
	"GUARD BAR",
	"NORMAL", "INFINITE",
};

std::vector<const char*> vMETER = {
	"METER",
	"X1", "X2", "X3",
};

std::vector<const char*> vHEALTH = {
	"HEALTH",
	"X1", "X2", "X3",
};

std::vector<const char*> vHITS_UNTIL_BURST = {
	"HITS UNTIL BURST",
	"OFF", "X1", "X2", "X3"
};

std::vector<std::vector<const char*>> P2_Options = {
	vPENALTY_RESET, vSPACE_ELEMENT, vEX_GUARD, vGUARD_BAR, vSPACE_ELEMENT, vMETER, vHEALTH, vSPACE_ELEMENT, vHITS_UNTIL_BURST
};

int nPENALTY_RESET = 0;
int nEX_GUARD = 0;
int nGUARD_BAR = 0;
int nMETER = 0;
int nHEALTH = 0;
int nHITS_UNTIL_BURST = 0;

int nTRUE_METER = 15000;
int nTRUE_HEALTH = 11400;
int nTRUE_HITS_UNTIL_BURST = 0;

std::vector<int*> P2_Settings = {
	&nPENALTY_RESET, &nEX_GUARD, &nGUARD_BAR, &nMETER, &nHEALTH, &nHITS_UNTIL_BURST
};

//Page 3
std::vector<const char*> vHIGHLIGHTS = {
	"HIGHLIGHTS",
	"OFF", "ON"
};

std::vector<const char*> vGUARD = {
	"GUARD",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

std::vector<const char*> vHIT = {
	"HIT",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

std::vector<const char*> vARMOR = {
	"ARMOR",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

std::vector<const char*> vTHROW_PROTECTION = {
	"THROW PROTECTION",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

std::vector<const char*> vIDLE = {
	"IDLE",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

std::vector<std::vector<const char*>> P3_Options = {
	vHIGHLIGHTS, vGUARD, vHIT, vARMOR, vTHROW_PROTECTION, vIDLE
};

int nHIGHLIGHTS = 0;
int nGUARD = 0;
int nHIT = 0;
int nARMOR = 0;
int nTHROW_PROTECTION = 0;
int nIDLE = 0;

std::vector<int*> P3_Settings = {
	&nHIGHLIGHTS, &nGUARD, &nHIT, &nARMOR, &nTHROW_PROTECTION, &nIDLE
};

//Page 4
std::vector<const char*> vRESET_TO_POSITIONS = {
	"RESET TO POSITIONS",
	"NO", "YES"
};

std::vector<const char*> vP1_X_LOC = {
	"P1 X-LOC",
	"X1", "X2", "X3"
};

std::vector<const char*> vP2_X_LOC = {
	"P2 X-LOC",
	"X1", "X2", "X3"
};

std::vector<const char*> vINVERT = {
	"INVERT"
};

std::vector<std::vector<const char*>> P4_Options = {
	vRESET_TO_POSITIONS, vSPACE_ELEMENT, vP1_X_LOC, vP2_X_LOC, vSPACE_ELEMENT, vINVERT
};

int nRESET_TO_POSITIONS = 0;
int nP1_X_LOC = 0;
int nP2_X_LOC = 0;

std::vector<int*> P4_Settings = {
	&nRESET_TO_POSITIONS, &nP1_X_LOC, &nP2_X_LOC
};

//Page 5
std::vector<const char*> vSION_BULLETS = {
	"SION BULLETS",
	"INFINITE", "NORMAL", "X1", "X2", "X3"
};

std::vector<const char*> vROA_VISIBLE_CHARGE = {
	"ROA VISIBLE CHARGE",
	"INFINITE", "NORMAL", "X1", "X2", "X3"
};

std::vector<const char*> vROA_HIDDEN_CHARGE = {
	"ROA HIDDEN CHARGE",
	"INFINITE", "NORMAL", "X1", "X2", "X3"
};

std::vector<const char*> vF_MAIDS_HEARTS = {
	"F-MAIDS HEARTS",
	"INFINITE", "5", "4", "3", "2", "1" ,"0"
};

std::vector<const char*> vRYOUGI_KNIFE = {
	"RYOUGI KNIFE",
	"NORMAL", "INFINITE"
};

std::vector<std::vector<const char*>> P5_Options = {
	vSION_BULLETS, vSPACE_ELEMENT, vROA_VISIBLE_CHARGE, vROA_HIDDEN_CHARGE, vSPACE_ELEMENT, vF_MAIDS_HEARTS, vSPACE_ELEMENT, vRYOUGI_KNIFE
};

int nSION_BULLETS = 1;
int nROA_VISIBLE_CHARGE = 1;
int nROA_HIDDEN_CHARGE = 1;
int nF_MAIDS_HEARTS = 1;
int nRYOUGI_KNIFE = 0;

std::vector<int*> P5_Settings = {
	&nSION_BULLETS, &nROA_VISIBLE_CHARGE, &nROA_HIDDEN_CHARGE, &nF_MAIDS_HEARTS, &nRYOUGI_KNIFE
};

//Page 6
std::vector<const char*> vDISPLAY_HITBOXES = {
	"DISPLAY HITBOXES",
	"OFF", "ON"
};

std::vector<const char*> vHITBOX_STYLE = {
	"HITBOX STYLE",
	"LAYERED", "BLENDED"
};

std::vector<const char*> vCOLOR_BLIND_MODE = {
	"COLOR BLIND MODE",
	"OFF", "ON"
};

std::vector<const char*> vORIGIN_STYLE = {
	"ORIGIN STYLE",
	"STANDARD", "EXTENDED"
};

std::vector<const char*> vDRAW_GROUND = {
	"DRAW GROUND",
	"OFF", "ON"
};

std::vector<std::vector<const char*>> P6_Options = {
	vDISPLAY_HITBOXES, vHITBOX_STYLE, vCOLOR_BLIND_MODE, vSPACE_ELEMENT, vORIGIN_STYLE, vSPACE_ELEMENT, vDRAW_GROUND
};

int nDISPLAY_HITBOXES = 0;
int nHITBOX_STYLE = 0;
int nCOLOR_BLIND_MODE = 0;
int nORIGIN_STYLE = 0;
int nDRAW_GROUND = 0;

std::vector<int*> P6_Settings = {
	&nDISPLAY_HITBOXES, &nHITBOX_STYLE, &nCOLOR_BLIND_MODE, &nORIGIN_STYLE, &nDRAW_GROUND
};

//Page 7
std::vector<const char*> vSAVE_STATE_SLOT = {
	"SAVE STATE SLOT",
	"NONE", "SLOT 01", "SLOT 02", "SLOT 03"
};

std::vector<const char*> vSAVE_STATE = {
	"SAVE STATE"
};

std::vector<const char*> vCLEAR_ALL_SAVES = {
	"CLEAR ALL SAVES"
};

std::vector<const char*> vIMPORT_SAVE = {
	"IMPORT SAVE"
};

std::vector<const char*> vEXPORT_SAVE = {
	"EXPORT SAVE"
};

std::vector<const char*> vLOAD_RNG = {
	"LOAD RNG",
	"OFF", "ON"
};

std::vector<std::vector<const char*>> P7_Options = {
	vSAVE_STATE_SLOT, vSPACE_ELEMENT, vSAVE_STATE, vCLEAR_ALL_SAVES, vSPACE_ELEMENT, vIMPORT_SAVE, vEXPORT_SAVE, vSPACE_ELEMENT, vLOAD_RNG
};

int nSAVE_STATE_SLOT = 1;
int nLOAD_RNG = 0;

std::vector<int*> P7_Settings = {
	&nSAVE_STATE_SLOT, &nLOAD_RNG
};

//Page 8
std::vector<const char*> vFRAME_DATA = {
	"FRAME DATA",
	"NORMAL", "ADVANCED"
};

std::vector<const char*> vIN_GAME_FRAMEBAR = {
	"IN-GAME FRAMEBAR",
	"OFF", "ON"
};

std::vector<const char*> vSHOW_FREEZE_AND_INPUTS = {
	"SHOW FREEZE & INPUTS",
	"OFF", "ON"
};

std::vector<const char*> vSHOW_CANCEL_WINDOWS = {
	"SHOW CANCEL WINDOWS",
	"OFF", "ON"
};

std::vector<const char*> vSCROLL_DISPLAY = {
	"SCROLL DISPLAY",
	"X1", "X2", "X3", "0"
};

std::vector<const char*> vCOLOR_GUIDE = {
	"COLOR GUIDE"
};

std::vector<std::vector<const char*>> P8_Options = {
	vFRAME_DATA, vIN_GAME_FRAMEBAR, vSPACE_ELEMENT, vSHOW_FREEZE_AND_INPUTS, vSHOW_CANCEL_WINDOWS, vSPACE_ELEMENT, vSCROLL_DISPLAY, vSPACE_ELEMENT, vCOLOR_GUIDE
};

int nFRAME_DATA = 0;
int nIN_GAME_FRAMEBAR = 0;
int nSHOW_FREEZE_AND_INPUTS = 0;
int nSHOW_CANCEL_WINDOWS = 0;
int nSCROLL_DISPLAY = 3;

std::vector<int*> P8_Settings = {
	&nFRAME_DATA, &nIN_GAME_FRAMEBAR, &nSHOW_FREEZE_AND_INPUTS, &nSHOW_CANCEL_WINDOWS, &nSCROLL_DISPLAY
};

//Page 9
std::vector<const char*> vCUSTOM_RNG = {
	"CUSTOM RNG",
	"OFF", "SEED", "VALUE"
};

std::vector<const char*> vRATE = {
	"RATE",
	"EVERY FRAME", "EVERY RESET"
};

std::vector<const char*> vSEED = {
	"SEED",
	"X1", "X2", "X3"
};

std::vector<std::vector<const char*>> P9_Options = {
	vCUSTOM_RNG, vSPACE_ELEMENT, vRATE, vSEED
};

int nCUSTOM_RNG = 0;
int nRATE = 0;
int nSEED = 0;

std::vector<int*> P9_Settings = {
	&nCUSTOM_RNG, &nRATE, &nSEED
};

//Page 10
std::vector<const char*> vSHOW_STATS = {
	"SHOW STATS",
	"OFF", "ON"
};

std::vector<const char*> vP1_INPUT_DISPLAY = {
	"P1 INPUT DISPLAY",
	"OFF", "LIST", "ARCADE", "BOTH"
};

std::vector<const char*> vP2_INPUT_DISPLAY = {
	"P2 INPUT DISPLAY",
	"OFF", "LIST", "ARCADE", "BOTH"
};

std::vector<const char*> vFRAME_DISPLAY_Y = {
	"FRAME DISPLAY Y",
	"X1", "X2", "X3"
};

std::vector<const char*> vATTACK_DISPLAY = {
	"ATTACK DISPLAY",
	"OFF", "ON"
};

std::vector<std::vector<const char*>> P10_Options = {
	vSHOW_STATS, vSPACE_ELEMENT, vP1_INPUT_DISPLAY, vP2_INPUT_DISPLAY, vSPACE_ELEMENT, vFRAME_DISPLAY_Y, vSPACE_ELEMENT, vATTACK_DISPLAY
};

int nSHOW_STATS = 0;
int nP1_INPUT_DISPLAY = 0;
int nP2_INPUT_DISPLAY = 0;
int nFRAME_DISPLAY_Y = 0;
int nATTACK_DISPLAY = 0;

std::vector<int*> P10_Settings = {
	&nSHOW_STATS, &nP1_INPUT_DISPLAY, &nP2_INPUT_DISPLAY, &nFRAME_DISPLAY_Y, &nATTACK_DISPLAY
};

//Page 11
std::vector<const char*> vSLOW_MOTION = {
	"SLOW MOTION",
	"OFF", "ON"
};

std::vector<const char*> vHIDE_HUD = {
	"HIDE HUD",
	"OFF", "ON"
};

std::vector<const char*> vHIDE_SHADOWS = {
	"HIDE SHADOWS",
	"OFF", "ON"
};

std::vector<const char*> vHIDE_EXTRAS = {
	"HIDE EXTRAS",
	"OFF", "ON"
};

std::vector<const char*> vBACKGROUND = {
	"BACKGROUND",
	"NORMAL", "WHITE", "BLACK", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE"
};

std::vector<std::vector<const char*>> P11_Options = {
	vSLOW_MOTION, vSPACE_ELEMENT, vHIDE_HUD, vHIDE_SHADOWS, vHIDE_EXTRAS, vSPACE_ELEMENT, vBACKGROUND
};

int nSLOW_MOTION = 0;
int nHIDE_HUD = 0;
int nHIDE_SHADOWS = 0;
int nHIDE_EXTRAS = 0;
int nBACKGROUND = 0;

std::vector<int*> P11_Settings = {
	&nSLOW_MOTION, &nHIDE_HUD, &nHIDE_SHADOWS, &nHIDE_EXTRAS, &nBACKGROUND
};

//All pages
std::vector<std::vector<std::vector<const char*>>> Page_Options = {
	P1_Options, P2_Options, P3_Options, P4_Options, P5_Options, P6_Options, P7_Options, P8_Options, P9_Options, P10_Options, P11_Options
};

std::vector<std::vector<int*>> Page_Settings = {
	P1_Settings, P2_Settings, P3_Settings, P4_Settings, P5_Settings, P6_Settings, P7_Settings, P8_Settings, P9_Settings, P10_Settings, P11_Settings
};

uint8_t nEXTENDED_SETTINGS_PAGE = 0;
uint8_t nEXTENDED_SETTINGS_CURSOR[11] = { 0 };

bool bOldFN1Input = 0;
bool bOldFN2Input = 0;

//Hotkey settings

//Page 1
std::vector<const char*> vFREEZE = {
	"FREEZE",
	"X"
};

std::vector<const char*> vSTEP_FRAME = {
	"STEP FRAME",
	"X"
};

std::vector<const char*> vTOGGLE_HITBOXES = {
	"TOGGLE HITBOXES",
	"X"
};

std::vector<const char*> vTOGGLE_FRAME_BAR = {
	"TOGGLE FRAME BAR",
	"X"
};

std::vector<const char*> vTOGGLE_HIGHLIGHTS = {
	"TOGGLE HIGHLIGHTS",
	"X"
};

std::vector<const char*> vQUEUE_REVERSAL = {
	"QUEUE REVERSAL",
	"X"
};

std::vector<const char*> vINCREMENT_RNG = {
	"INCREMENT RNG",
	"X"
};

std::vector<const char*> vDECREMENT_RNG = {
	"DECREMENT RNG",
	"X"
};

std::vector<std::vector<const char*>> HK_P1_Options = {
	vFREEZE, vSTEP_FRAME, vTOGGLE_HITBOXES, vTOGGLE_FRAME_BAR, vTOGGLE_HIGHLIGHTS, vQUEUE_REVERSAL, vINCREMENT_RNG, vDECREMENT_RNG
};

int nFREEZE = 0;
int nSTEP_FRAME = 0;
int nTOGGLE_HITBOXES = 0;
int nTOGGLE_FRAME_BAR = 0;
int nTOGGLE_HIGHLIGHTS = 0;
int nQUEUE_REVERSAL = 0;
int nINCREMENT_RNG = 0;
int nDECREMENT_RNG = 0;

std::vector<int*> HK_P1_Settings = {
	&nFREEZE, &nSTEP_FRAME, &nTOGGLE_HITBOXES, &nTOGGLE_FRAME_BAR, &nTOGGLE_HIGHLIGHTS, &nQUEUE_REVERSAL, &nINCREMENT_RNG, &nDECREMENT_RNG
};

//Page 2
std::vector<const char*> vSAVE_STATE_HK = {
	"SAVE STATE",
	"X"
};

std::vector<const char*> vPREV_SAVE_SLOT = {
	"PREV SAVE SLOT",
	"X"
};

std::vector<const char*> vNEXT_SAVE_SLOT = {
	"NEXT SAVE SLOT",
	"X"
};

std::vector<const char*> vFRAME_BAR_LEFT = {
	"FRAME BAR LEFT",
	"X"
};

std::vector<const char*> vFRAME_BAR_RIGHT = {
	"FRAME BAR RIGHT",
	"X"
};

std::vector<std::vector<const char*>> HK_P2_Options = {
	vSAVE_STATE_HK, vPREV_SAVE_SLOT, vNEXT_SAVE_SLOT, vFRAME_BAR_LEFT, vFRAME_BAR_RIGHT
};

int nSAVE_STATE = 0;
int nPREV_SAVE_SLOT = 0;
int nNEXT_SAVE_SLOT = 0;
int nFRAME_BAR_LEFT = 0;
int nFRAME_BAR_RIGHT = 0;

std::vector<int*> HK_P2_Settings = {
	&nSAVE_STATE, &nPREV_SAVE_SLOT, &nNEXT_SAVE_SLOT, &nFRAME_BAR_LEFT, &nFRAME_BAR_RIGHT
};

//All pages
std::vector<std::vector<std::vector<const char*>>> HK_Page_Options = {
	HK_P1_Options, HK_P2_Options
};

std::vector<std::vector<int*>> HK_Page_Settings = {
	HK_P1_Settings, HK_P2_Settings
};

uint8_t nHOTKEY_SETTINGS_PAGE = 0;
uint8_t nHOTKEY_SETTINGS_CURSOR[2] = { 0 };