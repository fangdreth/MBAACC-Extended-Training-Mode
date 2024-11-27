
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
#include <initializer_list>
#include <stdarg.h>
#include <d3d9.h>
#include <d3dx9.h> // https://www.microsoft.com/en-us/download/details.aspx?id=6812
#include <dxerr.h>
#include <algorithm>
#include <dsound.h>
#include <mmsystem.h>
#include <thread>

#include "..\Common\Common.h"
#include "..\Common\CharacterData.h"

#define FFMPEG_STATIC
#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS

extern "C" {

	// for the love of all that is holy, build ffpeg in a powershel dev console in visual studio 2022 with:
	// bash ./configure --toolchain=msvc --arch=x86 --disable-shared --enable-static --disable-network
	// bash ./configure --toolchain=msvc --arch=x86 --disable-shared --enable-static --disable-network --enable-encoder=png
	// and then just run make
	// you will also need NASM in path
	// this will take a while, TODO, see what ffmpeg features i can disable
	// maybe --disable-doc --disable-debug
	// due to building on windows, my shit is neutered
	// i dont even have pngs?

	#define FFMPEG_STATIC
	#define __STDC_CONSTANT_MACROS
	#define __STDC_FORMAT_MACROS
	                      
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
	#include <libavcodec/avcodec.h>
	#include <libavutil/opt.h>
	#include <libswresample/swresample.h>


	
}

// the order of these libraries matter. i love linking
#pragma comment(lib, "FFmpeg/libavformat/libavformat.a")
#pragma comment(lib, "FFmpeg/libavcodec/libavcodec.a")
#pragma comment(lib, "FFmpeg/libswscale/libswscale.a")
#pragma comment(lib, "FFmpeg/libavutil/libavutil.a")	
#pragma comment(lib, "FFmpeg/libavutil/libavutil.a")	
#pragma comment(lib, "FFmpeg/libavdevice/libavdevice.a")
#pragma comment(lib, "FFmpeg/libavfilter/libavfilter.a")
#pragma comment(lib, "FFmpeg/libswresample/libswresample.a")

// the 5 following libs are all needed for ffmpeg????
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "strmiids") 
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "Secur32.lib")

#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")




#pragma push_macro("optimize")
//#pragma optimize("t", on) 


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))

static_assert(sizeof(int*) == 4, "COMPILE PROGRAM AS 32BIT");

typedef DWORD ADDRESS;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef uint32_t uint;

void enemyReversal();
void initDrawTextureHook();
void messUpTexture();
void miscDirectX();
void initIdk();

static double getSeconds() {
	long long temp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return ((double)temp) / 1000000.0;
}

D3DXVECTOR3 RGBtoHSV(const D3DXVECTOR3& rgb) {
	float r_f = rgb.x / 255.0f;
	float g_f = rgb.y / 255.0f;
	float b_f = rgb.z / 255.0f;

	float max = MAX(MAX( r_f, g_f), b_f );

	float min = MIN(MIN( r_f, g_f), b_f );
	float delta = max - min;

	D3DXVECTOR3 hsv;
	
	hsv.x = 0.0f;

	hsv.z = max;
	hsv.y = 0.0f;

	if (max != 0.0f) { // should i be epsiloning here
		hsv.y = delta / max;
	}

	if (delta == 0) {
		return hsv;
	}

	if (r_f > g_f && r_f > b_f) { // RMAX
		hsv.x = fmod((g_f-b_f)/delta, 6.0f);

		hsv.x /= 6.0f;
	} else if (g_f > r_f && g_f > b_f) { // GMAX
		hsv.x = ((b_f - r_f) / delta) + 2;

		hsv.x /= 6.0f;

		//hsv.x = 1 - hsv.x;
	} else { // BMAX
		hsv.x = ((r_f - g_f) / delta) + 4;

		hsv.x /= 6.0f;
	}

	return hsv;
}

D3DXVECTOR3 HSVtoRGB(const D3DXVECTOR3& hsv) {
	float h = hsv.x * 360.0f;
	float s = hsv.y;
	float v = hsv.z;

	float r_f, g_f, b_f;

	float c = v * s;
	float x = c * (1 - fabs(fmod(h / 60.0f, 2.0f) - 1));
	float m = v - c;

	switch ((int)(h / 60.0f)) {
	default:
	case 0:
		r_f = c;
		g_f = x;
		b_f = 0.0f;
		break;
	case 1:
		r_f = x;
		g_f = c;
		b_f = 0.0f;
		break;
	case 2:
		r_f = 0.0f;
		g_f = c;
		b_f = x;
		break;
	case 3:
		r_f = 0.0f;
		g_f = x;
		b_f = c;
		break;
	case 4:
		r_f = x;
		g_f = 0.0f;
		b_f = c;
		break;
	case 5:
		r_f = c;
		g_f = 0.0f;
		b_f = x;
		break;
	}

	D3DXVECTOR3 rgb;
	rgb.x = (r_f + m) * 255.0f;
	rgb.y = (g_f + m) * 255.0f;
	rgb.z = (b_f + m) * 255.0f;

	return rgb;
}

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

const ADDRESS dwBaseAddress = (0x00400000);

DWORD addrEndScene = 0x663fb900;
DWORD addrEndScenePatch = 0x663fb996;

bool bCasterInit = false;
ADDRESS dwCasterBaseAddress = 0;
ADDRESS dwCasterFullBaseAddress = 0;
DWORD dwDevice = 0; // MASM is horrid when it comes to writing pointers vs value of pointer bc it has type checking. thats why this cant be a pointer
IDirect3DDevice9* device = NULL;

DWORD dwSoundDevice = 0;
IDirectSound8* soundDevice = NULL;

bool shouldPlaySong = false;

const ADDRESS INPUTDISPLAYTOGGLE = (dwBaseAddress + 0x001585f8);

DWORD tempRegister1;
DWORD tempRegister2;
DWORD tempRegister3;
DWORD tempRegister4;

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

int TESTVAR1 = 0;
int TESTVAR2 = 0;
int trigBreak = 0;

KeyState upKey(VK_UP);
KeyState downKey(VK_DOWN);
KeyState leftKey(VK_LEFT);
KeyState rightKey(VK_RIGHT);
KeyState rControl(VK_RCONTROL);

KeyState lBracketKey(VK_OEM_4);
KeyState rBracketKey(VK_OEM_6);

KeyState semicolonKey(VK_OEM_1);
KeyState quoteKey(VK_OEM_7);

KeyState backslashKey(VK_OEM_5);

int increaseLogging = 0;
KeyState F10Key(VK_F10); // reload shader
KeyState F11Key(VK_F11); // reload shader
KeyState F12Key(VK_F12); // non release caster builds use this
bool useCustomPalettes = true;

void __stdcall ___log(const char* msg)
{
	
	const char* ipAddress = "127.0.0.1";
	unsigned short port = 17474;

	int msgLen = strlen(msg);

	/*
	char* message = new char[msgLen + 4];

	strncpy_s(message, msgLen + 4, msg, msgLen);
	message[msgLen + 0] = '\r';
	message[msgLen + 1] = '\n';
	message[msgLen + 2] = '\0';
	*/

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

	//delete[] message;
}

void __stdcall log(const char* format, ...) {
	
	static char buffer[1024]; // no more random char buffers everywhere.
	va_list args;
	va_start(args, format);
	vsnprintf_s(buffer, 1024, format, args);
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

bool __stdcall safeWrite() {
	BYTE PauseFlag = *reinterpret_cast<BYTE*>(dwBaseAddress + dwPausedFlag);
	BYTE GameState = *reinterpret_cast<BYTE*>(dwBaseAddress + dwGameState);
	int FrameTimer = *reinterpret_cast<int*>(dwBaseAddress + dwFrameTimer);

	if (PauseFlag == 1 ||
		GameState != 1 ||
		FrameTimer == 0)
		return false;
	
	return true;
}

bool __stdcall isAddrValid(DWORD addr) {
	__try {
		volatile BYTE test = *(BYTE*)addr; 
		return true;
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return false; 
	}
}

// i am unsure of how big to make this array. i dont want to have crashes from reading uninited mem
// loop until 0040d355, which is the ret addr of gamestart. set
// i didnt find that in the stack until somewhere like 4C0, might as well alloc 0x1000 
DWORD saveTexture_stack[0x1000];
DWORD stackAddr = 0;
int stackSaveIndex = 0;
void saveStack() {
	
	if (stackAddr == 0) {
		return;
	}

	log("attemping to save stack");

	DWORD temp;
	
	int i = 0;
	for (; i < 0x1000 / 4; i++) {

		if (!isAddrValid(stackAddr + (i * 4))) {
			break;
		}

		//log("saving %04X %08X", i, stackAddr + (i * 4));
		temp = *(DWORD*)(stackAddr + (i * 4));

		saveTexture_stack[i] = temp;

		if (temp == 0x0040d355 || 
			(temp >= 0x00401770 && temp <= 0x0040182f) // mainMelty
			) {
			break;
		}
	}

	stackSaveIndex = i;

	log("stack saved");
}

void logStack() {

	// sections:
	// .text ram:00401000-ram:0051afff
	// .rdata ram:0051b000-ram:0054afff 

	DWORD temp;

	// havng this many log calls aint ideal. i should snprintf!
	for (int i = 0; i < stackSaveIndex; i++) {

		temp = saveTexture_stack[i];

		if (temp >= 0x00401000 && temp < 0x0051afff) {
			log("possible ret    attr: [esp + %04X] %08X", i * 4, temp);
		}

		if (temp >= 0x00555130 && temp < 0x555130 + (5 * 0xAFC)) {
			log("possible char   attr: [esp + %04X] %08X", i * 4, temp);
		}

		if (temp >= 0x0067bde8 && temp < 0x67bde8 + (1000 * 0x33c)) {
			log("possible effect attr: [esp + %04X] %08X", i * 4, temp);
		}

		//log("stack: [%08X + %04X] %08X", stackAddr, i * 4, temp);

		if (temp == 0x0040d355) {
			break;
		}
	}

	stackSaveIndex = 0;
}

DWORD needHookReset = false;

std::string getExtraDataPath() {

	std::string loadPath = std::string(__FILE__);
	size_t pos = loadPath.find_last_of('\\');
	loadPath = loadPath.substr(0, pos);
	//loadPath += "\\testShader.hlsl";

	loadPath += "\\";

	//return loadPath;
	return ".\\"; // for distribution, just have the extra files in the melty folder
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

// the patch func being templated causes problems when calling from asm
void __stdcall asmPatchMemcpy(void* dest, void* source, DWORD n) {
	DWORD oldProtect;
	VirtualProtect(dest, n, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, source, n);
	VirtualProtect(dest, n, oldProtect, NULL);
}

void __stdcall patchFunction(auto patchAddr_, auto newAddr_)
{
	// todo, rename this to patchCall pls
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

// actual functions 

extern "C" int asmDrawText(int w, int h, int x, int y, const char* text, int alpha, int shade, int layer, void* addr, int spacing, int idek, char* out);

void drawText(int x, int y, int w, int h, const char* text, int alpha, int shade, int layer = 0x2cc, void* font = (void*)adFont2) {
	// text was initially just char*, i made it const, is that going to be ok?
	asmDrawText(w, h, x, y, text, alpha, shade, layer, font, 0, 0, 0);
}

void drawText(int x, int y, const char* text, int textSize = 16, ADDRESS font = adFont1) {

	static_assert(sizeof(font) == 4, "Type must be 4 bytes");

	int len = strnlen_s(text, 1024);

	// 0x80 - 0x20 - 1, max (non extended) ascii val - 0x20(space). -1 is for the delete. this is almost definitely stored elsewhere in melty, i just dont know where
	// actually its at 0041d5b0, but still, not worth
	// https://gist.github.com/aminnj/5ca372aa2def72fb017b531c894afdca as a starter
	constexpr float charWidths[0x80 - 0x20 - 1] = {
		4.4453125,   // ' '
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
		4.4453125,   // '.'
		4.4453125,   // '/'
		8.8984375,   // '0'
		7.7228125,   // '1'
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

	while (*c) {
		if (*c < ' ' || *c > '~') {
			c++;
			continue;
		}
		tempWidth += charWidths[*c - 0x20];
		c++;
	}

	tempWidth *= textSize * 0.0625;

	//tempWidth = ((float)textSize / 5.0) * (float)len;

	drawText(x, y, (int)tempWidth, textSize, text, 0xFF, 0xFF, 0x02CC, (void*)font);
}

void drawTextWithBorder(int x, int y, int w, int h, const char* text) {

	drawText(x, y, w, h, text, 0xFF, 0xFF);
	
	drawText(x - 1, y, w, h, text, 0xFF, 0x00);
	drawText(x + 1, y, w, h, text, 0xFF, 0x00);
	drawText(x, y - 1, w, h, text, 0xFF, 0x00);
	drawText(x, y + 1, w, h, text, 0xFF, 0x00);

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

#include "D3DHooks.h"
#include "textureModification.h"
#include "CSSModification.h"

// -----

template<size_t size>
class AdjustableVar {
public:
	// here so i can more easily make adjustments to things at runtime

	AdjustableVar(std::function<const DWORD()> addrFunc_, const BYTE(&bytes_)[size]) {
		
		if (size == 0) {
			throw std::invalid_argument("Invalid size.");
		}

		if (addrFunc_ == NULL) {
			throw std::invalid_argument("Invalid addr func.");
		}

		addrFunc = addrFunc_; // lambda func to return the addr
		memcpy(bytes, bytes_, size);
	}

	void update() {
		DWORD addr = addrFunc();
	
		int incAmount = rControl.keyHeld() ? 0x10 : 0x1;

		if (leftKey.keyDown()) {
			index = (index == 0) ? size - 1 : (index - 1) % size;
		}

		if (rightKey.keyDown()) {
			index = (index + 1) % size;
		}

		if (upKey.keyDown()) {
			bytes[index] = (bytes[index] + incAmount) & 0xFF;
		}

		if (downKey.keyDown()) {
			bytes[index] = (bytes[index] - incAmount) & 0xFF;
		}

		static char buffer[256];

		snprintf(buffer, 256, "%08X:", addr);
		drawText(50, 100, 10, 14, buffer, 0xFF, 0xFF);

		if (addr != 0) {
			patchMemcpy(addr, &bytes, size);
		}

		int x = 140;
		for (size_t i = 0; i < size; i++) {
			snprintf(buffer, 256, "%02X", bytes[i]);
			BYTE shade = (i == index) ? 0xFF : 0xAA;
			drawText(x, 100, 10, 14, buffer, 0xFF, shade);
			x += 20;
		}
	}

private:
	BYTE bytes[size];
	size_t index = 0;
	std::function<const DWORD()> addrFunc = NULL;
};

// -----

void dumpAddr(auto addr) {
	// display a hexdump to the screen
	static_assert(sizeof(addr) == 4, "Type must be 4 bytes");

	if (addr < dwBaseAddress) {
		return;
	}
	
	static char buffer[256];

	int x = 50;
	int y = 100;

	for (unsigned offset = 0; offset < 0x10; offset++) {
		snprintf(buffer, 256, "%08X", (DWORD)(addr)+(offset * 0x10));
		drawText(x, y, 10, 14, buffer, 0xFF, 0xFF);
		x += 80;

		snprintf(buffer, 256, "%02X", offset * 0x10);
		drawText(x, y, 10, 14, buffer, 0xFF, 0xFF);
		x += 30;

		for (unsigned subOffset = 0; subOffset < 0x10; subOffset += 0x4) {
			snprintf(buffer, 256, "%08X", *(DWORD*)(addr + (offset * 0x10) + subOffset));
			drawText(x, y, 10, 14, buffer, 0xFF, 0xFF);
			x += 80;
		}
		x = 50;
		y += 14;
	}
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

	DWORD baseFrameDataPtr1 = *(DWORD*)(objAddr + 0x330); // HA6 data pointer
	if (baseFrameDataPtr1 == 0) { return 0; }

	DWORD baseFrameDataPtr2 = *(DWORD*)(baseFrameDataPtr1); // HA6 data
	if (baseFrameDataPtr2 == 0) { return 0; }

	DWORD unknownPtr1 = *(DWORD*)(baseFrameDataPtr2 + 0x4); // HA6 patterns
	if (unknownPtr1 == 0) { return 0; }

	DWORD unknownPtr2 = *(DWORD*)(unknownPtr1 + 0x4); // I do not know
	if (unknownPtr2 == 0) { return 0; }

	DWORD basePatternPtr = *(DWORD*)(unknownPtr2 + (objPattern * 0x4)); // actual index to the pattern?
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

	/*
	objFramePtr + 
	
	*/

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

void miscAdjustTests() {
	static AdjustableVar<5> currentTexture([]() -> DWORD {
		if (!((*(int*)(0x67bde8) != 0) && (*(char*)(0x67be09) == '\0'))) {
			return 0;
		}
		DWORD res = getObjFrameDataPointer(0x67bde8);
		if (res == 0) {
			return 0;
		}
		return res + 0x18;
		}, { 0x00, 0x00, 0xFF, 0x00, 0xFF });

	//currentTexture.update();

	//ID3DXFont* pFont = nullptr;

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
	
	if (!safeWrite()) {
		return;
	}

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

	//miscAdjustTests();
}

void highlightStates()
{

	if (!safeWrite()) {
		return;
	}

	static bool settingWasChanged = false;

	if (!settingWasChanged && (
		(arrIdleHighlightSetting != std::array<BYTE, 3>({ 255, 255, 255 })) ||
		(arrBlockingHighlightSetting != std::array<BYTE, 3>({255, 255, 255})))) {
		settingWasChanged = true;
	} else {
		return;
	}

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

unsigned callBackFrameCount = 0;
void __stdcall pauseCallback(DWORD dwMilliseconds)
{
	// windows Sleep, the func being overitten is an stdcall, which is why we have __stdcall
	static KeyState pKey('P');
	static KeyState nKey('N');
	static bool bIsPaused = false;
	static KeyState rKey('R');
	static bool trigDelay = false;
	static int delay = 0;
	
	/*
	if (pKey.keyDown())
	{
		bIsPaused = !bIsPaused;		
	}

	if (rKey.keyDown()) {
		trigDelay = true;
		delay = 16*5;
		bIsPaused = true;
	}

	if (!bIsPaused && nKey.keyDown())
	{
		bIsPaused = true;
	}
	*/

	bool ok = true;
	MSG msg;
	while (bIsPaused) {
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

		if (trigDelay) {
			if (delay > 0) {
				delay--;
			}
			else {
				trigDelay = false;
				bIsPaused = false;
				break;
			}
		}

		if (pKey.keyDown())
		{
			bIsPaused = !bIsPaused;
		}

		if (nKey.keyDown()) {
			break;
		}
	}	

	
	//log("frame %d", callBackFrameCount);
	callBackFrameCount++;

	static std::thread* loadVideoThread = NULL;
	static bool initTextureIsGood = false;

	if (initTextureIsGood && loadVideoThread != NULL && loadBadAppleFinished) {
		log("joining loader thread");
		loadVideoThread->join();
		loadVideoThread = NULL;
	}

	
	if (!initTextureIsGood) {
		initTextureIsGood = initTextureModifications();

		if (initTextureIsGood) {
			// this needs to be called with a thread loadBadApple
			log("launching loader thread");
			loadVideoThread = new std::thread(loadBadApple);
		}
	}

	/*static bool initCSSIsGood = false;
	if (!initCSSIsGood) {
		initCSSIsGood = initCSSModifications();
	}*/
	

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


DWORD tempD3DHook = 0;
__declspec(naked) void tempD3DFunc() {

	PUSH_ALL;
	log("endsceneCalled");
	POP_ALL;

	__asm {
		jmp [tempD3DHook];
	};
}


// Vertex structure
struct CUSTOMVERTEX {
	D3DVECTOR position;
	D3DCOLOR color;
};

// Vertex format
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

void DrawTriangle(IDirect3DDevice9* d3ddev) {
	if (!d3ddev) return;

	// todo, redo drawing and text with this
	// make sure it works fullscreen and not, and scaled fullscreen and not.

	constexpr float whatIsThis = 0.0f;

	// Define the vertices of the triangle
	constexpr CUSTOMVERTEX vertices[] = {
		{ D3DVECTOR(0.0f, 0.5f, whatIsThis), D3DCOLOR_XRGB(255, 0, 0) },
		{ D3DVECTOR(0.5f, -0.5f, whatIsThis), D3DCOLOR_XRGB(0, 255, 0) },
		{ D3DVECTOR(-0.5f, -0.5f, whatIsThis), D3DCOLOR_XRGB(0, 0, 255) }
	};

	// Set up the vertex buffer and draw
	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;
		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		//d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		//d3ddev->BeginScene();
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetFVF(D3DFVF_CUSTOMVERTEX);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		//d3ddev->EndScene();
		//d3ddev->Present(NULL, NULL, NULL, NULL);
		v_buffer->Release();
	}
}

//std::vector<DWORD> textureAddrs;
void frameDoneCallback()
{

	/*
	if (lBracketKey.keyDown()) {
		TESTVAR1--;
		TESTVAR1 = MAX(TESTVAR1, -1);
	}

	if (rBracketKey.keyDown()) {
		TESTVAR1++;
	}

	if (semicolonKey.keyDown()) {
		TESTVAR2--;
		TESTVAR2 = MAX(TESTVAR2, -1);
	}

	if (quoteKey.keyDown()) {
		TESTVAR2++;
	}

	if (trigBreak) {
		trigBreak = 0;
	}

	if (backslashKey.keyDown()) {
		trigBreak = 1;
	}

	if (F12Key.keyDown()) { // this setting should be saved, and also shouldnt override ur own palettes! but thats a we need caster thing for
		useCustomPalettes = !useCustomPalettes;
	}

	if (F11Key.keyDown()) {
		//initShaderFromFile();
	}

	if (increaseLogging > 0) {
		increaseLogging--;
	}
	if (F10Key.keyDown()) {
		increaseLogging = 2;
	}

	static KeyState hKey('H');
	static bool bShowHitboxes = false;

	static KeyState bKey('B');
	static bool bDrawFramebar = false;

	if (hKey.keyDown()) {
		bShowHitboxes = !bShowHitboxes;
	}

	if (bKey.keyDown()) {
		bDrawFramebar = !bDrawFramebar;
	}

	if (bShowHitboxes) {
		drawFrameData();
	}

	if (bDrawFramebar) {
		drawFrameBar();
	}
	*/

	//if (safeWrite()) {
	//messUpTexture();
	//}
	//textureAddrs.clear();	

	static bool deviceInit = false;
	if (!deviceInit && dwDevice != NULL) {
		deviceInit = true;

		device = (IDirect3DDevice9*)dwDevice;

		log("dwDevice: %08X", dwDevice);

		// repatch original bytes and remove my code 
		BYTE bytes[4] = { 0x5B, 0xC2, 0x04, 0x00 };
		patchMemcpy(dwCasterBaseAddress + addrEndScenePatch, &bytes, 4);

		__asm {
			nop;
			nop;
			nop;
			nop;
			nop;
		};

		unsigned avalTexMem = device->GetAvailableTextureMem();

		__asm {
			nop;
			nop;
			nop;
			nop;
			nop;
		};

		log("avail tex mem is %08X", avalTexMem);

		HookThisShit(device);
	}

	static bool soundDeviceInit = false;
	if (!soundDeviceInit) {

		//playSong();
		
	}

	if (shouldPlaySong) {
		playSong();
	} else {
		pauseSong();
	}
	shouldPlaySong = false;

	// this,,, actually does manage to change the volume?
	/*
	PUSH_ALL;
	__asm {
		push 00000000h;
		mov eax, 004de160h;
		call eax;
		add esp, 4;
	}
	POP_ALL;
	*/
}

__declspec(naked) void nakedFrameDoneCallback() {
	PUSH_ALL;
	frameDoneCallback();
	POP_ALL;
	__asm {
		add esp, 4Ch;
		ret;
	};
}

int nTempP1MeterGain = 0;
int nTempP2MeterGain = 0;
int nP1MeterGain = 0;
int nP2MeterGain = 0;
DWORD prevComboPtr = 0;
void attackMeterDisplayCallback() {

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

	snprintf(buffer, 256, "P1 METER GAIN %12d.%02d%%", nP1MeterGain / 100, nP1MeterGain % 100);
	drawTextWithBorder(420, 186, 10, 14, buffer);

	snprintf(buffer, 256, "P2 METER GAIN %12d.%02d%%", nP2MeterGain/ 100, nP2MeterGain % 100);
	drawTextWithBorder(420, 186 + 14, 10, 14, buffer);
}

DWORD drawTextureHookRegisterSave1 = 0;
DWORD drawTextureHookRegisterSave2 = 0;
DWORD tempTextureAddr1 = 0;
DWORD tempTextureAddr2 = 0;
void messUpTextureActual(DWORD addr) {

	if (device == NULL) {
		//log("device null, returning");
		return;
	}

	HRESULT hr;
	//DWORD addr;

	//addr = tempTextureAddr2;

	//log("whatttf %08X", addr);

	if (addr == 0) {
		//log("init addr is %08X, returning", addr);
		return;
	}

	/*
	addr = *(DWORD*)tempTextureAddr1;

	if (addr == 0) {
		log("deref addr is %08X, returning", addr);
		return;
	}
	*/
	
	//IDirect3DResource9* pResource = (IDirect3DResource9*)addr;
	//DWORD wtfType = pResource->GetType();
	
	// 698D229C bad

	log("wtf %08X", *(DWORD*)addr);

	if (*(DWORD*)addr == 0) {
		return;
	}


	IDirect3DResource9* pResource = (IDirect3DResource9*)addr;

	DWORD resourceType = pResource->GetType();
	log("resource type was %d", resourceType);

	IDirect3DDevice9* ugh = NULL;
	pResource->GetDevice(&ugh);
	if (ugh == NULL) {
		log("got device NULL");
	}
	else {
		log("got device %08X", *ugh);
	}
	

	pResource->Release(); // do i have to fucking do this
	
	IDirect3DTexture9* pTex = (IDirect3DTexture9*)addr;
	
	log("getting type");
	DWORD type = pTex->GetType();
	log("type was %3d", type);

	log("getting lvl count");
	DWORD levelCount = pTex->GetLevelCount();
	log("lvl count was %d", levelCount);

	D3DSURFACE_DESC desc;

	tempTextureAddr2 = 0;

	log("getting level desc");
	hr = pTex->GetLevelDesc(0, &desc);
	log("got lvl desc");

	if (FAILED(hr)) {
		log("getlvldesc failed??");
		//pTex->Release();
		log("ret");
		return;
	}

	D3DPOOL pool = desc.Pool;
	D3DFORMAT format = desc.Format;
	DWORD usage = desc.Usage;

	DWORD dwCol;
	WORD wCol;

	//D3DCOLOR col = D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00);
	// whats the most efficient way of doing these copies such that i dont overwrite the buffer? probs just a overall
	size_t colSize = -1;

	switch (format) {
	case D3DFMT_A8R8G8B8:
		dwCol = 0xFFFF00FF;
		colSize = 4;
		break;
	case D3DFMT_X8R8G8B8:
		dwCol = 0xFF00FFFF;
		colSize = 4;
		break;
	case D3DFMT_A1R5G5B5:
		wCol = 0b1000001111100000;
		colSize = 2;
		break;
	default:
		log("wtf is that format, bye");
		//pTex->Release();
		log("ret");
		return;
		break;
	}

	
	//if (format != D3DFMT_A1R5G5B5) {
	/*
	if (format == D3DFMT_X8R8G8B8) {
		log("dont like the format");
		pTex->Release();
		log("ret");
		return;
	}
	*/

	log("addr: %08X val: %08X type: %3d level: %3d pool: %3d format: %3d usage: %08X", addr, *(DWORD*)addr, type, levelCount, pool, format, usage);
	
	//IDirect3DPixelShader9* pixelShaderBackup;
	//device->GetPixelShader(&pixelShaderBackup);

	//device->SetPixelShader(pPixelShader);
	//device->SetTexture(0, pTex);
  
	//device->SetPixelShader(pixelShaderBackup);


	log("attempting to release");
	//pTex->Release();

	log("DONE, RETURNING");
	return;

	D3DLOCKED_RECT lockedRect;
	//hr = pTex->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
	//hr = pTex->LockRect(0, &lockedRect, NULL, D3DLOCK_NOOVERWRITE);
	hr = pTex->LockRect(0, &lockedRect, NULL, 0);
	if (FAILED(hr)) {
		log("lock failed!!!");
		pTex->Release();
		log("ret");
		return;
	}

	//int width = lockedRect.Pitch / 4;
	int width = desc.Width;
	int height = desc.Height;

	// there has to be a better way to do this
	if (colSize == 4) {
		/*DWORD* pImage = (DWORD*)lockedRect.pBits;

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; x += 2) {
				pImage[x] = dwCol;
			}
			pImage += width;
		}*/

	}
	else if (colSize == 2) {
		WORD* pImage = (WORD*)lockedRect.pBits;

		BYTE r = 0;
		BYTE g = 0;
		BYTE b = 0;

		WORD col = 0;

		int startX = 0;

		for (int y = 0; y < height; ++y) {
			for (int x = startX; x < width; x += 2) {
				if (pImage[x] & 0x80) {
					//pImage[x] = wCol;
					//pImage[x] = (pImage[x] & ~0x80);
				
					r = 0x1F - (x % 0x1F);
					g = 0;
					b = 0x1F - (y % 0x1F);

					//col = (0x0) | ((r & 0x1F) << 10) | ((g & 0x1F) << 5) | ((b & 0x1F) << 0);

					col = addr & 0x7F;

					pImage[x] = col;
					//pImage[x] = 0b1111110000011111;
				}
			}
			startX = 1 - startX;
			pImage += width;
		}

	}

	pTex->UnlockRect(0);


	pTex->Release();
}

void MASMSucks() {

	if (tempTextureAddr1 == 0) {
		return;
	}

	tempTextureAddr2 = *(DWORD*)tempTextureAddr1;

	//textureAddrs.push_back(tempTextureAddr2);
}

void drawCharacterTextureCallback() {
	//DWORD backup = tempTextureAddr1;
	//tempTextureAddr1 = tempTextureAddr2;
	//messUpTexture();
	//tempTextureAddr1 = backup;
}

DWORD _nakedDrawCharacterTextureRegister;
__declspec(naked) void nakedDrawCharacterTextureCallback() {

	/*
	__asm {
		mov _nakedDrawCharacterTextureRegister, edx;

		mov edx, dword ptr[EDI + 038h];
		mov edx, dword ptr[edx + 04h];

		mov tempTextureAddr2, edx;

		mov edx, _nakedDrawCharacterTextureRegister;
	};
	*/

	PUSH_ALL;
	__asm {
		call drawCharacterTextureCallback;
	}
	POP_ALL;
	

	__asm {
		pop esi;
		pop ebp;
		pop ebx;
		add esp, 0B8h;
		ret;
	};
}

void miscDirectX() {

	if (device == NULL) {
		return;
	}


	return;

	IDirect3DSurface9* pRenderTarget = nullptr;

	for (DWORD i = 0; i < 4; ++i) {
		if (SUCCEEDED(device->GetRenderTarget(i, &pRenderTarget))) {
			if (pRenderTarget) {
				// Do something with the render target, like getting its description
				D3DSURFACE_DESC desc;
				pRenderTarget->GetDesc(&desc);

				// Output render target information
				log("Render Target %d: Width = %d, Height = %d, Format = %d\n",
					i, desc.Width, desc.Height, desc.Format);

				// Release the surface when done
				pRenderTarget->Release();
				pRenderTarget = nullptr;
			}
			else {
				// No render target set for this index
				log("Render Target %d is not set.\n", i);
			}
		}
		else {
			// Error in getting render target
			log("Failed to get Render Target %d\n", i);
		}
	}

	
	log("-----");
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

BYTE arrMeterGainHookOrig[10];
BYTE arrMeterGainHookMod[10];
__declspec(naked) void meterGainHook() {
	
	/*
	the hooked func func(0x00476ce0) takes params.
	the hooked func,,, does not clean its stack. its cdecl 

	this is a massive issue because the compiler is,,, doing compile things, making a new stack frame, pushing and popping, etc
	that will not work here. sorry about this code  
	additionally, c++17 decided to remove the register keyword! 
	therefore, out of paranoia, i will be doing this all in asm

	https://en.wikipedia.org/wiki/X86_calling_conventions
	we are under cdecl currently
	return val EAX by callee
	Registers EAX, ECX, and EDX are caller-saved
	the rest( EBX, ESI, EDI, EBP(?), ESP(?) 

	EIP doesnt count 

	https://stackoverflow.com/questions/25129743/confusing-brackets-in-masm32

	*/

	// call this when the pattern is written

	__asm {
		pop tempRegister1; // HOOKED CALL ADDR
		pop tempRegister2;  // CALLER ADDR
	};

	__asm {
		// at this point, i now have direct access to the 4 func params, and a lack of worry over messing things up
		mov ecx, [esp + 0h]; // CharacterSubObj
		mov edx, [esp + 4h]; // METER AMOUNT;

		cmp ecx, 00555134h; // P1 ADDR
		JE _P1;
		cmp ecx, 00555C30h; // P2 ADDR
		JE _P2;
		cmp ecx, 0055672Ch; // P3 ADDR
		JE _P1;
		cmp ecx, 00557228h; // P4 ADDR
		JE _P2;
		// this means a projectile hit.
		mov ecx, [ecx + 02F0h]; // owner offset
		test ecx, 1;
		JE _P1;

	_P2 : // add to P2 meter gain
		mov nTempP2MeterGain, edx;
		JMP _END;
	_P1: // add to P1 meter gain
		mov nTempP1MeterGain, edx;
	_END:
	};

	PUSH_CALLEE;
	PUSH_FRAME;

	// these calls could(SHOULD) be made in asm, but the assembler really doesnt like it when i do 
	// although, it is more readable
	// if any of these happen to put a temp var on the stack, or dont use stdcall format, this whole thing will break.

	asmPatchMemcpy((void*)0x00476ce0, (void*)arrMeterGainHookOrig, 10);

	POP_FRAME;
	POP_CALLEE;

	// perform the actual function call
	__asm {
		mov eax, 00476CE0h;
		call eax;
		// do i need to clean the stack here?
		// NO. the original caller will.
	};

	PUSH_CALLEE;
	PUSH_FRAME;

	asmPatchMemcpy((void*)0x00476ce0, (void*)arrMeterGainHookMod, 10);

	POP_FRAME;
	POP_CALLEE;

	// restore the proper stack state and ret
	__asm {
		push tempRegister2;
		push tempRegister1;
		ret;
	};
}

void battleResetCallback() {
	nTempP1MeterGain = 0;
	nTempP2MeterGain = 0;
	nP1MeterGain = 0;
	nP2MeterGain = 0;
	prevComboPtr = 0;
}

__declspec(naked) void drawTextureHook3() {

	__asm {
		pop drawTextureHookRegisterSave1;
	};

	// modify the vars in the stack.
	__asm {
		//mov[esp + 1Ch], 1;
		//mov[esp + 10h], 1; // miplevels
		//or [esp + 14h], D3DUSAGE_DYNAMIC;
		//mov [esp + 14h], D3DUSAGE_DYNAMIC;
	};

	__asm { // saves the out addr. im not sure if directx funcs are stdcall or not? i think they are tbh
		// no return addr on stack at this moment!
		mov drawTextureHookRegisterSave2, eax;

		mov eax, [esp + 1Ch];
		//mov eax, [eax];
		mov tempTextureAddr1, eax;

		mov eax, drawTextureHookRegisterSave2;
	};


	// look. any other form of jump simply did not cooperate.
	// i hate and despise this
	// why does windows not allow me to just jmp [0x1234] or jmp 0x1234??? why even when i declare it as a variable does it somehow create inoperable asm?
	// how many bugs have been caused by this
	__asm {
		_emit 0xFF;
		//_emit 0x25; // JMP 
		_emit 0x15; // CALL

		_emit 0xF4;
		_emit 0xB3;
		_emit 0x51;
		_emit 0x00;
	};

	PUSH_ALL;
	__asm {
		call MASMSucks;
		//call messUpTexture;
	};
	POP_ALL;

	__asm {
		push drawTextureHookRegisterSave1;
		ret;
	}
}

__declspec(naked) void directXHook() {
	// should this even be used?

	__asm {
		mov dwDevice, ebx;
	};

	//PUSH_ALL;
	//__asm {
	//	call miscDirectX;
	//};
	//POP_ALL;

	__asm {
		pop ebx;
		ret 4;
	};
}

DWORD actualFunc = 0x004be290;
DWORD primHookRet;

DWORD primHookESI;
void unknownPrimHookTests() {
	
	//log("\t\tPRIMDRAW actually called with ESI: %08X", primHookESI);
}

DWORD callsLeadToDrawPrim_esi;
std::vector<DWORD> linkedListPointers;
void unknownPrimHookTests2() {

	//log("callWhichLeadsToPrimDraw1 called!!!");

	//return;

	BYTE unknownSwitch;

	unsigned validCount = 0;
	
	linkedListPointers.clear();

	while (true) {

		if (callsLeadToDrawPrim_esi == 0) {
			//log("\tESI 0");
			break;
		}

		unknownSwitch = *(BYTE*)(callsLeadToDrawPrim_esi + 4);

		//log("\tESI: %08X SWITCH: %02X", callsLeadToDrawPrim_esi, unknownSwitch);

		if (unknownSwitch) {
			linkedListPointers.push_back(callsLeadToDrawPrim_esi);
		}
		
		callsLeadToDrawPrim_esi = *(DWORD*)callsLeadToDrawPrim_esi;

	}

	// the linked list seems not preserve these modifications. is it regenerated on every frame? seems so

	//if (linkedListPointers.size() > 6) {
	if(TESTVAR2 > TESTVAR1 && (TESTVAR1 < linkedListPointers.size()) && TESTVAR2 < linkedListPointers.size()) {

		*(DWORD*)linkedListPointers[TESTVAR1] = linkedListPointers[TESTVAR2];
		log("trigging a weird swap %08X", linkedListPointers[TESTVAR1]);
	}

	log("state: %d %d %d", TESTVAR1, TESTVAR2, linkedListPointers.size());



	//log("call done");
}

DWORD callsLeadToDrawPrim_ret;
DWORD callsLeadToDrawPrim_actual = 0x004be0e0;
__declspec(naked) void callsLeadToDrawPrim() {
	__asm {
		pop callsLeadToDrawPrim_ret;
		mov callsLeadToDrawPrim_esi, esi;
	}

	PUSH_ALL;
	unknownPrimHookTests2();
	POP_ALL;

	__asm {
		call[callsLeadToDrawPrim_actual];
	}

	__asm {
		push callsLeadToDrawPrim_ret;
		ret;
	}
}

__declspec(naked) void callsDrawIndexedPrimHook() {
	__asm {
		pop primHookRet;
		mov primHookESI, esi;
	};

	PUSH_ALL;
	unknownPrimHookTests();
	POP_ALL;

	__asm {
		call[actualFunc];
	}

	__asm {
		push primHookRet;
		ret;
	};
}

DWORD linkedListAppend_EDI;
void linkedListAppendCallback() {

	unsigned length = 0;

	while (true) {

		//log("EDI: %08X", linkedListAppend_EDI);

		if (linkedListAppend_EDI == 0) {
			break;
		}

		linkedListAppend_EDI = *(DWORD*)linkedListAppend_EDI;
		length++;
	}

	// this length is 1600, aka 0x640, which is a number i have seen before! i cannot remember where tho
	log("appendLength is %d", length);
}

__declspec(naked) void _naked_linkedListAppendCallback() {
	__asm {
		mov linkedListAppend_EDI, edi;
	};
	PUSH_ALL;
	linkedListAppendCallback();
	POP_ALL;
	__asm {
		ret;
	}
}

DWORD _tempRand;
DWORD _renderEDI;
void unknownRenderCallback() {
	unsigned length = 0;

	_tempRand = rand();

	while (true) {

		if (_renderEDI == 0) {
			break;
		}

		_renderEDI = *(DWORD*)_renderEDI;
		length++;
	}
	//log("rendercallback length is %d", length);
}


__declspec(naked) void _naked_renderCallback() {

	/// this func ALMOST DEFINITELY is the one that we would need to modify! this appends to the linked list!!
	__asm {

		test _tempRand, 1;
		JNE _IDEK;

		mov eax, dword ptr[edi];
		mov dword ptr[esi], eax;

		mov dword ptr[edi], esi;

	_IDEK:

		//mov _renderEDI, edi;
		mov _renderEDI, 005550a8h;

		mov eax, 1;
		pop edi;
	};
	PUSH_ALL;
	unknownRenderCallback();
	POP_ALL;
	__asm {
		ret;
	}
}

// init funcs

void initFrameDoneCallback() {

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

void initCasterMods() {

	// if caster ever updates, these offsets will most likely(basically definitely) need to be changed!

	// 0x665f17d5 is total meter gain 
	patchByte(dwCasterBaseAddress + 0x665f17d5, '\0');

	// 0x665f17c1 is meter gain 
	patchByte(dwCasterBaseAddress + 0x665f17c1, '\0');

	// 0x665f17cd is the actual string passed to printf
	patchByte(dwCasterBaseAddress + 0x665f17cd, '\0');

	// it would be better to just patch this func out
}

void initAttackMeterDisplay() {

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

void initMeterGainHook() {
	void* funcAddress = (void*)0x00476ce0;
	// backup
	patchMemcpy(arrMeterGainHookOrig, funcAddress, 10);
	// new bytes
	patchFunction(funcAddress, meterGainHook);
	// ret
	patchByte(((BYTE*)funcAddress) + 5, 0xC3);
	// backup modded bytes 
	patchMemcpy(arrMeterGainHookMod, funcAddress, 10);
}

void initBattleResetCallback() {
	// this func rets early, this jump prevents that 
	void* patchAddr = (void*)0x004234b9;
	patchJump(patchAddr, 0x004234e1);

	void* funcAddr = (void*)0x004234e4;
	patchFunction(funcAddr, battleResetCallback);

	// this patch is a bit funky, since we need to have ret dec the stack pointer
	BYTE tempCode[3] = {0xc2, 0x04, 0x00};
	patchMemcpy(((BYTE*)funcAddr) + 5, tempCode, 3);
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

void initDrawTextureHook() {
	
	//patchJump(0x004de5ae, drawTextureHook3);
	
	// todo, hook D3DXCreateTextureFromFileInMemoryEx

}

void initDrawCharacterTextureCallback() {

	// skip early ret
	patchJump(0x00407761, 0x00407806);

	// actual callback
	patchJump(0x00407806, nakedDrawCharacterTextureCallback);

}

void initDirectX() {

	/*
	caster hooks dx
	ideally, i would,,,, hook those hooks if those hooks were hooked, or i would make my own hooks if not
	im just going to hook casters hooks, and not hook this if caster isnt running
	DX9_Endscene

	goal of this is to find the texture pointer, lock it, modify it, unlock it, and then continue
	arbitrary drawing as well

	todo, actually learn directx backend
	does it(im assuming it does) move all textures to vram, and the sprite index is what indexes them? meaning that all
	sprite moving and loading would be done either on boot(unlikely) after char select when loading into a game(likely)?

	*/

	if (!bCasterInit) {
		return;
	}

	HMODULE hD3dll = GetModuleHandle(L"d3d9.dll");
	DWORD d3dBase = (DWORD)hD3dll;
	if (hD3dll == NULL) {
		return;
	}

	// todo, move this blocking to its own thread 
	DWORD* device = (DWORD*)0x0076e7d4;
	while (*device == NULL) {
		Sleep(16 * 10);
	}

	log("endScene at %08X", dwCasterBaseAddress + addrEndScene);
	log("endScene patch at %08X", dwCasterBaseAddress + addrEndScenePatch);

	patchJump(dwCasterBaseAddress + addrEndScenePatch, directXHook);
}

void initIdk() {
	//patchJump(0x004be524, omfgResetShader);
	patchFunction(0x004c03df, callsDrawIndexedPrimHook);

	patchFunction(0x004c03b0, callsLeadToDrawPrim);

	//patchJump(0x00414e57, _naked_linkedListAppendCallback);
	//patchJump(0x004c0279, _naked_renderCallback);
	patchJump(0x004c0273, _naked_renderCallback);

}

void threadFunc() 
{
	srand(time(NULL));

	log("dll injected, threadfunc running");

	

	// make sure that caster has time to hook at the start
	//Sleep(16 * 5);
	//Sleep(3000);

	// todo, put something here to prevent mult injection

	// check if we are running with caster
	HMODULE hModule = GetModuleHandleA("hook.dll");

	if (hModule != NULL) { // we are running with caster
		bCasterInit = true;
		// 0x66380000 is the base address when looking through caster in decomp
		dwCasterBaseAddress = ((DWORD)hModule) - 0x66380000;
		dwCasterFullBaseAddress = (DWORD)hModule;
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

	initDirectX();

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

	//log("dllmain");

	(void)hinstDLL;
	(void)lpReserved;

	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadFunc, 0, 0, 0);
	}

	return TRUE;
}

#pragma pop_macro("optimize")
