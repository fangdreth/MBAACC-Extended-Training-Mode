#pragma once

#include <map>
#include <fstream>

extern IDirect3DDevice9* device;

// actual funcs

DWORD drawPaletteColumn_Player;
DWORD drawPaletteColumn_IsInPalette;
DWORD drawPaletteColumn_Param3;
void drawPaletteColumn() {
	
	if (drawPaletteColumn_Param3 == 0) {
		return;
	}

	if (!drawPaletteColumn_IsInPalette) {
		return;
	}

	//log("param1: %08X param2: %08X param3: %08X", drawPaletteColumn_Player, drawPaletteColumn_IsInPalette, drawPaletteColumn_Param3);

	int x = 180;
	int palette = *(DWORD*)0x0074d904;

	if (drawPaletteColumn_Player == 1) {
		x = 640 - 280;
		palette = *(DWORD*)0x0074D928;

		// gaslight ppl into thinking they are not 6 palettes above where they actually are
		if (palette >= 36) {
			palette -= 6;
		}
	}

	static char buffer[256];

	/*
	
	very weird shit is occuring here

	lets say 2 players, both on warc, both on custom 37
	for unknown reasons(probs due to optimizing somwhere else in the code)
	this lets P2 select P1's palette

	solution? either give each player a seperate 6 to choose from, or find the code that causes this optimization
	the seperate 6 is a bit jank, but like
	it would work
	i could also add the ability to choose enemy palettes? but it would be annoying to have urs stolen?
	TODO, talk with ppl abt this

	actually, wouldnt doing this completely fix my issues with,,, which palettes go to which chars?
	ill restrict what pals can be selected by what person
	and then easily sync up the .pal formats, first 6 from p1, second from p2

	*/


	// see if P1 random at [0074d808]+0x38
	// see if P2 random at [0074d808]+0x38+0x1DC

	if (*(DWORD*)0x0074d808 == 0) {
		return;
	}

	DWORD baseCSSAddr = *(DWORD*)0x0074d808;

	DWORD p1Rand = *(DWORD*)(baseCSSAddr + 0x38);
	DWORD p2Rand = *(DWORD*)(baseCSSAddr + 0x38 + 0x1DC);

	if(drawPaletteColumn_Player == 0 && p1Rand) {
		drawTextWithBorder(x, 300, 10, 14, "Palette: RAND");
		return;
	}

	if (drawPaletteColumn_Player == 1 && p2Rand) {
		drawTextWithBorder(x, 300, 10, 14, "Palette: RAND");
		return;
	}

	//snprintf(buffer, 256, "what %08X %08X %08X", drawPaletteColumn_Player, drawPaletteColumn_IsInPalette, drawPaletteColumn_Param3);
	snprintf(buffer, 256, "Palette: %4d", palette + 1);
	drawTextWithBorder(x, 300, 10, 14, buffer);

}

DWORD _naked_PalettePatcherHook_ebx = 0;
DWORD _naked_PalettePatcherCallback_eax = 0;
void palettePatcherCallback() {

	if (!useCustomPalettes) {
		return;
	}

	if (_naked_PalettePatcherHook_ebx == 0) {
		log("palettePatcherCallback ebx was NULL");
		return;
	}

	const char* loadString = (char*)_naked_PalettePatcherHook_ebx;

	size_t len = strnlen_s(loadString, 256);

	if (len == 0) {
		log("palettePatcherCallback ebx was NULL");
		return;
	}

	log("palettePatcherCallback ebx: %s", loadString);

	const char* fileType = loadString + len - 3;

	const char* tempStringPtr = loadString + len - 1;

	const char* fileName = NULL;

	while (tempStringPtr > loadString) {
		if (*tempStringPtr == '\\') {
			fileName = tempStringPtr+1;
			break;
		}
		tempStringPtr--;
	}

	if (fileName == NULL) {
		return;
	}

	// P_ARC_D gave a crash? what evem is that

	if (strcmp(fileType, "pal")) { // not a palette, do nothing
		return;
	}

	if (!strcmp(fileName, "P_ARC_D.pal")) {
		log("bailing out on weird hime thing");
		return;
	}

	// is a palette

	bool isWarc = false;

	if (!strcmp(fileName, "WARC.pal")) {
		isWarc = true;
	}

	DWORD* colors = (DWORD*)(_naked_PalettePatcherCallback_eax + 4);
	
	for (int i = 36; i < 48; i++) {
		for (int j = 0; j < 256; j++) {
			
			DWORD tempColor = colors[(256 * i) + j];

			BYTE a = (tempColor & 0xFF000000) >> 24;

			D3DXVECTOR3 rgb = D3DXVECTOR3(
				(float)((tempColor & 0x000000FF) >> 0),
				(float)((tempColor & 0x0000FF00) >> 8),
				(float)((tempColor & 0x00FF0000) >> 16)
				);

			D3DXVECTOR3 hsv = RGBtoHSV(rgb);

	
			if (i < 42) {
				hsv.x = 0.5f;
			} else {
				hsv.x = 0.0f;
			}

			hsv.y = (hsv.y / 2.0f) + 0.5f;
			hsv.z = (hsv.z / 2.0f) + 0.5f;


			D3DXVECTOR3 tempRGB = HSVtoRGB(hsv);

			BYTE r = tempRGB.x;
			BYTE g = tempRGB.y;
			BYTE b = tempRGB.z;

			DWORD tempNewColor = (a << 24) | (b << 16) | (g << 8) | (r << 0);

			colors[(256 * i) + j] = tempNewColor;

		}		
	}

	
}

DWORD _naked_changePaletteIndex_playerInput;
DWORD _naked_changePaletteIndex_index;
DWORD _naked_changePaletteIndex_playerIndex;
void changePaletteIndex() {
	if (_naked_changePaletteIndex_playerInput == 0) {
		return;
	}

	//log("_naked_changePaletteIndex_playerInput: %3d _naked_changePalette_eax: %3d _naked_changePalette_playerIndex: %3d", _naked_changePaletteIndex_playerInput, _naked_changePaletteIndex_index, _naked_changePaletteIndex_playerIndex);

	if (_naked_changePaletteIndex_playerIndex == 0) { // player 1
		if (_naked_changePaletteIndex_index >= 42) {
			if (_naked_changePaletteIndex_playerInput == 4) {
				_naked_changePaletteIndex_index -= 6;
			} else if (_naked_changePaletteIndex_playerInput == 6) {
				_naked_changePaletteIndex_index = _naked_changePaletteIndex_index % 6;
			} // no clue if any other vals could ever occur under current states
		}
	} else { // player 2
		if (_naked_changePaletteIndex_index < 42 && _naked_changePaletteIndex_index >= 36) {
			if (_naked_changePaletteIndex_playerInput == 6) {
				_naked_changePaletteIndex_index += 6;
			}
			else if (_naked_changePaletteIndex_playerInput == 4) {
				_naked_changePaletteIndex_index -= 6;
			} // no clue if any other vals could ever occur under current states
		}
	}

}

// naked funcs 

DWORD _naked_drawPaletteColumn_reg;
__declspec(naked) void _naked_drawPaletteColumn() {

	__asm {
		mov _naked_drawPaletteColumn_reg, eax;

		mov eax, [esp + 4];
		mov drawPaletteColumn_Player, eax;

		mov eax, [esp + 8];
		mov drawPaletteColumn_IsInPalette, eax;

		mov eax, [esp + 12];
		mov drawPaletteColumn_Param3, eax;

		mov eax, _naked_drawPaletteColumn_reg;
	}

	PUSH_ALL;
	drawPaletteColumn();
	POP_ALL;

	__asm {
		ret 0Ch;
	}

}

__declspec(naked) void _naked_PalettePatcherHook() {

	__asm {
		mov _naked_PalettePatcherHook_ebx, ebx;
	}

	// overridden bytes
	__asm _emit 0x55;

	__asm _emit 0x8B;
	__asm _emit 0xEC;

	__asm _emit 0x83;
	__asm _emit 0xE4;
	__asm _emit 0xF8;


	__asm {
		push 0041f7c6h
		ret;
	}
}

__declspec(naked) void _naked_PalettePatcherCallback() {

	__asm {
		mov _naked_PalettePatcherCallback_eax, eax;
		mov stackAddr, esp;
	}

	PUSH_ALL;
	palettePatcherCallback();
	POP_ALL;

	__asm {
		ret;
	}
}

DWORD _naked_changePaletteIndex_funcAddr = 0x0048abd0;
__declspec(naked) void _naked_changePaletteIndex() {
	__asm {
		mov _naked_changePaletteIndex_playerInput, eax;
		mov _naked_changePaletteIndex_playerIndex, edx;

		call[_naked_changePaletteIndex_funcAddr];

		mov _naked_changePaletteIndex_index, eax;
	};

	PUSH_ALL;
	changePaletteIndex();
	POP_ALL;

	__asm {

		mov eax, _naked_changePaletteIndex_index;
		mov ecx, _naked_changePaletteIndex_index;

		push 0048ab4ch;
		ret;
	};
}

// init 

void initMorePalettes() {

	//const BYTE paletteCount = 0x2A;
	const BYTE paletteCount = 36 + (6 * 2);

	// input adjustments
	patchByte(0x0048ac12 + 2, paletteCount);
	patchByte(0x0048ac17 + 2, paletteCount);
	patchByte(0x0048ac1e + 2, paletteCount);

	patchJump(0x00486328, 0x0048637b);
}

void initDrawPaletteColumn() {
	patchJump(0x0048add0, _naked_drawPaletteColumn);
}

void initPalettePatcher() {
	patchJump(0x0041f7c0, _naked_PalettePatcherHook);
	patchJump(0x0041f87a, _naked_PalettePatcherCallback);
}

void initChangePalette() {
	patchJump(0x0048ab47, _naked_changePaletteIndex);
}

bool initCSSModifications() {

	if (device == NULL) {
		return false;
	}

	static bool init = false;
	if (init) { // im paranoid
		return true;
	}

	initMorePalettes();
	initDrawPaletteColumn();
	initPalettePatcher();
	initChangePalette();

	log("initCSSModifications ran");

	init = true;
	return true;
}