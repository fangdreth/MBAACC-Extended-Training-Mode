#pragma once
#include <cstddef>
#include <vector>
#include "..\Common\Common.h"

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define UNUSED(length) BYTE LINE_NAME[length]

#define CHECKOFFSET(s, v, n) static_assert(offsetof(s, v) == n, #s " offset incorrect for " #v ". Expected " #n);
#define CHECKSIZE(s, n) static_assert(sizeof(s) == n, #s " size must be " #n);

#pragma pack(push, 1)

/*
* Most of this is copied directly from Ghidra
* Currently unused
* Could be used for editing the menus if the menu functionality is ever moved to the dll
* Could also potentially be used to create and implement new menus from scratch but good luck with that
*/

struct Item //scrolling items on right
{
	void* resource;
	char label[20];
	void* effect;
	char name[20];
	UNUSED(0x8);
	int value;
};

CHECKOFFSET(Item, effect, 0x18)
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
	UNUSED(0x4);
	char displayedName[20];
	UNUSED(0x8);
	char name[20];
	UNUSED(0x4);
	int selectedItem;
	int selectItemLabelXOffset;
	int ListInput;
	Item** ItemList;
	Item** ItemListEnd;
	UNUSED(0x04);

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

CHECKOFFSET(Element, displayedName, 0x24)
CHECKOFFSET(Element, name, 0x40)
CHECKOFFSET(Element, ItemList, 0x64)
CHECKSIZE(Element, 0x70)

struct MenuWindow;

struct MenuInfo
{
	void* vftable;
	MenuWindow* parentWindow;
	UNUSED(0x38);
	int selectedElement;
	int prevSelectedElement;
	int ListInput;
	Element** ElementList;
	Element** ElementListEnd;
	UNUSED(0x14);
	int finishedDrawing;
	int timeDisplayed;
	UNUSED(0x4);
	int close;
};

CHECKOFFSET(MenuInfo, ElementList, 0x4c)
CHECKSIZE(MenuInfo, 0x78)

struct MenuWindow
{
	void* vftable;
	int menuInfoIndex;
	UNUSED(0x4);
	int ListInput;
	MenuInfo** MenuInfoList;
	MenuInfo** MenuInfoListEnd;
	UNUSED(0x4);
	int didPress;
	UNUSED(0x4);
	int unknown_0x24;
	UNUSED(0x1C);
	int yOffset;
	int someYOffset;
	int xOffset;
	int unknown_0x50;
	UNUSED(0x8);
	int unknown_0x5c;
	char label[20];
	UNUSED(0x8);
	int isRootMenu;
	int timeOpened;
	int openSubmenuIndex;
	int timeSubmenuOpened;
	float closeAnimProgress;
	float menuOpacity;
	float progressionRate;
	float degressionRate;
	int isBlurred;
	UNUSED(0x4);
	int isMenuBackgroundDisplayed;
	int u_layer;
	int paragraphMode;
	UNUSED(0x4);
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
};

CHECKOFFSET(MenuWindow, MenuInfoList, 0x10)
CHECKOFFSET(MenuWindow, unknown_0x5c, 0x5c)
CHECKOFFSET(MenuWindow, isRootMenu, 0x7c)
CHECKOFFSET(MenuWindow, dimScreenPercentage, 0xc0)
CHECKSIZE(MenuWindow, 0xe4)

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
	"OFF", "X1", "X2", "X3"
};

std::vector<const char*> vREVERSAL_SLOT_2 = {
	"REVERSAL SLOT 2",
	"OFF", "X1", "X2", "X3"
};

std::vector<const char*> vREVERSAL_SLOT_3 = {
	"REVERSAL SLOT 3",
	"OFF", "X1", "X2", "X3"
};

std::vector<const char*> vREVERSAL_SLOT_4 = {
	"REVERSAL SLOT 4",
	"OFF",
	"X1", "X2", "X3"
};

std::vector<const char*> vREVERSAL_DELAY = {
	"REVERSAL DELAY",
	"0", "X1", "X2", "X3"
};

std::vector<std::vector<const char*>> P1_Options = {
	vREVERSAL_TYPE, vSPACE_ELEMENT, vREVERSAL_SLOT_1, vREVERSAL_SLOT_2, vREVERSAL_SLOT_3, vREVERSAL_SLOT_4, vSPACE_ELEMENT, vREVERSAL_DELAY
};

int nREVERSAL_TYPE = 1;
int nREVERSAL_SLOT_1 = 0;
int nREVERSAL_SLOT_2 = 0;
int nREVERSAL_SLOT_3 = 0;
int nREVERSAL_SLOT_4 = 0;
int nREVERSAL_DELAY = 0;

std::vector<int> P1_Settings = {
	nREVERSAL_TYPE, nREVERSAL_SLOT_1, nREVERSAL_SLOT_2, nREVERSAL_SLOT_3, nREVERSAL_SLOT_4, nREVERSAL_DELAY
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
int nHITS_UNTIL_BURST;

std::vector<int> P2_Settings = {
	nPENALTY_RESET, nEX_GUARD, nGUARD_BAR, nMETER, nHEALTH, nHITS_UNTIL_BURST
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

std::vector<int> P3_Settings = {
	nHIGHLIGHTS, nGUARD, nHIT, nARMOR, nTHROW_PROTECTION, nIDLE
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

std::vector<int> P4_Settings = {
	nRESET_TO_POSITIONS, nP1_X_LOC, nP2_X_LOC
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

std::vector<int> P5_Settings = {
	nSION_BULLETS, nROA_VISIBLE_CHARGE, nROA_HIDDEN_CHARGE, nF_MAIDS_HEARTS, nRYOUGI_KNIFE
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

std::vector<int> P6_Settings = {
	nDISPLAY_HITBOXES, nHITBOX_STYLE, nCOLOR_BLIND_MODE, nORIGIN_STYLE, nDRAW_GROUND
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

std::vector<int> P7_Settings = {
	nSAVE_STATE_SLOT
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

std::vector<int> P8_Settings = {
	nFRAME_DATA, nIN_GAME_FRAMEBAR, nSHOW_FREEZE_AND_INPUTS, nSHOW_CANCEL_WINDOWS, nSCROLL_DISPLAY
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

std::vector<int> P9_Settings = {
	nCUSTOM_RNG, nRATE, nSEED
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

std::vector<int> P10_Settings = {
	nSHOW_STATS, nP1_INPUT_DISPLAY, nP2_INPUT_DISPLAY, nFRAME_DISPLAY_Y, nATTACK_DISPLAY
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

std::vector<int> P11_Settings = {
	nSLOW_MOTION, nHIDE_HUD, nHIDE_SHADOWS, nHIDE_EXTRAS, nBACKGROUND
};

//All pages
std::vector<std::vector<std::vector<const char*>>> Page_Options = {
	P1_Options, P2_Options, P3_Options, P4_Options, P5_Options, P6_Options, P7_Options, P8_Options, P9_Options, P10_Options, P11_Options
};

std::vector<std::vector<int>> Page_Settings = {
	P1_Settings, P2_Settings, P3_Settings, P4_Settings, P5_Settings, P6_Settings, P7_Settings, P8_Settings, P9_Settings, P10_Settings, P11_Settings
};

uint8_t nEXTENDED_SETTINGS_PAGE = 0;
uint8_t nEXTENDED_SETTINGS_CURSOR = 0;

bool bOldFN1Input = 0;
bool bOldFN2Input = 0;