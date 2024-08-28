#pragma once

#include <map>

extern IDirect3DDevice9* device;

// actual funcs

std::map<DWORD, BYTE*> bufferMap; // map input ECX addr to our emulated buffer

DWORD somethingPaletty_ECX;
void somethingPaletty() {

	DWORD start_somethingPaletty_ECX = somethingPaletty_ECX;

	BYTE* buffer = NULL;

	if (bufferMap.find(somethingPaletty_ECX) != bufferMap.end()) {
		
		// we found our emulated buffer, return.
		buffer = bufferMap[somethingPaletty_ECX];
		somethingPaletty_ECX = (DWORD)(buffer + 4); // REMEMBER THAT THE ECX IS ACTUALLY INDEXED STARTING AT - 4 ? ? ? ? ?

		log("matched ECX: %08X at %08X", start_somethingPaletty_ECX, somethingPaletty_ECX);

		return;
	}

	// we need to construct a new buffer, and copy the vals
	// each "entry" seems to be 0x30 bytes
	// currently, there are 6 * 0x30 bytes, goal is 7*0x30 bytes

	buffer = (BYTE*)malloc(7 * 0x30);
	bufferMap[somethingPaletty_ECX] = buffer;
	
	for (int i = 0; i < 7 * 0x30; i++) {
		if (i < 6 * 0x30) {
			buffer[i] = ((BYTE*)somethingPaletty_ECX)[i - 4];
		} else {
			buffer[i] = ((BYTE*)somethingPaletty_ECX)[(i - 0x30) - 4];
		}
	}

	somethingPaletty_ECX = (DWORD)(buffer + 4);

	log("nbew buffer made for ECX: %08X at %08X", start_somethingPaletty_ECX, somethingPaletty_ECX);
}

// naked funcs 

__declspec(naked) void _naked_somethingPaletty() {

	__asm {
		push ecx;
	};
	// bytes taken from 0x00486140
	__asm _emit 0x8B;
	__asm _emit 0x4C;
	__asm _emit 0x24;
	__asm _emit 0x08;

	__asm {
		push ebp;
		push esi;
		push edi;
		mov edi, 2;
		add ecx, 18h;
	}

	__asm {
		mov somethingPaletty_ECX, ecx;
	};

	PUSH_ALL;
	somethingPaletty();
	POP_ALL;

	__asm {
		mov ecx, somethingPaletty_ECX;
	};

	__asm {
		// doesnt require an extra var at least 
		push 00486150h;
		ret;
	}
}

// init 

void initSomethingPaletty() {
	patchJump(0x00486140, _naked_somethingPaletty);
}

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
	patchByte(0x004862ef + 2, paletteCount);
	
	// change some sort of bounds checking?
	patchByte(0x0048630c + 2, paletteCount);
	patchByte(0x00486311 + 2, paletteCount);
	
	// THESE TWO CAUSE ISSUES WITH PLAYER NOT SHOWING 
	//patchByte(0x00486361 + 2, paletteCount);
	//patchByte(0x0048636a + 2, paletteCount);
	//
	//patchByte(0x0048636d + 2, 0x50);
	//patchByte(0x00486373 + 2, 0x50);

	
	patchJump(0x00486328, 0x0048637b);
	
	// in the draw func ( i think ) changes the number of pages rendered
	patchByte(0x0048b0db + 2, paletteCount);

	//patchByte(0x0048af10 + 4, 0x07);

	// in 0048b250
	// the while loop end cond and the fist addr loaded have a difference 0x48,
	// which is ofc 0x24*2, so now its 0x2A*2, 0x54
	// 0x74d940
	//patchByte(0x0048b276 + 2, 0x40);

	// possibly the page number?
	//patchByte(0x0048631c + 4, 0x7);

	//idek 
	//patchByte(0x004862e6 + 2, 0x7);
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
	initSomethingPaletty();

	log("initCSSModifications ran");

	init = true;
	return true;
}