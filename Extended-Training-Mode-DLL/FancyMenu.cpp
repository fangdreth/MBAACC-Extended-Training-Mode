
#include "FancyMenu.h"

Menu baseMenu("Base");

// -----

Menu::Menu(std::string name_) {
	name = name_;
}

Menu::Menu(std::string name_, std::function<void(int, int&)> optionFunc_, std::function<std::string(int)> nameFunc_, std::wstring key_) {
	name = name_;
	optionFunc = optionFunc_;
	nameFunc = nameFunc_;
	key = key_;

	if (key.size() != 0) {
		ReadFromRegistry(key, &optionIndex);
	}
	
}

Menu::~Menu() {

}

void Menu::add(const Menu& newItem) {
	items.push_back(newItem);
}

void Menu::add(std::string name_, std::function<void(int, int&)> optionFunc_, std::function<std::string(int)> nameFunc_, std::wstring key_) {
	add(Menu(name_, optionFunc_, nameFunc_, key_));
}

void Menu::draw(Point& p) {
	p += Point(10.0f, 0.0f);
	Rect bounds;
	bool inside = false;
	DWORD col = 0xFFFFFFFF;

	if (items.size() == 0) {

		bounds = TextDraw(p, 10, 0xFFFFFFFF, "%s %s", name.c_str(), nameFunc(optionIndex).c_str());
		inside = bounds.inside(mousePos);

		if (lClick && inside) {
			optionFunc(1, optionIndex);
			if (key.size() != 0) {
				SetRegistryValue(key, optionIndex);
			}
		} else if (rClick && inside) {
			optionFunc(-1, optionIndex);
			if (key.size() != 0) {
				SetRegistryValue(key, optionIndex);
			}
		}

	} else {
		bounds = TextDraw(p, 10, 0xFFFFFFFF, "%s", name.c_str());
		inside = bounds.inside(mousePos);

		if (unfolded) {
			for (int i = 0; i < items.size(); i++) {
				p += Point(0.0f, 10.0f);
				items[i].draw(p);
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

	BorderDraw(bounds, col);
}

// -----

void initMenu() {

	std::function<std::string(int)> defaultOnOffNameFunc = [](int opt) -> std::string {
		return opt & 0b1 ? "ON" : "OFF";
	};

	// -----

	Menu ui("UI");

	ui.add("P1 Input Display",
		[](int inc, int& opt) {
			opt += inc;
			opt = CLAMP(opt, INPUT_OFF, INPUT_BOTH);

			*(BYTE*)(0x00400000 + adSharedP1InputDisplay) = opt;
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
		L"P1InputDisplay"
	);

	ui.add("P2 Input Display",
		[](int inc, int& opt) {
			opt += inc;
			opt = CLAMP(opt, INPUT_OFF, INPUT_BOTH);

			*(BYTE*)(0x00400000 + adSharedP2InputDisplay) = opt;
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
		L"P2InputDisplay"
	);

	baseMenu.add(ui);

	Menu hitboxes("Hitboxes");

	hitboxes.add("Hitbox Style",
		[](int inc, int& opt) {
			opt += inc;
			opt = CLAMP(opt, HITBOX_DRAW_ALL, HITBOX_BLEND);

			*(BYTE*)(dwBaseAddress + adSharedHitboxStyle) = opt;
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
		L"HitboxStyle"
	);

	//hitboxes.add


	baseMenu.add(hitboxes);

	Menu debug("Debug");

	debug.add("Draw Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			debugMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add("Object Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			verboseMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add("Call Info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			overkillVerboseMode = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add("Profiler",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawProfiler = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add("vertex info",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawVertexInfo = opt;
		},
		defaultOnOffNameFunc
	);

	debug.add("Debug log",
		[](int inc, int& opt) {
			opt += inc;
			opt &= 0b1;

			doDrawLog = opt;
		},
		defaultOnOffNameFunc
	);

	baseMenu.add(debug);
	
	baseMenu.unfolded = true;

	log("menu setup done");
}

Point menuDragPoint(100.0f, 100.0f);
DragInfo menuDrag = DragInfo(&menuDragPoint.x, &menuDragPoint.y);

void drawFancyMenu() {

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