#pragma once
#include <cstddef>
#include <vector>
#include "..\Common\Common.h"
#include "..\Common\types.h"
#include <map>

extern Point trueFrameDisplayPt;

const DWORD MBAA_ReadDataFile = 0x00407c10;
const DWORD MBAA_StringCopyFromIndex = 0x004079d0;
const DWORD MBAA_SetInfoWindowText = 0x004d9ba0;
const DWORD MBAA_CompareSSOString = 0x0042bdc0;
//wrapper for call to ReadDataFile (misleading name? pulled straight from ghidra)
//subtract 4 bytes from actual destination to get dest input
void ReadDataFile(void* dest, const char* name, int nameLength);

void CopyStringFromIndex(void* dest, void* source, int startIndex, int copyLength);

void SetInfoWindowText(const char* text, void* dest);

struct MenuString
{
	int base;
	union {
		char* pLongString;
		char shortString[0x10];
	};
	int length;
	int maxLength;
};

bool CompareSSOString(const char* compareTo, MenuString* str);

template <typename T>
struct MenuList
{
	int base;
	T* listStart;
	T* listEnd;
	T* listMaxEnd;
};

int GetHotkeyPressed();

void CheckNewHotkey(bool bReset, KeyState& oHotkey, LPCTSTR sRegKey);

void GetKeyStateMenuLabel(char* buffer, KeyState oHotkey);

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define UNUSED(length) BYTE LINE_NAME[length]

#define CHECKOFFSET(s, v, n) static_assert(offsetof(s, v) == n, #s " offset incorrect for " #v ". Expected " #n);
#define CHECKSIZE(s, n) static_assert(sizeof(s) == n, #s " size must be " #n);

#pragma pack(push, 1)

struct Item //scrolling items on right
{
	MenuString label;
	MenuString tag;
	int value;
};

CHECKOFFSET(Item, tag, 0x1c)
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
	MenuString label;
	MenuString tag;
	int selectedItem;
	int selectItemLabelXOffset;
	MenuList<Item*> itemList;

	void SetItemLabel(const char* newLabel, int itemIndex);

	void SetCurItemLabel(const char* newLabel);

	//vftable[6]
	int GetItemValue(int itemIndex);
	
	//vftable[9] sets selectedItem

	//vftable[10] gets selectedItem

	//vftable[11]
	int GetItemListSize();
};

CHECKOFFSET(Element, label, 0x20)
CHECKOFFSET(Element, tag, 0x3c)
CHECKOFFSET(Element, itemList, 0x60)
CHECKSIZE(Element, 0x70)

struct MenuWindow;

struct MenuInfo
{
	void* vftable;
	MenuWindow* parentWindow;
	MenuString label;
	MenuString blank;
	int selectedElement;
	int prevSelectedElement;
	MenuList<Element*> elementList;
	MenuList<MenuString> selectableList;
	int finishedDrawing;
	int timeDisplayed;
	int field_0x70;
	int close;
};

CHECKOFFSET(MenuInfo, label, 0x8)
CHECKOFFSET(MenuInfo, elementList, 0x48)
CHECKOFFSET(MenuInfo, selectableList, 0x58)
CHECKSIZE(MenuInfo, 0x78)

struct TagInfoPair
{
	MenuString tag;
	MenuString info;
};

CHECKSIZE(TagInfoPair, 0x38)

struct InfoWindow
{
	int mode;
	int timeOpen;
	MenuString label;
	MenuString elementTag;
	MenuString itemTag;
	void* mainInfo;
	void* subInfo;
	MenuString font;
	int fontSize;
	int field_0x84;
	int fontThickness;
	MenuList<TagInfoPair*> tagInfoPairList;
	MenuString curTagChecked;
	MenuString curInfo;
	int zLayer;
	int yPosition;
	int windowHeight;
	int field_0xe0;
	int field_0xe4;
	float animationProgress;
};

CHECKOFFSET(InfoWindow, font, 0x64)
CHECKOFFSET(InfoWindow, curInfo, 0xb8)
CHECKSIZE(InfoWindow, 0xec)

struct MenuWindow
{
	void* vftable;
	int menuInfoIndex;
	int field_0x8;
	MenuList<MenuInfo*> menuInfoList;
	int didPress;
	MenuString hoveredTag;
	int field_0x3c;
	int bgXOffset;
	int yOffset;
	int someYOffset;
	int elementsXOffset;
	int field_0x50;
	int field_0x54;
	int field_0x58;
	MenuString label;
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
	InfoWindow* InformationMenu;
	MenuWindow* ExtendedSettings;
	MenuWindow* HotkeySettings;

	void SetLabel(const char* newLabel);
	void GetHoveredItemTag();
};

CHECKOFFSET(MenuWindow, menuInfoList, 0xc)
CHECKOFFSET(MenuWindow, label, 0x5c)
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

const std::vector<const char*> vSPACE_ELEMENT = {};

const std::vector<const char*> vDEFAULT_ELEMENT = {
	"DEFAULT"
};

extern int nPAGE;

const std::vector<const char*> vRETURN_ELEMENT = {
	"RETURN"
};

//Page 1
const std::vector<const char*> vREVERSAL_TYPE = {
	"REVERSALS",
	"OFF", "ON", "ON GUARD", "ON HIT", "ON WAKEUP", "ON SHIELD"
};

const std::vector<const char*> vREVERSAL_SLOT_1 = {
	"REVERSAL SLOT 1",
	"X1", "X2", "X3"
};

const std::vector<const char*> vREV_SLOT_1_WEIGHT = {
	"WEIGHT 1",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

const std::vector<const char*> vREVERSAL_SLOT_2 = {
	"REVERSAL SLOT 2",
	"X1", "X2", "X3"
};

const std::vector<const char*> vREV_SLOT_2_WEIGHT = {
	"WEIGHT 2",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

const std::vector<const char*> vREVERSAL_SLOT_3 = {
	"REVERSAL SLOT 3",
	"X1", "X2", "X3"
};

const std::vector<const char*> vREV_SLOT_3_WEIGHT = {
	"WEIGHT 3",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

const std::vector<const char*> vREVERSAL_SLOT_4 = {
	"REVERSAL SLOT 4",
	"X1", "X2", "X3"
};

const std::vector<const char*> vREV_SLOT_4_WEIGHT = {
	"WEIGHT 4",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

const std::vector<const char*> vNO_REV_WEIGHT = {
	"NO REVERSAL WEIGHT",
	"0", "1", "2" ,"3", "4", "5", "6" ,"7", "8", "9", "10"
};

const std::vector<const char*> vREVERSAL_DELAY = {
	"REVERSAL DELAY",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"
};

const std::vector<const char*> vP1_PAGE = {
	" ",
	"X1", "PAGE 1", "X3"
};

const std::vector<std::vector<const char*>> P1_Options = {
	vREVERSAL_TYPE, vSPACE_ELEMENT,
	vREVERSAL_SLOT_1, vREV_SLOT_1_WEIGHT,
	vREVERSAL_SLOT_2, vREV_SLOT_2_WEIGHT,
	vREVERSAL_SLOT_3, vREV_SLOT_3_WEIGHT,
	vREVERSAL_SLOT_4, vREV_SLOT_4_WEIGHT, vSPACE_ELEMENT,
	vNO_REV_WEIGHT, vSPACE_ELEMENT,
	vREVERSAL_DELAY, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP1_PAGE
};

enum class eREVERSALS {
	REVERSAL_TYPE,
	S0,
	REVERSAL_SLOT_1,
	WEIGHT_1,
	REVERSAL_SLOT_2,
	WEIGHT_2,
	REVERSAL_SLOT_3,
	WEIGHT_3,
	REVERSAL_SLOT_4,
	WEIGHT_4,
	S1,
	NO_REVERSAL_WEIGHT,
	S2,
	REVERSAL_DELAY,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

const int defREVERSAL_TYPE = 1;
const int defREVERSAL_SLOT_1 = 1;
const int defREV_SLOT_1_WEIGHT = 1;
const int defREVERSAL_SLOT_2 = 1;
const int defREV_SLOT_2_WEIGHT = 1;
const int defREVERSAL_SLOT_3 = 1;
const int defREV_SLOT_3_WEIGHT = 1;
const int defREVERSAL_SLOT_4 = 1;
const int defREV_SLOT_4_WEIGHT = 1;
const int defNO_REV_WEIGHT = 0;
const int defREVERSAL_DELAY = 0;

extern int nREVERSAL_TYPE;
extern int nREVERSAL_SLOT_1;
extern int nREV_SLOT_1_WEIGHT;
extern int nREVERSAL_SLOT_2;
extern int nREV_SLOT_2_WEIGHT;
extern int nREVERSAL_SLOT_3;
extern int nREV_SLOT_3_WEIGHT;
extern int nREVERSAL_SLOT_4;
extern int nREV_SLOT_4_WEIGHT;
extern int nNO_REV_WEIGHT;
extern int nREVERSAL_DELAY;

const int defREV_ID = 0;

extern int nREV_ID_1;
extern int nREV_ID_2;
extern int nREV_ID_3;
extern int nREV_ID_4;

const char* const REV_SHIELD_PREFIX[7] = { "", "5D > ", "5[D] > ", "2D > ", "2[D] > ", "j.D > ", "j.[D] > "};

const int NUM_REVERSALS = 4;

const int* const nREV_WEIGHTS[NUM_REVERSALS] = { &nREV_SLOT_1_WEIGHT, &nREV_SLOT_2_WEIGHT , &nREV_SLOT_3_WEIGHT , &nREV_SLOT_4_WEIGHT };
const int* const nREV_IDs[NUM_REVERSALS] = { &nREV_ID_1, &nREV_ID_2, &nREV_ID_3, &nREV_ID_4 };

const std::vector<int*> P1_Settings = {
	&nREVERSAL_TYPE,
	&nREVERSAL_SLOT_1, &nREV_SLOT_1_WEIGHT,
	&nREVERSAL_SLOT_2, &nREV_SLOT_2_WEIGHT,
	&nREVERSAL_SLOT_3, &nREV_SLOT_3_WEIGHT,
	&nREVERSAL_SLOT_4, &nREV_SLOT_4_WEIGHT,
	&nNO_REV_WEIGHT,
	&nREVERSAL_DELAY,
	&nPAGE
};

void DefaultP1(MenuInfo* menuInfo);

//Page 2
const std::vector<const char*> vPENALTY_RESET = {
	"PENALTY RESET",
	"NORMAL", "INSTANT",
};

const std::vector<const char*> vGUARD_BAR_RESET = {
	"GUARD BAR RESET",
	"NORMAL", "INSTANT",
};

const std::vector<const char*> vEX_GUARD = {
	"EX GUARD",
	"OFF", "ON", "RANDOM",
};

const std::vector<const char*> vP1_METER = {
	"P1 METER",
	"X1", "X2", "X3",
};

const std::vector<const char*> vP2_METER = {
	"P2 METER",
	"X1", "X2", "X3",
};

const std::vector<const char*> vP1_HEALTH = {
	"P1 HEALTH",
	"X1", "X2", "X3",
};

const std::vector<const char*> vP2_HEALTH = {
	"P2 HEALTH",
	"X1", "X2", "X3",
};

const std::vector<const char*> vHITS_UNTIL_BURST = {
	"HITS UNTIL BURST",
	"X1", "X2", "X3"
};

const std::vector<const char*> vHITS_UNTIL_BUNKER = {
	"HITS UNTIL BUNKER",
	"X1", "X2", "X3"
};

const std::vector<const char*> vHITS_UNTIL_FORCE_GUARD = {
	"HITS UNTIL FORCE GUARD",
	"X1", "X2", "X3"
};

const std::vector<const char*> vFORCE_GUARD_STANCE = {
	"FORCE GUARD STANCE",
	"STAND", "CROUCH"
};

const std::vector<const char*> vP2_PAGE = {
	" ",
	"X1", "PAGE 2", "X3"
};

const std::vector<std::vector<const char*>> P2_Options = {
	vPENALTY_RESET, vGUARD_BAR_RESET, vEX_GUARD, vSPACE_ELEMENT,
	vP1_METER, vP2_METER, vSPACE_ELEMENT,
	vP1_HEALTH, vP2_HEALTH, vSPACE_ELEMENT,
	vHITS_UNTIL_BURST, vHITS_UNTIL_BUNKER, vHITS_UNTIL_FORCE_GUARD, vFORCE_GUARD_STANCE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP2_PAGE
};

enum class eTRAINING {
	PENALTY_RESET,
	GUARD_BAR_RESET,
	EX_GUARD,
	S1,
	P1_METER,
	P2_METER,
	S2,
	P1_HEALTH,
	P2_HEALTH,
	S3,
	HITS_UNTIL_BURST,
	HITS_UNTIL_BUNKER,
	HITS_UNTIL_FORCE_GUARD,
	FORCE_GUARD_STANCE,
	S4,
	DEFAULT,
	S5,
	RETURN,
	S6,
	PAGE
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
const int defHITS_FORCE_GUARD = 0;
const int defFORCE_GUARD_STANCE = 0;

extern int nPENALTY_RESET;
extern int nGUARD_BAR_RESET;
extern int nEX_GUARD;
extern int nP1_METER;
extern int nP2_METER;
extern int nP1_HEALTH;
extern int nP2_HEALTH;
extern int nHITS_UNTIL_BURST;
extern int nHITS_UNTIL_BUNKER;
extern int nHITS_UNTIL_FORCE_GUARD;
extern int nFORCE_GUARD_STANCE;

const int defTRUE_P1_METER = 10000;
const int defTRUE_P2_METER = 10000;
const int defTRUE_P1_HEALTH = 11400;
const int defTRUE_P2_HEALTH = 11400;
const int defTRUE_HITS_BURST = 0;
const int defTRUE_HITS_BUNKER = 0;
const int defTRUE_HITS_FORCE_GUARD = 0;

extern int nTRUE_P1_METER;
extern int nTRUE_P2_METER;
extern int nTRUE_P1_HEALTH;
extern int nTRUE_P2_HEALTH;
extern int nTRUE_HITS_UNTIL_BURST;
extern int nTRUE_HITS_UNTIL_BUNKER;
extern int nTRUE_HITS_UNTIL_FORCE_GUARD;

const std::vector<int*> P2_Settings = {
	&nPENALTY_RESET, &nGUARD_BAR_RESET, &nEX_GUARD,
	&nP1_METER, &nP2_METER, &nP1_HEALTH, &nP2_HEALTH,
	&nHITS_UNTIL_BURST, &nHITS_UNTIL_BUNKER,
	&nHITS_UNTIL_FORCE_GUARD, &nFORCE_GUARD_STANCE,
	&nPAGE
};

void DefaultP2(MenuInfo* menuInfo);

//Page 3
const std::vector<const char*> vHIGHLIGHTS = {
	"HIGHLIGHTS",
	"OFF", "ON"
};

const std::vector<const char*> vGUARD = {
	"GUARD",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

const std::vector<const char*> vHIT = {
	"HIT",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

const std::vector<const char*> vARMOR = {
	"ARMOR",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

const std::vector<const char*> vTHROW_PROTECTION = {
	"THROW PROTECTION",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

const std::vector<const char*> vIDLE = {
	"IDLE",
	"OFF", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE", "BLACK"
};

const std::vector<const char*> vP3_PAGE = {
	" ",
	"X1", "PAGE 3", "X3"
};

const std::vector<std::vector<const char*>> P3_Options = {
	vHIGHLIGHTS, vSPACE_ELEMENT, vGUARD, vHIT, vARMOR, vTHROW_PROTECTION, vIDLE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP3_PAGE
};

enum class eHIGHLIGHTS {
	HIGHLIGHTS,
	S0,
	GUARD,
	HIT,
	ARMOR,
	THROW_PROTECTION,
	IDLE,
	S1,
	DEFAULT,
	S2,
	RETURN,
	S3,
	PAGE
};

const int defHIGHLIGHTS = 0;
const int defGUARD_HIGHLIGHT = 0;
const int defHIT_HIGHLIGHT = 0;
const int defARMOR_HIGHLIGHT = 0;
const int defTHROW_PROT_HIGHLIGHT = 0;
const int defIDLE_HIGHLIGHT = 0;

extern int nHIGHLIGHTS;
extern int nGUARD_HIGHLIGHT;
extern int nHIT_HIGHLIGHT;
extern int nARMOR_HIGHLIGHT;
extern int nTHROW_PROTECTION_HIGHLIGHT;
extern int nIDLE_HIGHLIGHT;

const std::vector<int*> P3_Settings = {
	&nHIGHLIGHTS, &nGUARD_HIGHLIGHT, &nHIT_HIGHLIGHT, &nARMOR_HIGHLIGHT, &nTHROW_PROTECTION_HIGHLIGHT, &nIDLE_HIGHLIGHT,
	&nPAGE
};

void DefaultP3(MenuInfo* menuInfo);

//Page 4
const std::vector<const char*> vRESET_TO_POSITIONS = {
	"RESET TO POSITIONS",
	"NO", "YES"
};

const std::vector<const char*> vP1_POSITION = {
	"P1 POSITION",
	"X1", "X2", "X3"
};

const std::vector<const char*> vP1_ASSIST_POSITION = {
	"P1 ASSIST POSITION",
	"X1", "X2", "X3"
};

const std::vector<const char*> vP2_POSITION = {
	"P2 POSITION",
	"X1", "X2", "X3"
};

const std::vector<const char*> vP2_ASSIST_POSITION = {
	"P2 ASSIST POSITION",
	"X1", "X2", "X3"
};

const std::vector<const char*> vMOVE_TO_POSITIONS = {
	"MOVE TO POSITIONS"
};

const std::vector<const char*> vINVERT = {
	"INVERT"
};

const std::vector<const char*> vP4_PAGE = {
	" ",
	"X1", "PAGE 4", "X3"
};

const std::vector<std::vector<const char*>> P4_Options = {
	vRESET_TO_POSITIONS, vSPACE_ELEMENT,
	vP1_POSITION, vP1_ASSIST_POSITION, vSPACE_ELEMENT,
	vP2_POSITION, vP2_ASSIST_POSITION, vSPACE_ELEMENT,
	vMOVE_TO_POSITIONS, vINVERT, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP4_PAGE
};

enum class ePOSITIONS {
	RESET_TO_POSITIONS,
	S0,
	P1_POSITION,
	P1_ASSIST_POSITION,
	S1,
	P2_POSITION,
	P2_ASSIST_POSITION,
	S2,
	MOVE_TO_POSITIONS,
	INVERT,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

const int defRESET_POS = 0;
const int defP1_X = 1;
const int defP1_ASSIST_X = 1;
const int defP2_X = 1;
const int defP2_ASSIST_X = 1;

extern int nRESET_TO_POSITIONS;
extern int nP1_X_LOC;
extern int nP1_ASSIST_X_LOC;
extern int nP2_X_LOC;
extern int nP2_ASSIST_X_LOC;

const int defTRUE_P1_X = -0x4000;
const int defTRUE_P1_ASSIST_X = -0x7200;
const int defTRUE_P2_X = 0x4000;
const int defTRUE_P2_ASSIST_X = 0x7200;

extern int nTRUE_P1_X_LOC;
extern int nTRUE_P1_ASSIST_X_LOC;
extern int nTRUE_P2_X_LOC;
extern int nTRUE_P2_ASSIST_X_LOC;

const std::vector<int*> P4_Settings = {
	&nRESET_TO_POSITIONS, &nP1_X_LOC, &nP1_ASSIST_X_LOC, &nP2_X_LOC, &nP2_ASSIST_X_LOC,
	&nPAGE
};

void DefaultP4(MenuInfo* menuInfo);

//Page 5
const std::vector<const char*> vSION_BULLETS = {
	"SION BULLETS",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"
};

const std::vector<const char*> vROA_VISIBLE_CHARGE = {
	"ROA VISIBLE CHARGE",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

const std::vector<const char*> vROA_HIDDEN_CHARGE = {
	"ROA HIDDEN CHARGE",
	"INFINITE", "NORMAL", "1", "2", "3", "4", "5", "6", "7", "8", "9"
};

const std::vector<const char*> vF_MAIDS_HEARTS = {
	"F-MAIDS HEARTS",
	"INFINITE", "NORMAL", "4", "3", "2", "1" ,"0"
};

const std::vector<const char*> vRYOUGI_KNIFE = {
	"RYOUGI KNIFE",
	"INFINITE", "NORMAL"
};

const std::vector<const char*> vP5_PAGE = {
	" ",
	"X1", "PAGE 5", "X3"
};

const std::vector<std::vector<const char*>> P5_Options = {
	vSION_BULLETS, vSPACE_ELEMENT, vROA_VISIBLE_CHARGE, vROA_HIDDEN_CHARGE, vSPACE_ELEMENT,
	vF_MAIDS_HEARTS, vSPACE_ELEMENT,
	vRYOUGI_KNIFE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP5_PAGE
};

enum class eCHARACTER {
	SION_BULLETS,
	S0,
	ROA_VISIBLE_CHARGES,
	ROA_HIDDEN_CHARGES,
	S1,
	F_MAIDS_HEARTS,
	S2,
	RYOUGI_KNIFE,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

const int defSION_BULLETS = 1;
const int defROA_VISIBLE = 1;
const int defROA_HIDDEN = 1;
const int defMAIDS_HEARTS = 1;
const int defRYOUGI_KNIFE = 1;

extern int nSION_BULLETS;
extern int nROA_VISIBLE_CHARGE;
extern int nROA_HIDDEN_CHARGE;
extern int nF_MAIDS_HEARTS;
extern int nRYOUGI_KNIFE;

const std::vector<int*> P5_Settings = {
	&nSION_BULLETS, &nROA_VISIBLE_CHARGE, &nROA_HIDDEN_CHARGE, &nF_MAIDS_HEARTS, &nRYOUGI_KNIFE,
	&nPAGE
};

void DefaultP5(MenuInfo* menuInfo);

//Page 6
const std::vector<const char*> vDISPLAY_HITBOXES = {
	"DISPLAY HITBOXES",
	"OFF", "ON"
};

const std::vector<const char*> vHITBOX_STYLE = {
	"HITBOX STYLE",
	"LAYERED", "BLENDED"
};

const std::vector<const char*> vCOLOR_BLIND_MODE = {
	"COLOR BLIND MODE",
	"OFF", "ON"
};

const std::vector<const char*> vORIGIN_STYLE = {
	"ORIGIN STYLE",
	"STANDARD", "EXTENDED"
};

const std::vector<const char*> vDRAW_GROUND = {
	"DRAW GROUND",
	"OFF", "ON"
};

const std::vector<const char*> vP6_PAGE = {
	" ",
	"X1", "PAGE 6", "X3"
};

const std::vector<std::vector<const char*>> P6_Options = {
	vDISPLAY_HITBOXES, vHITBOX_STYLE, vCOLOR_BLIND_MODE, vSPACE_ELEMENT,
	vORIGIN_STYLE, vSPACE_ELEMENT,
	vDRAW_GROUND, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP6_PAGE
};

enum class eHITBOXES {
	DISPLAY_HITBOXES,
	HITBOX_STYLE,
	COLOR_BLIND_MODE,
	S0,
	ORIGIN_STYLE,
	S1,
	DRAW_GROUND,
	S2,
	DEFAULT,
	S3,
	RETURN,
	S4,
	PAGE
};

const int defDISPLAY_HITBOXES = 0;
const int defHITBOX_STYLE = 0;
const int defCOLOR_BLIND = 0;
const int defORIGIN_STYLE = 0;
const int defDRAW_GROUND = 0;

extern int nDISPLAY_HITBOXES;
extern int nHITBOX_STYLE;
extern int nCOLOR_BLIND_MODE;
extern int nORIGIN_STYLE;
extern int nDRAW_GROUND;

const std::vector<int*> P6_Settings = {
	&nDISPLAY_HITBOXES, &nHITBOX_STYLE, &nCOLOR_BLIND_MODE, &nORIGIN_STYLE, &nDRAW_GROUND,
	&nPAGE
};

void DefaultP6(MenuInfo* menuInfo);

//Page 7
const std::vector<const char*> vSAVE_STATE_SLOT = {
	"SAVE STATE SLOT",
	"NONE", "SLOT 01", "SLOT 02", "SLOT 03"
};

const std::vector<const char*> vSAVE_STATE = {
	"SAVE STATE"
};

const std::vector<const char*> vCLEAR_ALL_SAVES = {
	"CLEAR ALL SAVES"
};

const std::vector<const char*> vIMPORT_SAVE = {
	"IMPORT SAVE"
};

const std::vector<const char*> vEXPORT_SAVE = {
	"EXPORT SAVE"
};

const std::vector<const char*> vLOAD_RNG = {
	"LOAD RNG",
	"OFF", "ON"
};

const std::vector<const char*> vP7_PAGE = {
	" ",
	"X1", "PAGE 7", "X3"
};

const std::vector<std::vector<const char*>> P7_Options = {
	vSAVE_STATE_SLOT, vSPACE_ELEMENT,
	vSAVE_STATE, vCLEAR_ALL_SAVES, vSPACE_ELEMENT,
	vIMPORT_SAVE, vEXPORT_SAVE, vSPACE_ELEMENT,
	vLOAD_RNG, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP7_PAGE
};

enum class eSAVE_STATES {
	SAVE_STATE_SLOT,
	S0,
	SAVE_STATE,
	CLEAR_ALL_SAVES,
	S1,
	IMPORT_SAVE,
	EXPORT_SAVE,
	S2,
	LOAD_RNG,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

const int defSAVE_SLOT = 1;
const int defLOAD_RNG = 0;

extern int nSAVE_STATE_SLOT;
extern int nLOAD_RNG;

const std::vector<int*> P7_Settings = {
	&nSAVE_STATE_SLOT, &nLOAD_RNG,
	&nPAGE
};

void DefaultP7(MenuInfo* menuInfo);

//Page 8
const std::vector<const char*> vFRAME_DATA = {
	"FRAME DATA",
	"NORMAL", "ADVANCED"
};

const std::vector<const char*> vIN_GAME_FRAME_DISPLAY = {
	"IN-GAME FRAME DISPLAY",
	"OFF", "ON"
};

const std::vector<const char*> vSHOW_HITSTOP_AND_FREEZE = {
	"SHOW HITSTOP & FREEZE",
	"OFF", "ON"
};

const std::vector<const char*> vSHOW_INPUTS = {
	"SHOW INPUTS",
	"OFF", "ON"
};

const std::vector<const char*> vSHOW_CANCEL_WINDOWS = {
	"SHOW CANCEL WINDOWS",
	"OFF", "ON"
};

const std::vector<const char*> vSCROLL_DISPLAY = {
	"SCROLL DISPLAY",
	"X1", "X2", "X3"
};

const std::vector<const char*> vCOLOR_GUIDE = {
	"COLOR GUIDE"
};

const std::vector<const char*> vP8_PAGE = {
	" ",
	"X1", "PAGE 8", "X3"
};

const std::vector<std::vector<const char*>> P8_Options = {
	vFRAME_DATA, vIN_GAME_FRAME_DISPLAY, vSPACE_ELEMENT,
	vSHOW_HITSTOP_AND_FREEZE, vSHOW_INPUTS, vSHOW_CANCEL_WINDOWS, vSPACE_ELEMENT,
	vSCROLL_DISPLAY, vSPACE_ELEMENT,
	vCOLOR_GUIDE, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP8_PAGE
};

enum class eFRAME_DATA {
	FRAME_DATA,
	IN_GAME_FRAME_DISPLAY,
	S0,
	SHOW_HITSTOP_AND_FREEZE,
	SHOW_INPUTS,
	SHOW_CANCEL_WINDOWS,
	S1,
	SCROLL_DISPLAY,
	S2,
	COLOR_GUIDE,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

const int defFRAME_DATA = 0;
const int defIN_GAME_FRAME_DISPLAY = 0;
const int defSHOW_HITSTOP_AND_FREEZE = 0;
const int defSHOW_INPUTS = 0;
const int defSHOW_CANCEL = 0;
const int defSCROLL_DISPLAY = 2;

extern int nFRAME_DATA;
extern int nIN_GAME_FRAME_DISPLAY;
extern int nSHOW_HITSTOP_AND_FREEZE;
extern int nSHOW_INPUTS;
extern int nSHOW_CANCEL_WINDOWS;
extern int nSCROLL_DISPLAY;

const bool defCOLOR_GUIDE = false;

extern bool bCOLOR_GUIDE;

const int defTRUE_SCROLL_DISPLAY = 0;

extern int nTRUE_SCROLL_DISPLAY;

const std::vector<int*> P8_Settings = {
	&nFRAME_DATA, &nIN_GAME_FRAME_DISPLAY, &nSHOW_HITSTOP_AND_FREEZE, &nSHOW_INPUTS, &nSHOW_CANCEL_WINDOWS, &nSCROLL_DISPLAY,
	&nPAGE
};

void DefaultP8(MenuInfo* menuInfo);

//Page 9
const std::vector<const char*> vCUSTOM_RNG = {
	"CUSTOM RNG",
	"OFF", "SEED", "VALUE"
};

const std::vector<const char*> vRATE = {
	"RATE",
	"EVERY FRAME", "EVERY RESET"
};

const std::vector<const char*> vSEED = {
	"SEED / VALUE",
	"X1", "X2", "X3"
};

const std::vector<const char*> vP9_PAGE = {
	" ",
	"X1", "PAGE 9", "X3"
};

const std::vector<std::vector<const char*>> P9_Options = {
	vCUSTOM_RNG, vSPACE_ELEMENT,
	vRATE, vSEED, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP9_PAGE
};

enum class eRNG {
	CUSTOM_RNG,
	S0,
	RATE,
	SEED,
	S1,
	DEFAULT,
	S2,
	RETURN,
	S3,
	PAGE
};

const int defCUSTOM_RNG = 0;
const int defRATE = 0;
const int defSEED = 1;

extern int nCUSTOM_RNG;
extern int nRATE;
extern int nSEED;

const int defTRUE_SEED = 0;

extern int nTRUE_SEED;

const std::vector<int*> P9_Settings = {
	&nCUSTOM_RNG, &nRATE, &nSEED,
	&nPAGE
};

void DefaultP9(MenuInfo* menuInfo);

//Page 10
const std::vector<const char*> vSHOW_STATS = {
	"SHOW STATS",
	"OFF", "ON"
};

const std::vector<const char*> vSHOW_ACCURATE_COMBO_DAMAGE = {
	"ACCURATE COMBO DAMAGE",
	"OFF", "ON"
};

const std::vector<const char*> vP1_INPUT_DISPLAY = {
	"P1 INPUT DISPLAY",
	"OFF", "LIST", "ARCADE", "BOTH"
};

const std::vector<const char*> vP2_INPUT_DISPLAY = {
	"P2 INPUT DISPLAY",
	"OFF", "LIST", "ARCADE", "BOTH"
};

const std::vector<const char*> vFRAME_DISPLAY_Y = {
	"FRAME DISPLAY Y",
	"X1", "X2", "X3"
};

const std::vector<const char*> vP10_PAGE = {
	" ",
	"X1", "PAGE 10", "X3"
};

const std::vector<std::vector<const char*>> P10_Options = {
	vSHOW_STATS, vSHOW_ACCURATE_COMBO_DAMAGE, vSPACE_ELEMENT,
	vP1_INPUT_DISPLAY, vP2_INPUT_DISPLAY, vSPACE_ELEMENT,
	vFRAME_DISPLAY_Y, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP10_PAGE
};

enum class eUI {
	SHOW_STATS,
	ACCURATE_COMBO_DAMAGE,
	S0,
	P1_INPUT_DISPLAY,
	P2_INPUT_DISPLAY,
	S1,
	FRAME_DISPLAY_Y,
	S2,
	DEFAULT,
	S3,
	RETURN,
	S4,
	PAGE
};

const int defSHOW_STATS = 1;
const int defACCURATE_COMBO_DAMAGE = 1;
const int defP1_INPUT = 0;
const int defP2_INPUT = 0;
const int defFRAME_DISPLAY_Y = 1;

extern int nSHOW_STATS;
extern int nACCURATE_COMBO_DAMAGE;
extern int nP1_INPUT_DISPLAY;
extern int nP2_INPUT_DISPLAY;
extern int nFRAME_DISPLAY_Y;

const int defTRUE_FRAME_DISPLAY_Y = 400;

extern int nTRUE_FRAME_DISPLAY_Y;

const std::vector<int*> P10_Settings = {
	&nSHOW_STATS, &nACCURATE_COMBO_DAMAGE, &nP1_INPUT_DISPLAY, &nP2_INPUT_DISPLAY, &nFRAME_DISPLAY_Y,
	&nPAGE
};

const float defP1_LIST_INPUT_X = 60.0f;
const float defP1_LIST_INPUT_Y = 174.0f;
const float defP2_LIST_INPUT_X = 595.0f;
const float defP2_LIST_INPUT_Y = 174.0f;

const float defP1_ARCADE_INPUT_X = 200.0f;
const float defP1_ARCADE_INPUT_Y = 112.0f;
const float defP2_ARCADE_INPUT_X = 378.0f;
const float defP2_ARCADE_INPUT_Y = 112.0f;

extern float fP1_LIST_INPUT_X;
extern float fP1_LIST_INPUT_Y;
extern float fP2_LIST_INPUT_X;
extern float fP2_LIST_INPUT_Y;

extern float fP1_ARCADE_INPUT_X;
extern float fP1_ARCADE_INPUT_Y;
extern float fP2_ARCADE_INPUT_X;
extern float fP2_ARCADE_INPUT_Y;

void DefaultP10(MenuInfo* menuInfo);

//Page 11
const std::vector<const char*> vGAME_SPEED = {
	"GAME SPEED",
	"100%", "75%", "50%", "25%"
};

const std::vector<const char*> vHIDE_HUD = {
	"HIDE HUD",
	"OFF", "ON"
};

const std::vector<const char*> vHIDE_SHADOWS = {
	"HIDE SHADOWS",
	"OFF", "ON"
};

const std::vector<const char*> vHIDE_EXTRAS = {
	"HIDE EXTRAS",
	"OFF", "ON"
};

const std::vector<const char*> vBACKGROUND = {
	"BACKGROUND",
	"NORMAL", "WHITE", "GRAY", "BLACK", "RED", "YELLOW", "GREEN", "BLUE", "PURPLE"
};

const std::vector<const char*> vP11_PAGE = {
	" ",
	"X1", "PAGE 11", "X3"
};

const std::vector<std::vector<const char*>> P11_Options = {
	vGAME_SPEED, vSPACE_ELEMENT,
	vHIDE_HUD, vHIDE_SHADOWS, vHIDE_EXTRAS, vSPACE_ELEMENT,
	vBACKGROUND, vSPACE_ELEMENT,
	vDEFAULT_ELEMENT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP11_PAGE
};

enum class eSYSTEM {
	GAME_SPEED,
	S0,
	HIDE_HUD,
	HIDE_SHADOWS,
	HIDE_EXTRAS,
	S1,
	BACKGROUND,
	S2,
	DEFAULT,
	S3,
	RETURN,
	S4,
	PAGE
};

const int defGAME_SPEED = 0;
const int defHIDE_HUD = 0;
const int defHIDE_SHADOWS = 0;
const int defHIDE_EXTRAS = 0;
const int defBACKGROUND = 0;

extern int nGAME_SPEED;
extern int nHIDE_HUD;
extern int nHIDE_SHADOWS;
extern int nHIDE_EXTRAS;
extern int nBACKGROUND;

const std::vector<int*> P11_Settings = {
	&nGAME_SPEED, &nHIDE_HUD, &nHIDE_SHADOWS, &nHIDE_EXTRAS, &nBACKGROUND,
	&nPAGE
};

void DefaultP11(MenuInfo* menuInfo);

//All pages
const std::vector<std::vector<std::vector<const char*>>> Page_Options = {
	P1_Options, P2_Options, P3_Options, P4_Options, P5_Options, P6_Options, P7_Options, P8_Options, P9_Options, P10_Options, P11_Options
};

const std::vector<std::vector<int*>> Page_Settings = {
	P1_Settings, P2_Settings, P3_Settings, P4_Settings, P5_Settings, P6_Settings, P7_Settings, P8_Settings, P9_Settings, P10_Settings, P11_Settings
};

const std::vector<const char*> Page_Labels = {
	"REVERSALS", "TRAINING", "HIGHLIGHTS", "POSITIONS", "CHARACTER", "HITBOXES", "SAVE STATES", "FRAME DATA", "RNG", "UI", "SYSTEM"
};

const enum class eXS_PAGES {
	REVERSALS,
	TRAINING,
	HIGHLIGHTS,
	POSITIONS,
	CHARACTER,
	HITBOXES,
	SAVE_STATES,
	FRAME_DATA,
	RNG,
	UI,
	SYSTEM
};

const int XS_NUM_PAGES = (int)eXS_PAGES::SYSTEM; // 0 indexed

extern uint8_t nEXTENDED_SETTINGS_PAGE;
extern uint8_t nEXTENDED_SETTINGS_CURSOR[XS_NUM_PAGES + 1];

extern bool bOldFN1Input;
extern bool bOldFN2Input;

//Hotkey settings

//Page 1
const std::vector<const char*> vFREEZE = {
	"FREEZE",
	"X"
};

const std::vector<const char*> vNEXT_FRAME = {
	"NEXT FRAME",
	"X"
};

const std::vector<const char*> vPREV_FRAME = {
	"PREV FRAME",
	"X"
};

const std::vector<const char*> vTOGGLE_HITBOXES = {
	"TOGGLE HITBOXES",
	"X"
};

const std::vector<const char*> vTOGGLE_FRAME_BAR = {
	"TOGGLE FRAME BAR",
	"X"
};

const std::vector<const char*> vTOGGLE_HIGHLIGHTS = {
	"TOGGLE HIGHLIGHTS",
	"X"
};

const std::vector<const char*> vQUEUE_REVERSAL = {
	"QUEUE REVERSAL",
	"X"
};

const std::vector<const char*> vINCREMENT_RNG = {
	"INCREMENT RNG",
	"X"
};

const std::vector<const char*> vDECREMENT_RNG = {
	"DECREMENT RNG",
	"X"
};

const std::vector<std::vector<const char*>> HK_P1_Options = {
	vFREEZE, vNEXT_FRAME, vPREV_FRAME, vTOGGLE_HITBOXES, vTOGGLE_FRAME_BAR, vTOGGLE_HIGHLIGHTS, vQUEUE_REVERSAL, vINCREMENT_RNG, vDECREMENT_RNG, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP1_PAGE
};

extern int nHOTKEYS;

extern KeyState oFreezeHotkey;
extern KeyState oNextFrameHotkey;
extern KeyState oPrevFrameHotkey;
extern KeyState oToggleHitboxesHotkey;
extern KeyState oToggleFrameBarHotkey;
extern KeyState oToggleHighlightsHotkey;
extern KeyState oQueueReversalHotkey;
extern KeyState oIncrementRNGHotkey;
extern KeyState oDecrementRNGHotkey;

const std::vector<int*> HK_P1_Settings = {
	&nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nPAGE
};

enum class eHK_PAGE1 {
	FREEZE,
	NEXT_FRAME,
	PREV_FRAME,
	TOGGLE_HITBOXES,
	TOGGLE_FRAME_BAR,
	TOGGLE_HIGHLIGHTS,
	QUEUE_REVERSAL,
	INCREMENT_RNG,
	DECREMENT_RNG,
	S0,
	RETURN,
	S1,
	PAGE
};

//Page 2
const std::vector<const char*> vSAVE_STATE_HK = {
	"SAVE STATE",
	"X"
};

const std::vector<const char*> vPREV_SAVE_SLOT = {
	"PREV SAVE SLOT",
	"X"
};

const std::vector<const char*> vNEXT_SAVE_SLOT = {
	"NEXT SAVE SLOT",
	"X"
};

const std::vector<const char*> vFRAME_BAR_LEFT = {
	"FRAME BAR LEFT",
	"X"
};

const std::vector<const char*> vFRAME_BAR_RIGHT = {
	"FRAME BAR RIGHT",
	"X"
};

const std::vector<std::vector<const char*>> HK_P2_Options = {
	vSAVE_STATE_HK, vPREV_SAVE_SLOT, vNEXT_SAVE_SLOT, vFRAME_BAR_LEFT, vFRAME_BAR_RIGHT, vSPACE_ELEMENT,
	vRETURN_ELEMENT, vSPACE_ELEMENT,
	vP2_PAGE
};

extern KeyState oSaveStateHotkey;
extern KeyState oPrevSaveSlotHotkey;
extern KeyState oNextSaveSlotHotkey;
extern KeyState oFrameBarLeftHotkey;
extern KeyState oFrameBarRightHotkey;

const std::vector<int*> HK_P2_Settings = {
	&nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nHOTKEYS, &nPAGE
};

enum class eHK_PAGE2 {
	SAVE_STATE,
	PREV_SAVE_SLOT,
	NEXT_SAVE_SLOT,
	FRAME_BAR_LEFT,
	FRAME_BAR_RIGHT,
	S0,
	RETURN,
	S1,
	PAGE
};

//All pages
const std::vector<std::vector<std::vector<const char*>>> HK_Page_Options = {
	HK_P1_Options, HK_P2_Options
};

const std::vector<std::vector<int*>> HK_Page_Settings = {
	HK_P1_Settings, HK_P2_Settings
};

const int HK_NUM_PAGES = 1; // 0 indexed

extern uint8_t nHOTKEY_SETTINGS_PAGE;
extern uint8_t nHOTKEY_SETTINGS_CURSOR[HK_NUM_PAGES + 1];

extern int nHOTKEY_CD_TIMER;

//Information Window Text
const char* const DEFAULT_INFO = "Restore \\@COLOR@<015, 183, 255, 255>default settings.";
const char* const RETURN_INFO = "Return to \\@COLOR@<015, 183, 255, 255>training menu.";
const char* const PAGE_INFO = "Switch \\@COLOR@<015,183,255,255>pages.";
const char* const MAIN_INFO_PREFIX = "\\@COLOR@<255,255,255,255>";
const char* const SUB_INFO_PREFIX = "\\@COLOR@<128,128,128,255>  >\\@COLOR@<255,255,255,255>";

extern const std::map<std::string, const char*> MAIN_INFORMATION_MAP;

extern const std::map<std::string, const char*> SUB_INFORMATION_MAP;