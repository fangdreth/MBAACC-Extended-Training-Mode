#pragma once

#include <map>

extern IDirect3DDevice9* device;

/*

overall plan:

each player will have access to 6 custom palettes per char (keep things simple)
said palettes can be created in palmod, put them in slots 36-42
projectile options will be configured by a seperate file of my format
(options for a HUE, or for a certain preprogrammed shader. allowing dynamic shader programming, while cool, may cause security issues)

i need to figure out how to DYNAMICALLY adjust the palettes in the same way that i do with textures. 
and, i need it on both CSS and gameplay, but omfg does CSS suck

both players running a 42 palette thing wont desync online. 
i could sync together the pal files of both players, but that causes more issues than it will solve

there are no shortcuts

are palettes done via D3D palettes? or by more traditional uv mapping?
i know where they are loaded, but where are they stored during runtime?
is it done via a pixel shader? a vertex shader? or some unknowable mystic call i have yet to find yet?
none of the textures drawn had a palette format. they were all D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, or D3DFMT_A1R5G5B5

https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxcreatetexturefromfileinmemoryex

there seems to be 2 copies of,,, everything? one for gameplay, one for css, whats with that

maybe set custom colors in caster??


palettes are possibly applied during the section after beginstate is called, which is very bad for me
it would make no sense for them to apply palettes every frame
a ton of createtextures are called on starting a battle
most likely, for the preview, but is it possible that those textures are the palette ones?

*/

// actual funcs


DWORD drawPaletteColumn_Player;
DWORD drawPaletteColumn_IsInPalette;
DWORD drawPaletteColumn_Param3;
void drawPaletteColumn() {
	
	// going to need to redo the palette drawing.
	// probs not a good idea
	// i dont want to make changes that are this destructive, but this is the best i can do

	/*
	
	todo, get:
	char side
	how many chars(will maids fuck me again?)
	palette index
	
	you already know where most of those are
	
	*/

	return;

	if (drawPaletteColumn_Param3 == 0) {
		return;
	}
	//DWORD idrk = *(DWORD*)drawPaletteColumn_Param3;
	DWORD idrk = drawPaletteColumn_Param3;

	IDirect3DResource9* pResource = (IDirect3DResource9*)idrk;

	log("attempting type");
	int type = pResource->GetType();
	log("type=%d", type);

	if (!drawPaletteColumn_IsInPalette) {
		return;
	}



	

	log("param1: %08X "
	"param2: %08X "
	"param3: %08X", drawPaletteColumn_Player, drawPaletteColumn_IsInPalette, drawPaletteColumn_Param3);




}

DWORD paletteLoad_EDI;

void paletteLoadHelper(DWORD colorAddr) {

	if (colorAddr == 0) {
		log("colAddr was null, ret");
		return;
	}

	DWORD* pPalette = (DWORD*)((*(DWORD*)colorAddr) + 4);// this naming style is growing on me

	if (pPalette == NULL) {
		log("pPalette was null, HOW DOES THIS HAPPEN? ret");
		return;
	}


	DWORD col1 = 0x00FF0080;
	DWORD col2 = 0x0000FF00;

	
	for (int i = 0; i < 0x100 * 42; i+=2) {
		pPalette[i + 0] = col1;
		pPalette[i + 1] = col2;
	}
	

	//log("%08X %08X %08X %08X", pPalette[0 + 0], pPalette[0 + 1], pPalette[0 + 2], pPalette[0 + 3]);




}

void paletteLoad() {

	DWORD pCSSaddr = *(DWORD*)0x0074D808; // pointer to the addr holding the CSS data

	DWORD p1ColorAddr = (pCSSaddr + 0x1AC);
	DWORD p2ColorAddr = (pCSSaddr + 0x1B8);
	DWORD p3ColorAddr = (pCSSaddr + 0x388);
	DWORD p4ColorAddr = (pCSSaddr + 0x394);

	log("p1ColorAddr: %08X p2ColorAddr: %08X p3ColorAddr: %08X p4ColorAddr: %08X", p1ColorAddr, p2ColorAddr, p3ColorAddr, p4ColorAddr);

	if (paletteLoad_EDI + 4 == p1ColorAddr) {
		log("player 1 palette");
		paletteLoadHelper(p1ColorAddr);
	} else if (paletteLoad_EDI + 4 == p2ColorAddr) {
		log("player 2 palette");
		paletteLoadHelper(p2ColorAddr);
	} else if (paletteLoad_EDI + 4 == p3ColorAddr) {
		log("player 3 palette");
		paletteLoadHelper(p3ColorAddr);
	} else if (paletteLoad_EDI + 4 == p4ColorAddr) {
		log("player 4 palette");
		paletteLoadHelper(p4ColorAddr);
	} else {
		log("unknown player palette");
	}


	Sleep(0x14); // this call was overwritten.
}

DWORD loadTextureFromPathLogger_needBreak = 0;
DWORD loadTextureFromPathLogger_retAddr = 0;
DWORD loadTextureFromPathLogger_strAddr = 0;
void loadTextureFromPathLogger() {
	if (loadTextureFromPathLogger_strAddr == 0) {
		return;
	}

	static char buffer[256];

	snprintf(buffer, 256, "%s", (char*)loadTextureFromPathLogger_strAddr);

	log("loadTexture: ret: %08X  %s", loadTextureFromPathLogger_retAddr, buffer);

	if (loadTextureFromPathLogger_retAddr == 0x0042640C) {
		log("a breakpoint SHOULD trig");
		loadTextureFromPathLogger_needBreak = 1;
		// at this point : [[edi]] points to a string saying WARC (,,, wait,,, how? ) 
		// edi is at esp + CC for some reason
		// [[[esp+0]]]
		__asm { 
			nop;
			nop;
			//int 3;
			nop;
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

__declspec(naked) void _naked_paletteLoad() {
	__asm {
		mov paletteLoad_EDI, edi;
	};

	PUSH_ALL;
	paletteLoad();
	POP_ALL;

	__asm {
		ret;
	};
}

DWORD _naked_loadTextureFromPathLogger_reg;
__declspec(naked) void _naked_loadTextureFromPathLogger() {

	__asm {
		mov _naked_loadTextureFromPathLogger_reg, eax;

		mov eax, [esp + 4];
		mov loadTextureFromPathLogger_strAddr, eax;

		mov eax, [esp + 0];
		mov loadTextureFromPathLogger_retAddr, eax;

		mov eax, _naked_loadTextureFromPathLogger_reg;
	}

	PUSH_ALL;
	loadTextureFromPathLogger();
	POP_ALL;

	// overwritten bytes
	__asm _emit 0x51;
	__asm _emit 0x53;
	__asm _emit 0x56;
	__asm _emit 0x57;
	__asm _emit 0x50;

	__asm {
		push 004c8aa5h
		ret;
	};

}

__declspec(naked) void _naked_loadTextureFromPathLoggerCallback() {

	__asm {

		cmp loadTextureFromPathLogger_needBreak, 1;
		JNE _SKIP;

		mov loadTextureFromPathLogger_needBreak, 0;

		nop;
		nop;
		//int 3;
		nop;

	_SKIP:

		add esp, 4;
		pop edi;
		pop esi;
		mov eax, ebx;
		pop ebx;
		pop ecx;
		ret;
	}


}

__declspec(naked) void _naked_loadTextureFromPathLoggerCallback2() {
	__asm {

		cmp loadTextureFromPathLogger_needBreak, 1;
		JNE _SKIP;

		mov loadTextureFromPathLogger_needBreak, 0;

		nop;
		nop;
		//int 3;
		nop;

	_SKIP:

		pop edi;
		pop esi;
		pop ebx;
		pop ecx;

		ret;
	}
}

DWORD _naked_loadTextureFromPathUnknown_FuncAddr = 0x004bd2d0;
__declspec(naked) void _naked_loadTextureFromPathUnknown() {

	__asm {
		// eax, at this point, on a break, has the addr to 
		// the rectangles used to color the palette things?
		//cmp loadTextureFromPathLogger_needBreak, 1;
		//JNE _SKIP;
		//
		//mov loadTextureFromPathLogger_needBreak, 0;

		nop;
		nop;
		//int 3;
		nop;
	_SKIP:
	}

	__asm {
		call[_naked_loadTextureFromPathUnknown_FuncAddr];
	};

	__asm {
		push 004c8ae7h;
		ret;
	}

}

// init 

void initMorePalettes() {

	/*
	
	have a thing where you can like
	
	*/

	// changing the hardcoded 36 (0x24) limit to 42 (0x2A)
	// also,,, the issue rn is def the # of pages and # of palettes per page 
	// look for 6

	// current theory:
	//

	const BYTE paletteCount = 0x2A;


	// input adjustments
	patchByte(0x0048ac12 + 2, paletteCount);
	patchByte(0x0048ac17 + 2, paletteCount);
	patchByte(0x0048ac1e + 2, paletteCount);

	// change setup loop index
	//patchByte(0x004862ef + 2, paletteCount);
	
	// change some sort of bounds checking?
	//patchByte(0x0048630c + 2, paletteCount);
	//patchByte(0x00486311 + 2, paletteCount);
	
	// THESE TWO CAUSE ISSUES WITH PLAYER NOT SHOWING 
	//patchByte(0x00486361 + 2, paletteCount);
	//patchByte(0x0048636a + 2, paletteCount);
	//
	//patchByte(0x0048636d + 2, 0x50);
	//patchByte(0x00486373 + 2, 0x50);

	
	patchJump(0x00486328, 0x0048637b);
	
	// in the draw func ( i think ) changes the number of pages rendered
	//patchByte(0x0048b0db + 2, paletteCount);

	//patchByte(0x0048af10 + 4, 0x07);

}

void initDrawPaletteColumn() {
	patchJump(0x0048add0, _naked_drawPaletteColumn);
}

void initPaletteLoad() {
	patchFunction(0x00489cd1, _naked_paletteLoad);
	// 3 extra nops needed to overwrite junk data
	patchByte(0x00489cd1 + 5, 0x90);
	patchByte(0x00489cd1 + 6, 0x90);
	patchByte(0x00489cd1 + 7, 0x90);
}

void initLoadTextureFromPathLogger() {
	patchJump(0x004c8aa0, _naked_loadTextureFromPathLogger);

	patchJump(0x004c8b06, _naked_loadTextureFromPathLoggerCallback);
	patchJump(0x004c8ac4, _naked_loadTextureFromPathLoggerCallback2);

	// midway load?
	patchJump(0x004c8ae2, _naked_loadTextureFromPathUnknown);
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
	initPaletteLoad();
	initLoadTextureFromPathLogger();

	log("initCSSModifications ran");

	init = true;
	return true;
}