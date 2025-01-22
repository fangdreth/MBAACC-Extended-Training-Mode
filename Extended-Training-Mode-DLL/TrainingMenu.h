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
	int isMenuDisabled;
	int MenuInfoIndex;
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

//pairs of labels (visible) and tags (invisible no spaces) for each element followed by each item
std::vector<const char*> Option1 = {
	"Element", "element",
	"Item1", "item1",
	"Item2", "item2"
};

std::vector<const char*> Option2 = {
	"Element2", "element2",
	"Item1", "item1",
	"Item2", "item2",
	"Item3", "item3",
	"Item4", "item4"
};

std::vector<std::vector<const char*>> Page1 = {
	Option1, Option2
};

int* MenuOption1 = (int*)(adMBAABase + adShareBase + 0x500);
int* MenuOption2 = (int*)(adMBAABase + adShareBase + 0x504);

std::vector<int*> Page1_Settings = {
	MenuOption1, MenuOption2
};