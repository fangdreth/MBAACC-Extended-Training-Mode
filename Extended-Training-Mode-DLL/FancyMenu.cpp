
#include "FancyMenu.h"

Menu baseMenu("Base");

// -----

Menu::Menu(const std::string& name_) {
	name = name_;
}

Menu::Menu(const std::string& name_, std::function<std::string(int)> optionFunc_) {
	name = name_;
	optionFunc = optionFunc_;
}

Menu::~Menu() {

}

void Menu::add(const Menu& newItem) {
	items.push_back(newItem);
}

void Menu::draw(Point& p) {
	p += Point(10.0f, 0.0f);
	if (optionFunc) {
		TextDraw(p, 10, 0xFFFFFFFF, "opt: %s %s", name.c_str(), optionFunc(optionIndex).c_str());
	} else {
		TextDraw(p, 10, 0xFFFFFFFF, "menu: %s", name.c_str());
		for (int i = 0; i < items.size(); i++) {
			p += Point(0.0f, 10.0f);
			items[i].draw(p);
		}
	}
	p -= Point(10.0f, 0.0f);
}

// -----

void initMenu() {

	Menu test1("test1");

	Menu test2("test2",
		[](int opt) -> std::string {

			static char buffer[128];
			snprintf(buffer, 128, "opt: %d", opt);

			return std::string(buffer);
		}
	);

	baseMenu.add(test1);

	baseMenu.add(test2);

	test1.add(Menu("sub1",
		[](int opt) -> std::string {
			static char buffer[128];
			snprintf(buffer, 128, "option omfg: %d", opt);

			return std::string(buffer);
		}
	));

	test1.add(Menu("sub2",
		[](int opt) -> std::string {
			static char buffer[128];
			snprintf(buffer, 128, "option omfg: %d", opt);

			return std::string(buffer);
		}
	));

}

void drawFancyMenu() {

	return;

	static bool menuInit = false;
	
	if (!menuInit) {
		initMenu();
		menuInit = true;
	}

	Point menuBasePos(100.0f, 100.0f);
	baseMenu.draw(menuBasePos);
}