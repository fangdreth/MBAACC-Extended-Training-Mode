#pragma once
#include "../Common/Common.h"

#include "dllmain.h"

extern HWND window;

extern bool bInit;

extern void* pTable[119];
extern LPDIRECT3DDEVICE9 device;

char* trampolineHook(char* src, char* dst, int len);

BOOL CALLBACK windowCallback(HWND handle, LPARAM lParam);

HWND getProcessWindow();

HRESULT APIENTRY hkPresent(LPDIRECT3DDEVICE9 pDevice, const RECT *pScourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);

typedef HRESULT(APIENTRY* tPresent)(LPDIRECT3DDEVICE9 pDevice, const RECT *pScourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);

extern tPresent oPresent;
