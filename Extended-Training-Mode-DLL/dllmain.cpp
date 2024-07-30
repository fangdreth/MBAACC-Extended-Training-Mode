
#include "pch.h"
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

#pragma comment(lib, "ws2_32.lib") 

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))

static_assert(sizeof(int*) == 4, "COMPILE PROGRAM AS 32BIT");

typedef unsigned char u8;
typedef DWORD ADDRESS;

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

const ADDRESS dwBaseAddress = (0x00400000);

const ADDRESS INPUTDISPLAYTOGGLE = (dwBaseAddress + 0x001585f8);

const ADDRESS dwP1Struct = (dwBaseAddress + 0x00155130);
const ADDRESS dwP2Struct = (dwP1Struct + 0xAFC);

const ADDRESS dwP1AnimPtr = (dwP1Struct + 0x320);
const ADDRESS dwP2AnimPtr = (dwP2Struct + 0x320);

const ADDRESS  dwP1Blocking = (dwBaseAddress + 0x1552AB);
const ADDRESS  dwP2Blocking = (dwBaseAddress + 0x1552AB + 0xAFC);

const ADDRESS  dwP1PatternRead = (dwBaseAddress + 0x155C3C - 0xAFC);
const ADDRESS  dwP2PatternRead = (dwBaseAddress + 0x155C3C);

// helpers

//int nIdleHighlightSetting = 0;
//int nBlockingHighlightSetting = 0;

std::array<u8, 3> arrIdleHighlightSetting({ 255, 255, 255 });
std::array<u8, 3> arrBlockingHighlightSetting({ 255, 255, 255 });

enum eHighlightSettings { NO_HIGHLIGHT, RED_HIGHLIGHT, GREEN_HIGHLIGHT, BLUE_HIGHLIGHT };

void log(const char* msg) {

	// i heavily would have prefered keeping the socket open. it did not like that

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
	if (result != 0) {
		return;
	}

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress, &destAddr.sin_addr) <= 0) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	int sendResult = sendto(sendSocket, message, strlen(message), 0, (sockaddr*)&destAddr, sizeof(destAddr));
	if (sendResult == SOCKET_ERROR) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	closesocket(sendSocket);
	WSACleanup();

	delete message;
}

void GetSharedMemory()
{


	static bool bSharedMemoryInit = false;
	static HANDLE hMapFile = NULL;
	static LPVOID pBuf = NULL;

	if (!bSharedMemoryInit)
	{
		const auto sSharedName = L"MBAACCExtendedTrainingMode";
		const int nSharedSize = 8;

		hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, sSharedName);
		if (hMapFile == NULL) {
			return;
		}

		pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, nSharedSize);
		if (pBuf == NULL)
		{
			CloseHandle(hMapFile);
			return;
		}
		bSharedMemoryInit = true;
	}

	int nIdleHighlightSetting = ((int*)pBuf)[0];
	int nBlockingHighlightSetting = ((int*)pBuf)[1];

	switch (nIdleHighlightSetting) {
	default:
	case NO_HIGHLIGHT:
		arrIdleHighlightSetting = { 255, 255, 255 };
		break;
	case RED_HIGHLIGHT:
		arrIdleHighlightSetting = { 255, 0, 0 };
		break;
	case GREEN_HIGHLIGHT:
		arrIdleHighlightSetting = { 0, 255, 0 };
		break;
	case BLUE_HIGHLIGHT:
		arrIdleHighlightSetting = { 0, 0, 255 };
		break;
	}

	switch (nBlockingHighlightSetting) {
	default:
	case NO_HIGHLIGHT:
		arrBlockingHighlightSetting = { 255, 255, 255 };
		break;
	case RED_HIGHLIGHT:
		arrBlockingHighlightSetting = { 255, 0, 0 };
		break;
	case GREEN_HIGHLIGHT:
		arrBlockingHighlightSetting = { 0, 255, 0 };
		break;
	case BLUE_HIGHLIGHT:
		arrBlockingHighlightSetting = { 0, 0, 255 };
		break;
	}
}


void debugLogBytes(u8* p) {

	static char buffer[256];

	for (int i = 0; i < 16; i++) {
		snprintf(buffer, 256, "%08X", *(u8*)p);
		p++;
	}
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

	u8 jumpCode[] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
	DWORD funcOffset = newAddr - (patchAddr + 5);
	*(unsigned*)(&jumpCode[1]) = funcOffset;
	patchMemcpy(patchAddr, jumpCode, sizeof(jumpCode));
}

void patchByte(auto addr, const u8 byte) 
{
	static_assert(sizeof(addr) == 4, "Type must be 4 bytes");

	u8 temp[] = { byte };

	patchMemcpy(addr, temp, 1);
}

// actual functions 

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

	auto updateAnimation = [](unsigned animDataAddr, unsigned blockState, unsigned patternState) -> void {
		//static char buffer[256];
		//snprintf(buffer, 256, "%d", blockState);
		//log(buffer);
		if (blockState == 1) {
			patchMemcpy(animDataAddr + 0x18, arrBlockingHighlightSetting.data(), 3);
		}
		else {
			switch (patternState) {
			case 0:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 17:
			case 18:
			case 19:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
			case 360:
				patchMemcpy(animDataAddr + 0x18, arrIdleHighlightSetting.data(), 3);
				break;
			default:
				//writeColor(animDataAddr, 0, 0, 255);
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
	if (*(unsigned*)dwP1AnimPtr > dwBaseAddress) {
		updateAnimation(*(unsigned*)dwP1AnimPtr, *(u8*)dwP1Blocking, *(unsigned*)dwP1PatternRead);
	}

	if (*(unsigned*)dwP2AnimPtr > dwBaseAddress) {
		updateAnimation(*(unsigned*)dwP2AnimPtr, *(u8*)dwP2Blocking, *(unsigned*)dwP2PatternRead);
	}
}

u8 animHookBytesOrig[10];
u8 animHookBytesMod[10];
void animHookFunc() {
	// does this func get called for both chars individually? 
	void* funcAddress = (void*)0x0045f650;
	// restore func to original state
	patchMemcpy(funcAddress, animHookBytesOrig, 10);
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
	patchMemcpy(funcAddress, animHookBytesMod, 10);
	// perform coloring code
	animLog();
	return;
}

void initAnimHook() {
	void* funcAddress = (void*)0x0045f650;
	// backup
	patchMemcpy(animHookBytesOrig, funcAddress, 10);
	// new bytes
	patchFunction(funcAddress, animHookFunc);
	// ret
	patchByte(((u8*)funcAddress) + 5, 0xC3);
	// backup modded bytes 
	patchMemcpy(animHookBytesMod, funcAddress, 10);
}

void threadFunc() {
	//log("DLL injection successful");
	srand(time(NULL));

	// todo, put something here to prevent mult injection

	initAnimHook();

	while (true) {
		/*
		this loop is no longer needed
		due to that, it would now be possible to do this without dll injection, and just modify the programs memory
		but getting enough space to put what is needed in there, may not be worth the effort
		*/

		// ideally, this would be done with signals
		GetSharedMemory();
		Sleep(8);
	}

}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	(void)hinstDLL;
	(void)lpReserved;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadFunc, 0, 0, 0);
	}

	return TRUE;
}

