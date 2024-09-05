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

DWORD ougi_warc[256] = { 0x00000000, 0x01DFECF5, 0x01BEC4D3, 0x018E92C8, 0x015A5E94, 0x0144474D, 0x01313939, 0x01212929, 0x01101818, 0x01C0C0F7, 0x019077BB, 0x017D58AB, 0x015B3F7D, 0x01544848, 0x013A3030, 0x01201818, 0x01D3E6F9, 0x010A0A0A, 0x010000FF, 0x0100009C, 0x0100669C, 0x0100EEFF, 0x015F5564, 0x01211C1F, 0x01292129, 0x01EAE7EA, 0x018D6C76, 0x014F242B, 0x01C3FFFF, 0x012BDCFF, 0x01219BD7, 0x01E9F2FC, 0x01BDDAF6, 0x01100908, 0x010E0E0E, 0x011E0F0E, 0x01361714, 0x012D1716, 0x011F1F1F, 0x01471C19, 0x01362A2A, 0x01692721, 0x015E2926, 0x01303030, 0x0151302F, 0x01822F29, 0x013D3D3D, 0x014F4443, 0x0172403F, 0x01843C3B, 0x01A43C35, 0x01505050, 0x01994643, 0x01684F4E, 0x018E5654, 0x01A05856, 0x01636363, 0x016D6161, 0x01BB5856, 0x01B45F5D, 0x01866B6A, 0x01717171, 0x01996D6C, 0x01DE6865, 0x01A7CEF3, 0x01FFFFFF, 0x01FFFFFF, 0x01DBCAC9, 0x017A9694, 0x01475756, 0x01DEFAF8, 0x01ABBBBA, 0x01C2EAE8, 0x01FCFFFF, 0x0198BCBE, 0x015C7172, 0x01C0D5D6, 0x0114191A, 0x01787D7E, 0x01232C2E, 0x01879092, 0x01313D42, 0x0195A1A6, 0x016E6E6E, 0x010A0A0A, 0x0195605D, 0x01091826, 0x0113304C, 0x011D4772, 0x01276098, 0x013177BE, 0x013B91E4, 0x01509CE7, 0x0166A9EA, 0x017CB5ED, 0x0192C1F0, 0x01B89593, 0x01060009, 0x010B000F, 0x010B001D, 0x010D090C, 0x01160317, 0x011F0120, 0x0113012D, 0x01270129, 0x01200C22, 0x01300133, 0x0117023C, 0x0127092C, 0x01191719, 0x011B014D, 0x013D023F, 0x0128132A, 0x0118015F, 0x014B024E, 0x013C0D42, 0x01341439, 0x0129212A, 0x01540258, 0x01310665, 0x01220173, 0x014C0E52, 0x01620165, 0x013F1C43, 0x015F0B63, 0x01310385, 0x01302F30, 0x01710375, 0x01511758, 0x014F2352, 0x01390399, 0x0167116E, 0x01800385, 0x013E353F, 0x013B1C76, 0x016A1A6E, 0x014101B2, 0x017B1284, 0x01900496, 0x01503257, 0x01622865, 0x014E0BA8, 0x01791F7D, 0x01464546, 0x014E02C7, 0x01461E96, 0x01A206AC, 0x018F1599, 0x016F3172, 0x015E4062, 0x015D03DE, 0x0186268A, 0x0153338C, 0x01B507BD, 0x01A317A8, 0x01535353, 0x015921BD, 0x016F4672, 0x016E08F1, 0x0188378B, 0x019A26A6, 0x01C50ACF, 0x01605E60, 0x019B34A0, 0x016B22DA, 0x01AD29B4, 0x01794F7E, 0x01D412D9, 0x01C120C7, 0x01944497, 0x01663DB7, 0x018219FB, 0x01A83EAC, 0x0187598D, 0x01C332CD, 0x01727072, 0x01DF21E5, 0x018035EB, 0x01A452A6, 0x01B944BE, 0x01794EC8, 0x01916894, 0x01CE40D7, 0x01B35AB6, 0x01838283, 0x018F4AEF, 0x01EA39EF, 0x01895ED6, 0x019C77A0, 0x01CD52D5, 0x01B96CBD, 0x01F04AF3, 0x01918F91, 0x01D85AE1, 0x019C65ED, 0x01D56ADA, 0x01F159F4, 0x01B882BB, 0x019E9D9E, 0x01A676ED, 0x01E96AEF, 0x01D67CDA, 0x01C295C5, 0x01B083F2, 0x01AAAAAB, 0x01EC79F1, 0x01DB8DDE, 0x01BB91F5, 0x01F08AF4, 0x01B8B4B7, 0x01D9A0DC, 0x01F197F6, 0x01C9BECA, 0x01CAA6FA, 0x01EFA7F2, 0x01EDB5F0, 0x01D0CFD0, 0x01E9BCFA, 0x01E8CBE8, 0x01E1DCE0, 0x01F4CEFB, 0x01EEE3EF, 0x01F4EEF4, 0x01FDFDFD, 0x01AD7574, 0x01BD7372, 0x01807E7D, 0x01CE7678, 0x019D8383, 0x01E07A7B, 0x018A8A8A, 0x01B58A8A, 0x01D88989, 0x01CF8F90, 0x01999999, 0x01A99999, 0x01BD9B9B, 0x01EE9A9C, 0x01A8A8A8, 0x01D5A1A0, 0x01B3A8A8, 0x01CEB2B0, 0x01F1ACAD, 0x01E1B4B3, 0x01BBBABA, 0x01F0B6B6, 0x01DEC2C1, 0x01F2C2C2, 0x01CCCCCC, 0x01EFC9CA, 0x01D9D0CF, 0x01EFD2D3, 0x01DFDFDF, 0x01F6DEDE, 0x01F3E6E7, 0x01F9F6F6 };

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

			if (isWarc && (i == 37 || i == 43)) {
				//log("%08X", colors[(256 * i) + j]);
				colors[(256 * i) + j] = ougi_warc[j];
				continue;
			}
			
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