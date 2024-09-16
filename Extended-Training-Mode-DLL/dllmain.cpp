
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
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

#include "..\Common\Common.h"
#include "..\Common\CharacterData.h"
#include "FrameBar.h"

#include <d3d9.h>
#include <d3dx9.h> // https://www.microsoft.com/en-us/download/details.aspx?id=6812
#include <dxerr.h>
#include <dsound.h>

#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dsound.lib")

#pragma push_macro("optimize")
#pragma optimize("t", on) 


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))

static_assert(sizeof(int*) == 4, "COMPILE PROGRAM AS 32BIT");

typedef DWORD ADDRESS;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef uint32_t uint;

void enemyReversal();
void frameStartCallback();

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

const ADDRESS dwBaseAddress = (0x00400000);

DWORD addrEndScene = 0x663fb900;
DWORD addrEndScenePatch = 0x663fb996;

bool bCasterInit = false;
ADDRESS dwCasterBaseAddress = 0;

DWORD dwDevice = 0; // MASM is horrid when it comes to writing pointers vs value of pointer bc it has type checking. thats why this cant be a pointer
IDirect3DDevice9* device = NULL;

const ADDRESS INPUTDISPLAYTOGGLE = (dwBaseAddress + 0x001585f8);

DWORD tempRegister1;
DWORD tempRegister2;
DWORD tempRegister3;
DWORD tempRegister4;

DWORD __frameDoneCount;

int nFreezeKey;
int nFrameStepKey;
int nHitboxesDisplayKey;
int nFrameDataDisplayKey;
int nHighlightsOnKey;
int nSaveStateKey;

bool bFreeze = false;
bool bFrameDataDisplay = false;
bool bHitboxesDisplay = false;
bool bHighlightsOn = true;

uint8_t nRNGMode = RNG_OFF;
uint8_t nRNGRate = RNG_EVERY_FRAME;
uint32_t nCustomSeed = 0;
uint32_t nCustomRN = 0;

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

std::array<uint8_t, 4> arrDefaultHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrIdleHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrBlockingHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrHitHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrArmorHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrThrowProtectionHighlightSetting({ 255, 255, 255, 0 });

long long getMicroSec() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void __stdcall ___log(const char* msg)
{
	const char* ipAddress = "127.0.0.1";
	unsigned short port = 17474;

	int msgLen = strlen(msg);

	const char* message = msg;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		return;
	}

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}

	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress, &destAddr.sin_addr) <= 0)
	{
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	int sendResult = sendto(sendSocket, message, strlen(message), 0, (sockaddr*)&destAddr, sizeof(destAddr));
	if (sendResult == SOCKET_ERROR)
	{
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	closesocket(sendSocket);
	WSACleanup();
}

void __stdcall log(const char* format, ...) {
	static char buffer[1024]; // no more random char buffers everywhere.
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	___log(buffer);
	va_end(args);
}

void __stdcall debugLogBytes(BYTE* p)
{

	static char buffer[256];

	for (int i = 0; i < 16; i++) 
	{
		snprintf(buffer, 256, "%08X", *(BYTE*)p);
		p++;
	}
}

bool __stdcall safeWrite()
{
	//BYTE PauseFlag = *reinterpret_cast<BYTE*>(dwBaseAddress + dwPausedFlag);
	BYTE GameState = *reinterpret_cast<BYTE*>(dwBaseAddress + dwGameState);
	int FrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);

	if (//PauseFlag == 1 ||
		GameState != 1 ||
		FrameTimer == 0)
		return false;
	
	return true;
}

bool __stdcall isPaused()
{
	return *reinterpret_cast<BYTE*>(dwBaseAddress + dwPausedFlag);
}

static KeyState oSaveStateKey;
static KeyState oPrevSaveSlotKey;
static KeyState oNextSaveSlotKey;
static KeyState oFreezeKey;
static KeyState oFrameStepKey;
static KeyState oFrameDataDisplayKey;
static KeyState oHitboxesDisplayKey;
static KeyState oHighlightsOnKey;
static KeyState oFrameBarLeftScrollKey;
static KeyState oFrameBarRightScrollKey;
static KeyState oDecRNG;
static KeyState oIncRNG;
void setAllKeys()
{
	oSaveStateKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedSaveStateKey));
	oPrevSaveSlotKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedPrevSaveSlotKey));
	oNextSaveSlotKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedNextSaveSlotKey));
	
	oFreezeKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedFreezeKey));
	oFrameStepKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedFrameStepKey));
	oFrameDataDisplayKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedFrameDataDisplayKey));
	oHitboxesDisplayKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedHitboxesDisplayKey));
	oHighlightsOnKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedHighlightsOnKey));
	oFrameBarLeftScrollKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedFrameBarScrollLeftKey));
	oFrameBarRightScrollKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedFrameBarScrollRightKey));
	oIncRNG.setKey(*(uint8_t*)(dwBaseAddress + adSharedRNGIncKey));
	oDecRNG.setKey(*(uint8_t*)(dwBaseAddress + adSharedRNGDecKey));
}

// patch funcs
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

// the patch func being templated causes problems when calling from asm
void __stdcall asmPatchMemcpy(void* dest, void* source, DWORD n)
{
	DWORD oldProtect;
	VirtualProtect(dest, n, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, source, n);
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

#include "DirectX.h"

// actual functions 

extern "C" int asmDrawText(int w, int h, int x, int y, const char* text, int alpha, int shade, int layer, void* addr, int spacing, int idek, char* out);

void drawText(int x, int y, int w, int h, const char* text, int alpha, int shade, int layer = 0x2cc, void* font = (void*)adFont2)
{
	// text was initially just char*, i made it const, is that going to be ok?
	asmDrawText(w, h, x, y, text, alpha, shade, layer, font, 0, 0, 0);
}

void drawText(int x, int y, const char* text, int textSize = 16, ADDRESS font = adFont1)
{

	static_assert(sizeof(font) == 4, "Type must be 4 bytes");

	int len = strnlen_s(text, 1024);

	// 0x80 - 0x20 - 1, max (non extended) ascii val - 0x20(space). -1 is for the delete. this is almost definitely stored elsewhere in melty, i just dont know where
	// actually its at 0041d5b0, but still, not worth
	// https://gist.github.com/aminnj/5ca372aa2def72fb017b531c894afdca as a starter
	constexpr float charWidths[0x80 - 0x20 - 1] = {
		//4.4453125,   // ' '
		8.8984375, // ' '
		4.4453125,   // '!'
		5.6796875,   // '"'
		8.8984375,   // '#'
		8.8984375,   // '$'
		14.2265625,  // '%'
		10.671875,   // '&'
		3.0546875,   // '''
		5.328125,    // '('
		5.328125,    // ')'
		6.2265625,   // '*'
		9.34375,     // '+'
		4.4453125,   // ','
		5.328125,    // '-'
		8.8984375,   // '.'
		4.4453125,   // '/'
		8.8984375,   // '0'
		8.8984375,   // '1'
		8.8984375,   // '2'
		8.8984375,   // '3'
		8.8984375,   // '4'
		8.8984375,   // '5'
		8.8984375,   // '6'
		8.8984375,   // '7'
		8.8984375,   // '8'
		8.8984375,   // '9'
		4.4453125,   // ':'
		4.4453125,   // ';'
		9.34375,     // '<'
		9.34375,     // '='
		9.34375,     // '>'
		8.8984375,   // '?'
		16.2421875,  // '@'
		10.671875,   // 'A'
		10.671875,   // 'B'
		11.5546875,  // 'C'
		11.5546875,  // 'D'
		10.671875,   // 'E'
		9.7734375,   // 'F'
		12.4453125,  // 'G'
		11.5546875,  // 'H'
		4.4453125,   // 'I'
		8,           // 'J'
		10.671875,   // 'K'
		8.8984375,   // 'L'
		13.328125,   // 'M'
		11.5546875,  // 'N'
		12.4453125,  // 'O'
		10.671875,   // 'P'
		12.4453125,  // 'Q'
		11.5546875,  // 'R'
		10.671875,   // 'S'
		9.7734375,   // 'T'
		11.5546875,  // 'U'
		10.671875,   // 'V'
		15.1015625,  // 'W'
		10.671875,   // 'X'
		10.671875,   // 'Y'
		9.7734375,   // 'Z'
		4.4453125,   // '['
		4.4453125,   // '\'
		4.4453125,   // ']'
		7.5078125,   // '^'
		8.8984375,   // '_'
		5.328125,    // '`'
		8.8984375,   // 'a'
		8.8984375,   // 'b'
		8,           // 'c'
		8.8984375,   // 'd'
		8.8984375,   // 'e'
		4.15921875,  // 'f'
		8.8984375,   // 'g'
		8.8984375,   // 'h'
		3.5546875,   // 'i'
		3.5546875,   // 'j'
		8,           // 'k'
		3.5546875,   // 'l'
		13.328125,   // 'm'
		8.8984375,   // 'n'
		8.8984375,   // 'o'
		8.8984375,   // 'p'
		8.8984375,   // 'q'
		5.328125,    // 'r'
		8,           // 's'
		4.4453125,   // 't'
		8.8984375,   // 'u'
		8,           // 'v'
		11.5546875,  // 'w'
		8,           // 'x'
		8,           // 'y'
		8,           // 'z'
		5.34375,     // '{'
		4.15625,     // '|'
		5.34375,     // '}'
		9.34375,     // '~'
	};
	
	float tempWidth = 0;
	const char* c = text;

	char tempChar[2] = { '\0', '\0' };

	float xVal = x;
	tempWidth = (8.0f * textSize * 0.0625);

	while (*c) {
		if (*c < ' ' || *c > '~') {
			c++;
			continue;
		}
		//tempWidth += charWidths[*c - 0x20];
		//tempWidth += 8.8984375;


		tempChar[0] = *c;
		drawText((int)xVal, y, (int)tempWidth, textSize, tempChar, 0xFF, 0xFF, 0x02CC, (void*)font); // this is horrid

		xVal += tempWidth;
		//tempWidth += 15.0;
		c++;
	}

	tempWidth = strnlen_s(text, 256) * 15.0f;

	tempWidth *= textSize * 0.0625;

	//drawText(x, y, (int)tempWidth, textSize, text, 0xFF, 0xFF, 0x02CC, (void*)font);
}

void drawTextWithBorder(int x, int y, int w, int h, const char* text)
{
	profileFunction();
	// i am not proud of this.

	/*
	drawText(x, y, w, h, text, 0xFF, 0xFF);
	
	drawText(x - 1, y, w, h, text, 0xFF, 0x00);
	drawText(x + 1, y, w, h, text, 0xFF, 0x00);
	drawText(x, y - 1, w, h, text, 0xFF, 0x00);
	drawText(x, y + 1, w, h, text, 0xFF, 0x00);
	*/

	char tempBuf[2] = { '\0', '\0' };

	w = w - 1;

	const char* c = text;

	while (*c) {

		if (*c == ' ') {
			x += w;
			c++;
			continue;
		}

		tempBuf[0] = *c;

		drawText(x, y, w, h, tempBuf, 0xFF, 0xFF);

		drawText(x - 1, y, w, h, tempBuf, 0xFF, 0x00);
		drawText(x + 1, y, w, h, tempBuf, 0xFF, 0x00);
		drawText(x, y - 1, w, h, tempBuf, 0xFF, 0x00);
		drawText(x, y + 1, w, h, tempBuf, 0xFF, 0x00);

		x += w;

		c++;
	}
}

extern "C" int asmDrawRect(int screenXAddr, int screenYAddr, int width, int height, int A, int B, int C, int D, int layer);

void drawRect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a, int layer = 0x2cc)
{
	int colVal = (a << 24) | (r << 16) | (g << 8) | (b << 0);
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void drawRect(int x, int y, int w, int h, DWORD colVal, int layer = 0x2cc)
{
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void drawBorder(int x, int y, int w, int h, DWORD ARGB=0x8042e5f4)
{
	// there must be a better way of doing this than using 4 rects
	// framestop draws less intrusive rects. figure out how
	// the lines are much clearer. most likely not calling melty draw methods but dxd3 methods
	// will need to look into more melty drawing methods

	constexpr int lineWidth = 1;

	//drawRect(x, y, w, lineWidth, ARGB);
	//drawRect(x, y + h - 1, w, lineWidth, ARGB);
	//drawRect(x, y + 1, lineWidth, h - 2, ARGB);
	//drawRect(x + w - 1, y + 1, lineWidth, h - 2, ARGB);

	drawRect(x, y, w, lineWidth, ARGB);
	drawRect(x, y, lineWidth, h, ARGB);

	drawRect(x + w - 1, y, lineWidth, h, ARGB);
	drawRect(x, y + h - 1, w, lineWidth, ARGB);
}

void drawBorderWithHighlight(int x, int y, int w, int h, DWORD ARGB = 0x8042e5f4)
{
	drawBorder(x, y, w, h, ARGB);

	BYTE r = (ARGB & 0x00FF0000) >> 16;
	BYTE g = (ARGB & 0x0000FF00) >> 8;
	BYTE b = (ARGB & 0x000000FF) >> 0;
	
	drawRect(x, y, w, h, r, g, b, 0x38);
}

// -----

void scaleCords(const float xOrig, const float yOrig, float& x1Cord, float& y1Cord, float& x2Cord, float& y2Cord)
{
	x1Cord = xOrig + (x1Cord - xOrig) * 0.5;
	y1Cord = yOrig + (y1Cord - yOrig) * 0.5;
	x2Cord = xOrig + (x2Cord - xOrig) * 0.5;
	y2Cord = yOrig + (y2Cord - yOrig) * 0.5;
}

DWORD getObjFrameDataPointer(DWORD objAddr)
{
	
	int objState = *(DWORD*)(objAddr + 0x14);
	int objPattern = *(DWORD*)(objAddr + 0x10);

	DWORD baseFrameDataPtr1 = *(DWORD*)(objAddr + 0x330);
	if (baseFrameDataPtr1 == 0) { return 0; }

	DWORD baseFrameDataPtr2 = *(DWORD*)(baseFrameDataPtr1);
	if (baseFrameDataPtr2 == 0) { return 0; }

	DWORD unknownPtr1 = *(DWORD*)(baseFrameDataPtr2 + 0x4);
	if (unknownPtr1 == 0) { return 0; }

	DWORD unknownPtr2 = *(DWORD*)(unknownPtr1 + 0x4);
	if (unknownPtr2 == 0) { return 0; }

	DWORD basePatternPtr = *(DWORD*)(unknownPtr2 + (objPattern * 0x4));
	if (basePatternPtr == 0) { return 0; }

	DWORD unknownPtr3 = *(DWORD*)(basePatternPtr + 0x34);
	if (unknownPtr3 == 0) { return 0; }

	DWORD unknownPtr4 = *(DWORD*)(unknownPtr3 + 0x4);
	if (unknownPtr4 == 0) { return 0; }

	DWORD baseStatePtr = unknownPtr4 + (objState * 0x54);

	return baseStatePtr;
}

void drawObject(DWORD objAddr, bool isProjectile)
{
	int xPos = *(DWORD*)(objAddr + 0x108);
	int yPos = *(DWORD*)(objAddr + 0x10C);
	bool facingLeft = *(BYTE*)(objAddr + 0x314);
	
	DWORD objFramePtr = getObjFrameDataPointer(objAddr);
	
	if (objFramePtr == 0) {
		return;
	}

	// -----

	float isRight = 1;
	if (facingLeft) {
		isRight = -1;
	}

	float windowWidth = *(uint32_t*)0x0054d048;
	float windowHeight = *(uint32_t*)0x0054d04c;

	int cameraX = *(int*)(0x0055dec4);
	int cameraY = *(int*)(0x0055dec8);
	float cameraZoom = *(float*)(0x0054eb70);

	float xCamTemp = ((((float)(xPos - cameraX) * cameraZoom) / 128.0) * (windowWidth / 640.0) + windowWidth / 2.0);
	float yCamTemp = ((((float)(yPos - cameraY) * cameraZoom) / 128.0 - 49.0) * (windowHeight / 480.0) + windowHeight);
	
	float tempFloat;

	float x1Cord, x2Cord, y1Cord, y2Cord;

	uint32_t drawColor;

	// -----

	// origin?

	drawColor = 0xFF42E5F4;

	x1Cord = ((float)xCamTemp - (windowWidth / 640.0) * cameraZoom * 5.0);
	x2Cord = ((windowWidth / 640.0) * cameraZoom * 5.0 + (float)xCamTemp);
	y1Cord = ((float)yCamTemp - (windowWidth / 640.0) * cameraZoom * 5.0);
	y2Cord = yCamTemp;

	x1Cord = floor((float)x1Cord * (640.0 / windowWidth));
	x2Cord = floor((float)x2Cord * (640.0 / windowWidth));
	y1Cord = floor((float)y1Cord * (480.0 / windowHeight));
	y2Cord = floor((float)y2Cord * (480.0 / windowHeight));

	if(!isProjectile) {
		drawBorder((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
	}

	// current vibes say that the origin is in the bottom center of the above rectangle, needs more non vibe based confirmation though
	float xOrig = x1Cord + ((x2Cord - x1Cord) / 2);
	float yOrig = y2Cord;

	// lots of stuff here seems to interact with 0x330. this is off of 0x320, is this an issue?
	// should have made a box/poit class
	DWORD animDataPtr = *(DWORD*)(objAddr + 0x320);
	bool isPat = *(BYTE*)(animDataPtr + 0x0);
	
	// non hitboxes
	if (*(DWORD*)(objFramePtr + 0x4C) != 0) {
		unsigned unknownLoopLimit = *(BYTE*)(objFramePtr + 0x42);
		for (unsigned index = 0; index < unknownLoopLimit; index++) {

			if (*(int*)(*(int*)(objFramePtr + 0x4c) + index * 4) == 0) {
				continue;
			}

			switch (index) {
				case 0x0:
					drawColor = 0xFFD0D0D0;
					break;
				case 0x9:
				case 0xA:
					drawColor = 0xFFFF00FF;
					break;
				case 0xB:
					drawColor = 0xFFFFFF00;
					break;
				default:
					if (index < 0xC) {
						drawColor = 0xFF00FF00;
					} else {
						drawColor = 0xFF0000FF;
					}
					break;
			}

			tempFloat = (float)isRight * (windowWidth / 640.0) * cameraZoom;
			x1Cord = ((float)**(short**)(*(int*)(objFramePtr + 0x4c) + index * 4) * (tempFloat + tempFloat) + (float)xCamTemp);

			tempFloat = (float)isRight * (windowWidth / 640.0) * cameraZoom;
			x2Cord = ((float)*(short*)(*(int*)(*(int*)(objFramePtr + 0x4c) + index * 4) + 4) * (tempFloat + tempFloat) + (float)xCamTemp);

			y1Cord = ((float)((int)*(short*)(*(int*)(*(int*)(objFramePtr + 0x4c) + index * 4) + 2) << 1) * (windowHeight / 480.0) * cameraZoom + (float)yCamTemp);

			tempFloat = (windowHeight / 480.0) * cameraZoom;
			y2Cord = ((float)*(short*)(*(int*)(*(int*)(objFramePtr + 0x4c) + index * 4) + 6) * (tempFloat + tempFloat) + (float)yCamTemp);

			x1Cord = floor((float)x1Cord * (640.0 / windowWidth));
			x2Cord = floor((float)x2Cord * (640.0 / windowWidth));
			y1Cord = floor((float)y1Cord * (480.0 / windowHeight));
			y2Cord = floor((float)y2Cord * (480.0 / windowHeight));

			if (isPat) {
				scaleCords(xOrig, yOrig, x1Cord, y1Cord, x2Cord, y2Cord);
			}
			
			drawBorderWithHighlight((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
		}
	}

	// hitboxes
	if (*(DWORD*)(objFramePtr + 0x50) != 0) {
		drawColor = 0xFFFF0000;
		unsigned unknownLoopLimit = *(BYTE*)(objFramePtr + 0x43);
		for (unsigned index = 0; index < unknownLoopLimit; index++) {
			if (*(int*)(*(int*)(objFramePtr + 0x50) + index * 4) == 0) {
				continue;
			}

			tempFloat = (float)isRight * (windowWidth / 640.0) * cameraZoom;
			x1Cord = ((float)**(short**)(*(int*)(objFramePtr + 0x50) + index * 4) * (tempFloat + tempFloat) + (float)xCamTemp);

			x2Cord = ((float)(*(short*)(*(int*)(*(int*)(objFramePtr + 0x50) + index * 4) + 4) * 2 + -1) * (float)isRight * (windowWidth / 640.0) * cameraZoom + (float)xCamTemp);

			y1Cord = ((float)(*(short*)(*(int*)(*(int*)(objFramePtr + 0x50) + index * 4) + 2) * 2 + 1) * (windowHeight / 480.0) * cameraZoom + (float)yCamTemp);

			y2Cord = ((float)((int)*(short*)(*(int*)(*(int*)(objFramePtr + 0x50) + index * 4) + 6) << 1) * (windowHeight / 480.0) * cameraZoom + (float)yCamTemp);

			x1Cord = floor((float)x1Cord * (640.0 / windowWidth));
			x2Cord = floor((float)x2Cord * (640.0 / windowWidth));
			y1Cord = floor((float)y1Cord * (480.0 / windowHeight));
			y2Cord = floor((float)y2Cord * (480.0 / windowHeight));

			if (isPat) {
				scaleCords(xOrig, yOrig, x1Cord, y1Cord, x2Cord, y2Cord);
			}

			drawBorderWithHighlight((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
		}
	}
}

//In-game frame bar
void drawFrameBar()
{
	profileFunction();

	if (!safeWrite())
		return;

	FrameBar(P1, P2, P3, P4);

	int nBarDrawCounter = 0;

	nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
	short sAdjustedScroll = min(min(nBarCounter - DISPLAY_RANGE, BAR_MEMORY_SIZE - DISPLAY_RANGE), nBarScrolling);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - DISPLAY_RANGE - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= DISPLAY_RANGE)
	{
		nForStart = 0;
		nForEnd = DISPLAY_RANGE;
	}

	int j = 0;
	for (int i = nForStart; i < nForEnd; i++)
	{
		if (i < 0)
		{
			j = i + BAR_MEMORY_SIZE;
		}
		else
		{
			j = i;
		}
		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 1, 7, 2, (*Player1).dwColorBar2[j][0]);
		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 11, 7, 2, (*Player1).dwColorBar2[j][1]);

		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 14, 7, 2, (*Player2).dwColorBar2[j][0]);
		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 24, 7, 2, (*Player2).dwColorBar2[j][1]);

		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 2, 4, 10, (*Player1).dwColorBar1[j][0]);
		drawRect(20 + 8 * nBarDrawCounter + 4, nFrameBarY + 2, 3, 10, (*Player1).dwColorBar1[j][1]);
		drawRect(20 + 8 * nBarDrawCounter, nFrameBarY + 15, 4, 10, (*Player2).dwColorBar1[j][0]);
		drawRect(20 + 8 * nBarDrawCounter + 4, nFrameBarY + 15, 3, 10, (*Player2).dwColorBar1[j][1]);

		static char buffer[256];

		if ((*Player1).nNumBar[j][0] >= 0)
		{
			int nLength = floor(log10((*Player1).nNumBar[j][0]));
			snprintf(buffer, 256, "%i", (*Player1).nNumBar[j][0]);
			drawTextWithBorder(20 + 8 * nBarDrawCounter - 7 * nLength, nFrameBarY + 3, 7, 10, buffer);
		}

		if ((*Player2).nNumBar[j][0] >= 0)
		{
			int nLength = floor(log10((*Player2).nNumBar[j][0]));
			snprintf(buffer, 256, "%i", (*Player2).nNumBar[j][0]);
			drawTextWithBorder(20 + 8 * nBarDrawCounter - 7 * nLength, nFrameBarY + 16, 7, 10, buffer);
		}
		nBarDrawCounter++;
	}
	static char buffer[256];
	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", (*Player1).nFirstActive % 1000, (*Player1).nInactiveMemory % 1000, nPlayerAdvantage % 1000);
	drawTextWithBorder(20, nFrameBarY - 11, 7, 10, buffer);

	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", (*Player2).nFirstActive % 1000, (*Player2).nInactiveMemory % 1000, -nPlayerAdvantage % 1000);
	drawTextWithBorder(20, nFrameBarY + 28, 7, 10, buffer);

	drawRect(18, nFrameBarY, 602, 27, 0xFF000000); //Background

}

void drawStats()
{
	int nFrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);
	int nResetOffset = 0;
	if (nFrameTimer < 20)
		nResetOffset = 300.0f * (1.0f - nFrameTimer / 19.0f);

	static char buffer[256];
	
	int nP1RedHealth = *(int*)(dwBaseAddress + dwP1RedHealth);
	int nP1RedHealthX;
	if (nP1RedHealth >= 9200)
		nP1RedHealthX = 60;
	else if (nP1RedHealth <= 2200)
		nP1RedHealthX = 190;
	else
		nP1RedHealthX = 20.0f + (1.0f - (float)nP1RedHealth / 11400.0f) * (230.0f - 20.0f);
	snprintf(buffer, 256, "%5d", nP1RedHealth);
	drawTextWithBorder(nP1RedHealthX - nResetOffset, 40, 10, 10, buffer);

	snprintf(buffer, 256, "%5d", *(int*)(dwBaseAddress + dwP1Health));
	drawTextWithBorder(230 - nResetOffset, 40, 10, 10, buffer);

	int nP2RedHealth = *(int*)(dwBaseAddress + dwP2RedHealth);
	int nP2RedHealthX;
	if (nP2RedHealth >= 9200)
		nP2RedHealthX = 535;
	else if (nP2RedHealth <= 2200)
		nP2RedHealthX = 406;
	else
		nP2RedHealthX = 575.0f - (1.0f - (float)nP2RedHealth / 11400.0f) * (575.0f - 366.0f);
	snprintf(buffer, 256, "%5d", nP2RedHealth);
	drawTextWithBorder(nP2RedHealthX + nResetOffset, 40, 10, 10, buffer);

	snprintf(buffer, 256, "%5d", *(int*)(dwBaseAddress + dwP2Health));
	drawTextWithBorder(366 + nResetOffset, 40, 10, 10, buffer);


	snprintf(buffer, 256, "%5.0f", *(float*)(dwBaseAddress + dwP1GuardAmount));
	drawTextWithBorder(234 - nResetOffset, 58, 8, 9, buffer);
	snprintf(buffer, 256, "%1.3f", *(float*)(dwBaseAddress + dwP1GuardQuality));
	drawTextWithBorder(244 - nResetOffset, 67, 6, 9, buffer);

	snprintf(buffer, 256, "%5.0f", *(float*)(dwBaseAddress + dwP2GuardAmount));
	drawTextWithBorder(368 + nResetOffset, 58, 8, 9, buffer);
	snprintf(buffer, 256, "%1.3f", *(float*)(dwBaseAddress + dwP2GuardQuality));
	drawTextWithBorder(369 + nResetOffset, 67, 6, 9, buffer);
}

void drawFrameData()
{

	if (!safeWrite())
		return;

	/*
	// for unknown reasons, this func sometimes gets double called?
	// this fixes this, but then causes flickering. more needs to be looked into on this
	static int prevFrameCalled = -1;
	int currentFrame = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);

	if (prevFrameCalled == currentFrame) {
		return;
	}
	prevFrameCalled = currentFrame;
	*/

	drawObject(0x00555130 + (0xAFC * 0), false); // P1
	drawObject(0x00555130 + (0xAFC * 1), false); // P2
	drawObject(0x00555130 + (0xAFC * 2), false); // P3
	drawObject(0x00555130 + (0xAFC * 3), false); // P4

	// draw all effects

	for(unsigned index = 0; index < 1000; index++) {
		if (((*(int*)(index * 0x33c + 0x67bde8) != 0) && (*(char*)(index * 0x33c + 0x67be09) == '\0'))) {
			drawObject(index * 0x33c + 0x67bde8, true);
		}
	}
}

void highlightStates()
{
	static int nP1ThrowProtectionTimer = 0;
	static int nP2ThrowProtectionTimer = 0;

	if (!safeWrite()) {
		return;
	}

	static int nOldFrameTimer = 0;
	int nFrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);	// frametimer is prob not needed
	int nGlobalExFlash = *reinterpret_cast<int*>(dwBaseAddress + dwGlobalExFlash);
	int nP1ExFlash = *reinterpret_cast<int*>(dwBaseAddress + dwP1ExFlash);
	int nP2ExFlash = *reinterpret_cast<int*>(dwBaseAddress + dwP2ExFlash);
	if (nFrameTimer != nOldFrameTimer && nGlobalExFlash == 0 && nP1ExFlash == 0 && nP2ExFlash == 0)
	{
		nP1ThrowProtectionTimer = max(0, nP1ThrowProtectionTimer - 1);
		nP2ThrowProtectionTimer = max(0, nP2ThrowProtectionTimer - 1);
	}
	nOldFrameTimer = nFrameTimer;

	if (!bHighlightsOn)
	{
		arrIdleHighlightSetting = { 255, 255, 255, 0 };
		arrBlockingHighlightSetting = { 255, 255, 255, 0 };
		arrHitHighlightSetting = { 255, 255, 255, 0 };
		arrArmorHighlightSetting = { 255, 255, 255, 0 };
		arrThrowProtectionHighlightSetting = { 255, 255, 255, 0 };
	}

	auto updateAnimation = [](DWORD animDataAddr, BYTE blockState, DWORD patternState, DWORD notInCombo, BYTE armorTimer, DWORD throwInvuln) -> void
		{
			// the order of this if block denotes the priority for each highlight
			if (arrBlockingHighlightSetting[3] == 1 && blockState == 1)	// BLOCKING
				patchMemcpy(animDataAddr + 0x18, arrBlockingHighlightSetting.data(), 3);
			else if (arrArmorHighlightSetting[3] == 1 && armorTimer != 0)
				patchMemcpy(animDataAddr + 0x18, arrArmorHighlightSetting.data(), 3);
			else if (arrHitHighlightSetting[3] == 1 && notInCombo == 0)
				patchMemcpy(animDataAddr + 0x18, arrHitHighlightSetting.data(), 3);
			else if (arrThrowProtectionHighlightSetting[3] == 1 && throwInvuln != 0)
				patchMemcpy(animDataAddr + 0x18, arrThrowProtectionHighlightSetting.data(), 3);
			else if (arrIdleHighlightSetting[3] == 1 && blockState != 1) // last check is IDLE
			{
				switch (patternState)
				{
				case 0:			// stand
				case 10:		// walk forward
				case 11:		// walk back
				case 12:		// to crouch
				case 13:		// crouch
				case 14:		// from crouch
				case 15:		// turn around
				case 17:		// stand block
				case 18:		// crouch block
				case 19:		// jump block
				case 35:		// j9
				case 36:		// j8
				case 37:		// j7
				case 38:		// dj9
				case 39:		// dj 8
				case 40:		// dj7
				case 360:		// sj8
					// this does not work for every character.
					// come back to this and fix it later.
					patchMemcpy(animDataAddr + 0x18, arrIdleHighlightSetting.data(), 3);
					break;
				default:
					break;
				}
			}
			else
			{
				// reset the color in case it falls through
				patchMemcpy(animDataAddr + 0x18, arrDefaultHighlightSetting.data(), 3);
			}

			
		};

	/*
	the states that are used here, are they reading the previous frames info?

	when blocking is turned on, this program displays the idle as red, fangs doesnt, why
	is it my thing, or one of theirs?
	either they are modifying blocking, or this is the a frame late

	*/

	// i strongly dislike my method of derefing here. a class could fix this
	if (*(DWORD*)(dwBaseAddress + dwP1AnimationPtr) > dwBaseAddress)
	{
		updateAnimation(*(DWORD*)(dwBaseAddress + dwP1AnimationPtr),
						*(BYTE*)(dwBaseAddress + dwP1Blocking),
						*(DWORD*)(dwBaseAddress + dwP1PatternRead),
						*(DWORD*)(dwBaseAddress + dwP1NotInCombo),
						*(BYTE*)(dwBaseAddress + dwP1ArmorTimer),
						*(BYTE*)(dwBaseAddress + dwP1ThrowInvuln));
	}

	if (*(DWORD*)(dwBaseAddress + dwP2AnimationPtr) > dwBaseAddress)
	{
		updateAnimation(*(DWORD*)(dwBaseAddress + dwP2AnimationPtr),
						*(BYTE*)(dwBaseAddress + dwP2Blocking),
						*(DWORD*)(dwBaseAddress + dwP2PatternRead),
						*(DWORD*)(dwBaseAddress + dwP2NotInCombo),
						*(BYTE*)(dwBaseAddress + dwP2ArmorTimer),
						*(BYTE*)(dwBaseAddress + dwP2ThrowInvuln));
	}
}

void SetSeed(uint32_t nSeed)
{
	*(uint32_t*)(dwBaseAddress + adRNGIndex) = 55;

	std::srand(nSeed);
	
	uint8_t nOffset = 1;
	do
	{
		*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * nOffset++) = std::rand() + std::rand() * 0x10000;
	} while (nOffset < 56);
}

void SetRN(uint32_t nRN)
{
	if (nRNGRate == RNG_EVERY_FRAME)
	{
		*(uint32_t*)(dwBaseAddress + adRNGIndex) = 55;
		uint8_t nOffset = 1;
		do
		{
			*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * nOffset++) = (nOffset < 22) ? nRN : 0;
		} while (nOffset < 56);
	}
	else if (nRNGRate == RNG_EVERY_RESET)
	{
		SetSeed(nRN);
		*(uint32_t*)(dwBaseAddress + adRNGIndex) = 55;
		uint32_t nRand = std::rand() + std::rand() * 0x10000;
		*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * 1) = nRand + nRN;
		*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * 22) = nRand;
	}
}

// windows Sleep, the func being overitten is an stdcall, which is why we have __stdcall
void __stdcall pauseCallback(DWORD dwMilliseconds)
{
	
	
	// i am unsure if doing this here is the best location, but it has been working
	// and weird things happen if i call it right after i grab the device
	// please never move this
	static bool isDirectXHooked = false;
	if (!isDirectXHooked) {
		isDirectXHooked = HookDirectX();
	}
	
	
	
	
	
	Sleep(dwMilliseconds);
}


int nOldMot;
int nMot = 0;
bool bReversaled = false;
bool bDelayingReversal = false;
int nTempReversalDelayFrames = 0;
int nReversalIndex1 = 0;
int nReversalIndex2 = 0;
int nReversalIndex3 = 0;
int nReversalIndex4 = 0;
int nReversalDelayFrames = 0;
int nReversalType = REVERSAL_NORMAL;
std::vector<std::string> vPatternNames = GetEmptyPatternList();
std::vector<int> vAirReversals;
std::vector<int> vGroundReversals;
#ifdef not_working
void enemyReversal()
{
	BYTE PauseFlag = *reinterpret_cast<BYTE*>(dwBaseAddress + dwPausedFlag);
	if (PauseFlag == 1)
		return;

	nOldMot = nMot;
	int nMot = *reinterpret_cast<int*>(dwBaseAddress + dwMot);
	int nHitstun = *reinterpret_cast<int*>(dwBaseAddress + dwP1HitstunRemaining + dwP2Offset);
	int nP2Y = *reinterpret_cast<int*>(dwBaseAddress + dwP2Y);
	int nFrameCounter = *reinterpret_cast<int*>(dwBaseAddress + dwRoundTime);
	int nP2CharacterNumber = *reinterpret_cast<int*>(dwBaseAddress + dwP2CharNumber);
	int nP2Moon = *reinterpret_cast<int*>(dwBaseAddress + dwP2CharMoon);
	int nP2CharacterID = 10 * nP2CharacterNumber + nP2Moon;
	vPatternNames = GetPatternList(nP2CharacterID);
	PopulateAirAndGroundReversals(&vAirReversals, &vGroundReversals, nP2CharacterID, &vPatternNames, nReversalIndex1, nReversalIndex2, nReversalIndex3, nReversalIndex4);

	char arrTextBuffer[256];
	snprintf(arrTextBuffer, 256, "%3d", nHitstun);
	drawText(230, 420 + (16 * 0), arrTextBuffer, 64);

	if (nFrameCounter == 0)
		bReversaled = true;
	if (nFrameCounter == 2)
		bReversaled = false;

	if (nHitstun != 0)
	{
		//int x = 3;
		//patchMemcpy(dwBaseAddress + dwP2PatternSet, &x, 4);
	}
	return;
	
	// if training was not just reset and if at least one move was selected
	if (nFrameCounter != 0 && GetPattern(nP2CharacterID, vPatternNames[nReversalIndex1]) != 0)
	{
		if ((nHitstun != 0) && (nMot != nOldMot))
		{
			if (bReversaled)
				bReversaled = false;
		}
		else if (!bReversaled)
		{
			bReversaled = true;
			if (nReversalType != REVERSAL_RANDOM || rand() % 2 == 0)
			{
				std::vector<int> vValidReversals = (nP2Y == 0 ? vGroundReversals : vAirReversals);
				if (vValidReversals.size() != 0)
				{
					int nTempReversalIndex = 0;
					while (1)
					{
						nTempReversalIndex = rand() % vValidReversals.size();
						if (vValidReversals[nTempReversalIndex] != 0)
							break;
					}
					int nWriteBuffer = vValidReversals[nTempReversalIndex];
					nWriteBuffer = 1;
					patchMemcpy(dwBaseAddress + dwP2PatternSet, &nWriteBuffer, 4);
					nTempReversalDelayFrames = nReversalDelayFrames;
				}
			}
		}
	}
}
#endif

void frameStartCallback() {

	// this is called right after directx present displays a new frame

	/*
	setAllKeys();

	bool ok = true;
	MSG msg;
	while (bFreeze || *(uint8_t*)(dwBaseAddress + adSharedFreezeOverride) == 1)
	{
		Sleep(1);

		while (ok = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!ok) {
				PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				return;
			}

			switch (msg.message) {
			case WM_QUIT:
			case WM_DESTROY:
				PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				return;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (*(uint8_t*)(dwBaseAddress + adSharedFreezeOverride) != 1)
		{
			if (oFrameBarLeftScrollKey.keyHeld())
				oFrameBarLeftScrollKey.nHeldKeyCounter++;
			else
				oFrameBarLeftScrollKey.nHeldKeyCounter = 0;
			if (oFrameBarLeftScrollKey.keyDown() || oFrameBarLeftScrollKey.nHeldKeyCounter >= 150)
			{
				nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
				nBarScrolling++;
				WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
			}

			if (oFrameBarRightScrollKey.keyHeld())
				oFrameBarRightScrollKey.nHeldKeyCounter++;
			else
				oFrameBarRightScrollKey.nHeldKeyCounter = 0;
			if (oFrameBarRightScrollKey.keyDown() || oFrameBarRightScrollKey.nHeldKeyCounter >= 150)
			{
				nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
				nBarScrolling--;
				WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
			}

			if (oHitboxesDisplayKey.keyDown())
			{
				bHitboxesDisplay = !bHitboxesDisplay;
			}

			if (oFrameDataDisplayKey.keyDown())
			{
				bFrameDataDisplay = !bFrameDataDisplay;
			}

			if (oHighlightsOnKey.keyDown())
				bHighlightsOn = !bHighlightsOn;

			if (oSaveStateKey.keyDown() && safeWrite())
			{
				*(char*)(dwBaseAddress + adSharedDoSave) = 1;
			}

			if (oFreezeKey.keyDown())
			{
				bFreeze = !bFreeze;
			}

			if (oPrevSaveSlotKey.keyDown())
			{
				uint8_t nTempSaveSlot;
				ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
				nTempSaveSlot = max(0, nTempSaveSlot - 1);
				WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
			}
			if (oNextSaveSlotKey.keyDown())
			{
				uint8_t nTempSaveSlot;
				ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
				nTempSaveSlot = min(nTempSaveSlot + 1, MAX_SAVES);
				WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
			}

			if (oFrameStepKey.keyHeld())
				oFrameStepKey.nHeldKeyCounter++;
			else
				oFrameStepKey.nHeldKeyCounter = 0;
			if (oFrameStepKey.keyDown() || oFrameStepKey.nHeldKeyCounter >= 150)
			{
				break;
			}
		}
	}

	*/

}

void frameDoneCallback()
{
	profileFunction();

	//log("%4d %4d", __frameDoneCount, *reinterpret_cast<int*>(dwBaseAddress + adFrameCount));

	setAllKeys();

	// this hooks directx
	static bool deviceInit = false;
	if (!deviceInit && *(DWORD*)0x0076e7d4 != 0) {
		deviceInit = true;

		dwDevice = *(DWORD*)0x0076e7d4;

		device = (IDirect3DDevice9*)dwDevice;

		unsigned avalTexMem = device->GetAvailableTextureMem();

		log("directx device has been acquired! texmem: %08X", avalTexMem);

		//HookDirectX();
	}

	if (oFrameBarLeftScrollKey.keyHeld())
		oFrameBarLeftScrollKey.nHeldKeyCounter++;
	else
		oFrameBarLeftScrollKey.nHeldKeyCounter = 0;
	if (oFrameBarLeftScrollKey.keyDown() || oFrameBarLeftScrollKey.nHeldKeyCounter >= 20)
	{
		nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
		nBarScrolling++;
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
	}

	if (oFrameBarRightScrollKey.keyHeld())
		oFrameBarRightScrollKey.nHeldKeyCounter++;
	else
		oFrameBarRightScrollKey.nHeldKeyCounter = 0;
	if (oFrameBarRightScrollKey.keyDown() || oFrameBarRightScrollKey.nHeldKeyCounter >= 20)
	{
		nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
		nBarScrolling--;
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
	}

	if (oHitboxesDisplayKey.keyDown())
	{
		bHitboxesDisplay = !bHitboxesDisplay;
		nDrawTextTimer = TEXT_TIMER;
		if (bHitboxesDisplay)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES OFF");
	}

	if (oFrameDataDisplayKey.keyDown())
	{
		bFrameDataDisplay = !bFrameDataDisplay;
		nDrawTextTimer = TEXT_TIMER;
		if (bFrameDataDisplay)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA OFF");
	}

	if (oHighlightsOnKey.keyDown())
	{
		bHighlightsOn = !bHighlightsOn;
		nDrawTextTimer = TEXT_TIMER;
		if (bHighlightsOn)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS OFF");
	}

	if (oFreezeKey.keyDown())
	{
		bFreeze = !bFreeze;
	}

	if (oSaveStateKey.keyDown() && safeWrite())
	{
		*(char*)(dwBaseAddress + adSharedDoSave) = 1;
		nDrawTextTimer = TEXT_TIMER;
		if (*(char*)(dwBaseAddress + adSharedSaveSlot) == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SAVED SLOT", *(char*)(dwBaseAddress + adSharedSaveSlot));
	}

	if (oSaveStateKey.keyHeld() && safeWrite() && *(char*)(dwBaseAddress + adSharedSaveSlot) != 0)
	{
		nClearSaveTimer++;
		if (nClearSaveTimer == SAVE_RESET_TIME)
		{
			*(char*)(dwBaseAddress + adSharedDoClearSave) = 1;
			nDrawTextTimer = TEXT_TIMER;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "CLEARED SAVE", *(char*)(dwBaseAddress + adSharedSaveSlot));
		}
	}
	else
	{
		nClearSaveTimer = 0;
	}

	if (oPrevSaveSlotKey.keyDown())
	{
		uint8_t nTempSaveSlot;
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
		nTempSaveSlot = max(0, nTempSaveSlot - 1);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
		nDrawTextTimer = TEXT_TIMER;
		if (*(char*)(dwBaseAddress + adSharedSaveSlot) == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SELECTED SAVE", *(char*)(dwBaseAddress + adSharedSaveSlot));
	}
	if (oNextSaveSlotKey.keyDown())
	{
		uint8_t nTempSaveSlot;
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
		nTempSaveSlot = min(nTempSaveSlot + 1, MAX_SAVES);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSaveSlot), &nTempSaveSlot, 1, 0);
		nDrawTextTimer = TEXT_TIMER;
		if (*(char*)(dwBaseAddress + adSharedSaveSlot) == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SELECTED SAVE", *(char*)(dwBaseAddress + adSharedSaveSlot));
	}

	if (oIncRNG.keyHeld())
		oIncRNG.nHeldKeyCounter++;
	else
		oIncRNG.nHeldKeyCounter = 0;
	if (nRNGMode != 0 && oIncRNG.keyDown() || oIncRNG.nHeldKeyCounter >= 20)
	{
		char pcTemp[19];
		nDrawTextTimer = TEXT_TIMER;
		if (nRNGMode == RNG_SEED)
		{
			nCustomSeed = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomSeed);
			nCustomSeed++;
			*(uint32_t*)(dwBaseAddress + adSharedRNGCustomSeed) = nCustomSeed;
			std::string sSeedString = std::format("{:x}", nCustomSeed);
			std::transform(sSeedString.begin(), sSeedString.end(), sSeedString.begin(), ::toupper);
			strcpy_s(pcTemp, std::string("0x" + sSeedString).c_str());
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "SEED: %s", pcTemp);
		}
		else if (nRNGMode == RNG_RN)
		{
			nCustomRN = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomRN);
			nCustomRN++;
			*(uint32_t*)(dwBaseAddress + adSharedRNGCustomRN) = nCustomRN;
			std::string sRNString = std::format("{:x}", nCustomRN);
			std::transform(sRNString.begin(), sRNString.end(), sRNString.begin(), ::toupper);
			strcpy_s(pcTemp, std::string("0x" + sRNString).c_str());
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RN: %s", pcTemp);
		}
	}

	if (oDecRNG.keyHeld())
		oDecRNG.nHeldKeyCounter++;
	else
		oDecRNG.nHeldKeyCounter = 0;
	if (nRNGMode != 0 && oDecRNG.keyDown() || oDecRNG.nHeldKeyCounter >= 20)
	{
		char pcTemp[19];
		nDrawTextTimer = TEXT_TIMER;
		if (nRNGMode == RNG_SEED)
		{
			nCustomSeed = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomSeed);
			nCustomSeed = max(0, nCustomSeed - 1);
			*(uint32_t*)(dwBaseAddress + adSharedRNGCustomSeed) = nCustomSeed;
			std::string sSeedString = std::format("{:x}", nCustomSeed);
			std::transform(sSeedString.begin(), sSeedString.end(), sSeedString.begin(), ::toupper);
			strcpy_s(pcTemp, std::string("0x" + sSeedString).c_str());
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "SEED: %s", pcTemp);
		}
		else if (nRNGMode == RNG_RN)
		{
			nCustomRN = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomRN);
			nCustomRN = max(0, nCustomRN - 1);
			*(uint32_t*)(dwBaseAddress + adSharedRNGCustomRN) = nCustomRN;
			std::string sRNString = std::format("{:x}", nCustomRN);
			std::transform(sRNString.begin(), sRNString.end(), sRNString.begin(), ::toupper);
			strcpy_s(pcTemp, std::string("0x" + sRNString).c_str());
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RN: %s", pcTemp);
		}
	}

	char pcMessageBuffer[32];
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcMessageBuffer, 32, 0);
	if (strcmp(pcMessageBuffer, "") != 0)
	{
		char pcEmpty[32] = "";
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedMessageBuffer), &pcEmpty, 32, 0);

		nDrawTextTimer = TEXT_TIMER;
		snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", pcMessageBuffer);
	}

	if (nDrawTextTimer != 0 && safeWrite())
	{
		static char buffer[256];

		drawTextWithBorder(243, 460, 15, 15, pcTextToDisplay);

		/*if (*(char*)(dwBaseAddress + adSharedSaveSlot) == 0)
		{
			strcpy_s(pcTextToDisplay, "SAVE STATES DISABLED");
			snprintf(buffer, 256, "%s", pcTextToDisplay);
			drawTextWithBorder(243, 470, 10, 10, buffer);
		}
		else
		{
			snprintf(buffer, 256, pcTextPattern, pcTextToDisplay, *(char*)(dwBaseAddress + adSharedSaveSlot));
			drawTextWithBorder(260, 470, 10, 10, buffer);
		}*/
		nDrawTextTimer--;
	}
	
	// heres a lil example for the new draw funcs
	// i can change the syntax up if desired
	// also, i dont have any new text funcs yet, sry
	// top left of screen is (0.0, 0.0), bottom right is (1.333333, 1.0)

	/*
	
	//         x1   x2   y1   y2    A R G B
	LineDraw( 0.1, 0.1, 0.9, 0.9, 0x800000FF );

	//        x    y    w    h		A R G B
	RectDraw(0.1, 0.1, 0.2, 0.2, 0x80FF0000 );

	for (float i = 0.0; i < 1.0; i += 0.1) {
		RectDraw( i, 0.9f - i, 0.1, 0.1, 0x8000FF00 );
		RectDraw( i+0.05f, 0.9f - i - 0.05f, 0.1, 0.1, 0x8000FF00 );
	}

	RectDraw( 1.23333, 0.5, 0.1, 0.1, 0xFFFF0000 );
	RectDraw( 1.23333, 0.6, 0.1, 0.1, 0xFFFFFFFF );
	RectDraw( 1.23333, 0.7, 0.1, 0.1, 0xFFFF0000 );

	BorderDraw( 1.23333, 0.6, 0.1, 0.1, 0x800000FF );

	DWORD _tempCol = 0x8000FFFF;
	for (float i = 0.0; i < 1.0; i += 0.1) {
		BorderRectDraw( 0.7, i, 0.1, 0.1, _tempCol );
		_tempCol ^= 0x00FF00000;
	}

	LineDraw(0.0, 1.0, 1.3333, 0.0, 0xFFFF00FF);
	
	
	//LineDraw(0.0, 1.0, 1.3333, 0.0, 0xFFFF00FF);

	TextDraw(0.1, 0.1, 0.025, 0xFF00FFFF, "test %d %s %d", 123, "abc", 456);
	
	*/

	// don't draw on the pause menu, but do on VIEW SCREEN
	DWORD nSubMenuPointer = *reinterpret_cast<DWORD*>(dwBaseAddress + dwBasePointer) + 0x84;
	//int nSubMenu;
	//ReadProcessMemory(GetCurrentProcess(), (LPVOID)(nSubMenuPointer), &nSubMenu, 4, 0);
	if ((safeWrite() && !isPaused()) || (isPaused() && *(uint8_t*)(nSubMenuPointer) == 12)) {
		if (*(bool*)(dwBaseAddress + adSharedHoveringScroll))
			drawFrameBar();
		
		if (bHitboxesDisplay)
			drawFrameData();
		
		if (bFrameDataDisplay)
			drawFrameBar();

		drawStats();
	}

	int nFrameTimer = *(int*)(dwBaseAddress + dwFrameTimer);
	nRNGMode = *(uint8_t*)(dwBaseAddress + adSharedRNGMode);
	nRNGRate = *(uint8_t*)(dwBaseAddress + adSharedRNGRate);
	nCustomSeed = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomSeed);
	nCustomRN = *(uint32_t*)(dwBaseAddress + adSharedRNGCustomRN);
	if (nRNGRate == RNG_EVERY_FRAME ||
		(nRNGRate == RNG_EVERY_RESET && nFrameTimer == 1))
	{
		if (nRNGMode == RNG_SEED)
			SetSeed(nCustomSeed);
		if (nRNGMode == RNG_RN)
			SetRN(nCustomRN);
	}

	if (*(bool*)(dwBaseAddress + adSharedOnExtendedSettings))
	{
		char pcMainInfoText[64];
		char pcSubInfoText[64];
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedMainInfoText), &pcMainInfoText, 64, 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedSubInfoText), &pcSubInfoText, 64, 0);
		RectDraw(-0.5f, 382.5f, 640.0f, 70.0f, 0xFF101010);
		TextDraw(47.5f, 393.5f, 21.0f, 0xFFFFFFFF, pcMainInfoText);
		TextDraw(66.5f, 420.5f, 21.0f, 0xFFFFFFFF, pcSubInfoText);
	}
}

__declspec(naked) void nakedFrameDoneCallback()
{
	__asm {
		add __frameDoneCount, 1;
		add esp, 4Ch;
		call frameDoneCallback;
		ret;
	};
}

DWORD newPauseCallback_IsFramestepMenu = false;

void hidePauseMenu() {
	if (!newPauseCallback_IsFramestepMenu) {
		return;
	}

	DWORD menuPtr = *(DWORD*)0x0074d7fc;

	if (menuPtr == 0) {
		log("menuPtr was null while trying to hide it!");
		return;
	}

	*(BYTE*)(menuPtr + 0x7C + 0x00) = 0x00;
	*(BYTE*)(menuPtr + 0x7C + 0x08) = 0x0C;
	*(WORD*)(menuPtr + 0x7C + 0x12) = 0x0000;
	*(BYTE*)(menuPtr + 0x7C + 0x5C) = 0x64;

	// [[0x0074d7fc]+DC] <- 3
	// this was here for,, hiding the information thingy. it however, hides input and attack display. thats not right
	if (*(DWORD*)(menuPtr + 0xDC) != 0) {
		//*(DWORD*)(*(DWORD*)(menuPtr + 0xDC)) = 3;  
	}


}

void newPauseCallback() {

	//static KeyState F10Key(VK_F10);
	
	/*
	
	the section that writes when we enter viewscreen:
	0047ef0d

	the section that writes when we exit viewscreen: 
	0047e68e

	ebp+7C gives,, something

	+0 is 1 when vis, 0 when invis
	+8 is 2 when vis, C when invis

	+12 is 0x3F80 when vis, 0 when invis. fade out timer.
	most likely 4 bytes, but only mess with those two

	1F05E918 - 1F05E8BC, 5C
	+5C is 0 when vis, 0x64 when invis, fade in timer.

	lastly, how do i get ebp.

	[0x0074d7fc]

	also, [[0x0074d7fc]+DC]
	has something for hiding the info menu


	
	*/

	constexpr auto unpause = []() -> void {
		DWORD menuPtr = *(DWORD*)0x0074d7fc;

		*(DWORD*)(0x0055d203) = 0;
		*(DWORD*)(0x0055d256) = 2;
		*(DWORD*)(0x00562a64) = 0;

		newPauseCallback_IsFramestepMenu = false;
	};

	constexpr auto pause = []() -> void {

		if (*(DWORD*)(0x0055d203) == 1) {
			// this means are paused in the pause menu, dont freeze;
			return;
		}

		*(DWORD*)(0x0055d203) = 1;
		*(DWORD*)(0x0055d256) = 1;
		*(DWORD*)(0x00562a64) = 1;

		// there is a 1f delay when,, setting these vars (in the place im setting them) and when the menu is actually created
		newPauseCallback_IsFramestepMenu = true;
	};

	
	if (oFreezeKey.keyDown()) {
		if (newPauseCallback_IsFramestepMenu) { // unpause
			unpause();
		} else { // pause
			pause();
		}
	}

	static bool needPause = false;

	if (oFrameStepKey.keyDown() && newPauseCallback_IsFramestepMenu) {
		unpause();
		needPause = true;
	} else if(needPause) {
		needPause = false;
		pause();
	}

}

DWORD _naked_newPauseCallback_Func_Addr = 0x0044c7b0;
__declspec(naked) void _naked_newPauseCallback() {

	__asm {
		call[_naked_newPauseCallback_Func_Addr];
	};

	PUSH_ALL;
	newPauseCallback();
	POP_ALL;

	__asm {
		push 0044c501h;
		ret;
	}

}

__declspec(naked) void _naked_createPauseMenuCallback() {
	
	PUSH_ALL;
	hidePauseMenu();
	POP_ALL;
	
	__asm {
		pop ecx;
		pop esi;
		pop ebx;
		add esp, 010h;
		ret;
	};
}

DWORD _naked_pauseMenuProcessInput_Func_Addr = 0x0041f5a0;
__declspec(naked) void _naked_pauseMenuProcessInput() {
	__asm {

		cmp newPauseCallback_IsFramestepMenu, 1;
		JE _SKIP;

		call[_naked_pauseMenuProcessInput_Func_Addr];

		push 00477fcah;
		ret;

	_SKIP:
		push 00478029h;
		ret;

	};
}

DWORD _naked_pauseInputDisplay_FUN_004790a0 = 0x004790a0;
DWORD _naked_pauseInputDisplay_FUN_004796a0 = 0x004796a0;
__declspec(naked) void _naked_pauseInputDisplay() {

	__asm {

		push eax;

		// i hate masm so much
		mov eax, 0x0055d203;
		mov eax, [eax];

		// i need to check if the menu is open in general, freeze menu, or normal
		cmp eax, 0;
		JE _SKIP;

		pop eax;

		// menu is open, dont do input bs
		push 004794dch;
		ret;

	_SKIP:
		pop eax;
	}



	// not risking having to argue with MASM.
	// bytes taken from 004794c4

	__asm {
		push esi;
		call[_naked_pauseInputDisplay_FUN_004790a0];
	}
	;
	// MOVZX EBX,byte ptr [ESI + 0x2e7]
	__asm _emit 0x0F;
	__asm _emit 0xB6;
	__asm _emit 0x9E;
	__asm _emit 0xE7;
	__asm _emit 0x02;
	__asm _emit 0x00;
	__asm _emit 0x00;

	__asm {
		push eax;
		mov eax, edi;
		call[_naked_pauseInputDisplay_FUN_004796a0];
		add esp, 08h;
	};


	__asm {
		push 004794dch;
		ret;
	};
}

DWORD _naked_newPauseCallback2_IsPaused = 0;
void newPauseCallback2() {


	if (oFreezeKey.keyDown()) {
		_naked_newPauseCallback2_IsPaused = !_naked_newPauseCallback2_IsPaused;
	}

	static bool needPause = false;

	if (oFrameStepKey.keyDown() && _naked_newPauseCallback2_IsPaused) {
		needPause = true;
		_naked_newPauseCallback2_IsPaused = false;
	} else if (needPause) {
		needPause = false;
		_naked_newPauseCallback2_IsPaused = true;
	}
	
}

//DWORD _naked_newPauseCallback2_Func_Addr = 0x00432c50;
DWORD _naked_newPauseCallback2_Func_Addr = 0x00423630;
__declspec(naked) void _naked_newPauseCallback2() {

	PUSH_ALL;
	newPauseCallback2();
	POP_ALL;

	__asm {
		cmp _naked_newPauseCallback2_IsPaused, 1;
		JE _SKIP;

		call[_naked_newPauseCallback2_Func_Addr];

	_SKIP:

		//push 0040e476h;
		push 004235d6h;
		ret;
	};
}

int nTempP1MeterGain = 0;
int nTempP2MeterGain = 0;
int nP1MeterGain = 0;
int nP2MeterGain = 0;
DWORD prevComboPtr = 0;
void attackMeterDisplayCallback()
{

	int iVar2 = (*(BYTE*)0x0055df0f) * 0x20c;
	int iVar1 = *(int*)(0x00557e20 + iVar2);

	DWORD comboPtr = (0x00557e28 + iVar1 * 0x18 + iVar2);

	if (prevComboPtr != comboPtr) {
		nP1MeterGain = 0;
		nP2MeterGain = 0;
		prevComboPtr = comboPtr;
	}

	nP1MeterGain += nTempP1MeterGain;
	nP2MeterGain += nTempP2MeterGain;

	nTempP1MeterGain = 0;
	nTempP2MeterGain = 0;

	static char buffer[256];

	drawTextWithBorder(420, 186, 10, 14, "P1 METER GAIN");
	snprintf(buffer, 256, "%4d.%02d%%", nP1MeterGain / 100, nP1MeterGain % 100);
	drawTextWithBorder(420 + 130, 186, 10, 14, buffer);

	drawTextWithBorder(420, 186 + 14, 10, 14, "P2 METER GAIN");
	snprintf(buffer, 256, "%4d.%02d%%", nP2MeterGain / 100, nP2MeterGain % 100);
	drawTextWithBorder(420 + 130, 186 + 14, 10, 14, buffer);
}

// hook funcs

BYTE arrAnimHookBytesOrig[10];
BYTE arrAnimHookBytesMod[10];
void animHookFunc() 
{
	// does this func get called for both chars individually? 
	void* funcAddress = (void*)0x0045f650;
	// restore func to original state
	patchMemcpy(funcAddress, arrAnimHookBytesOrig, 10);
	/*

	x86 asm does not allow for direct calls to an intermediate addr, only relative
	UNLESS, a register is used as a jump point

	*/
	// execute original func
	__asm {
		mov eax, funcAddress;
		call eax;
	};
	// patch hook back into func
	patchMemcpy(funcAddress, arrAnimHookBytesMod, 10);
	// perform coloring code
	highlightStates();
}


DWORD _meterGainHook_CharacterAddr;
DWORD _meterGainHook_MeterAmount;
DWORD _meterGainHook_ObjectOwner;
void meterGainHook() {

	unsigned playerIndex = -1;
	bool hasExGuardPenalty = false;
	BYTE moon = 0;

	// i am assuming that if either of the chars on a teams' EX guard penalties are set, that it will be 0.1
	// this is an assumption, might be wrong

	switch (_meterGainHook_CharacterAddr) {
	case 0x00555134: // P1 offset
	case 0x0055672C: // P3 offset
		playerIndex = 0;
		break;
	case 0x00555C30: // P2 offset
	case 0x00557228: // P4 offset
		playerIndex = 1;
		break;
	default: // projectile is what hit
		playerIndex = _meterGainHook_ObjectOwner & 0x1;
		break;
	}

	// a class to read these vars would be very nice tbh
	if (playerIndex == 0) {
		hasExGuardPenalty = !!((*(BYTE*)(0x55520c)) | (*(BYTE*)(0x556804)));
		moon = *(BYTE*)0x55513C;
	}
	else {
		hasExGuardPenalty = !!((*(BYTE*)(0x555d08)) | (*(BYTE*)(0x557300)));
		moon = *(BYTE*)0x555C38;
	}

	switch (moon) {
		// unsure if the rounding here is ok 
	default:
	case 0: // CMOON, do nothing
		break;
	case 1: // FMOON, 0.9
		_meterGainHook_MeterAmount = (DWORD)(0.9f * ((float)_meterGainHook_MeterAmount));
		break;
	case 2: // HMOON, 0.7
		_meterGainHook_MeterAmount = (DWORD)(0.7f * ((float)_meterGainHook_MeterAmount));
		break;
	}

	if (hasExGuardPenalty) {
		_meterGainHook_MeterAmount = (DWORD)(0.1f * ((float)_meterGainHook_MeterAmount));
	}
	if (playerIndex == 0) {
		nTempP1MeterGain = _meterGainHook_MeterAmount;
	}
	else {
		nTempP2MeterGain = _meterGainHook_MeterAmount;
	}
}

DWORD _naked_meterGainHook_reg;
__declspec(naked) void _naked_meterGainHook()
{
	// our code
	__asm {
		mov _naked_meterGainHook_reg, eax;

		mov eax, [esp + 8h];
		mov _meterGainHook_MeterAmount, eax;

		mov eax, [esp + 4h];
		mov _meterGainHook_CharacterAddr, eax;

		mov eax, [eax + 02F0h];
		mov _meterGainHook_ObjectOwner, eax;

		mov eax, _naked_meterGainHook_reg;
	};

	PUSH_ALL;
	meterGainHook();
	POP_ALL;

	// actual code for the function that was overwritten by our jump
	// taken from 00476ce0
	// i do not trust MASM to compile things with addresses correctly, which is why i am emiting the bytes raw here

	// sub esp 0x8
	__asm _emit 0x83;
	__asm _emit 0xEC;
	__asm _emit 0x08;

	// cmp byte ptr [0x00562a6f], 0x0 
	__asm _emit 0x80;
	__asm _emit 0x3D;
	__asm _emit 0x6F;
	__asm _emit 0x2A;
	__asm _emit 0x56;
	__asm _emit 0x00;
	__asm _emit 0x00;

	// jump to old code.
	// i could use a global variable here, but that is annoying, which is why i am instead doing this
	__asm {
		push 00476ceah;
		ret;
	}
}

void battleResetCallback()
{
	nTempP1MeterGain = 0;
	nTempP2MeterGain = 0;
	nP1MeterGain = 0;
	nP2MeterGain = 0;
	prevComboPtr = 0;
}

// init funcs

void initFrameDoneCallback()
{

	/*
	// this might be getting called a frame late. unsure 
	// this callback, is very bad. caster uses it.
	void* funcAddress = (void*)0x0041d815;
	patchByte(((BYTE*)funcAddress) + 0, 0x50); // push eax?
	patchFunction(((BYTE*)funcAddress) + 1, frameDoneCallback); // call
	patchByte(((BYTE*)funcAddress) + 6, 0x58); // pop eax
	patchByte(((BYTE*)funcAddress) + 7, 0xC3); // ret
	*/


	// caster hooks the start of this func. ill have to hook the end of it 
	void* patchAddr = (void*)0x00432e2b;
	patchJump(patchAddr, nakedFrameDoneCallback);

}

void initAnimHook() 
{
	void* funcAddress = (void*)0x0045f650;
	// backup
	patchMemcpy(arrAnimHookBytesOrig, funcAddress, 10);
	// new bytes
	patchFunction(funcAddress, animHookFunc);
	// ret
	patchByte(((BYTE*)funcAddress) + 5, 0xC3);
	// backup modded bytes 
	patchMemcpy(arrAnimHookBytesMod, funcAddress, 10);
}

void initCasterMods()
{

	// if caster ever updates, these offsets will most likely(basically definitely) need to be changed!

	// 0x665f17d5 is total meter gain 
	patchByte(dwCasterBaseAddress + 0x665f17d5, '\0');

	// 0x665f17c1 is meter gain 
	patchByte(dwCasterBaseAddress + 0x665f17c1, '\0');

	// 0x665f17cd is the actual string passed to printf
	patchByte(dwCasterBaseAddress + 0x665f17cd, '\0');

	// it would be better to just patch this func out
}

void initAttackMeterDisplay()
{

	// this func rets early, this jump prevents that 
	void* patchAddr = (void*)0x00478fe2;
	patchJump(patchAddr, 0x00478ffd);
	
	void* funcAddr = (void*)0x00479005;
	patchFunction(funcAddr, attackMeterDisplayCallback);
	patchByte(((BYTE*)funcAddr) + 5, 0xC3);

	// remove max combo and max damage lines
	patchJump(0x00478ee0, 0x00478fcf);

	// remove vs damage line
	patchByte(0x00537f58, '\0');
	BYTE tempCode[5] = {0x90, 0x90, 0x90, 0x90, 0x90};
	patchMemcpy(0x00478dba, tempCode, 5);

	// adjust line y positions

	constexpr DWORD pushAddresses[] = {
		0x00478c61, 0x00478cbe, // combo
		0x00478ce6, 0x00478d37, // damage
		0x00478dd7, 0x00478e39, // correction val
		0x00478e61, 0x00478ec9, // reverse penalty
	};

	BYTE yVal = 118;

	// 14 dist for close, 18 for far
	for (int i = 0; i < sizeof(pushAddresses) / sizeof(pushAddresses[0]); i += 2) {
		if (i % 4 == 0) {
			yVal += 4; // extra 4 to go to 18
		}
		patchByte(pushAddresses[i] + 1, yVal);
		patchByte(pushAddresses[i + 1] + 1, yVal);
		yVal += 14;
	}
}

void initMeterGainHook()
{
	patchJump(0x00476ce0, _naked_meterGainHook);
}

void initBattleResetCallback()
{
	// this func rets early, this jump prevents that 
	void* patchAddr = (void*)0x004234b9;
	patchJump(patchAddr, 0x004234e1);

	void* funcAddr = (void*)0x004234e4;
	patchFunction(funcAddr, battleResetCallback);

	// this patch is a bit funky, since we need to have ret dec the stack pointer
	BYTE tempCode[3] = {0xc2, 0x04, 0x00};
	patchMemcpy(((BYTE*)funcAddr) + 5, tempCode, 3);
}

void initPauseCallback()
{

	/*
	this func seems to, be responsible for slowing the game down to 60fps, i think?
	*/
	void* addr = (void*)0x0041fe05;

	patchFunction(addr, pauseCallback);

	// the call being patched is 6 bytes long, patch the extra byte with a nop
	patchByte((BYTE*)addr + 5, 0x90);

}

void initNewPauseCallback() {
	
	/*
	patchJump(0x0044c4fc, _naked_newPauseCallback);
	patchJump(0x004781a8, _naked_createPauseMenuCallback);
	patchJump(0x00477fc5, _naked_pauseMenuProcessInput);

	// when paused, input display and attack display are not displayed, because thats how shit works. this fixes that.
	// 00477f00. has a compare for if the menu is open 
	// removing it would be fine, but im not sure what the code at 00477f45 does.
	// however, if its something that should be drawn when the menu is closed, then it should be fine

	patchMemset(0x00477f00, 0x90, 9); // patch the cmp and jump with nops 7 for cmp, 2 for jmp

	// input display is updated even while paused. 
	patchJump(0x004794c4, _naked_pauseInputDisplay);
	*/

	//patchJump(0x0040e471, _naked_newPauseCallback2);

	patchJump(0x004235d1, _naked_newPauseCallback2);
	
}

void threadFunc() 
{
	srand(time(NULL));

	// make sure that caster has time to hook at the start
	Sleep(32);

	// todo, put something here to prevent mult injection

	// check if we are running with caster
	HMODULE hModule = GetModuleHandleA("hook.dll");

	if (hModule != NULL) { // we are running with caster
		bCasterInit = true;
		// 0x66380000 is the base address when looking through caster in decomp
		dwCasterBaseAddress = ((DWORD)hModule) - 0x66380000;
		initCasterMods();
	}

	initPauseCallback();
	initFrameDoneCallback();
	initAnimHook();
	InitializeCharacterMaps();
	// when running with caster, the prints to this area are disabled
	// when not running with caster, they arent even there, so this is fine to run regardless of caster 
	initAttackMeterDisplay();
	initMeterGainHook();
	initBattleResetCallback();

	initNewPauseCallback();

	
	while (true) 
	{
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedIdleHighlight), &arrIdleHighlightSetting, 4, 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedBlockingHighlight), &arrBlockingHighlightSetting, 4, 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedHitHighlight), &arrHitHighlightSetting, 4, 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedArmorHighlight), &arrArmorHighlightSetting, 4, 0);
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(dwBaseAddress + adSharedThrowProtectionHighlight), &arrThrowProtectionHighlightSetting, 4, 0);
		Sleep(8);
	}
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) 
{
	(void)hinstDLL;
	(void)lpReserved;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadFunc, 0, 0, 0);
	}

	return TRUE;
}

#pragma pop_macro("optimize")
