#pragma once

// contains things which modify melty's rendering system internally.


// general (non patching) funcs
DWORD linkedListLength = 0;
float drawY = 0.0f;
void _drawDebugMenu() {

	linkedListLength = 0;

	drawY = 0.0f;

}

// actual funcs

DWORD _naked_drawCallHook_eax;
DWORD _naked_drawCallHook_ebx;
DWORD _naked_drawCallHook_ecx;
DWORD _naked_drawCallHook_edx;
void drawLoopHook() {
	
	/*
	BYTE resourceType = *(BYTE*)(_naked_drawLoopHook_esi + 0x4);

	if (resourceType != 1) {
		return;
	}
	*/

	DWORD unknown1 = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	DWORD unknown2 = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	DWORD unknown3 = *(DWORD*)(unknown1 + 0x4);
	DWORD unknown4 = *(DWORD*)(_naked_drawCallHook_ebx + 0x58);
	

	TextDraw(0, drawY, 6, 0xFFFFFFFF, "%4d %08X %08X %08X %08X %08X", linkedListLength, _naked_drawCallHook_ebx, unknown1, unknown2, unknown3, unknown4);

	drawY += 6.0f;


	linkedListLength++;

}

// naked funcs

DWORD _naked_drawCallHook_Func = 0x004be290;
__declspec(naked) void _naked_drawCallHook() {

	__asm {
		mov _naked_drawCallHook_eax, eax;
		mov _naked_drawCallHook_ebx, ebx;
		mov _naked_drawCallHook_ecx, ecx;
		mov _naked_drawCallHook_edx, edx;
	}

	PUSH_ALL;
	drawLoopHook();
	POP_ALL;

	__asm {
		call[_naked_drawCallHook_Func];
	}

	__asm {
		push 004c03e4h;
		ret;
	}

}

// init 

void initDrawCallHook() {
	patchJump(0x004c03df, _naked_drawCallHook);
}

bool initRenderModifications() {

	//initDrawCallHook();





	return true;
}