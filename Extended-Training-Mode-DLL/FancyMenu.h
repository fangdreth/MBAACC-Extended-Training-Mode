#pragma once

#include "DirectX.h"
#include <any>
#include <vector>
#include <functional>
#include <string>
#include <variant>
#include <type_traits>
#include <utility>


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

template <typename T = int>
class Menu {
public:

	Menu(std::string name_);

	Menu(std::string name_, std::function<void(int, T&)> optionFunc_, std::function<std::string(T)> nameFunc_, std::wstring key_ = L"");

	~Menu();

	template <typename U = int>	
	void add(const Menu<U>& newItem);

	template <typename U = int>
	void add(std::string name_, std::function<void(int, U&)> optionFunc_, std::function<std::string(U)> nameFunc_, std::wstring key_ = L"");


	void draw(Point& p);

	//std::vector<std::variant<Menu<int>, Menu<float>>> items;
	//std::vector<Menu> items;
	using MenuVariant = std::variant<Menu<int>, Menu<float>>;
	std::vector<MenuVariant> items;

	std::string name = "NULL";
	std::wstring key = L"";

	// i dislike std::string, but it makes this much simpler.
	std::function<void(int, T&)> optionFunc = nullptr; // execute code upon menu change
	std::function<std::string(T)> nameFunc = nullptr; // give the name of the option
	
	T optionState;

	bool unfolded = false;

};

extern Menu<int> baseMenu;

void drawFancyMenu();