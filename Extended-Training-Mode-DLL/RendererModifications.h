#pragma once
#include <set>
#include <map>

// todo, please move this over to a cpp
#include "DebugInfo.h"

// contains things which modify melty's rendering system internally.

extern bool useCustomShaders;
extern bool useDeerMode;

bool shouldThisBeColored(BYTE charID, DWORD pattern);


// 0x1C4 000111000100
// 0x004			0x040				0x080			0x100
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1
typedef struct MeltyVertexOrig {
	float x, y, z;
	float rhw; 
	DWORD diffuse;      
	DWORD specular;     
	float u, v;
} MeltyVertexOrig;

// general (non patching) funcs
DWORD linkedListLength = 0;
float drawY = 0.0f;
void _drawDebugMenu() {

	linkedListLength = 0;

	drawY = 0.0f;

}

// key is mystery texture addr, val is its obj offset
typedef struct LinkedListData {
	bool shouldColor = false;
	bool isDeer = false;
	DWORD object = 0;
	DWORD caller = 0;
} LinkedListData;
std::map<DWORD, LinkedListData> textureToObject;

bool pixelShaderNeedsReset = false;
IDirect3DPixelShader9* pPixelShader_backup = NULL;
IDirect3DPixelShader9* pPixelShader = NULL;
IDirect3DPixelShader9* pCustomShader = NULL;

void loadCustomShader();

bool hasTextureAddr(DWORD test) {
	return textureToObject.find(test) != textureToObject.end();
}

std::set<DWORD> skipTextureAddrs;

void renderModificationsFrameDone();

void describeObject(char* buffer, size_t buflen, const LinkedListData& info);

// actual funcs

DWORD listAppendHook_effectRetAddr = 0;
DWORD listAppendHook_objAddr = 0;
DWORD listAppendHook_texAddr = 0;

DWORD listAppendHook_effectRetAddr_pat = 0;
DWORD listAppendHook_objAddr_pat = 0;

DWORD listAppendHook_hitEffectRetAddr = 0;
DWORD listAppendHook_objAddr_hit = 0;

DWORD listAppendHook_callerAddr = 0;

DWORD _naked_drawCallHook_ebx;
void drawLoopHook();

void listAppendHook();

DWORD leadToDrawPrimHook_ret = 0;
DWORD drawPrimHook_texAddr = 0;
DWORD skipTextureDraw = 0;
void drawPrimHook();

void drawPrimCallback();


// init 

void initDrawIndexPrimHook();

void initEffectSelector();

bool initRenderModifications();

