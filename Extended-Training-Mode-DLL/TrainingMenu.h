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

	void SetCurItemLabel(const char* newLabel) {
		int itemIndex = selectedItem;
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

// --- Vectors guide ---
//Empty vectors are blank spaces (only need the one defined)
//Vectors with one value are elements with only a label (like the vanilla "default" and "return" elements)
//Vectors with more than one value are elements with selectable options

std::vector<const char*> vSPACE_ELEMENT = {};

std::vector<const char*> vDEFAULT_ELEMENT = {
	"DEFAULT"
};

std::vector<const char*> vPAGE_ELEMENT = {
	" ",
	"X1", "X2", "X3"
};

int nPAGE = 1;

std::vector<const char*> vRETURN_ELEMENT = {
	"RETURN"
};

//Page 1
std::vector<const char*> vREVERSAL_TYPE = {
	"DO REVERSALS",
	"OFF", "ON"
};

enum eREVERSAL_TYPE { revOFF, revNORMAL, revREPEAT };

std::vector<const char*> vREVERSAL_SLOT_1 = {
	"REVERSAL SLOT 1",
	"X1", "X2", "X3"
};

std::vector<const char*> vREV_SLOT_1_WEIGHT = {
	"WEIGHT 1",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_2 = {
	"REVERSAL SLOT 2",
	"X1", "X2", "X3"
};

std::vector<const char*> vREV_SLOT_2_WEIGHT = {
	"WEIGHT 2",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_3 = {
	"REVERSAL SLOT 3",
	"X1", "X2", "X3"
};

std::vector<const char*> vREV_SLOT_3_WEIGHT = {
	"WEIGHT 3",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_SLOT_4 = {
	"REVERSAL SLOT 4",
	"X1", "X2", "X3"
};

std::vector<const char*> vREV_SLOT_4_WEIGHT = {
	"WEIGHT 4",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vNO_REV_WEIGHT = {
	"NO REVERSAL WEIGHT",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

std::vector<const char*> vREVERSAL_DELAY = {
	"REVERSAL DELAY",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

std::vector<const char*> vP1_PAGE = {
	" ",
	"X1", "PAGE 1", "X3"
};

std::vector<std::vector<const char*>> P1_Options = {
	vREVERSAL_TYPE, vSPACE_ELEMENT,
	vREVERSAL_SLOT_1, vREV_SLOT_1_WEIGHT,
	vREVERSAL_SLOT_2, vREV_SLOT_2_WEIGHT,
	vREVERSAL_SLOT_3, vREV_SLOT_3_WEIGHT,
	vREVERSAL_SLOT_4, vREV_SLOT_4_WEIGHT, vSPACE_ELEMENT,
	vNO_REV_WEIGHT, vSPACE_ELEMENT,
	vREVERSAL_DELAY, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP1_PAGE
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
int nNO_REV_WEIGHT = 0;
int nREVERSAL_DELAY = 0;

int nREV_ID_1 = 0;
int nREV_ID_2 = 0;
int nREV_ID_3 = 0;
int nREV_ID_4 = 0;

int nREV_SHIELD_1 = 0; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_2 = 0; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_3 = 0; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]
int nREV_SHIELD_4 = 0; //0 = no shield, D, [D], 2D, 2[D], jD, j[D]

const char* REV_SHIELD_PREFIX[7] = { "", "5D > ", "5[D] > ", "2D > ", "2[D] > ", "j.D > ", "j.[D] > "};

const int NUM_REVERSALS = 4;
int* nREV_WEIGHTS[NUM_REVERSALS] = { &nREV_SLOT_1_WEIGHT, &nREV_SLOT_2_WEIGHT , &nREV_SLOT_3_WEIGHT , &nREV_SLOT_4_WEIGHT };
int* nREV_IDs[NUM_REVERSALS] = { &nREV_ID_1, &nREV_ID_2, &nREV_ID_3, &nREV_ID_4 };
int* nREV_SHIELDS[NUM_REVERSALS] = { &nREV_SHIELD_1, &nREV_SHIELD_2, &nREV_SHIELD_3, &nREV_SHIELD_4 };

std::vector<int*> P1_Settings = {
	&nREVERSAL_TYPE,
	&nREVERSAL_SLOT_1, &nREV_SLOT_1_WEIGHT,
	&nREVERSAL_SLOT_2, &nREV_SLOT_2_WEIGHT,
	&nREVERSAL_SLOT_3, &nREV_SLOT_3_WEIGHT,
	&nREVERSAL_SLOT_4, &nREV_SLOT_4_WEIGHT,
	&nNO_REV_WEIGHT,
	&nREVERSAL_DELAY,
	&nPAGE
};

//Page 2
std::vector<const char*> vPENALTY_RESET = {
	"PENALTY RESET",
	"NORMAL", "INSTANT",
};

std::vector<const char*> vGUARD_BAR_RESET = {
	"GUARD BAR RESET",
	"NORMAL", "INSTANT",
};

std::vector<const char*> vEX_GUARD = {
	"EX GUARD",
	"OFF", "ON", "RANDOM",
};

std::vector<const char*> vP1_METER = {
	"P1 METER",
	"X1", "X2", "X3",
};

std::vector<const char*> vP2_METER = {
	"P2 METER",
	"X1", "X2", "X3",
};

std::vector<const char*> vP1_HEALTH = {
	"P1 HEALTH",
	"X1", "X2", "X3",
};

std::vector<const char*> vP2_HEALTH = {
	"P2 HEALTH",
	"X1", "X2", "X3",
};

std::vector<const char*> vHITS_UNTIL_BURST = {
	"HITS UNTIL BURST",
	"X1", "X2", "X3"
};

std::vector<const char*> vHITS_UNTIL_BUNKER = {
	"HITS UNTIL BUNKER",
	"X1", "X2", "X3"
};

std::vector<const char*> vP2_PAGE = {
	" ",
	"X1", "PAGE 2", "X3"
};

std::vector<std::vector<const char*>> P2_Options = {
	vPENALTY_RESET, vGUARD_BAR_RESET, vSPACE_ELEMENT,
	vEX_GUARD, vSPACE_ELEMENT,
	vP1_METER, vP2_METER, vSPACE_ELEMENT,
	vP1_HEALTH, vP2_HEALTH, vSPACE_ELEMENT,
	vHITS_UNTIL_BURST, vHITS_UNTIL_BUNKER, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP2_PAGE
};

const int defPEN_RESET = 0;
const int defGUARD_RESET = 0;
const int defEX_GUARD = 0;
const int defP1_METER = 1;
const int defP2_METER = 1;
const int defP1_HEALTH = 2;
const int defP2_HEALTH = 2;
const int defHITS_BURST = 0;
const int defHITS_BUNKER = 0;

int nPENALTY_RESET = defPEN_RESET;
int nGUARD_BAR_RESET = defGUARD_RESET;
int nEX_GUARD = defEX_GUARD;
int nP1_METER = defP1_METER;
int nP2_METER = defP2_METER;
int nP1_HEALTH = defP1_HEALTH;
int nP2_HEALTH = defP2_HEALTH;
int nHITS_UNTIL_BURST = defHITS_BURST;
int nHITS_UNTIL_BUNKER = defHITS_BUNKER;

const int defTRUE_P1_METER = 10000;
const int defTRUE_P2_METER = 10000;
const int defTRUE_P1_HEALTH = 11400;
const int defTRUE_P2_HEALTH = 11400;
const int defTRUE_HITS_BURST = 0;
const int defTRUE_HITS_BUNKER = 0;

int nTRUE_P1_METER = defTRUE_P1_METER;
int nTRUE_P2_METER = defTRUE_P2_METER;
int nTRUE_P1_HEALTH = defTRUE_P1_HEALTH;
int nTRUE_P2_HEALTH = defTRUE_P2_HEALTH;
int nTRUE_HITS_UNTIL_BURST = defTRUE_HITS_BURST;
int nTRUE_HITS_UNTIL_BUNKER = defTRUE_HITS_BUNKER;

std::vector<int*> P2_Settings = {
	&nPENALTY_RESET, &nGUARD_BAR_RESET, &nEX_GUARD,
	&nP1_METER, &nP2_METER, &nP1_HEALTH, &nP2_HEALTH,
	&nHITS_UNTIL_BURST, &nHITS_UNTIL_BUNKER,
	&nPAGE
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

std::vector<const char*> vP3_PAGE = {
	" ",
	"X1", "PAGE 3", "X3"
};

std::vector<std::vector<const char*>> P3_Options = {
	vHIGHLIGHTS, vSPACE_ELEMENT, vGUARD, vHIT, vARMOR, vTHROW_PROTECTION, vIDLE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP3_PAGE
};

const int defHIGHLIGHTS = 0;
const int defGUARD = 0;
const int defHIT = 0;
const int defARMOR = 0;
const int defTHROW_PROT = 0;
const int defIDLE = 0;

int nHIGHLIGHTS = defHIGHLIGHTS;
int nGUARD = defGUARD;
int nHIT = defHIT;
int nARMOR = defARMOR;
int nTHROW_PROTECTION = defTHROW_PROT;
int nIDLE = defIDLE;

std::vector<int*> P3_Settings = {
	&nHIGHLIGHTS, &nGUARD, &nHIT, &nARMOR, &nTHROW_PROTECTION, &nIDLE,
	&nPAGE
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

std::vector<const char*> vP1_ASSIST_X_LOC = {
	"P1 ASSIST X-LOC",
	"X1", "X2", "X3"
};

std::vector<const char*> vP2_X_LOC = {
	"P2 X-LOC",
	"X1", "X2", "X3"
};

std::vector<const char*> vP2_ASSIST_X_LOC = {
	"P2 ASSIST X-LOC",
	"X1", "X2", "X3"
};

std::vector<const char*> vMOVE_TO_POSITIONS = {
	"MOVE TO POSITIONS"
};

std::vector<const char*> vINVERT = {
	"INVERT"
};

std::vector<const char*> vP4_PAGE = {
	" ",
	"X1", "PAGE 4", "X3"
};

std::vector<std::vector<const char*>> P4_Options = {
	vRESET_TO_POSITIONS, vSPACE_ELEMENT,
	vP1_X_LOC, vP1_ASSIST_X_LOC, vSPACE_ELEMENT,
	vP2_X_LOC, vP2_ASSIST_X_LOC, vSPACE_ELEMENT,
	vMOVE_TO_POSITIONS, vINVERT, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP4_PAGE
};

const int defRESET_POS = 0;
const int defP1_X = 1;
const int defP1_ASSIST_X = 1;
const int defP2_X = 1;
const int defP2_ASSIST_X = 1;

int nRESET_TO_POSITIONS = defRESET_POS;
int nP1_X_LOC = defP1_X;
int nP1_ASSIST_X_LOC = defP1_ASSIST_X;
int nP2_X_LOC = defP2_X;
int nP2_ASSIST_X_LOC = defP2_ASSIST_X;

const int defTRUE_P1_X = -0x4000;
const int defTRUE_P1_ASSIST_X = -0x7200;
const int defTRUE_P2_X = 0x4000;
const int defTRUE_P2_ASSIST_X = 0x7200;

int nTRUE_P1_X_LOC = defTRUE_P1_X;
int nTRUE_P1_ASSIST_X_LOC = defTRUE_P1_ASSIST_X;
int nTRUE_P2_X_LOC = defTRUE_P2_X;
int nTRUE_P2_ASSIST_X_LOC = defTRUE_P2_ASSIST_X;

std::vector<int*> P4_Settings = {
	&nRESET_TO_POSITIONS, &nP1_X_LOC, &nP1_ASSIST_X_LOC, &nP2_X_LOC, &nP2_ASSIST_X_LOC,
	&nPAGE
};

//Page 5
std::vector<const char*> vSION_BULLETS = {
	"SION BULLETS",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"
};

std::vector<const char*> vROA_VISIBLE_CHARGE = {
	"ROA VISIBLE CHARGE",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

std::vector<const char*> vROA_HIDDEN_CHARGE = {
	"ROA HIDDEN CHARGE",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

std::vector<const char*> vF_MAIDS_HEARTS = {
	"F-MAIDS HEARTS",
	"INFINITE", "NORMAL", "4", "3", "2", "1" ,"0"
};

std::vector<const char*> vRYOUGI_KNIFE = {
	"RYOUGI KNIFE",
	"INFINITE", "NORMAL"
};

std::vector<const char*> vP5_PAGE = {
	" ",
	"X1", "PAGE 5", "X3"
};

std::vector<std::vector<const char*>> P5_Options = {
	vSION_BULLETS, vSPACE_ELEMENT, vROA_VISIBLE_CHARGE, vROA_HIDDEN_CHARGE, vSPACE_ELEMENT,
	vF_MAIDS_HEARTS, vSPACE_ELEMENT,
	vRYOUGI_KNIFE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP5_PAGE
};

const int defSION_BULLETS = 1;
const int defROA_VISIBLE = 1;
const int defROA_HIDDEN = 1;
const int defMAIDS_HEARTS = 1;
const int defRYOUGI_KNIFE = 1;

int nSION_BULLETS = defSION_BULLETS;
int nROA_VISIBLE_CHARGE = defROA_VISIBLE;
int nROA_HIDDEN_CHARGE = defROA_HIDDEN;
int nF_MAIDS_HEARTS = defMAIDS_HEARTS;
int nRYOUGI_KNIFE = defRYOUGI_KNIFE;

std::vector<int*> P5_Settings = {
	&nSION_BULLETS, &nROA_VISIBLE_CHARGE, &nROA_HIDDEN_CHARGE, &nF_MAIDS_HEARTS, &nRYOUGI_KNIFE,
	&nPAGE
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

std::vector<const char*> vP6_PAGE = {
	" ",
	"X1", "PAGE 6", "X3"
};

std::vector<std::vector<const char*>> P6_Options = {
	vDISPLAY_HITBOXES, vHITBOX_STYLE, vCOLOR_BLIND_MODE, vSPACE_ELEMENT,
	vORIGIN_STYLE, vSPACE_ELEMENT,
	vDRAW_GROUND, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP6_PAGE
};

const int defDISPLAY_HITBOXES = 0;
const int defHITBOX_STYLE = 0;
const int defCOLOR_BLIND = 0;
const int defORIGIN_STYLE = 0;
const int defDRAW_GROUND = 0;

int nDISPLAY_HITBOXES = defDISPLAY_HITBOXES;
int nHITBOX_STYLE = defHITBOX_STYLE;
int nCOLOR_BLIND_MODE = defCOLOR_BLIND;
int nORIGIN_STYLE = defORIGIN_STYLE;
int nDRAW_GROUND = defDRAW_GROUND;

std::vector<int*> P6_Settings = {
	&nDISPLAY_HITBOXES, &nHITBOX_STYLE, &nCOLOR_BLIND_MODE, &nORIGIN_STYLE, &nDRAW_GROUND,
	&nPAGE
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

std::vector<const char*> vP7_PAGE = {
	" ",
	"X1", "PAGE 7", "X3"
};

std::vector<std::vector<const char*>> P7_Options = {
	vSAVE_STATE_SLOT, vSPACE_ELEMENT,
	vSAVE_STATE, vCLEAR_ALL_SAVES, vSPACE_ELEMENT,
	vIMPORT_SAVE, vEXPORT_SAVE, vSPACE_ELEMENT,
	vLOAD_RNG, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP7_PAGE
};

const int defSAVE_SLOT = 1;
const int defLOAD_RNG = 0;

int nSAVE_STATE_SLOT = defSAVE_SLOT;
int nLOAD_RNG = defLOAD_RNG;

std::vector<int*> P7_Settings = {
	&nSAVE_STATE_SLOT, &nLOAD_RNG,
	&nPAGE
};

//Page 8
std::vector<const char*> vFRAME_DATA = {
	"FRAME DATA",
	"NORMAL", "ADVANCED"
};

std::vector<const char*> vIN_GAME_FRAME_DISPLAY = {
	"IN-GAME FRAME DISPLAY",
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
	"X1", "X2", "X3"
};

std::vector<const char*> vCOLOR_GUIDE = {
	"COLOR GUIDE"
};

std::vector<const char*> vP8_PAGE = {
	" ",
	"X1", "PAGE 8", "X3"
};

std::vector<std::vector<const char*>> P8_Options = {
	vFRAME_DATA, vIN_GAME_FRAME_DISPLAY, vSPACE_ELEMENT,
	vSHOW_FREEZE_AND_INPUTS, vSHOW_CANCEL_WINDOWS, vSPACE_ELEMENT,
	vSCROLL_DISPLAY, vSPACE_ELEMENT,
	vCOLOR_GUIDE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP8_PAGE
};

const int defFRAME_DATA = 0;
const int defIN_GAME_FRAME_DISPLAY = 0;
const int defSHOW_FREEZE_INPUTS = 0;
const int defSHOW_CANCEL = 0;
const int defSCROLL_DISPLAY = 2;

int nFRAME_DATA = defFRAME_DATA;
int nIN_GAME_FRAME_DISPLAY = defIN_GAME_FRAME_DISPLAY;
int nSHOW_FREEZE_AND_INPUTS = defSHOW_FREEZE_INPUTS;
int nSHOW_CANCEL_WINDOWS = defSHOW_CANCEL;
int nSCROLL_DISPLAY = defSCROLL_DISPLAY;

const int defTRUE_SCROLL_DISPLAY = 0;

int nTRUE_SCROLL_DISPLAY = defTRUE_SCROLL_DISPLAY;

std::vector<int*> P8_Settings = {
	&nFRAME_DATA, &nIN_GAME_FRAME_DISPLAY, &nSHOW_FREEZE_AND_INPUTS, &nSHOW_CANCEL_WINDOWS, &nSCROLL_DISPLAY,
	&nPAGE
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

std::vector<const char*> vP9_PAGE = {
	" ",
	"X1", "PAGE 9", "X3"
};

std::vector<std::vector<const char*>> P9_Options = {
	vCUSTOM_RNG, vSPACE_ELEMENT,
	vRATE, vSEED, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP9_PAGE
};

const int defCUSTOM_RNG = 0;
const int defRATE = 0;
const int defSEED = 0;

int nCUSTOM_RNG = defCUSTOM_RNG;
int nRATE = defRATE;
int nSEED = defSEED;

const int defTRUE_SEED = 0;

int nTRUE_SEED = defTRUE_SEED;

std::vector<int*> P9_Settings = {
	&nCUSTOM_RNG, &nRATE, &nSEED,
	&nPAGE
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

std::vector<const char*> vP10_PAGE = {
	" ",
	"X1", "PAGE 10", "X3"
};

std::vector<std::vector<const char*>> P10_Options = {
	vSHOW_STATS, vSPACE_ELEMENT,
	vP1_INPUT_DISPLAY, vP2_INPUT_DISPLAY, vSPACE_ELEMENT,
	vFRAME_DISPLAY_Y, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP10_PAGE
};

const int defSHOW_STATS = 1;
const int defP1_INPUT = 0;
const int defP2_INPUT = 0;
const int defFRAME_DISPLAY_Y = 1;

int nSHOW_STATS = defSHOW_STATS;
int nP1_INPUT_DISPLAY = defP1_INPUT;
int nP2_INPUT_DISPLAY = defP2_INPUT;
int nFRAME_DISPLAY_Y = defFRAME_DISPLAY_Y;

const int defTRUE_FRAME_DISPLAY_Y = 400;

int nTRUE_FRAME_DISPLAY_Y = defTRUE_FRAME_DISPLAY_Y;

std::vector<int*> P10_Settings = {
	&nSHOW_STATS, &nP1_INPUT_DISPLAY, &nP2_INPUT_DISPLAY, &nFRAME_DISPLAY_Y,
	&nPAGE
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

std::vector<const char*> vP11_PAGE = {
	" ",
	"X1", "PAGE 11", "X3"
};

std::vector<std::vector<const char*>> P11_Options = {
	vSLOW_MOTION, vSPACE_ELEMENT,
	vHIDE_HUD, vHIDE_SHADOWS, vHIDE_EXTRAS, vSPACE_ELEMENT,
	vBACKGROUND, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP11_PAGE
};

const int defSLOW_MOTION = 0;
const int defHIDE_HUD = 0;
const int defHIDE_SHADOWS = 0;
const int defHIDE_EXTRAS = 0;
const int defBACKGROUND = 0;

int nSLOW_MOTION = defSLOW_MOTION;
int nHIDE_HUD = defHIDE_HUD;
int nHIDE_SHADOWS = defHIDE_SHADOWS;
int nHIDE_EXTRAS = defHIDE_EXTRAS;
int nBACKGROUND = defBACKGROUND;

std::vector<int*> P11_Settings = {
	&nSLOW_MOTION, &nHIDE_HUD, &nHIDE_SHADOWS, &nHIDE_EXTRAS, &nBACKGROUND,
	&nPAGE
};

//All pages
std::vector<std::vector<std::vector<const char*>>> Page_Options = {
	P1_Options, P2_Options, P3_Options, P4_Options, P5_Options, P6_Options, P7_Options, P8_Options, P9_Options, P10_Options, P11_Options
};

std::vector<std::vector<int*>> Page_Settings = {
	P1_Settings, P2_Settings, P3_Settings, P4_Settings, P5_Settings, P6_Settings, P7_Settings, P8_Settings, P9_Settings, P10_Settings, P11_Settings
};

const int XS_NUM_PAGES = 11;

uint8_t nEXTENDED_SETTINGS_PAGE = 0;
uint8_t nEXTENDED_SETTINGS_CURSOR[XS_NUM_PAGES] = { 0 };

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