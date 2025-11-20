
#include "DirectX.h"
#include "FancyMenu.h"
#include "DebugInfo.h"
#include "dllmain.h"
#include "..\Common\Common.h"

#include "TrainingMenu.h"
extern bool shouldDisplayDebugInfo;
extern bool shouldDisplayLinkedListInfo;
extern bool shouldDebugImportantDraw;
extern bool enableTAS;
extern bool randomTAS;
extern bool regenTAS;
extern bool useCustomShaders;
extern bool useDeerMode;
extern bool useWind;
extern int xWindVel;
extern int changeWindDir;
extern int showRoaHiddenCharge;
extern bool fixTAS2v2;

//put these wherever you want in the menu
extern bool showMinAirdashHeight; // 64 pixels above ground
extern bool showMinATHeight; // 128 pixels above ground

Menu<int> baseMenu("Debug Menu");
Menu<int>* disableFpsMenuOption = NULL;

bool enableEffectColors = false;
float effectColorHue = 0.0f;
bool enableCursor = true;

template <typename T>
struct always_false : std::false_type { };

template <typename T>
constexpr bool always_false_v = always_false<T>::value;

template <typename T>
void printType(T&&) {
	static_assert(always_false_v<T>, "Compilation failed because you wanted to know the type; see below:");
}

// -----

template <typename T>
Menu<T>::Menu(std::string name_) {
	name = name_;
}

template <typename T>
Menu<T>::Menu(std::string name_, std::function<void(int, T&)> optionFunc_, std::function<std::string(T)> nameFunc_, std::wstring key_, T startVal) {
	name = name_;
	optionFunc = optionFunc_;
	nameFunc = nameFunc_;
	key = key_;

	optionState = startVal;

	long regRes = 0;

	if (key.size() != 0) {
		if constexpr (std::is_pointer<T>::value) {
			regRes = ReadFromRegistry(key, (int*)optionState);
		} else {
			regRes = ReadFromRegistry(key, (int*)&optionState);
		}		
	}

}

template <typename T>
Menu<T>::~Menu() {

}

template <typename T>
template <typename U>
void Menu<T>::add(const Menu<U>& newItem) {
	items.emplace_back(newItem);
}

template <typename T>
template <typename U>
void Menu<T>::add(std::string name_, std::function<void(int, U&)> optionFunc_, std::function<std::string(U)> nameFunc_, std::wstring key_, U startVal) {
	items.push_back(Menu<U>(name_, optionFunc_, nameFunc_, key_, startVal));
}

template <typename T>
void Menu<T>::draw(Point& p) {
	p += Point(10.0f, 0.0f);
	Rect bounds;
	bool inside = false;
	DWORD col = 0xFFFFFFFF;

	if (items.size() == 0) {

		bounds = TextDraw(p, 10, 0xFFFFFFFF, "%s %s", name.c_str(), nameFunc(optionState).c_str());
		inside = bounds.inside(mousePos);

		if (lClick && inside) {
			optionFunc(1, optionState);
			if (key.size() != 0) {
				if constexpr (std::is_pointer<T>::value) {
					SetRegistryValue(key, *(int*)optionState);
				} else {
					SetRegistryValue(key, *(int*)&optionState);
				}
			}
		} else if (rClick && inside) {
			optionFunc(-1, optionState);
			if (key.size() != 0) {
				if constexpr (std::is_pointer<T>::value) {
					SetRegistryValue(key, *(int*)optionState);
				} else {
					SetRegistryValue(key, *(int*)&optionState);
				}
			}
		}

	} else {
		bounds = TextDraw(p, 10, 0xFFFFFFFF, "%s", name.c_str());
		inside = bounds.inside(mousePos);

		if (unfolded) {
			for (int i = 0; i < items.size(); i++) {
				p += Point(0.0f, 10.0f);
				//items[i].draw(p);
				std::visit([&p](auto& m) { m.draw(p); }, items[i]);
			}
		}

		if (lClick && inside) {
			unfolded = !unfolded;
		}
	}
	p -= Point(10.0f, 0.0f);

	if (inside) {
		col = 0xFF00FFFF;
	}

	//log("%s %f %f %f %f", name.c_str(), bounds.x1, bounds.y1, bounds.x2, bounds.y2);

	BorderDraw(bounds, col);
}

// -----

void initMenu() {

	std::function<std::string(int)> defaultOnOffNameFunc = [](int opt) -> std::string {
		return opt & 0b1 ? "ON" : "OFF";
		};

	std::function<std::string(float)> defaultSliderNameFunc = [](float opt) -> std::string {
		//return std::string("-", (int)(opt * 10.0f)) + std::string("+") + std::string("-", (int)((1.0f - opt) * 10.0f));
		//return opt & 0b1 ? "ON" : "OFF";

		static char buffer[256];
		snprintf(buffer, 256, "%5.2f", opt);
		return std::string(buffer);
		};

	// returns a func which modifies the variable passed in
	std::function<std::function<void(int, int&)>(void*)> getDefaultOnOffOptionFunc = [](void* optPtr) -> std::function<void(int, int&)> {
		return [optPtr](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			*(BYTE*)(optPtr) = opt;
			};
		};

	// -----

	Menu ui("UI");

	// wack syntax. should honestly be able to be inferred.
	ui.add<int>("P1 Input Display",
		[](int inc, int& opt) mutable -> void {
			opt += inc;
			opt = CLAMP(opt, INPUT_OFF, INPUT_BOTH);

			nP1_INPUT_DISPLAY = opt;
		},
		[](int opt) mutable -> std::string {

			switch ((eInputDisplay)opt) {
			case INPUT_OFF:
				return "OFF";
			case INPUT_LIST:
				return "LIST";
			case INPUT_ARCADE:
				return "ARCADE";
			case INPUT_BOTH:
				return "BOTH";
			default:
				break;
			}

			return "unknown" + std::to_string(opt);
		},
		sP1_INPUT_DISPLAY
	);

	ui.add<int>("P2 Input Display",
		[](int inc, int& opt) {
			opt += inc;
			opt = CLAMP(opt, INPUT_OFF, INPUT_BOTH);

			nP2_INPUT_DISPLAY = opt;
		},
		[](int opt) -> std::string {

			switch ((eInputDisplay)opt) {
			case INPUT_OFF:
				return "OFF";
			case INPUT_LIST:
				return "LIST";
			case INPUT_ARCADE:
				return "ARCADE";
			case INPUT_BOTH:
				return "BOTH";
			default:
				break;
			}

			return "unknown" + std::to_string(opt);
		},
		sP2_INPUT_DISPLAY
	);

	ui.add<int>("Show Framebar",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			nIN_GAME_FRAME_DISPLAY = opt;
		},
		defaultOnOffNameFunc,
		sFRAME_DISPLAY
	);

	ui.add<int>("Framebar Y",
		[](int inc, int& opt) {
			opt += (inc * 10.0f);
			opt = CLAMP(opt, 10.0f, 440.0f);

			nTRUE_FRAME_DISPLAY_Y = opt;
			if (nTRUE_FRAME_DISPLAY_Y == 440) nFRAME_DISPLAY_Y = 2;
			else if (nTRUE_FRAME_DISPLAY_Y == 10) nFRAME_DISPLAY_Y = 0;
			else nFRAME_DISPLAY_Y = 1;
		},
		defaultSliderNameFunc,
		sFRAME_BAR_Y
	);

	ui.add<int>("Show Stats",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			nSHOW_STATS = opt;
		},
		defaultOnOffNameFunc,
		L"",
		1
	);

	ui.add<int>("Hide Extras",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			nHIDE_EXTRAS = opt;
		},
		defaultOnOffNameFunc
	);

	ui.add<int>("Show Cursor",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			enableCursor = opt;
		},
		defaultOnOffNameFunc,
		L"",
		true
	);

	baseMenu.add(ui);

	Menu hitboxes("Hitboxes");

	hitboxes.add<int>("Hitbox Style",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			nHITBOX_STYLE = opt;
		},
		[](int opt) -> std::string {

			switch ((eHitboxStyle)opt) {
			case HITBOX_DRAW_ALL:
				return "LAYERED";
			case INPUT_LIST:
				return "BLENDED";
			default:
				break;
			}

			return "unknown" + std::to_string(opt);
		},
		sHITBOX_STYLE
	);

	hitboxes.add<int>("Color Blind Mode",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			nCOLOR_BLIND_MODE = opt;
		},
		defaultOnOffNameFunc
	);

	hitboxes.add<float>("Opacity",
		std::function<void(int, float&)>([](int inc, float& opt) -> void {
			opt += (inc * 0.1f);
			opt = CLAMP(opt, 0.0f, 1.0f);

			hitboxOpacity = opt;
			}),
		defaultSliderNameFunc
	);

	// wow thats some syntax
	std::get<Menu<float>>(hitboxes.items[hitboxes.items.size() - 1]).optionState = 0.20f;

	baseMenu.add(hitboxes);

	Menu misc("Misc");

	Menu miscMenu("Misc Options");

	miscMenu.add<int*>("Show Roa hidden charge",
		[](int inc, int*& opt) {
			*opt += inc;
			*opt = CLAMP(*opt, 0, 1);
		},
		[](int* opt) -> std::string {
			return *opt ? "ON" : "OFF";
		},
		L"SHOWROAHIDDENCHARGE",
		&showRoaHiddenCharge
	);

	misc.add(miscMenu);

	Menu tasMenu("TAS options");

	tasMenu.add<int>("enable on reset",
		getDefaultOnOffOptionFunc(&enableTAS),
		defaultOnOffNameFunc
	);

	tasMenu.add<int>("regenTAS",
		getDefaultOnOffOptionFunc(&regenTAS),
		defaultOnOffNameFunc
	);
	std::get<Menu<int>>(tasMenu.items[tasMenu.items.size() - 1]).optionState = regenTAS;

	tasMenu.add<int>("randomize???",
		getDefaultOnOffOptionFunc(&randomTAS),
		defaultOnOffNameFunc
	);

	tasMenu.add<int>("fix tas for 2v2",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			*(BYTE*)0x0077C1E8 = MANUAL; // set dummy to manual

			// fix the port numbers.
			for (int i = 0; i < 4; i++) {
				playerDataArr[i].subObj.ownerIndex = i;
			}

			*(BYTE*)(&fixTAS2v2) = opt;
		},
		defaultOnOffNameFunc
	);

	misc.add(tasMenu);

	Menu subColorMenu("color options");

	subColorMenu.add<int>("enable effect colors",
		getDefaultOnOffOptionFunc(&enableEffectColors),
		defaultOnOffNameFunc,
		L"ENABLEEFFECTHUECOLOR"
	);

	subColorMenu.add<float>("effect hue",
		std::function<void(int, float&)>([](int inc, float& opt) -> void {
			opt += (inc * 0.01f);
			opt = CLAMP(opt, 0.0f, 1.0f);

			effectColorHue = opt;
			}),
		defaultSliderNameFunc,
		L"EFFECTHUECOLOR"
	);
	effectColorHue = std::get<Menu<float>>(subColorMenu.items[subColorMenu.items.size() - 1]).optionState; // i really need a rewrite on the menu class to allow for more easy access

	subColorMenu.add<int>("enable custom shaders",
		getDefaultOnOffOptionFunc(&useCustomShaders),
		defaultOnOffNameFunc,
		L"ENABLECUSTOMSHADERS"
	);
	useCustomShaders = subColorMenu.getLastItem<int>().optionState;

	subColorMenu.add<int>("deer mode",
		getDefaultOnOffOptionFunc(&useDeerMode),
		defaultOnOffNameFunc
	);

	misc.add(subColorMenu);

	Menu findWhiskMenu("find wara");

	findWhiskMenu.add<int*>("enable",
		[](int inc, int*& opt) {
			*opt += inc;
			*opt = CLAMP(*opt, 0, 1);
		},
		[](int* opt) -> std::string {
			return *opt ? "ON" : "OFF";
		},
		L"ENABLEFINDWHISK",
		&enableWaraSearch
	);

	findWhiskMenu.add<int*>("search for",
		[](int inc, int*& opt) {
			*opt += inc;
			*opt = CLAMP(*opt, 0, 1);
		},
		[](int* opt) -> std::string {
			return *opt ? "wara" : "Whisk";
		},
		L"SEARCHFORWARAORWHISK",
		&findWara
	);


	// i wonder if this would work with pointers
	// UGH this menu is such a pain when,, idk i either need to have it modify existing values, or read from the menus values
	// and ideally, its existing, but ugh
	// i could,,, just add it to the variant?	
	findWhiskMenu.add<int*>("crowd size",
		[](int inc, int*& opt) {
			*opt += inc * 25;
			*opt = CLAMP(*opt, 0, 10000); // let them have fun. why not
		},
		[](int* opt) -> std::string {
			return std::to_string(*opt);
		},
		L"FINDWHISKCROWDSIZE",
		&crowdSize
	);

	// very nice, not ideal, but very nice. i am proud of this code. sucks that it doesnt remember the type, but thats c++ for u
	// even less proud bc of the issues with registry loading. i did a good job predicting what i would need ahead of time, and ill do better next time
	// i wonder if there is a way to see if a type is a pointer
	findWhiskMenu.getLastItem<int*>().optionState = &crowdSize;

	findWhiskMenu.add<int*>("max crowd velocity",
		[](int inc, int*& opt) {
			*opt += inc;
			*opt = CLAMP(*opt, 2, 100);
		},
		[](int* opt) -> std::string {
			return std::to_string(*opt);
		},
		L"FINDWHISKMAXCROWDVEL", // i really need these to support default values. god im fucking up this code
		&maxCrowdVel
	);
	findWhiskMenu.getLastItem<int*>().optionState = &maxCrowdVel;

	misc.add(findWhiskMenu);

	Menu windMenu("wind");

	windMenu.add<int>("enable",
		getDefaultOnOffOptionFunc(&useWind),
		defaultOnOffNameFunc
	);

	windMenu.add<int>("velocity",
		[](int inc, int& opt) {
			opt += inc * 10;
			opt = CLAMP(opt, -500, 500);
			xWindVel = opt;
		},
		[](int opt) -> std::string {
			return std::to_string(opt);
		}
	);

	windMenu.add<int>("change dir every n frames",
		[](int inc, int& opt) {
			opt += inc * 60;
			opt = CLAMP(opt, 0, 60 * 60);
			changeWindDir = opt;
		},
		[](int opt) -> std::string {
			return std::to_string(opt);
		}
	);

	misc.add(windMenu);

	baseMenu.add(misc);

	Menu debug("Debug");

	debug.add<int>("Draw Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			debugMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Object Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			verboseMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Call Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			overkillVerboseMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Profiler",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawProfiler = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Vertex info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawVertexInfo = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Debug log",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawLog = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add<int>("Power Info",
		getDefaultOnOffOptionFunc(&logPowerInfo),
		defaultOnOffNameFunc
	);

	debug.add<int>("SaveState Info",
		getDefaultOnOffOptionFunc(&logSaveState),
		defaultOnOffNameFunc
	);

	debug.add<int>("Verbose FPS Info",
		getDefaultOnOffOptionFunc(&logVerboseFps),
		defaultOnOffNameFunc
	);

	debug.add<int>("Disable FPS Limit",
		getDefaultOnOffOptionFunc(&disableFPSLimit),
		defaultOnOffNameFunc
	);

	// i despise that this option didnt work. i made specific effort to copy by reference, but that wasnt enough
	//disableFpsMenuOption = &(std::get<Menu<int>>(debug.items[debug.items.size() - 1]));

	int disableFPSIndex = debug.items.size() - 1;

	debug.add<int>("Show structs",
		getDefaultOnOffOptionFunc(&shouldDisplayDebugInfo),
		defaultOnOffNameFunc
	);

	debug.add<int>("Show Linked List Hex",
		getDefaultOnOffOptionFunc(&shouldDisplayLinkedListInfo),
		defaultOnOffNameFunc
	);

	debug.add<int>("Show ImportantDraw Hex",
		getDefaultOnOffOptionFunc(&shouldDebugImportantDraw),
		defaultOnOffNameFunc
	);

	debug.add<int>("Save screenshot each frame",
		getDefaultOnOffOptionFunc(&doSaveScreenshot),
		defaultOnOffNameFunc
	);

	debug.add<int>("Mouse:",
		[](int inc, int& opt) {
		},
		[](int opt) -> std::string {
			char buffer[64];
			snprintf(buffer, 64, "%7.3f, %7.3f", mousePos.x, mousePos.y);
			return std::string(buffer);
		}
	);
	

	baseMenu.add(debug);

	// i need to find a way to make this better. its so fucking stupid
	// options being stored in the menu is so much nicer
	disableFpsMenuOption = &std::get<Menu<int>>(std::get<Menu<int>>(baseMenu.items[baseMenu.items.size() - 1]).items[disableFPSIndex]);

	baseMenu.unfolded = true;

	log("menu setup done");
}

Point menuDragPoint(100.0f, 100.0f);
DragInfo menuDrag = DragInfo(&menuDragPoint.x, &menuDragPoint.y);

void drawFancyMenu() {

	static KeyState lShiftKey(VK_LSHIFT);

	if (mClick || (lClick && lShiftKey.keyHeld())) {
		showDebugMenu = !showDebugMenu;
		SetRegistryValue(L"ShowDebugMenu", showDebugMenu);
		if (showDebugMenu) {
			dragManager.add(&menuDrag);
		} else {
			dragManager.remove(&menuDrag);
		}
	}

	if (!showDebugMenu) {
		return;
	}
	
	menuDrag.enable = showDebugMenu;

	static bool menuInit = false;
	
	if (!menuInit) {
		initMenu();
		menuInit = true;
		dragManager.add(&menuDrag);
		menuDrag.enable = true;
	}

	joystickDraw(menuDragPoint.x - 2.0f, menuDragPoint.y - 2.0f, 12, 0xFFFFFFFF);

	menuDrag.bottomRight = menuDragPoint + Point(12.0f, 12.0f);
	menuDrag.topLeft = menuDragPoint - Point(4.0f, 4.0f);


	Point menuBasePos = menuDragPoint; // copy bc the menu messes with this point value
	baseMenu.draw(menuBasePos);
}

