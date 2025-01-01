#pragma once
#include <cstddef>
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
	UNUSED(0x5);
};

CHECKOFFSET(Item, effect, 0x18)
CHECKSIZE(Item, 0x35)

struct Element //listed elements on left
{
	void* functionTable;
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
	UNUSED(0x4);
	Item** ItemsList;
	Item** ItemsListEnd;
	UNUSED(0x14);
};

CHECKOFFSET(Element, displayedName, 0x24)
CHECKOFFSET(Element, name, 0x40)
CHECKSIZE(Element, 0x80)

struct MenuInfo
{
	void* functionTable;
	void* menuManager;
	UNUSED(0x38);
	int selectedElement;
	int prevSelectedElement;
	UNUSED(0x4);
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
	void* functionTable;
	int isMenuDisabled;
	UNUSED(0x8);
	MenuInfo** TrainingMenu;
	UNUSED(0x30);
	int yOffset;
	int someYOffset;
	int xOffset;
	UNUSED(0x10);
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
};

CHECKOFFSET(MenuWindow, isRootMenu, 0x7c)
CHECKSIZE(MenuWindow, 0xe0)

#pragma pack(pop, 1)
#undef CHECKOFFSET
#undef CHECKSIZE