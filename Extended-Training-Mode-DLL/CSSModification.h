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

	int x = 100;
	int palette = *(DWORD*)0x0074d904;

	if (drawPaletteColumn_Player == 1) {
		x = 640 - 200;
		palette = *(DWORD*)0x0074D928;
	}


	

	

	static char buffer[256];

	//snprintf(buffer, 256, "what %08X %08X %08X", drawPaletteColumn_Player, drawPaletteColumn_IsInPalette, drawPaletteColumn_Param3);
	snprintf(buffer, 256, "Palette: %3d", palette + 1);
	drawTextWithBorder(x, 300, 10, 14, buffer);

}

DWORD _naked_PalettePatcherHook_ebx = 0;
DWORD _naked_PalettePatcherCallback_eax = 0;
void palettePatcherCallback() {

	// todo, i can probs,, recall the func that loads palettes, and swap the pointers
	// hopefully
	// i just have to find the func

	// palette loads take a different route. this route.

	// this crashes on fhime. like all things do 
	// doesnt she have 2 loading anims? whats up with that?

	/*
	
	ok:
	changing things going into gameplay is easy. both clients will have each others data, and when loading in, i just use that to choose/figure out
	the issue is CSS. i need to reload these dynamically. 
	
	actually,,,, do i?
	if i exchange all palette data before boot, and hook these calls before load,,, i should/would be fine, wouldnt i be?
	i also need to fix the menus

	
	*/

	if (_naked_PalettePatcherHook_ebx == 0) {
		log("miscTests ebx was NULL");
		return;
	}

	const char* loadString = (char*)_naked_PalettePatcherHook_ebx;

	size_t len = strnlen_s(loadString, 256);

	if (len == 0) {
		log("miscTests ebx was NULL");
		return;
	}

	log("miscTests ebx: %s", loadString);

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

	// textures, when loaded in, are in 4 byte RGBA format.
	// weirdly, there is a 40 00 00 00 at the start. what is that?
	// its 64, which is the num of palettes? but idrk

	DWORD* colors = (DWORD*)(_naked_PalettePatcherCallback_eax + 4);

	/*
	if (isWarc) {
		log("warc palette:");
		for (int j = 0; j < 256; j++) {
			log("%3d %08X", j, colors[(256 * 36) + j]);
		}
		log("-----");
	}
	*/
	
	for (int i = 0; i < 64; i++) {
	//for (int i = 36; i < 42; i++) {
		for (int j = 0; j < 256; j++) {
			
			DWORD tempColor = colors[(256 * i) + j];

			BYTE a = (tempColor & 0xFF000000) >> 24;

			if (isWarc && (i == 36 || i == 37)) {
				//colors[(256 * i) + j] = (a << 24) | ougi_warc[j];
				//colors[(256 * i) + j] = (a << 24) | (0xFF80);
				continue;
			}
			
			D3DXVECTOR3 rgb = D3DXVECTOR3(
				(float)((tempColor & 0x00FF0000) >> 16),
				(float)((tempColor & 0x0000FF00) >> 8),
				(float)((tempColor & 0x000000FF) >> 0)
				);

			D3DXVECTOR3 hsv = RGBtoHSV(rgb);

			/*
			switch (i % 3) {
			default:
			case 0:
				hsv.x = 0.0f;
				break;
			case 1:
				hsv.x = 120.0f;
				break;
			case 2:
				hsv.x = 240.0f;
				break;
			}
			*/

			hsv.x = fmod(hsv.x - 120.0f, 360.0f);
			
			//log("h: %5.2f s: %5.2f v: %5.2f", hsv.x, hsv.y, hsv.z);

			D3DXVECTOR3 tempRGB = HSVtoRGB(hsv);

			BYTE r = tempRGB.x;
			BYTE g = tempRGB.y;
			BYTE b = tempRGB.z;

			//g = 0xF0;

			DWORD tempNewColor = (a << 24) | (r << 16) | (g << 8) | (b << 0);

			//if (j >= 16) {
			//	continue;
			//}
			colors[(256 * i) + j] = tempNewColor;

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
	saveStack();
	logStack();
	palettePatcherCallback();
	POP_ALL;

	__asm {
		ret;
	}
}

// init 

void initMorePalettes() {

	const BYTE paletteCount = 0x2A;

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

	log("initCSSModifications ran");

	init = true;
	return true;
}