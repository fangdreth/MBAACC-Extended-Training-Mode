#pragma once

#include "..\Common\Common.h"
//#include <ws2tcpip.h>
//#include <winsock2.h>
//#include <windows.h>
#include <cstring> 
#include <cstdio>
#include <psapi.h>
#include <time.h>
#include <functional>
#include <TlHelp32.h>
#include <array>
#include <stdint.h>
#include <cmath>
#include <random>
#include <format>
#include <string.h>
#include <cstdarg>
#include <chrono>
#include <deque>
#include "resource.h"
#include <thread>

#include "..\Common\CharacterData.h"


#include <d3d9.h>
#include <d3dx9.h> // https://www.microsoft.com/en-us/download/details.aspx?id=6812
#include <dxerr.h>
#include <dsound.h>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))
#define SAFEMOD(a, b) (((b) + ((a) % (b))) % (b))

#define PUSH_CALLEE __asm \
{                         \
   __asm push ebx   \
   __asm push esi   \
   __asm push edi   \
}

#define POP_CALLEE __asm \
{                        \
   __asm pop edi   \
   __asm pop esi   \
   __asm pop ebx   \
}

#define PUSH_FRAME __asm \
{                        \
   __asm push ebp        \
   __asm mov ebp, esp    \
}

#define POP_FRAME __asm \
{                       \
   __asm pop ebp        \
}

#define PUSH_ALL __asm \
{                      \
__asm push esp		   \
__asm push ebp		   \
__asm push edi		   \
__asm push esi		   \
__asm push edx		   \
__asm push ecx		   \
__asm push ebx  	   \
__asm push eax		   \
__asm push ebp         \
__asm mov ebp, esp     \
}

#define POP_ALL __asm  \
{                      \
__asm pop ebp          \
__asm pop eax		   \
__asm pop ebx  	       \
__asm pop ecx		   \
__asm pop edx		   \
__asm pop esi		   \
__asm pop edi		   \
__asm pop ebp		   \
__asm pop esp		   \
}

// this was needed due to the preprocessor being a bitch and not accepting multiple emits on the same line
// i have had so many issues with the way this compiler works
#define emitByte(b) __asm _emit b;
#define emitWord(b) \
    __asm { _emit b & 0xFF } \
    __asm { _emit b >> 8 } 
#define emitDword(b) \
    __asm { _emit b & 0xFF } \
    __asm { _emit (b & 0xFF00) >> 8 } \
    __asm { _emit (b & 0xFF0000) >> 16 } \
    __asm { _emit (b & 0xFF000000) >> 24 } 
// if i wanted to make tolerable jump things that didnt use a variable, i would have to use a dummy call, and read the EIP from there, just to do one fucking call
// wait. i can only jump from reads from addresses, not addresses. 
// can i use eip as a addr?
// or tbh, i can just use pushes and be real weird about it
// ugh 

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define emitCall(addr) \
    __asm { push LINE_NAME } \
	__asm { push addr } \
    __asm { ret } \
    __asm { LINE_NAME: }

#define emitJump(addr) \
	__asm { push addr } \
    __asm { ret }


static_assert(sizeof(int*) == 4, "COMPILE PROGRAM AS 32BIT");

const DWORD dwBaseAddress = (0x00400000);

extern IDirect3DDevice9* device;

extern DWORD _naked_newPauseCallback2_IsPaused;
extern int needPause;

extern DWORD shouldDrawBackground;
extern DWORD shouldDrawHud;
extern DWORD shouldDrawGroundLine;
extern DWORD backgroundColor;
extern DWORD shouldDrawShadow;
extern DWORD fastReversePenalty;

extern DWORD __frameDoneCount;
extern DWORD unpausedFrameCount;

extern ADDRESS dwCasterBaseAddress;

extern bool logSaveState;

extern bool enableTAS;
extern bool disableFPSLimit;
extern bool bFreeze;

extern int showDebugMenu;

// -----

void __stdcall ___log(const char* msg);

void __stdcall log(const char* format, ...);

inline long long getMicroSec() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	//return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).count();
}

inline long long getNanoSec() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

bool __stdcall safeWrite();

bool __stdcall isPaused();

// -----

void frameStartCallback();

// non templated patch funcs
void __stdcall asmPatchMemcpy(void* dest, void* source, DWORD n);

// patch funcs, since these are templated they are ok(and honestly semi needed) to be declared in header
void __stdcall patchMemcpy(auto dst, auto src, size_t n)
{

	static_assert(sizeof(dst) == 4, "Type must be 4 bytes");
	static_assert(sizeof(src) == 4, "Type must be 4 bytes");

	LPVOID dest = reinterpret_cast<LPVOID>(dst);
	LPVOID source = reinterpret_cast<LPVOID>(src);

	DWORD oldProtect;
	VirtualProtect(dest, n, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, source, n);
	VirtualProtect(dest, n, oldProtect, NULL);
}

void __stdcall patchMemset(auto dst, BYTE v, size_t n)
{

	static_assert(sizeof(dst) == 4, "Type must be 4 bytes");

	LPVOID dest = reinterpret_cast<LPVOID>(dst);

	DWORD oldProtect;
	VirtualProtect(dest, n, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(dest, v, n);
	VirtualProtect(dest, n, oldProtect, NULL);
}

void __stdcall patchFunction(auto patchAddr_, auto newAddr_)
{

	static_assert(sizeof(patchAddr_) == 4, "Type must be 4 bytes");
	static_assert(sizeof(newAddr_) == 4, "Type must be 4 bytes");

	DWORD patchAddr = (DWORD)(patchAddr_);
	DWORD newAddr = (DWORD)(newAddr_);

	BYTE callCode[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
	DWORD funcOffset = newAddr - (patchAddr + 5);
	*(unsigned*)(&callCode[1]) = funcOffset;
	patchMemcpy(patchAddr, callCode, sizeof(callCode));
}

void __stdcall patchJump(auto patchAddr_, auto newAddr_)
{

	static_assert(sizeof(patchAddr_) == 4, "Type must be 4 bytes");
	static_assert(sizeof(newAddr_) == 4, "Type must be 4 bytes");

	DWORD patchAddr = (DWORD)(patchAddr_);
	DWORD newAddr = (DWORD)(newAddr_);

	BYTE callCode[] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	DWORD funcOffset = newAddr - (patchAddr + 5);
	*(unsigned*)(&callCode[1]) = funcOffset;
	patchMemcpy(patchAddr, callCode, sizeof(callCode));
}

void __stdcall patchByte(auto addr, const BYTE byte)
{
	static_assert(sizeof(addr) == 4, "Type must be 4 bytes");

	BYTE temp[] = { byte };

	patchMemcpy(addr, temp, 1);
}

bool __stdcall isAddrValid(DWORD addr);

void SetSeed(uint32_t nSeed);

void setFPSLimiter(bool b);