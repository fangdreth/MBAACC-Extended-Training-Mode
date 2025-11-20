
extern bool enableEffectColors;
extern float effectColorHue;
#include "DirectX.h"
#include "RendererModifications.h"
#include <string>
#include <functional>
#include "dllmain.h" 

bool useCustomShaders = false;
bool useDeerMode = false;

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

	{ himeID, 166 }, // 236a, rocks on the ground
	{ himeID, 168 }, // more rocks
	{ himeID, 167 }, // more rocks

	{ himeID, 298 }, // AAD
	{ himeID, 302 }, // AAD
	{ himeID, 301 }, // AAD
	{ himeID, 300 }, // AAD
	{ himeID, 312 }, // AAD
	{ himeID, 318 }, // AAD
	{ himeID, 306 }, // AAD
	{ himeID, 307 }, // AAD
	{ himeID, 319 }, // AAD
	{ himeID, 316 }, // AAD
	{ himeID, 310 }, // AAD

	{ himeID, 277 }, // grab??? what?

	// arc

	// TODO
	{ arcID, 125 }, // 214
	{ arcID, 126 }, // 214
	{ arcID, 127 }, // 214

	{ arcID, 116 }, // lil hime
	{ arcID, 115 }, // last arc
	{ arcID, 118 }, // last arc

	// pceil, her 214s are,,, way to bright? im just disabling them for now. sorry pciel players

	{ pceilID, 336 }, // 214
	{ pceilID, 335 }, // 214

	// aoko

	{ aokoID, 161 }, // last arc
	{ aokoID, 160 }, // last arc
	{ aokoID, 167 }, // last arc
	{ aokoID, 159 }, // last arc
	{ aokoID, 162 }, // last arc
	{ aokoID, 163 }, // last arc
	{ aokoID, 164 }, // last arc
	{ aokoID, 165 }, // last arc

	// ceil

	{ ceilID, 201 }, // AD/AAD, her cape shouldnt change color
	{ ceilID, 202 }, // AD/AAD, her cape shouldnt change color
	{ ceilID, 207 }, // what is this? more needs to be looked into 

	// ries

	{ riesID , 500 }, // last arc
	{ riesID , 496 }, // last arc
	{ riesID , 505 }, // AAD bg

	// vsion

	{ vsionID, 505 }, // last arc
	{ vsionID, 506 }, // last arc
	{ vsionID, 507 }, // last arc
	{ vsionID, 508 }, // last arc
	{ vsionID, 509 }, // last arc
	{ vsionID, 510 }, // last arc
	{ vsionID, 511 }, // last arc
	{ vsionID, 512 }, // last arc
	{ vsionID, 513 }, // last arc
	{ vsionID, 514 }, // last arc
	{ vsionID, 515 }, // last arc
	{ vsionID, 516 }, // last arc
	{ vsionID, 517 }, // last arc
	{ vsionID, 518 }, // last arc

	{ vsionID, 206 }, // AAD
	{ vsionID, 209 }, // AAD

	// i should maybe disable the coloration on her summons

	// wara

	{ waraID, 83 }, // last arc, 81 and 82 maybe should get it as well?
	{ waraID, 82 },
	{ waraID, 81 },
	{ waraID, 84 },
	{ waraID, 85 },
	{ waraID, 86 },
	{ waraID, 87 },
	{ waraID, 88 },

	{ waraID, 156 }, // AD

	// roa 

	{ roaID, 198 }, // last arc bg
	{ roaID, 196 }, // last arc
	{ roaID, 195 }, // last arc

	{ roaID, 169 }, // AD BG

	{ roaID, 155 }, // AD
	{ roaID, 156 }, // AD
	{ roaID, 157 }, // AD
	{ roaID, 177 }, // AD
	{ roaID, 171 }, // AD
	{ roaID, 172 }, // AD
	{ roaID, 173 }, // AD

	{ roaID, 160 }, // AAD
	{ roaID, 161 }, // AAD
	{ roaID, 162 }, // AAD
	{ roaID, 163 }, // AAD
	{ roaID, 164 }, // AAD
	{ roaID, 165 }, // AAD
	{ roaID, 166 }, // AAD
	{ roaID, 167 }, // AAD

	{ roaID, 184 }, // AAD
	{ roaID, 185 }, // AAD

	// maids. this is a confusing case. can i ,,,, duplicate hisu and koha stuff here? or like, will i have to do them seperately

	// all maids, koha, hisui, mech, and neco need to be looked over
	// check and see if can just,,, apply all maids blacklists to both maids? i think i can?

	// hisumaids

	{ maidsID, 153 }, // last arc when on hisui, are they different per char?
	{ maidsID, 139 },
	{ maidsID, 140 },
	{ maidsID, 141 },
	{ maidsID, 142 },
	{ maidsID, 143 },
	{ maidsID, 144 },
	{ maidsID, 145 },
	{ maidsID, 146 },
	{ maidsID, 147 },
	{ maidsID, 148 },
	{ maidsID, 154 },
	{ maidsID, 155 },

	{ maidsID, 98 }, // jc table
	{ maidsID, 96 }, // jc table
	{ maidsID, 90 }, // jc table
	{ maidsID, 91 }, // jc table
	{ maidsID, 92 }, // jc table
	{ maidsID, 93 }, // jc table
	{ maidsID, 94 }, // jc table
	{ maidsID, 95 }, // jc table

	{ maidsID, 45 }, // charge 5c chair
	{ maidsID, 46 }, // charge 5c chair
	{ maidsID, 47 }, // charge 5c chair

	{ maidsID, 414 }, // 22 picnic
	{ maidsID, 415 }, // 22 picnic
	{ maidsID, 416 }, // 22 picnic

	{ maidsID, 418 }, // 22 picnic
	{ maidsID, 419 }, // 22 picnic
	{ maidsID, 420 }, // 22 picnic
	{ maidsID, 423 }, // 22 picnic
	{ maidsID, 424 }, // 22 picnic
	{ maidsID, 481 }, // 22c picnic when it hits???

	{ maidsID, 202 }, // hisui AAD arms
	{ maidsID, 201 }, // hisui AAD arms

	{ maidsID, 431 }, // fmoon hisui 22 chair
	{ maidsID, 432 }, // fmoon hisui 22 chair
	{ maidsID, 433 }, // fmoon hisui 22 chair
	{ maidsID, 434 }, // fmoon hisui 22 chair
	{ maidsID, 435 }, // fmoon hisui 22 chair

	{ maidsID, 438 }, // fmoon hisui 22 chair
	{ maidsID, 439 }, // fmoon hisui 22 chair
	{ maidsID, 440 }, // fmoon hisui 22 chair
	{ maidsID, 441 }, // fmoon hisui 22 chair
	{ maidsID, 442 }, // fmoon hisui 22 chair

	{ maidsID, 446 }, // fmoon hisui 22 chair
	{ maidsID, 447 }, // fmoon hisui 22 chair
	{ maidsID, 448 }, // fmoon hisui 22 chair
	{ maidsID, 449 }, // fmoon hisui 22 chair
	{ maidsID, 450 }, // fmoon hisui 22 chair

	// kohamaids

	{ maidsID, 183 }, // koha last arc
	{ maidsID, 184 },
	{ maidsID, 185 },
	{ maidsID, 186 },
	{ maidsID, 169 },

	// koha plants
	{ maidsID, 129 }, // koha 22
	{ maidsID, 130 }, // koha 22
	{ maidsID, 131 }, // koha 22
	{ maidsID, 132 }, // koha 22
	{ maidsID, 133 }, // koha 22
	{ maidsID, 134 }, // koha 22
	{ maidsID, 135 }, // koha 22
	{ maidsID, 136 }, // koha 22
	{ maidsID, 460 }, // koha 22
	{ maidsID, 462 }, // koha 22

	{ maidsID, 202 }, // koha AAD
	{ maidsID, 203 }, // koha AAD
	{ maidsID, 228 }, // koha AAD
	{ maidsID, 212 }, // koha AAD
	{ maidsID, 213 }, // koha AAD
	{ maidsID, 214 }, // koha AAD
	{ maidsID, 226 }, // koha AAD

	// koha pretzel oh no i cant even input that


	// the things for each koha could probs/should be copied to each maid


	// hisui 

	{ hisuID, 202 }, // arms
	{ hisuID, 201 }, // arms
	{ hisuID, 414 }, // 22 picnic
	{ hisuID, 415 }, // 22 picnic
	{ hisuID, 416 }, // 22 picnic

	{ hisuID, 418 }, // 22 picnic
	{ hisuID, 419 }, // 22 picnic
	{ hisuID, 420 }, // 22 picnic
	{ hisuID, 423 }, // 22 picnic
	{ hisuID, 424 }, // 22 picnic
	{ hisuID, 481 }, // 22c picnic when it hits???

	{ hisuID, 431 }, // fmoon hisui 22 chair
	{ hisuID, 432 }, // fmoon hisui 22 chair
	{ hisuID, 433 }, // fmoon hisui 22 chair
	{ hisuID, 434 }, // fmoon hisui 22 chair
	{ hisuID, 435 }, // fmoon hisui 22 chair

	{ hisuID, 438 }, // fmoon hisui 22 chair
	{ hisuID, 439 }, // fmoon hisui 22 chair
	{ hisuID, 440 }, // fmoon hisui 22 chair
	{ hisuID, 441 }, // fmoon hisui 22 chair
	{ hisuID, 442 }, // fmoon hisui 22 chair

	{ hisuID, 446 }, // fmoon hisui 22 chair
	{ hisuID, 447 }, // fmoon hisui 22 chair
	{ hisuID, 448 }, // fmoon hisui 22 chair
	{ hisuID, 449 }, // fmoon hisui 22 chair
	{ hisuID, 450 }, // fmoon hisui 22 chair

	// koha

	{ kohaID, 183 }, // koha last arc
	{ kohaID, 184 },
	{ kohaID, 185 },
	{ kohaID, 186 },
	{ kohaID, 169 },

	// koha plants
	{ kohaID, 129 }, // koha 22
	{ kohaID, 130 }, // koha 22
	{ kohaID, 131 }, // koha 22
	{ kohaID, 132 }, // koha 22
	{ kohaID, 133 }, // koha 22
	{ kohaID, 134 }, // koha 22
	{ kohaID, 135 }, // koha 22
	{ kohaID, 136 }, // koha 22
	{ kohaID, 460 }, // koha 22
	{ kohaID, 462 }, // koha 22

	{ kohaID, 202 }, // koha AAD
	{ kohaID, 203 }, // koha AAD
	{ kohaID, 228 }, // koha AAD
	{ kohaID, 212 }, // koha AAD
	{ kohaID, 213 }, // koha AAD
	{ kohaID, 214 }, // koha AAD
	{ kohaID, 226 }, // koha AAD

	// kohamech (H: 214C, 624C

	// mech	todo, her rare summons?

	{ mechID, 134 }, // j214

	{ mechID, 110 }, // jetpack
	{ mechID, 113 }, // jetpack
	{ mechID, 116 }, // jetpack
	{ mechID, 117 }, // jetpack
	{ mechID, 118 }, // jetpack
	{ mechID, 119 }, // jetpack

	{ mechID, 98 }, // j22c dropped weapon

	{ mechID, 218 }, // the GOONS
	{ mechID, 219 }, // the GOONS
	{ mechID, 220 }, // the GOONS

	{ mechID, 45 }, // chainsaw on floor, its dark so doesnt show but still

	{ mechID, 544 }, // fmech 22a bomb thing 
	{ mechID, 422 },
	{ mechID, 428 },
	{ mechID, 541 }, // mech,,, bomb countdown
	{ mechID, 438 },
	{ mechID, 434 },

	// aki

	{ akiID, 212 }, // last arc
	//{ akiID, 240 }, // last arc (maybe?
	//{ akiID, 241 }, // last arc (maybe?
	//{ akiID, 242 }, // last arc (maybe?
	//{ akiID, 243 }, // last arc (maybe?
	//{ akiID, 244 }, // last arc (maybe?
	//{ akiID, 245 }, // last arc (maybe?
	//{ akiID, 246 }, // last arc (maybe?

	// warc
	{ warcID, 220 }, // chains in last arc
	{ warcID, 221 }, // chains in last arc

	{ warcID, 155 }, // teleport afterimage
	{ warcID, 156 }, // teleport afterimage
	{ warcID, 157 }, // teleport afterimage
	{ warcID, 158 }, // teleport afterimage
	{ warcID, 159 }, // teleport afterimage
	{ warcID, 160 }, // teleport afterimage
	{ warcID, 161 }, // teleport afterimage
	{ warcID, 162 }, // teleport afterimage

	// vaki

	{ vakiID, 537 }, // last arc
	{ vakiID, 168 }, // on c with a 22 out, she will sometimes flicker. is this the cause?

	// roog

	{ roogID, 20 }, // blinker
	{ roogID, 608 }, // last arc bg
	{ roogID, 640 }, // last arc 

	// nero 

	{ neroID, 202 }, // last arc
	{ neroID, 203 }, // last arc
	{ neroID, 169 }, // last arc
	{ neroID, 175 }, // AAD
	{ neroID, 178 }, // AAD

	// nac

	{ nacID, 115 }, // lil neros on 214

	{ nacID, 162 }, // last arc
	{ nacID, 163 }, // last arc

	{ nacID, 233 }, // AAD
	{ nacID, 234 }, // AAD
	{ nacID, 235 }, // AD
	{ nacID, 236 }, // AD
	{ nacID, 237 }, // AAD

	// nec

	{ necoID, 115 }, // lil necos on 214

	{ necoID, 162 }, // last arc
	{ necoID, 163 }, // last arc

	{ necoID, 219 }, // AD
	{ necoID, 220 }, // AD
	{ necoID, 225 }, // AAD
	{ necoID, 203 }, // AAD
	{ necoID, 204 }, // AAD

	// things are pretty much done, but more work needs to be done on assist/dual chars

	{0xFF, 0} // i was very tired of adding commas to prev lines
};

constexpr size_t blacklistLen = (sizeof(blacklist) / sizeof(blacklist[0])) - 1; // minus 1 bc of the trailing 0xFF, 0

constexpr auto textureModificationData = []() constexpr -> auto {
	std::array<std::array<BYTE, 125>, 32> res{}; // roughly 4kb, worth it vs using a map tbh

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

#pragma push_macro("optimize")
#pragma optimize("t", on)
// go check godbolt. this opt level is shit. /O2 is a flag which exists, but has to be enabled for the whole project
// the asm for this with and without t on is the same. why.
// i write my shit to be fast, and this compiler spits on me

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

	temp >>= (pattern % 8);

	return !(temp & 1);
}

void renderModificationsFrameDone() {
	//textureAddrs.clear();

	if (device == NULL) {
		return;
	}

	static D3DXVECTOR4 frameFloatOffset(0.0f, 0.0f, 0.0f, 0.0f);
	frameFloatOffset.x += (1.0f / 60.0f);
	if (frameFloatOffset.x > 1.0f) {
		frameFloatOffset.x = 0.0f;
	}

	device->SetPixelShaderConstantF(223, (float*)&frameFloatOffset, 1);

	if (!enableEffectColors) {
		return;
	}

	D3DXVECTOR4 temp(effectColorHue, 0.0f, 0.0f, 0.0f);

	device->SetPixelShaderConstantF(221, (float*)&temp, 1); // calling this every frame kinda sucks, but im tired
}

void describeObject(char* buffer, size_t buflen, const LinkedListData& info) {

	// -1 in this case means "not that type of object"
	int playerIndex = -1;
	int effectIndex = -1;

	switch (info.object) {
	case 0x00555140:
		playerIndex = 0;
		break;
	case 0x00555C3C:
		playerIndex = 1;
		break;
	case 0x00556738:
		playerIndex = 2;
		break;
	case 0x00557234:
		playerIndex = 3;
		break;
	default:
		break;
	}

	if (playerIndex == -1 && info.object > 0x0067BDE8) { // check if this thing is an effect. remember that they start +0x10 from where they are in cheat table

		if ((info.object - 0x0067BDF8) % 0x33C == 0) {
			effectIndex = (info.object - 0x0067BDF8) / 0x33C;
		}
	}

	if (playerIndex != -1) {

		DWORD pattern = *(DWORD*)(info.object + 0x0);
		DWORD state = *(DWORD*)(info.object + 0x4);

		//snprintf(buffer, buflen, "PLAYER%d\nP:%d\nS:%d", playerIndex, pattern, state);
		//playerDataArr[playerIndex].describe(buffer, buflen);
		//int offset = snprintf(buffer, buflen, "PLAYER%d ");
		if (verboseShowPlayers) {
			playerDataArr[playerIndex].describe(buffer, buflen);
		}
		
		return;
	}

	if (effectIndex != -1) {

		DWORD pattern = *(DWORD*)(info.object + 0x0);
		DWORD state = *(DWORD*)(info.object + 0x4);

		//snprintf(buffer, buflen, "EFFECT: %d\nP:%d\nS:%d", effectIndex, pattern, state);
		//int offset = snprintf(buffer, buflen, "EFFECT%d ");
		if (verboseShowEffects) {
			effectDataArr[effectIndex].describe(buffer, buflen);
		}
		
		return;
	}


	// default we dont know what this is case
	if (verboseShowUnknown) {
		snprintf(buffer, buflen, "???: %08X ", info.object);
	}
	

}

// -----

void drawLoopHook() {

	/*
	if (!debugMode) {
		return;
	}
	*/


	// before we setup all the stupid vertex shit, lets figure out what we are actually drawing here
	DWORD lineCol = 0xFFFFFFFF;
	LinkedListData info;

	DWORD unknownTexAddr;
	unknownTexAddr = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	unknownTexAddr = *(DWORD*)(unknownTexAddr + 0xC);

	bool isValidTex = false;
	if (isValidTexture(unknownTexAddr)) {
		isValidTex = true;
		//if (mDown) {
		//	saveTexture((IDirect3DBaseTexture9*)unknownTexAddr);
		//}
	}

	if (textureToObject.find(unknownTexAddr) != textureToObject.end()) {
		info = textureToObject[unknownTexAddr];
	}

	bool hasExtraDetail = false;
	static char extraDetail[2048];
	extraDetail[0] = '\0'; // "clear" the buffer

	const char* infoString = "NULL";
	switch (info.caller) {
	case 0:
		break;
	default:
		log("unknown ret %08X", info.caller);
		break;
	case 0x00415448:
		infoString = "FUN_004153b0";
		break;
	case 0x00415572:
		infoString = "DrawMenuCursor";
		lineCol = 0xFFbd1a0b;
		break;
	case 0x004159f0:
		infoString = "DrawEffectsTopUIandCharaSelect";
		lineCol = 0xFF00FF00;
		break;
	case 0x00415b8d:
		infoString = "DrawHitEffect";
		lineCol = 0xFFF4AA42;
		break;
	case 0x00415cd6:
		infoString = "DrawBlur";
		break;
	case 0x00415ee5:
		infoString = "DrawCharacterSelectBackground";
		break;
	case 0x0041619f:
		infoString = "DrawCharactersAndBackground";
		lineCol = 0xFF42e5f4;
		hasExtraDetail = true;
		describeObject(extraDetail, 2048, info);
		break;
	case 0x0041621f:
		infoString = "FUN_004161b0";
		break;
	case 0x00416583:
		infoString = "RenderOnScreen";
		break;
	}

	/*
	BYTE resourceType = *(BYTE*)(_naked_drawLoopHook_esi + 0x4);

	if (resourceType != 1) {
		return;
	}
	*/

	// 004be293
	DWORD unknown1 = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);

	// 004be296 // vertex data pointer???
	DWORD unknown2 = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	// 004be2a4 
	unknown2 = *(DWORD*)(unknown2 + 0x8);

	// FVF vert format?
	DWORD vertFormat = *(DWORD*)(unknown1 + 0x4); // 0x1C4 when,, its a  character? or well just,,, anything being actually drawn? its 0 normally

	// 000111000100
	// 0x004			0x040				0x080			0x100
	// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1



	DWORD unknown4 = *(DWORD*)(_naked_drawCallHook_ebx + 0x58);

	DWORD PrimitiveType;
	DWORD MinVertexIndex;
	DWORD NumVertices;
	DWORD PrimitiveCount;
	DWORD pIndexData;
	DWORD IndexDataFormat;
	DWORD pVertexStreamZeroData;
	DWORD VertexStreamZeroStride;

	PrimitiveType = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	PrimitiveType = *(DWORD*)(PrimitiveType);

	MinVertexIndex = 0x0;

	NumVertices = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	NumVertices = *(DWORD*)(NumVertices);

	PrimitiveCount = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	PrimitiveCount = *(DWORD*)(PrimitiveCount + 0x8);

	DWORD PrimitiveCountTestVar;
	PrimitiveCountTestVar = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	PrimitiveCountTestVar = *(DWORD*)(PrimitiveCountTestVar);

	DWORD PrimitiveCountBackup = PrimitiveCount;

	if (PrimitiveCount != 0) { // replicate code at 004be315
		if (PrimitiveCountTestVar == 2) {
			PrimitiveCount /= 2;
		} else if (PrimitiveCountTestVar == 4) {
			PrimitiveCount /= 3;
		}
	}

	pIndexData = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	pIndexData = *(DWORD*)(pIndexData + 0xC);
	DWORD* ptrIndexData = (DWORD*)(pIndexData);

	IndexDataFormat = D3DFMT_INDEX32;

	pVertexStreamZeroData = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	pVertexStreamZeroData = *(DWORD*)(pVertexStreamZeroData + 0x4);
	MeltyVertexOrig* ptrVertexStreamZeroData = (MeltyVertexOrig*)(pVertexStreamZeroData);

	VertexStreamZeroStride = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	VertexStreamZeroStride = *(DWORD*)(pVertexStreamZeroData + 0x10);

	MeltyVert outVerts[4];
	MeltyVertexOrig firstOutVert;

	if (ptrVertexStreamZeroData[ptrIndexData[0]].x > ptrVertexStreamZeroData[ptrIndexData[1]].x) {
		firstOutVert = ptrVertexStreamZeroData[ptrIndexData[1]];
	} else {
		firstOutVert = ptrVertexStreamZeroData[ptrIndexData[0]];
	}

	if (vertFormat != 0x1C4 && vertFormat != 0) {
		//log("vertFormat %08X", vertFormat);
	}

	DWORD col = 0xFFFFFFFF;

	// ugh
	D3DVIEWPORT9 view;

	device->GetViewport(&view);

	DWORD X = view.X;
	DWORD Y = view.Y;
	DWORD Width = view.Width;
	DWORD Height = view.Height;
	float MinZ = view.MinZ;
	float MaxZ = view.MaxZ;

	if (debugMode && vertFormat == 0x1C4 && PrimitiveCountTestVar == 4 && !(Height != 512 || Width != 1024)) {

		if (NumVertices % 4 != 0) {
			//log("ohno");
		}

		// hacky not ideal solution
		if (Height != 512 || Width != 1024) {
			return;
		}

		//log("%6d %5d %5d %5d %5d %7.3f %7.3f", __frameDoneCount, view.X, view.Y, view.Width, view.Height, view.MinZ, view.MaxZ);

		DWORD w = Width - X;
		DWORD h = Height - Y;

		DWORD index = 0;

		/*
		for (int j = 0; j < NumVertices; j += 3) {
			for (int i = 0; i < 3; i++) {

				DWORD tempIndex = ptrIndexData[i + j];
				MeltyVertex v = ptrVertexStreamZeroData[tempIndex];

				outVerts[i].position.x = v.x;
				outVerts[i].position.y = v.y;
				outVerts[i].position.z = v.z;
				outVerts[i].rhw = v.rhw;

				outVerts[i].color = 0x0000FF00 | 0xFF000000;

				outVerts[i].position.x += topLeftPos.x;
				outVerts[i].position.y += topLeftPos.y;

				outVerts[i].position.x *= renderModificationFactor.x;
				outVerts[i].position.y *= renderModificationFactor.y;
			}

			meltyLineData.add(outVerts[0], outVerts[1]);
			meltyLineData.add(outVerts[0], outVerts[2]);
			meltyLineData.add(outVerts[1], outVerts[2]);
		}
		*/

		for (int j = 0; j < PrimitiveCount; j++) { // the fact i didnt use this sooner is abysmal on my part
			for (int i = 0; i < 3; i++) {

				DWORD tempIndex = ptrIndexData[i + (j * 3)];
				MeltyVertexOrig v = ptrVertexStreamZeroData[tempIndex];

				outVerts[i].position.x = v.x;
				outVerts[i].position.y = v.y;
				outVerts[i].position.z = v.z;
				outVerts[i].rhw = v.rhw;

				//outVerts[i].color = 0x0000FF00 | 0xFF000000;
				outVerts[i].color = lineCol;

				outVerts[i].position.x += topLeftPos.x;
				outVerts[i].position.y += topLeftPos.y;

				outVerts[i].position.x *= renderModificationFactor.x;
				outVerts[i].position.y *= renderModificationFactor.y;
			}

			meltyLineData.add(outVerts[0], outVerts[1]);
			meltyLineData.add(outVerts[0], outVerts[2]);
			meltyLineData.add(outVerts[1], outVerts[2]);

			//meltyLineData.add(outVerts[1], outVerts[2]);
			//meltyLineData.add(outVerts[1], outVerts[3]);
			//meltyLineData.add(outVerts[2], outVerts[3]);

		}

		/*
		MeltyTestVert prev;
		MeltyTestVert curr;


		for (int j = 0; j < NumVertices; j++) {

			curr.position.x = ptrVertexStreamZeroData[ptrIndexData[j]].x;
			curr.position.y = ptrVertexStreamZeroData[ptrIndexData[j]].y;
			curr.position.z = ptrVertexStreamZeroData[ptrIndexData[j]].z;
			curr.rhw = ptrVertexStreamZeroData[ptrIndexData[j]].rhw;
			curr.color = 0x0000FF00 | 0xFF000000;

			curr.position.x += topLeftPos.x;
			curr.position.y += topLeftPos.y;

			curr.position.x *= renderModificationFactor.x;
			curr.position.y *= renderModificationFactor.y;

			// this makes the code cleaner. the optimizer BETTER, or else ill do a funky define
			if (j == 0) {
				prev = curr;
				continue;
			}

			meltyLineData.add(prev, curr);
			prev = curr;

		}

		*/

		if (PrimitiveCountTestVar == 4) {
			col = 0xFF42e5f4;
		} else {
			col = 0xFFFFFF00;
		}

		/*
		D3DMATRIX matrix;

		device->GetTransform(D3DTS_VIEW, &matrix);
		log("view:");
		logMatrix(matrix);

		device->GetTransform(D3DTS_PROJECTION, &matrix);
		log("proj:");
		logMatrix(matrix);

		device->GetTransform(D3DTS_WORLD, &matrix);
		log("world:");
		logMatrix(matrix);
		*/

	}

	// VertexStreamZeroStride might not be accurate?


	//TextDraw(0, drawY, 6, 0xFFFFFFFF, "%4d %08X %08X %08X %08X %08X", linkedListLength, _naked_drawCallHook_ebx, unknown1, unknown2, unknown3, unknown4);



	if (hasExtraDetail && verboseMode) {
		TextDraw(firstOutVert.x, firstOutVert.y, 6, lineCol, extraDetail);
	}

	if (overkillVerboseMode) {
		//TextDraw(0, drawY, 6, col, "%4d %08X %08X %08X %08X %08X %08X %08X %08X %s", linkedListLength, PrimitiveType, NumVertices, PrimitiveCount, pIndexData, pVertexStreamZeroData, VertexStreamZeroStride, unknownTexAddr, info.object, infoString);
		TextDraw(0, drawY, 6, lineCol, "%4d %s%08X %08X %s", linkedListLength, isValidTex ? "TEX" : "???", unknownTexAddr, info.object, infoString);
	}


	drawY += 6.0f;


	linkedListLength++;

}

void listAppendHook() { // for the life of me, why didnt i just not append this thing to the list??? i feel like that would have been better

	if (listAppendHook_effectRetAddr_pat == 0x0045410F) {
		listAppendHook_objAddr = listAppendHook_objAddr_pat;
	}

	if (listAppendHook_hitEffectRetAddr == 0x0042389C) { // i accidentally overwrote all my reversing on this :)
		listAppendHook_objAddr = 0x0061E170 + (listAppendHook_objAddr_hit * 0x60);
	}

	textureToObject.insert({ listAppendHook_texAddr, { false, false, listAppendHook_objAddr, listAppendHook_callerAddr } });

	if (listAppendHook_effectRetAddr == 0x0045410F || listAppendHook_effectRetAddr_pat == 0x0045410F) {

		if (listAppendHook_objAddr >= 0x0067BDE8) { // effect

			char source = *(char*)(listAppendHook_objAddr - 8);
			DWORD pattern = *(DWORD*)(listAppendHook_objAddr + 0x0);
			DWORD state = *(DWORD*)(listAppendHook_objAddr + 0x4);

			// what the hell is this doing??? is this doing something??? when did i write this????
			// ok maddy thanks for the shit comments
			// this is the code which prevents the flashing of the meter bar when hud is hidden.
			if (source == -2) {
				if (!shouldDrawHud) {
					// check effects.txt
					switch (pattern) {
					case 100:
					case 101:
					case 102:
					case 104:
					case 105:
					case 106:
					case 107:
					case 108:
					case 300:
					case 301:
					case 302:
					case 304:
					case 305:
					case 306:
					case 307:
					case 308:
						skipTextureAddrs.insert(listAppendHook_texAddr);
						return;
					default:
						return;
					}
				}
				return;
			}

			if (enableEffectColors) {

				BYTE owner = *(BYTE*)(listAppendHook_objAddr - 0x10 + 0x02F4);

				BYTE charID;

				if (owner == 0) {
					charID = *(BYTE*)0x555135;
				} else {
					charID = *(BYTE*)0x555C31;
				}

				if (*(DWORD*)(listAppendHook_objAddr - 0x10 + 0x20) != 0x00000101) { // mystery heat detection thingy
					textureToObject[listAppendHook_texAddr].shouldColor = shouldThisBeColored(charID, pattern); // tbh would having a seperate map for things to be colored be ideal.
				}
			}

			if (useDeerMode) {

				EffectData* effect = (EffectData*)(listAppendHook_objAddr - 0x10);

				if (effect->subObj.pattern == 103 || (effect->subObj.pattern >= 108 && effect->subObj.pattern <= 111)) {
					if (playerDataArr[effect->subObj.ownerIndex].subObj.charID == 10) { // we are nero.
						textureToObject[listAppendHook_texAddr].isDeer = true;
					}
				}
			}

		}
	}
}

void drawPrimHook() {

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		skipTextureAddrs.clear(); // calling this repeatedly is wasteful!
		textureToObject.clear();
		return;
	}



	// set lookups are trash. there has to be some way of,, getting the index of this texture or something??
	if (skipTextureAddrs.contains(drawPrimHook_texAddr)) {
		skipTextureDraw = 1;
	} else if (textureToObject.contains(drawPrimHook_texAddr)) {

		if (textureToObject[drawPrimHook_texAddr].isDeer) {
			device->GetPixelShader(&pPixelShader_backup); // does this inc a refcount?

			pixelShaderNeedsReset = true;
			device->SetPixelShader(pCustomShader);
		} else if (textureToObject[drawPrimHook_texAddr].shouldColor) {
			device->GetPixelShader(&pPixelShader_backup); // does this inc a refcount?

			pixelShaderNeedsReset = true;
			device->SetPixelShader(pPixelShader);
		}
	}

	if (useCustomShaders && !enableEffectColors && !useDeerMode) {
		pixelShaderNeedsReset = true;
		device->SetPixelShader(pCustomShader);
	}

}

void drawPrimCallback() {
	if (pixelShaderNeedsReset) {
		pixelShaderNeedsReset = false;
		device->SetPixelShader(pPixelShader_backup);
		pPixelShader_backup = NULL;
	}
}

// -----

#pragma pop_macro("optimize")


// -----

// naked funcs

__declspec(naked) void _naked_drawPrimCallback() {
	PUSH_ALL;
	drawPrimCallback();
	POP_ALL;
	__asm {
		ret;
	};
}

__declspec(naked) void _naked_drawIndexPrimHook() {
	__asm {
		mov _naked_drawCallHook_ebx, ebx;
	}

	PUSH_ALL;
	drawLoopHook();
	POP_ALL;

	__asm {
		// overwritten asm

		mov eax, dword ptr[ebx + 0014h];
		mov edx, dword ptr[eax + 0010h];

		push 004be474h;
		ret;

	}


}

DWORD _naked_listAppendHook_reg;
__declspec(naked) void _naked_listAppendHook() { // at 0x004c026b

	__asm {
		mov listAppendHook_texAddr, eax;

		mov eax, [esp + 350h];
		mov listAppendHook_objAddr, eax;

		mov eax, [esp + 3E0h];
		mov listAppendHook_effectRetAddr, eax;

		// this testing is for .pat effects
		mov eax, [esp + 470h];
		mov listAppendHook_objAddr_pat, eax;

		mov eax, [esp + 500h];
		mov listAppendHook_effectRetAddr_pat, eax;

		// this is for hit effects, 0042389C
		// i could also use +C0 for 0045a77a drawhiteffectwrapper?
		mov eax, [esp + 30Ch];
		mov listAppendHook_hitEffectRetAddr, eax;
		// as for, how im getting the actual drawn hit. im not sure if its addr is on the stack. im going to just get the loop index
		mov eax, [esp + 1ECh];
		mov listAppendHook_objAddr_hit, eax;


		mov eax, listAppendHook_texAddr;
	};

	// sorta smarter approach here
	// checking the direct caller of the func 
	__asm {

		mov _naked_listAppendHook_reg, eax;

		mov eax, [esp + 4h]; // there is one push at the start of the func that needs to be bypassed
		mov listAppendHook_callerAddr, eax;

		mov eax, _naked_listAppendHook_reg;

	}

	// mov ecx, dword ptr [eax]
	__asm _emit 0x8B;
	__asm _emit 0x08;

	// mov edx, dword ptr [ecx + 4]
	__asm _emit 0x8B;
	__asm _emit 0x51;
	__asm _emit 0x04;

	// push eax 
	__asm _emit 0x50;

	PUSH_ALL;
	listAppendHook();
	POP_ALL;

	__asm {
		push 004c0273h;
	};

	// jmp edx
	__asm _emit 0xFF;
	__asm _emit 0xE2;

}

DWORD _naked_drawPrimHook_reg;
DWORD _naked_drawPrimHook_jmp = 0x004be296;
__declspec(naked) void _naked_drawPrimHook() {
	__asm {
		mov _naked_drawPrimHook_reg, eax;

		mov eax, dword ptr[ebx + 54h];
		mov eax, dword ptr[eax + 0Ch];

		mov drawPrimHook_texAddr, eax;

		mov eax, _naked_drawPrimHook_reg;
	};
	PUSH_ALL;
	drawPrimHook();
	POP_ALL;
	__asm {

		cmp skipTextureDraw, 0;
		JE _CONT;

		mov skipTextureDraw, 0;

		// skip this texture
		ret;

	_CONT:
		sub esp, 8h;
		mov ecx, dword ptr[ebx + 54h];

		push 004be296h;
		ret;
	}
}

__declspec(naked) void _naked_leadToDrawPrimHook() {
	__asm {
		mov eax, [esp + 8h];
		mov leadToDrawPrimHook_ret, eax;
	}

	__asm _emit 0xA1;
	__asm _emit 0xC8;
	__asm _emit 0xE7;
	__asm _emit 0x76;
	__asm _emit 0x00;

	__asm {
		push 004c0385h;
		ret;
	}

}

// -----


// -----

void initDrawIndexPrimHook() {
	patchJump(0x004be46e, _naked_drawIndexPrimHook);
	patchJump(0x004be524, _naked_drawPrimCallback);
}

void initEffectSelector() {

	patchJump(0x004c026b, _naked_listAppendHook);
	patchJump(0x004be290, _naked_drawPrimHook);
	patchJump(0x004c0380, _naked_leadToDrawPrimHook);

}

bool initRenderModifications() {

	if (device == NULL) {
		return false;
	}

	initDrawIndexPrimHook();

	initEffectSelector();

	pPixelShader = createPixelShader(R"(

sampler2D textureSampler : register(s0);
float4 Hue : register(c221);

float3 HUEtoRGB(float hue)
{
	float3 rgb = abs(hue * 6. - float3(3, 2, 4)) * float3(1, -1, -1) + float3(-1, 2, 2);
	return clamp(rgb, 0., 1.);
}

float3 RGBtoHCV(float3 rgb)
{
	float4 p = (rgb.g < rgb.b) ? float4(rgb.bg, -1., 2. / 3.) : float4(rgb.gb, 0., -1. / 3.);
	float4 q = (rgb.r < p.x) ? float4(p.xyw, rgb.r) : float4(rgb.r, p.yzx);
	float c = q.x - min(q.w, q.y);
	float h = abs((q.w - q.y) / (6. * c + 1e-10) + q.z);
	return float3(h, c, q.x);
}

float3 RGBtoHSV(float3 rgb)
{
	// RGB [0..1] to Hue-Saturation-Value [0..1]
	float3 hcv = RGBtoHCV(rgb);
	float s = hcv.y / (hcv.z + 1e-10);
	return float3(hcv.x, s, hcv.z);
}

float3 HSVtoRGB(float3 hsv)
{
	// Hue-Saturation-Value [0..1] to RGB [0..1]
	float3 rgb = HUEtoRGB(hsv.x);
	return ((rgb - 1.) * hsv.y + 1.) * hsv.z;
}

float4 main(float2 texCoord : TEXCOORD0) : COLOR
{
		
	
	float4 orig = tex2D(textureSampler, texCoord);
	float3 hsvVal = RGBtoHSV(orig.rgb);
	hsvVal.x = Hue.x; // input hue from register.
	orig.rgb = HSVtoRGB(hsvVal);
	return orig;
	


}

	)");

	log("rendermodifications inited successfully");

	return true;
}

void loadCustomShader() {

	if (!useCustomShaders && !useDeerMode) {
		return;
	}

	if (pCustomShader != NULL) {
		pCustomShader->Release();
		pCustomShader = NULL;
	}

	pCustomShader = loadPixelShaderFromFile(L"testShader.hlsl");

}