
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

#include "..\Common\Common.h"
#include "..\Common\CharacterData.h"

#pragma comment(lib, "ws2_32.lib") 

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

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

const ADDRESS dwBaseAddress = (0x00400000);

const ADDRESS INPUTDISPLAYTOGGLE = (dwBaseAddress + 0x001585f8);

// helpers

std::array<BYTE, 3> arrIdleHighlightSetting({ 255, 255, 255 });
std::array<BYTE, 3> arrBlockingHighlightSetting({ 255, 255, 255 });

//enum eHighlightSettings { NO_HIGHLIGHT, RED_HIGHLIGHT, GREEN_HIGHLIGHT, BLUE_HIGHLIGHT };

class KeyState {
public:
	// please tell me if my use of classes here is overkill

	KeyState(int vKey_) : vKey(vKey_) {}

	bool isFocused() {
		HWND hActiveWindow = GetActiveWindow();
		HWND hForegroundWindow = GetForegroundWindow();
		return hActiveWindow == hForegroundWindow;
	}

	bool keyHeld() {
		return (GetAsyncKeyState(vKey) & 0x8000) ? true : false;
	}

	bool keyDown() {
		bool tempState = (GetAsyncKeyState(vKey) & 0x8000) ? true : false;
		bool res = false;
		if (prevState != tempState && tempState)
		{
			res = true;
		}
		prevState = tempState;

		if (!isFocused()) {
			return false;
		}

		return res;
	}

private:
	int vKey;
	bool prevState = false;
};

void log(const char* msg) 
{
	const char* ipAddress = "127.0.0.1";
	unsigned short port = 17474;

	int msgLen = strlen(msg);

	char* message = new char[msgLen + 4];

	strncpy_s(message, msgLen + 4, msg, msgLen);
	message[msgLen + 0] = '\r';
	message[msgLen + 1] = '\n';
	message[msgLen + 2] = '\0';

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

	delete[] message;
}

void debugLogBytes(BYTE* p)
{

	static char buffer[256];

	for (int i = 0; i < 16; i++) 
	{
		snprintf(buffer, 256, "%08X", *(BYTE*)p);
		p++;
	}
}

bool safeWrite() {
	BYTE PauseFlag = *reinterpret_cast<BYTE*>(dwBaseAddress + dwPausedFlag);
	BYTE GameState = *reinterpret_cast<BYTE*>(dwBaseAddress + dwGameState);
	int FrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);

	if (PauseFlag == 1 ||
		GameState != 1 ||
		FrameTimer == 0)
		return false;
	
	return true;
}

// patch funcs

void patchMemcpy(auto dst, auto src, size_t n) 
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

void patchFunction(auto patchAddr_, auto newAddr_)
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

void patchByte(auto addr, const BYTE byte)
{
	static_assert(sizeof(addr) == 4, "Type must be 4 bytes");

	BYTE temp[] = { byte };

	patchMemcpy(addr, temp, 1);
}

// actual functions 

extern "C" int asmDrawText(int w, int h, int x, int y, const char* text, int alpha, int shade, int layer, void* addr, int spacing, int idek, char* out);

void drawText(int x, int y, int w, int h, const char* text, int alpha, int shade, int layer = 0x2cc, void* font = (void*)adFont2) {
	// text was initially just char*, i made it const, is that going to be ok?
	asmDrawText(w, h, x, y, text, alpha, shade, layer, font, 0, 0, 0);
}

void drawText(int x, int y, const char* text, int textSize = 16, auto font = adFont2) {

	static_assert(sizeof(font) == 4, "Type must be 4 bytes");

	int len = strnlen_s(text, 1024);

	float tempWidth = ((float)textSize / 5.0) * (float)len;

	drawText(x, y, (int)tempWidth, textSize, text, 0xFF, 0xFF, 0x02CC, (void*)font);
}

extern "C" int asmDrawRect(int screenXAddr, int screenYAddr, int width, int height, int A, int B, int C, int D, int layer);

void drawRect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a, int layer = 0x2cc) {
	int colVal = (a << 24) | (r << 16) | (g << 8) | (b << 0);
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void drawRect(int x, int y, int w, int h, DWORD colVal, int layer = 0x2cc) {
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void drawBorder(int x, int y, int w, int h, DWORD ARGB=0x8042e5f4) {
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

void scaleCords(const float xOrig, const float yOrig, float& x1Cord, float& y1Cord, float& x2Cord, float& y2Cord) {
	x1Cord = xOrig + (x1Cord - xOrig) * 0.5;
	y1Cord = yOrig + (y1Cord - yOrig) * 0.5;
	x2Cord = xOrig + (x2Cord - xOrig) * 0.5;
	y2Cord = yOrig + (y2Cord - yOrig) * 0.5;
}

DWORD getObjFrameDataPointer(DWORD objAddr) {
	
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

void drawObject(DWORD objAddr, bool isProjectile) {

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
			
			drawBorder((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
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

			drawBorder((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
		}
	}
}

//In-game frame bar proof-of-concept
DWORD frameBarP1[50];
DWORD frameBarP2[50];

void updateFrameBar(DWORD frameBar[50], DWORD playerStruct, DWORD inactionOffset)
{
	int currentFrame = *(int*)(dwBaseAddress + 0x15D1CC) % 50;
	if (*(int*)(dwBaseAddress + 0x15D1CC) == 1) //Sense reset
	{
		for (int i = 0; i < 50; i++)
		{
			frameBar[i] = 0xFF101010;
		}
	}

	if (*(char*)(dwBaseAddress + playerStruct + 0x172) != 0)
	{
		frameBar[currentFrame] = 0xFF008080;
	}
	else if (*(int*)(dwBaseAddress + playerStruct + 0x324) != 0)
	{
		frameBar[currentFrame] = 0xFFFF0000;
	}
	else if (*(int*)(dwBaseAddress + inactionOffset) != 0)
	{
		frameBar[currentFrame] = 0xFF00FF00;
	}
	else
	{
		frameBar[currentFrame] = 0xFF101010;
	}
	
}

void drawFrameBar() {
	updateFrameBar(frameBarP1, dwP1Struct, 0x157FC0);
	updateFrameBar(frameBarP2, dwP2Struct, 0x1581CC);

	for (unsigned i = 0; i < 50; i++) //P1 Bar
	{
		drawRect(20 + 12 * i, 400, 10, 10, frameBarP1[i]);
	}
	for (unsigned i = 0; i < 50; i++) //P2 Bar
	{
		drawRect(20 + 12 * i, 412, 10, 10, frameBarP2[i]);
	}
	drawRect(18, 398, 602, 26, 0x80FFFFFF); //Background
}

void drawFrameData() {

	if (!safeWrite()) {
		return;
	}

	drawFrameBar();

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

void __stdcall pauseCallback(DWORD dwMilliseconds)
{
	// windows Sleep, the func being overitten is an stdcall, which is why we have __stdcall
	static KeyState pKey('P');
	static KeyState nKey('N');
	static bool bIsPaused = false;

	if (pKey.keyDown())
	{
		bIsPaused = !bIsPaused;		
	}

	if (!bIsPaused && nKey.keyDown())
	{
		
		bIsPaused = true;
	}

	MSG msg;

	while (bIsPaused) {
		Sleep(1);

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (pKey.keyDown())
		{
			bIsPaused = !bIsPaused;
		}

		if (nKey.keyDown()) {
			break;
		}
	} 
	
	Sleep(dwMilliseconds);
}

void initPauseCallback() {

	/*
	this func seems to, be responsible for slowing the game down to 60fps, i think?
	*/
	void* addr = (void*)0x0041fe05;

	patchFunction(addr, pauseCallback);

	// the call being patched is 6 bytes long, patch the extra byte with a nop
	patchByte((BYTE*)addr + 5, 0x90);

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
	drawText(300, 420 + (16 * 0), std::to_string(nHitstun).c_str(), 64, adFont0);

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

void frameDoneCallback()
{

	static KeyState hKey('H');
	static bool bShowHitboxes = false;

	if (hKey.keyDown()) {
		bShowHitboxes = !bShowHitboxes;
	}

	if (bShowHitboxes) {
		drawFrameData();

		drawText(300, 420 + (16 * 0), "wow", 16, adFont0);
		drawText(300, 420 + (16 * 1), "wow", 16, adFont1);
		drawText(300, 420 + (16 * 2), "wow", 16, adFont2);
	}

	enemyReversal();
}

void initRenderCallback() {

	// this might be getting called a frame late. unsure 

	void* funcAddress = (void*)0x0041d815;
	patchByte(((BYTE*)funcAddress) + 0, 0x50); // push eax?
	patchFunction(((BYTE*)funcAddress) + 1, frameDoneCallback); // call
	patchByte(((BYTE*)funcAddress) + 6, 0x58); // pop eax
	patchByte(((BYTE*)funcAddress) + 7, 0xC3); // ret
}

void animLog()
{
	const DWORD dwPauseFlag = 0x162A64; //1=paused
	const DWORD dwGameState = 0x14EEE8; //1=training
	const DWORD dwFrameTimer = 0x15D1CC;
	BYTE PauseFlag = *reinterpret_cast<BYTE*>(dwBaseAddress + dwPauseFlag);
	BYTE GameState = *reinterpret_cast<BYTE*>(dwBaseAddress + dwGameState);
	int FrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);

	if (
		PauseFlag == 1 || 
		GameState != 1 || 
		FrameTimer == 0)
		return;


	auto updateAnimation = [](DWORD animDataAddr, BYTE blockState, DWORD patternState) -> void
	{
		if (blockState == 1) 
		{
			patchMemcpy(animDataAddr + 0x18, arrBlockingHighlightSetting.data(), 3);
		}
		else 
		{
			switch (patternState) 
			{
			case 0:			// stand
			//case 10:		// walk forward
			//case 11:		// walk back
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
			//case 38:		// dj9
			//case 39:		// dj 8
			//case 40:		// dj7
			//case 360:		// sj8
				patchMemcpy(animDataAddr + 0x18, arrIdleHighlightSetting.data(), 3);
				break;
			default:
				break;
			}
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
		updateAnimation(*(DWORD*)(dwBaseAddress + dwP1AnimationPtr), *(BYTE*)(dwBaseAddress + dwP1Blocking), *(DWORD*)(dwBaseAddress + dwP1PatternRead));
	}

	if (*(DWORD*)(dwBaseAddress + dwP2AnimationPtr) > dwBaseAddress) 
	{
		updateAnimation(*(DWORD*)(dwBaseAddress + dwP2AnimationPtr), *(BYTE*)(dwBaseAddress + dwP2Blocking), *(DWORD*)(dwBaseAddress + dwP2PatternRead));
	}
}

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
	animLog();
	return;
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

void threadFunc() 
{
	//log("DLL injection successful");
	srand(time(NULL));

	// todo, put something here to prevent mult injection

	initPauseCallback();
	initRenderCallback();
	initAnimHook();
	InitializeCharacterMaps();

	while (true) 
	{

		// ideally, this would be done with signals. also unknown if this is thread safe
		GetSharedMemory(&arrIdleHighlightSetting, &arrBlockingHighlightSetting, NULL, NULL, NULL, NULL,
						&nReversalIndex1, &nReversalIndex2, &nReversalIndex3, &nReversalIndex4, 
						&nReversalDelayFrames,
						&nReversalType);
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
