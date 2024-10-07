
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

void Menu::draw(Point p) {
	if (optionFunc) {
		TextDraw(p, 16, 0xFFFFFFFF, "opt: %s %s", name.c_str(), optionFunc(optionIndex).c_str());
	} else {
		TextDraw(p, 16, 0xFFFFFFFF, "menu: %s", name.c_str());
		for (int i = 0; i < items.size(); i++) {
			items[i].draw(p + Point(16.0f, (i + 1) * 16.0f));
		}
	}
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

}

void drawFancyMenu() {

	return;

	static bool menuInit = false;
	
	if (!menuInit) {
		initMenu();
		menuInit = true;
	}

	baseMenu.draw();
}