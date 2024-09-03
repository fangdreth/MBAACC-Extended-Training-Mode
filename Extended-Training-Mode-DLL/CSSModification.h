#pragma once

#include <map>
#include <fstream>

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

Hime crashes on CSS. ofcs

i need the ability to dynamically reload this shit
but they,,, doing it via pixel shader is so much easier

tbh i could;,
load the palette on my own and then use that as a pixel shader map? but the colors being altered would make this a world of pain

i could possibly(hopefully)
reload the chat sprites with the new palette, and swap the buffers somehow? but god that’s a pain

alternatively,

i remake the palette until, and therefore have a guarantee that slots 36-42 would be constant
i then now know what the colors are, and can write a shader
the issues with that may be,,, how things like orbs and kohaku chair are classified, will they be shaded?
but this way of doing it is non destructive to the base texture, and so much easier

will i have to write a ui? yes, but i would have had to do at least something for the effects colors

this guarantees me sanity
ok, no more tangents, no more running.
options are:

pixel shader
i would need to either prerecord all chars 37 palette, or load them and use that
but wouldn’t finding that possibly give me solution #2?

at the end of the day, i know how to do shit with pixel shaders, and that shit gives me MORE options than,,, without. 
so its the right path right?
i just dont want to have to rewrite palmod

i know i should just focus on getting something out, but i cant figure this basic shit out

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


	Sleep(1000);

	__asm {
		nop;
		int 3;
		nop;
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

DWORD _naked_loadTextureFromPathUnknown_pngAddr;
DWORD _naked_loadTextureFromPathUnknown_pngLen;
void loadTextureFromPathUnknown() {

	return; 

	// todo, overwrite the text that floats in the back of CSS

	log("png at %08X filesize: %08X", _naked_loadTextureFromPathUnknown_pngAddr, _naked_loadTextureFromPathUnknown_pngLen);

	const char* writePath = "C:/Users/Meepster99/Documents/Programming/MBAACC-Extended-Training-Mode/temp/";

	static unsigned imageCounter = 0;

	static char fileName[256];
	snprintf(fileName, 256, "%s%4d.png", writePath, imageCounter);

	// def not the fastest approach, but it will work, and be safe. 
	// actually,,, there has to be an easier way for this right? the length of the buffer might be somewhere
	// yup! the value of eax before its edi is the file length. easy.

	std::ofstream outFile;
	outFile.open(fileName, std::ios::binary | std::ios::out);
	outFile.write((char*)_naked_loadTextureFromPathUnknown_pngAddr, _naked_loadTextureFromPathUnknown_pngLen);
	outFile.close();

	imageCounter++;;
	log("we wrote the png successfully");

}

DWORD ougi_warc[256] = { 0x00000000, 0x00F5ECDF, 0x00D3C4BE, 0x00C8928E, 0x00945E5A, 0x004D4744, 0x00393931, 0x00292921, 0x00181810, 0x00F7C0C0, 0x00BB7790, 0x00AB587D, 0x007D3F5B, 0x00484854, 0x0030303A, 0x00181820, 0x00F9E6D3, 0x000A0A0A, 0x00FF0000, 0x009C0000, 0x009C6600, 0x00FFEE00, 0x0064555F, 0x001F1C21, 0x00292129, 0x00EAE7EA, 0x00766C8D, 0x002B244F, 0x00FFFFC3, 0x00FFDC2B, 0x00D79B21, 0x00FCF2E9, 0x00F6DABD, 0x00080910, 0x000E0E0E, 0x000E0F1E, 0x00141736, 0x0016172D, 0x001F1F1F, 0x00191C47, 0x002A2A36, 0x00212769, 0x0026295E, 0x00303030, 0x002F3051, 0x00292F82, 0x003D3D3D, 0x0043444F, 0x003F4072, 0x003B3C84, 0x00353CA4, 0x00505050, 0x00434699, 0x004E4F68, 0x0054568E, 0x005658A0, 0x00636363, 0x0061616D, 0x005658BB, 0x005D5FB4, 0x006A6B86, 0x00717171, 0x006C6D99, 0x006568DE, 0x00F3CEA7, 0x00FFFFFF, 0x00FFFFFF, 0x00C9CADB, 0x0094967A, 0x00565747, 0x00F8FADE, 0x00BABBAB, 0x00E8EAC2, 0x00FFFFFC, 0x00BEBC98, 0x0072715C, 0x00D6D5C0, 0x001A1914, 0x007E7D78, 0x002E2C23, 0x00929087, 0x00423D31, 0x00A6A195, 0x006E6E6E, 0x000A0A0A, 0x005D6095, 0x00261809, 0x004C3013, 0x0072471D, 0x00986027, 0x00BE7731, 0x00E4913B, 0x00E79C50, 0x00EAA966, 0x00EDB57C, 0x00F0C192, 0x009395B8, 0x00090006, 0x000F000B, 0x001D000B, 0x000C090D, 0x00170316, 0x0020011F, 0x002D0113, 0x00290127, 0x00220C20, 0x00330130, 0x003C0217, 0x002C0927, 0x00191719, 0x004D011B, 0x003F023D, 0x002A1328, 0x005F0118, 0x004E024B, 0x00420D3C, 0x00391434, 0x002A2129, 0x00580254, 0x00650631, 0x00730122, 0x00520E4C, 0x00650162, 0x00431C3F, 0x00630B5F, 0x00850331, 0x00302F30, 0x00750371, 0x00581751, 0x0052234F, 0x00990339, 0x006E1167, 0x00850380, 0x003F353E, 0x00761C3B, 0x006E1A6A, 0x00B20141, 0x0084127B, 0x00960490, 0x00573250, 0x00652862, 0x00A80B4E, 0x007D1F79, 0x00464546, 0x00C7024E, 0x00961E46, 0x00AC06A2, 0x0099158F, 0x0072316F, 0x0062405E, 0x00DE035D, 0x008A2686, 0x008C3353, 0x00BD07B5, 0x00A817A3, 0x00535353, 0x00BD2159, 0x0072466F, 0x00F1086E, 0x008B3788, 0x00A6269A, 0x00CF0AC5, 0x00605E60, 0x00A0349B, 0x00DA226B, 0x00B429AD, 0x007E4F79, 0x00D912D4, 0x00C720C1, 0x00974494, 0x00B73D66, 0x00FB1982, 0x00AC3EA8, 0x008D5987, 0x00CD32C3, 0x00727072, 0x00E521DF, 0x00EB3580, 0x00A652A4, 0x00BE44B9, 0x00C84E79, 0x00946891, 0x00D740CE, 0x00B65AB3, 0x00838283, 0x00EF4A8F, 0x00EF39EA, 0x00D65E89, 0x00A0779C, 0x00D552CD, 0x00BD6CB9, 0x00F34AF0, 0x00918F91, 0x00E15AD8, 0x00ED659C, 0x00DA6AD5, 0x00F459F1, 0x00BB82B8, 0x009E9D9E, 0x00ED76A6, 0x00EF6AE9, 0x00DA7CD6, 0x00C595C2, 0x00F283B0, 0x00ABAAAA, 0x00F179EC, 0x00DE8DDB, 0x00F591BB, 0x00F48AF0, 0x00B7B4B8, 0x00DCA0D9, 0x00F697F1, 0x00CABEC9, 0x00FAA6CA, 0x00F2A7EF, 0x00F0B5ED, 0x00D0CFD0, 0x00FABCE9, 0x00E8CBE8, 0x00E0DCE1, 0x00FBCEF4, 0x00EFE3EE, 0x00F4EEF4, 0x00FDFDFD, 0x007475AD, 0x007273BD, 0x007D7E80, 0x007876CE, 0x0083839D, 0x007B7AE0, 0x008A8A8A, 0x008A8AB5, 0x008989D8, 0x00908FCF, 0x00999999, 0x009999A9, 0x009B9BBD, 0x009C9AEE, 0x00A8A8A8, 0x00A0A1D5, 0x00A8A8B3, 0x00B0B2CE, 0x00ADACF1, 0x00B3B4E1, 0x00BABABB, 0x00B6B6F0, 0x00C1C2DE, 0x00C2C2F2, 0x00CCCCCC, 0x00CAC9EF, 0x00CFD0D9, 0x00D3D2EF, 0x00DFDFDF, 0x00DEDEF6, 0x00E7E6F3, 0x00F6F6F9 };

DWORD _naked_PalettePatcherHook_ebx = 0;
DWORD _naked_PalettePatcherCallback_eax = 0;
void palettePatcherCallback() {
	// palette loads take a different route. this route.

	// this crashes on fhime. like all things do 
	// doesnt she have 2 loading anims? whats up with that?

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

	if (isWarc) {
		log("warc palette:");
		for (int j = 0; j < 256; j++) {
			log("%3d %08X", j, colors[(256 * 36) + j]);
		}
		log("-----");
	}
	
	for (int i = 0; i < 64; i++) {
	//for (int i = 36; i < 42; i++) {
		for (int j = 0; j < 256; j++) {
			
			DWORD tempColor = colors[(256 * i) + j];

			BYTE a = (tempColor & 0xFF000000) >> 24;

			if (isWarc && i == 36) {
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
		// eax, at this point, on a break, has the addr to a png????
		// the rectangles used to color the palette things?
		//cmp loadTextureFromPathLogger_needBreak, 1;
		//JNE _SKIP;
		//
		//mov loadTextureFromPathLogger_needBreak, 0;

		nop;
		nop;

		mov _naked_loadTextureFromPathUnknown_pngAddr, eax;

		mov eax, [esp + 0];
		mov _naked_loadTextureFromPathUnknown_pngLen, eax;

		mov eax, _naked_loadTextureFromPathUnknown_pngAddr;


		//int 3;
		nop;
	_SKIP:
	}

	PUSH_ALL;
	loadTextureFromPathUnknown();
	POP_ALL;

	__asm {
		call[_naked_loadTextureFromPathUnknown_FuncAddr];
	};

	__asm {
		push 004c8ae7h;
		ret;
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
	}

	PUSH_ALL;
	palettePatcherCallback();
	POP_ALL;

	__asm {
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
	//initPaletteLoad();
	initLoadTextureFromPathLogger();
	initPalettePatcher();

	log("initCSSModifications ran");

	init = true;
	return true;
}