#pragma once
#include <string>
#include <functional>
/*

as far as i can tell, best way to decide if to color an effect
is regrettably, on a case by case basis
theres no good way to tell, be it draw data, or looking at what boxes a move has

data structure:
each char has a max of 1000 patterns.
no reason for an unordered map when 1000/8 = 125 bytes(1 bit per pattern), for 31 chars, less than 4kb to store all the data

ill need to crowdsource this.

*/

typedef struct {
	const WORD charID;
	const WORD pattern;
} CharPattern;

constexpr WORD sionID = 0;
constexpr WORD arcID = 1;
constexpr WORD ceilID = 2;
constexpr WORD akiID = 3;
constexpr WORD maidsID = 4;
constexpr WORD hisuID = 5;
constexpr WORD kohaID = 6;
constexpr WORD tonoID = 7;
constexpr WORD miyaID = 8;
constexpr WORD waraID = 9;
constexpr WORD neroID = 10;
constexpr WORD vsionID = 11;
constexpr WORD warcID = 12;
constexpr WORD vakiID = 13;
constexpr WORD mechID = 14;
constexpr WORD nanaID = 15;
constexpr WORD dukaID = 16;
constexpr WORD lenID = 17;
constexpr WORD pceilID = 18;
constexpr WORD necoID = 19;
constexpr WORD aokoID = 20;
constexpr WORD wlenID = 21;
constexpr WORD nacID = 22;
constexpr WORD koumaID = 23;
constexpr WORD seiD = 24;
constexpr WORD riesID = 25;
constexpr WORD roaID = 26;
constexpr WORD roogID = 27;
constexpr WORD necmecID = 28;
constexpr WORD kohmecID = 29;
constexpr WORD himeID = 30;

constexpr const CharPattern blacklist[] = {
	//{ warcID, 120  },
	
	// kouma:

	{ koumaID, 139 }, // ckouma 22c
	{ koumaID, 140 },
	{ koumaID, 141 },
	{ koumaID, 142 },

	{ koumaID, 103 }, // ckouma 22b

	{ koumaID, 447 }, // j22a/j22b
	{ koumaID, 451 }, // j22c

	{ koumaID, 223 }, // fkouma 421c

	// len
	{ lenID, 75 }, // clen 2c
	{ lenID, 80 }, // clen 3c

	{ lenID, 55 }, // happens when she crouches??
	{ lenID, 56 }, // uncrouch?

	{ lenID, 203 }, // clen 421B
	{ lenID, 204 }, // clen 421C

	{ lenID, 165 }, // clen AAD
	{ lenID, 166 }, // clen AAD
	{ lenID, 167 }, // clen AAD
	{ lenID, 168 }, // clen AAD
	{ lenID, 169 }, // clen AAD

	{ lenID, 183 }, // clen cake (last arc)

	{ lenID, 468 }, // bonk hammer

	// wlen

	// todo, need to check out her icicles
	// grounded 236a, uncharged, icicle is colored, it breaking isnt. why??

	{ wlenID, 170 }, // dash forward
	{ wlenID, 173 }, // dash forward
	{ wlenID, 175 }, // dash back

	{ wlenID, 212 }, // last arc
	{ wlenID, 213 }, // last arc???
	{ wlenID, 214 }, // last arc
	{ wlenID, 215 }, // last arc, dont know what this is, doing it anyway
	{ wlenID, 208 }, // last arc, dont know what this is, doing it anyway
	{ wlenID, 216 }, // last arc, dont know what this is, doing it anyway

	{ wlenID, 189 }, // unknown, 22a
	{ wlenID, 190 }, // 22b
	{ wlenID, 191 }, // 22c
	{ wlenID, 187 }, // 22d (assumed)
	{ wlenID, 189 }, // 22d

	{ wlenID, 157 }, // AAD
	{ wlenID, 152 }, // AAD
	{ wlenID, 153 }, // AAD
	{ wlenID, 155 }, // AAD

	{ wlenID, 496 }, // hwlen 22b

	// nanana

	{ nanaID, 206 }, // AAD
	{ nanaID, 501 }, // H 623C
	
	// hime 

	// TODO, FINISH THIS UP
	{ himeID, 143 }, // 214C
	{ himeID, 144 }, // 214C
	{ himeID, 145 }, // 214C
	{ himeID, 146 }, // 214C

	{ himeID, 147 }, // 214A-A
	{ himeID, 148 }, // 214A-A
	{ himeID, 149 }, // 214A-A

	{ himeID, 150 }, // 214A-A
	{ himeID, 151 }, // 214A-A
	{ himeID, 152 }, // 214A-A

	{ himeID, 76 }, // fly
	{ himeID, 208}, // fly
	{ himeID, 209 }, // fly
	{ himeID, 78 }, // misc fly?
	{ himeID, 77 }, // misc fly?

	// roog

	// TODO
	{ roogID, 20 }, // blinker

	// arc

	// TODO
	{ arcID, 125 }, // 214
	{ arcID, 126 }, // 214
	{ arcID, 127 }, // 214

	{ arcID, 116 }, // lil hime
	{ arcID, 115 }, // last arc
	{ arcID, 118 }, // last arc

	// kohamech (H: 214C, 624C
	

	// mech	, jetpack, 236

	// hisui arc drive, arms
	{ hisuID, 202 }, // arms
	{ hisuID, 201 }, // arms
	{ hisuID, 414 }, // 22 picnic
	{ hisuID, 415 }, // 22 picnic
	{ hisuID, 416 }, // 22 picnic

	{ hisuID, 418 }, // 22 picnic
	{ hisuID, 419 }, // 22 picnic
	{ hisuID, 420 }, // 22 picnic


	{0xFF, 0} // i was very tired of adding commas to prev lines
};

constexpr size_t blacklistLen = (sizeof(blacklist) / sizeof(blacklist[0])) - 1;

constexpr auto textureModificationData = []() constexpr -> auto {
	std::array<std::array<BYTE, 125>, 32> res{};

	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 125; y++) {
			res[x][y] = 0;
		}
	}
	
	for (size_t i = 0; i < blacklistLen; ++i) {
		if (blacklist[i].pattern >= 1000) { 
			throw std::logic_error("???"); 
			// visual studio doesnt actually display the throw here, and just says it couldnt be evaled at constexpr. 
			// but this at least works while static_assert didnt
		}
	}

	for (size_t i = 0; i < blacklistLen; ++i) {
		res[blacklist[i].charID][blacklist[i].pattern / 8] |= (1 << (blacklist[i].pattern % 8));
	}

	return res;
}();

uint_fast8_t getPatternIndex(uint_fast8_t base) {
	// translate the games id system into the one listed above

	switch (base) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		return base;
	case 17:
	case 18:
	case 19:
	case 20:
		return base - 1;
	case 22:
	case 23:
		return base - 2;
	case 25:
		return base - 3;
	case 28:
	case 29:
	case 30:
	case 31:
		return base - 5;
	case 33:
	case 34:
	case 35:
		return base - 6;
	case 51:
		return 30; // hime
	default:
		return 0xFF;
	}

	return 0xFF;
}

bool shouldThisBeColored(BYTE charID, DWORD pattern) {

	BYTE index = getPatternIndex(charID);
	if (index == 0xFF) {
		return false;
	}
	
	BYTE temp = textureModificationData[index][pattern / 8];

	bool printCond = pattern == 128;

	printCond = false;

	if (printCond) {
		log("index: %02X byte is %02X", index, temp);
	}

	temp >>= (pattern % 8);

	if (printCond) {
		log("ret is %d", !(temp & 1));
	}

	return !(temp & 1);
}