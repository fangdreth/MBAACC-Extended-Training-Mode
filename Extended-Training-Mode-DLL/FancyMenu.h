#pragma once

#include "DirectX.h"

#include <vector>
#include <functional>
#include <string>

/*

what is a menu?
i need:
menu to have menus as subobjects
ideally, if this will be dragable, there will be only one anchor point(make it small) at the top left
function pointers to exec things, i can do the thing where i have params saved/passed through/give the function access to the base/owner menu object?
just make sure to learn for the horrid things you did in gbastranger

options. how am i doing those
and will the base menu manager be something different than the menu? maybe?
how do i supply the params to the funcs? should i have multiple per thing?

also,, ugh i would like the lambdas to be constexpr, but probs not happening

or could i have 2 objects, one for base menu, and another for an option which execs things?
or i could have that exec thing just be,,, determined by what the menu object has inside it?
i would prefer things being with one object

something is decided is/not is a menu/option based on whether or not it has subitems, or a optionFunc

also worried abt lambda captures. i hope that menus dont ever need more than one param

*/

class Menu {
public:

	Menu(const std::string& name_);

	Menu(const std::string& name_, std::function<std::string(int)> optionFunc_);

	~Menu();

	void add(const Menu& newItem);

	void draw(Point& p);

	std::string name = "NULL";

	std::vector<Menu> items; // should i have these be pointers? 

	std::function<std::string(int)> optionFunc = nullptr; // i dislike std::string, but it makes this much simpler.
	int optionIndex = 0;
	bool unfold = false;

};

extern Menu baseMenu;

void drawFancyMenu();