#ifndef UNICODE
#define UNICODE
#endif
#include "DirectXHook.h"
#include "DirectX.h"

void* pTable[119];
tPresent oPresent = nullptr;

bool bInit = false;


char* trampolineHook(char* src, char* dst, int len) {
    if (len < 5) return nullptr; //len should be 7

    char* gate = (char*)VirtualAlloc(0, len+5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gate,src,len); //copy original data into trampoline

    uintptr_t gateRel = (uintptr_t)(src - gate - 5); //relative address is computed after jmp instruction so we subtract 5

    *(gate+len) = (char)(0xe9); //write jmp opcode to end of trampoline
    *(uintptr_t*)(gate + len + 1) = gateRel; //write relative jmp to trampoline

    DWORD defaultProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &defaultProtection); //gives write perms

    memset(src, 0x90, len); //nops bypassed bytes
    uintptr_t rel = (uintptr_t)(dst-src-5);
    *src = (0xe9); //write jump opcode
    *(uintptr_t*)(src+1) = (uintptr_t)rel; //write rel jmp

    VirtualProtect(src,len,defaultProtection,&defaultProtection);
    return gate;
}

HRESULT APIENTRY hkPresent(LPDIRECT3DDEVICE9 pDevice, const RECT *pScourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion)
{
	if(!bInit) {
		bInit = true;
		initFont();
	}
	_doDrawCalls();

	return oPresent(pDevice, pScourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
