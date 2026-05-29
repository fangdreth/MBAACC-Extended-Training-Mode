#pragma once
#include <cstddef>
#include <vector>
#include "..\Common\Common.h"
#include "..\Common\types.h"
#include <map>

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

//Setting struct for custom menu initialization and memory
//storage is for remembering the in-game option scroll state (always 1 for looping custom scroll options)
//value is the value you actually care about
struct Setting {
	std::string label = "";
	std::vector<std::string> itemLabels = {};
	int storage = 0;
	int storageDefault = 0;
	int* valuePtr = nullptr;
	int valueDefault = 0;
	Element* element = nullptr;
	KeyState* hotkey = nullptr;
	REGKEY valueRegKey = L"";
	REGKEY storageRegKey = L"";
	int settingType = 0; //0 = selection, 1 = button/label, 2 = space, 3 = bound custom, 4 = looping custom

	Setting();
	Setting(std::string label_);
	Setting(std::string label_, std::vector<std::string> itemLabels_, int* valuePtr, int storageDefault_, int settingType, REGKEY valueRegKey_ = L"", REGKEY storageRegKey_ = L"");
	Setting(std::string label_, std::vector<std::string> itemLabels_, KeyState* hotkey_, REGKEY hotkeyRegKey = L"");

	void _default();
};

struct Page {
	std::string label = "";
	std::vector<Setting> settings = {};
	int savedSelection = 0;

	Page();
	Page(std::string label_);

	void add(Setting& setting_);
	void add(std::string label_);
	void add(std::string label_, std::vector<std::string> itemLabels_, int* valuePtr_, REGKEY registryKey_ = L"");

	void addCustom(std::string label_, int* valuePtr_, int storageDefault_, bool isBound, REGKEY valueRegKey_ = L"", REGKEY storageRegKey_ = L"");
	void addNumeric(std::string label_, int min, int max, int* valuePtr_, REGKEY registryKey_ = L"");
	void addHotkey(std::string label_, KeyState* hotkey_, REGKEY registryKey_ = L"");

	void addDefault();
	void addReturn();
	void addPage();
	void addSpace();
	void addFooter();

	Setting* get(int i);
	int* getValuePtr(int i);
	int getValue(int i);
	int getStorage(int i);
	void setValue(int i, int value_);
	void setStorage(int i, int storage_);

	void _default();
};

struct MenuContainer {
	std::string label = "";
	std::vector<Page> pages = {};
	int savedSelection = 0;

	MenuContainer(std::string label_);

	void add(Page& page_);

	Page* get(int i);
};

const std::vector<std::string> defaultCustomItems = { "X1", "X2", "X3" };
const std::vector<std::string> offONItems = { "OFF", "ON" };
const std::vector<std::string> hotkeyItems = { "X" };

extern MenuContainer XS_Menu;
extern MenuContainer HK_Menu;

void initExtendedMenu();
void initHotkeyMenu();

void initMenuFromRegistry();

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

extern int XS_reversalType;
extern int XS_reversalSlot1;
extern int XS_weight1;
extern int XS_reversalSlot2;
extern int XS_weight2;
extern int XS_reversalSlot3;
extern int XS_weight3;
extern int XS_reversalSlot4;
extern int XS_weight4;
extern int XS_noReversalWeight;
extern int XS_reversalDelay;

extern int* XS_reversalSlots[4];
extern int* XS_reversalWeights[4];

const char* const REV_SHIELD_PREFIX[7] = { "", "5D > ", "5[D] > ", "2D > ", "2[D] > ", "j.D > ", "j.[D] > " };

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

extern int XS_penaltyReset;
extern int XS_guardBarReset;
extern int XS_exGuard;
extern int XS_p1Meter;
extern int XS_p2Meter;
extern int XS_p1Health;
extern int XS_p2Health;
extern int XS_hitsUntilBurst;
extern int XS_hitsUntilBunker;
extern int XS_hitsUntilForceGuard;
extern int XS_forceGuardStance;

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

extern int XS_highlights;
extern int XS_guardHighlight;
extern int XS_hitHighlight;
extern int XS_armorHighlight;
extern int XS_throwProtectionHighlight;
extern int XS_idleHighlight;

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
	SET_CURRENT_POSITIONS,
	INVERT,
	S3,
	DEFAULT,
	S4,
	RETURN,
	S5,
	PAGE
};

extern int XS_resetToPositions;
extern int XS_p1Position;
extern int XS_p1AssistPosition;
extern int XS_p2Position;
extern int XS_p2AssistPosition;

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

extern int XS_sionBullets;
extern int XS_roaVisibleCharges;
extern int XS_roaHiddenCharges;
extern int XS_fMaidsHearts;
extern int XS_ryougiKnife;

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

extern int XS_displayHitboxes;
extern int XS_hitboxStyle;
extern int XS_colorBlindMode;
extern int XS_originStyle;
extern int XS_drawGround;

enum class eSAVE_STATES {
	SAVE_STATE_SLOT,
	S0,
	SAVE_STATE,
	CLEAR_ALL_SAVES,
	S1,
	SYNC_SAVES_WITH_FILES,
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

extern int XS_saveStateSlot;
extern int XS_syncSavesWithFiles;
extern int XS_loadRNG;

enum class eFRAME_DATA {
	CONSOLE_DATA,
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

extern int XS_consoleData;
extern int XS_inGameFrameDisplay;
extern int XS_showHitstopAndFreeze;
extern int XS_showInputs;
extern int XS_showCancelWindows;
extern int XS_scrollDisplay;

extern bool XS_colorGuide;

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

extern int XS_customRNG;
extern int XS_rate;
extern int XS_seed;

enum class eUI {
	SHOW_STATS,
	ACCURATE_COMBO_DAMAGE,
	S0,
	P1_INPUT_DISPLAY,
	P2_INPUT_DISPLAY,
	S1,
	DEFAULT,
	S2,
	RETURN,
	S3,
	PAGE
};

extern int XS_showStats;
extern int XS_accurateComboDamage;
extern int XS_p1InputDisplay;
extern int XS_p2InputDisplay;

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

extern int XS_gameSpeed;
extern int XS_hideHUD;
extern int XS_hideShadows;
extern int XS_hideExtras;
extern int XS_background;

enum class eXS_PAGES {
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

enum class eHK_PAGE1 {
	FREEZE,
	ADVANCE_FRAME,
	NEXT_FRAME,
	PREV_FRAME,
	TOGGLE_HITBOXES,
	TOGGLE_FRAME_BAR,
	TOGGLE_HIGHLIGHTS,
	QUEUE_REVERSAL,
	S0,
	RETURN,
	S1,
	PAGE
};

extern KeyState oFreezeHotkey;
extern KeyState oAdvanceFrameHotkey;
extern KeyState oNextFrameHotkey;
extern KeyState oPrevFrameHotkey;
extern KeyState oToggleHitboxesHotkey;
extern KeyState oToggleFrameBarHotkey;
extern KeyState oToggleHighlightsHotkey;
extern KeyState oQueueReversalHotkey;

enum class eHK_PAGE2 {
	SAVE_STATE,
	PREV_SAVE_SLOT,
	NEXT_SAVE_SLOT,
	FRAME_BAR_LEFT,
	FRAME_BAR_RIGHT,
	INCREMENT_RNG,
	DECREMENT_RNG,
	S0,
	RETURN,
	S1,
	PAGE
};

extern KeyState oSaveStateHotkey;
extern KeyState oPrevSaveSlotHotkey;
extern KeyState oNextSaveSlotHotkey;
extern KeyState oFrameBarLeftHotkey;
extern KeyState oFrameBarRightHotkey;
extern KeyState oIncrementRNGHotkey;
extern KeyState oDecrementRNGHotkey;

extern int nHOTKEY_CD_TIMER;

//Information Window Text
const char* const DEFAULT_INFO = "Restore \\@COLOR@<015, 183, 255, 255>default settings.";
const char* const RETURN_INFO = "Return to \\@COLOR@<015, 183, 255, 255>training menu.";
const char* const PAGE_INFO = "Switch \\@COLOR@<015,183,255,255>pages.";
const char* const MAIN_INFO_PREFIX = "\\@COLOR@<255,255,255,255>";
const char* const SUB_INFO_PREFIX = "\\@COLOR@<128,128,128,255>  >\\@COLOR@<255,255,255,255>";

extern const std::map<std::string, const char*> MAIN_INFORMATION_MAP;

extern const std::map<std::string, const char*> SUB_INFORMATION_MAP;