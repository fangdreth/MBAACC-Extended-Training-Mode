

#include "dllmain.h"
#include "FrameBar.h"
#include "SaveState.h"
#include "ReplayManager.h"
#include "TASManager.h"
#include "FancyMenu.h"
#include "TrainingMenu.h"

#pragma push_macro("optimize")
#pragma optimize("t", on) 

typedef DWORD ADDRESS;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef uint32_t uint;

void enemyReversal();
void frameStartCallback();
void dualInputDisplayReset();
void doFastReversePenalty();
void drawFancyMenu();
void rollFancyInputDisplay(int n);
void loadCustomShader();

TASManager TASManagerObj;

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

DWORD addrEndScene = 0x663fb900;
DWORD addrEndScenePatch = 0x663fb996;

bool bCasterInit = false;
ADDRESS dwCasterBaseAddress = 0;

DWORD dwDevice = 0; // MASM is horrid when it comes to writing pointers vs value of pointer bc it has type checking. thats why this cant be a pointer
IDirect3DDevice9* device = NULL;

bool disableFPSLimit = false;

const ADDRESS INPUTDISPLAYTOGGLE = (dwBaseAddress + 0x001585f8);

DWORD tempRegister1;
DWORD tempRegister2;
DWORD tempRegister3;
DWORD tempRegister4;

DWORD __frameDoneCount = 0;
DWORD unpausedFrameCount = 0;

int nFreezeKey;
int nFrameStepKey;
int nHitboxesDisplayKey;
int nFrameDataDisplayKey;
int nHighlightsOnKey;
int nSaveStateKey;

DWORD showCSS = 1;
int showDebugMenu = 0;
bool bFreeze = false;
bool bSlow = false;
bool bFrameDataDisplay = false;
bool bHitboxesDisplay = false;
bool bHighlightsOn = true;
DWORD shouldDrawBackground = 1;
DWORD shouldDrawHud = 1;
DWORD shouldDrawGroundLine = 0;
DWORD backgroundColor = 0xFFFFFFFF;
DWORD shouldDrawShadow = 0;
DWORD fastReversePenalty = 0;
DWORD shouldDrawMeter = 1;
DWORD frameBarY = 400;
bool bDoResetBars = true;

uint8_t nRNGMode = RNG_OFF;
uint8_t nRNGRate = RNG_EVERY_FRAME;
uint32_t nCustomSeed = 0;
uint32_t nCustomRN = 0;

DWORD _naked_newPauseCallback2_IsPaused = 0;

std::array<uint8_t, 4> arrDefaultHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrIdleHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrBlockingHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrHitHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrArmorHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrThrowProtectionHighlightSetting({ 255, 255, 255, 0 });

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
static KeyState oReversalKey;
static KeyState oSlowKey;
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
	oReversalKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedReversalKey));
	oSlowKey.setKey(*(uint8_t*)(dwBaseAddress + adSharedSlowKey));
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

// the patch func being templated causes problems when calling from asm
void __stdcall asmPatchMemcpy(void* dest, void* source, DWORD n)
{
	DWORD oldProtect;
	VirtualProtect(dest, n, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(dest, source, n);
	VirtualProtect(dest, n, oldProtect, NULL);
}

// -----

#include "DirectX.h"
//#include "RendererModifications.h"


//#define ENABLEFILELOG 1

bool logFileInit = false;
void __stdcall initLogFile() {
#ifdef ENABLEFILELOG

	FILE* file = fopen("Extended-Training-Mode-DLL.log", "w");
	if (file == NULL) {
		log("opening log file failed?");
		return;
	}
	fclose(file);


	logFileInit = true;
#endif
}

void __stdcall writeLog(const char* msg) {
	if (!logFileInit) {
		return;
	}

	FILE* file = fopen("Extended-Training-Mode-DLL.log", "a");
	fprintf(file, "%s\n", msg); 
	fclose(file);

}

bool __stdcall isAddrValid(DWORD addr) {
	__try {
		volatile BYTE test = *(BYTE*)addr;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

std::map<DWORD, std::pair<DWORD, const char*>> timeMeltyStates;

DWORD _naked_timeMeltyCall_ret_temp;
std::vector<DWORD> _naked_timeMeltyCall_ret; // this is a stack to allow for recursive calls
std::vector<long long> timeMeltyLoggerStart; // stack of starting times;
DWORD timeMeltyCall_Func;

void timeMeltyCallLogger() {

	_naked_timeMeltyCall_ret.push_back(_naked_timeMeltyCall_ret_temp);

	DWORD retVal = _naked_timeMeltyCall_ret.back();
	timeMeltyCall_Func = 0;
	if (timeMeltyStates.contains(retVal - 5)) {
		timeMeltyCall_Func = timeMeltyStates[retVal - 5].first;
		const char* funcName = timeMeltyStates[retVal - 5].second;
		log(".%.*stiming %s: %08X", timeMeltyLoggerStart.size(), "\t\t\t\t\t\t\t\t\t\t", funcName, retVal - 5);
		timeMeltyLoggerStart.push_back(getMicroSec());
	}
}

void timeMeltyCallLoggerDone() {

	long long dur = getMicroSec() - timeMeltyLoggerStart.back();
	DWORD retVal = _naked_timeMeltyCall_ret.back();
	_naked_timeMeltyCall_ret_temp = retVal;

	_naked_timeMeltyCall_ret.pop_back();
	timeMeltyLoggerStart.pop_back();

	if (timeMeltyCall_Func == 0) {
		return;
	}

	double time = ((double)1000000000.0) / (double)dur;

	const char* funcName = "???";
	if (timeMeltyStates.contains(retVal - 5)) {
		funcName = timeMeltyStates[retVal - 5].second;
	}

	//log("%s took %lf", funcName, time);
	log(".%.*s%s took %lld", timeMeltyLoggerStart.size(), "\t\t\t\t\t\t\t\t\t\t", funcName, dur);

}

__declspec(naked) void _naked_timeMeltyCall() {
	__asm {
		pop _naked_timeMeltyCall_ret_temp;
	}

	PUSH_ALL;
	timeMeltyCallLogger();
	POP_ALL;

	__asm {
		cmp timeMeltyCall_Func, 0;
		JE _SKIP;

		call[timeMeltyCall_Func]; // PLEASE MASM DONT BE STUPID
	_SKIP:
	};

	PUSH_ALL;
	timeMeltyCallLoggerDone();
	POP_ALL;

	__asm {
		push _naked_timeMeltyCall_ret_temp;
		ret;
	}
}

void timeMeltyCall(DWORD patchAddr, const char* funcName) {

	// patchaddr is the address of the call, callAddr is the address of the originally called func by said call
	// actually i shouldnt even need callAddr, with math
	// 0x0040e499 0x00024ff2 -> 0x00433490
	// 0x0040e499 + 0x00024ff2 + 5. please note that the call offset is signed
	
	// i need to maintain state of where to go. ill use a map despite that being very stupid
	// can i even,, use my emitcall funcs with variables omfg

	// lets say ill only use this on calls, not jumps.
	// this will be slow and stupid.

	int callOffset = *(int*)(patchAddr + 1);
	DWORD funcAddr = patchAddr + callOffset + 5;
	timeMeltyStates[patchAddr] = { funcAddr, funcName };

	patchCall(patchAddr, _naked_timeMeltyCall);

}

// legacy melty draw funcs. please use the funcs in directx.h instead of these

extern "C" int asmDrawText(int w, int h, int x, int y, const char* text, int alpha, int shade, int layer, void* addr, int spacing, int idek, char* out);

void __stdcall drawText(int x, int y, int w, int h, const char* text, int alpha, int shade, int layer = 0x2cc, void* font = (void*)adFont2)
{
	// text was initially just char*, i made it const, is that going to be ok?
	asmDrawText(w, h, x, y, text, alpha, shade, layer, font, 0, 0, 0);
}

void __stdcall drawText(int x, int y, const char* text, int textSize = 16, ADDRESS font = adFont1)
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

void __stdcall drawTextWithBorder(int x, int y, int w, int h, const char* text)
{
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

void __stdcall drawRect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a, int layer = 0x2cc)
{
	int colVal = (a << 24) | (r << 16) | (g << 8) | (b << 0);
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void __stdcall drawRect(int x, int y, int w, int h, DWORD colVal, int layer = 0x2cc)
{
	asmDrawRect(x, y, w, h, colVal, colVal, colVal, colVal, layer);
}

void __stdcall drawBorder(int x, int y, int w, int h, DWORD ARGB=0x8042e5f4)
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

void __stdcall drawBorderWithHighlight(int x, int y, int w, int h, DWORD ARGB = 0x8042e5f4)
{
	drawBorder(x, y, w, h, ARGB);

	BYTE r = (ARGB & 0x00FF0000) >> 16;
	BYTE g = (ARGB & 0x0000FF00) >> 8;
	BYTE b = (ARGB & 0x000000FF) >> 0;
	
	drawRect(x, y, w, h, r, g, b, 0x38);
}

// hitbox draw funcs

void scaleCords(const float xOrig, const float yOrig, float& x1Cord, float& y1Cord, float& x2Cord, float& y2Cord)
{
	x1Cord = xOrig + (x1Cord - xOrig) * 0.5f;
	y1Cord = yOrig + (y1Cord - yOrig) * 0.5f;
	x2Cord = xOrig + (x2Cord - xOrig) * 0.5f;
	y2Cord = yOrig + (y2Cord - yOrig) * 0.5f;
}

bool drawObject(DWORD objAddr, bool isProjectile, int playerIndex)
{
	EffectData* effect = ((EffectData*)objAddr);

	//int xPos = *(DWORD*)(objAddr + 0x108);
	//int yPos = *(DWORD*)(objAddr + 0x10C);
	//bool facingLeft = *(BYTE*)(objAddr + 0x314);

	int xPos = effect->xPos;
	int yPos = effect->yPos;
	bool facingLeft = effect->facingLeft;
	
	AnimationData* animationData = effect->animationDataPtr;
	//DWORD objFramePtr = (DWORD)animationData;

	if (animationData == NULL) {
		return false;
	}

	// -----

	float isRight = 1;
	if (facingLeft) {
		isRight = -1;
	}

	BoxObjects* res = new BoxObjects; // res is dealloced during the draw call! so dont worry about it.

	float windowWidth = *(uint32_t*)0x0054d048;
	float windowHeight = *(uint32_t*)0x0054d04c;

	int cameraX = *(int*)(0x0055dec4);
	int cameraY = *(int*)(0x0055dec8);
	float cameraZoom = *(float*)(0x0054eb70);

	// what is that 49.
	float xCamTemp = ((((float)(xPos - cameraX) * cameraZoom) / 128.0f) * (windowWidth / 640.0f) + windowWidth / 2.0f);
	//float xCamTemp = ((((float)(xPos - cameraX) * cameraZoom) / 128.0f - 49.0f) * (windowWidth / 640.0f) + windowWidth / 2.0f);
	
	float yCamTemp = ((((float)(yPos - cameraY) * cameraZoom) / 128.0f - 49.0f) * (windowHeight / 480.0f) + windowHeight);
	
	xCamTemp = floor(xCamTemp);
	yCamTemp = floor(yCamTemp);

	float tempFloat;

	float x1Cord, x2Cord, y1Cord, y2Cord;

	uint32_t drawColor;
	BoxType boxType;// = BoxType::None;

	// -----

	// origin?

	drawColor = 0xFF42E5F4;
	boxType = BoxType::Origin;

	x1Cord = ((float)xCamTemp - (windowWidth / 640.0f) * cameraZoom * 5.0f);
	x2Cord = ((windowWidth / 640.0f) * cameraZoom * 5.0f + (float)xCamTemp);
	y1Cord = ((float)yCamTemp - (windowWidth / 640.0f) * cameraZoom * 5.0f);
	y2Cord = yCamTemp;

	//x1Cord = ((float)x1Cord / windowWidth);
	//x2Cord = ((float)x2Cord / windowWidth);
	//y1Cord = ((float)y1Cord / windowHeight);
	//y2Cord = ((float)y2Cord / windowHeight);

	x1Cord = ((float)x1Cord * (640.0f / windowWidth));
	x2Cord = ((float)x2Cord * (640.0f / windowWidth));
	y1Cord = ((float)y1Cord * (480.0f / windowHeight));
	y2Cord = ((float)y2Cord * (480.0f / windowHeight));


	if(!isProjectile) {
		//drawBorder((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
		//DrawHitbox(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord), BoxType::Origin, playerIndex);
		//(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
		
		(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
		/*
		if (facingLeft) {
			(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x2Cord, y1Cord, (x1Cord - x2Cord), (y2Cord - y1Cord)));
		} else {
			(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
		}
		*/
	}

	// current vibes say that the origin is in the bottom center of the above rectangle, needs more non vibe based confirmation though
	float xOrig = x1Cord + ((x2Cord - x1Cord) / 2.0f);
	float yOrig = y2Cord;

	// lots of stuff here seems to interact with 0x330. this is off of 0x320, is this an issue?
	// should have made a box/poit class
	DWORD animDataPtr = *(DWORD*)(objAddr + 0x320);
	bool isPat = *(BYTE*)(animDataPtr + 0x0);
	
	// non hitboxes
	//if (*(DWORD*)(objFramePtr + 0x4C) != 0) {
	if(animationData->nonHitboxData != NULL) {
		unsigned highestIndex = animationData->highestNonHitboxIndex;
		for (unsigned index = 0; index < highestIndex; index++) {

			/*if (*(int*)(*(int*)(objFramePtr + 0x4c) + index * 4) == 0) {
				continue;
			}*/
			if (animationData->nonHitboxData->boxes[index] == NULL) {
				continue;
			}

			switch (index) {
				case 0x0:
					drawColor = 0xFFD0D0D0;
					boxType = BoxType::Collision;
					break;
				case 0x9: // what is this?
					drawColor = 0xFFFFFFFF;
					boxType = BoxType::Shield;
					break;
				case 0xA:
					//drawColor = 0xFFFF00FF;
					drawColor = 0xFFF54298;
					boxType = BoxType::Shield;
					break;
				case 0xB:
					drawColor = 0xFFFFFF00;
					boxType = BoxType::Clash; 
					break;
				default:
					if (index < 0xC) {
						drawColor = 0xFF00FF00;
						boxType = BoxType::Hurtbox;
					} else {
						drawColor = 0xFF0000FF;
						boxType = BoxType::Blue;
					}
					break;
			}

			short x1 = animationData->nonHitboxData->boxes[index]->x1;
			short x2 = animationData->nonHitboxData->boxes[index]->x2 * 2;
			short y1 = animationData->nonHitboxData->boxes[index]->y1 * 2;
			short y2 = animationData->nonHitboxData->boxes[index]->y2 * 2;

			tempFloat = (float)isRight * (windowWidth / 640.0f) * cameraZoom;
			x1Cord = ((float)x1) * (tempFloat + tempFloat) + (float)xCamTemp;
			x2Cord = ((float)x2) * (float)isRight * (windowWidth / 640.0f) * cameraZoom + (float)xCamTemp;

			y1Cord = ((float)y1) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;
			y2Cord = ((float)y2) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;

			x1Cord = ((float)x1Cord * (640.0f / windowWidth));
			x2Cord = ((float)x2Cord * (640.0f / windowWidth));
			y1Cord = ((float)y1Cord * (480.0f / windowHeight));
			y2Cord = ((float)y2Cord * (480.0f / windowHeight));

			//x1Cord = ((float)x1Cord / windowWidth);
			//x2Cord = ((float)x2Cord / windowWidth);
			//y1Cord = ((float)y1Cord / windowHeight);
			//y2Cord = ((float)y2Cord / windowHeight);

			if (isPat) {
				scaleCords(xOrig, yOrig, x1Cord, y1Cord, x2Cord, y2Cord);
			}
			
			//drawBorderWithHighlight((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
			//DrawHitbox(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord), boxType, playerIndex);

			if (facingLeft) {
				(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x2Cord, y1Cord, (x1Cord - x2Cord), (y2Cord - y1Cord)));
			} else {
				(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
			}
		}
	}

	// hitboxes
	if (animationData->hitboxData != NULL) {
		drawColor = 0xFFFF0000;
		boxType = BoxType::Hitbox;
		unsigned highestHitboxIndex = animationData->highestHitboxIndex;
		for (unsigned index = 0; index < highestHitboxIndex; index++) {
			
			if (animationData->hitboxData->boxes[index] == NULL) {
				continue;
			}

			short x1 = animationData->hitboxData->boxes[index]->x1;
			short x2 = animationData->hitboxData->boxes[index]->x2 * 2;
			short y1 = animationData->hitboxData->boxes[index]->y1 * 2;
			short y2 = animationData->hitboxData->boxes[index]->y2 * 2;

			tempFloat = (float)isRight * (windowWidth / 640.0f) * cameraZoom;
			x1Cord = ((float)x1) * (tempFloat + tempFloat) + (float)xCamTemp;
			x2Cord = ((float)x2) * (float)isRight * (windowWidth / 640.0f) * cameraZoom + (float)xCamTemp;
	
			y1Cord = ((float)y1) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;
			y2Cord = ((float)y2) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;

			//log("%3d %3d %3d %3d", x1, y1, x2, y2);
			//log("%6.3f %6.3f %6.3f %6.3f", x1Cord, y1Cord, x2Cord, y2Cord);

			//x1Cord = ((float)x1Cord / windowWidth);
			//x2Cord = ((float)x2Cord / windowWidth);
			//y1Cord = ((float)y1Cord / windowHeight);
			//y2Cord = ((float)y2Cord / windowHeight);

			x1Cord = ((float)x1Cord * (640.0f / windowWidth));
			x2Cord = ((float)x2Cord * (640.0f / windowWidth));
			y1Cord = ((float)y1Cord * (480.0f / windowHeight));
			y2Cord = ((float)y2Cord * (480.0f / windowHeight));

			if (isPat) {
				scaleCords(xOrig, yOrig, x1Cord, y1Cord, x2Cord, y2Cord);
			}

			//drawBorderWithHighlight((int)x1Cord, (int)y1Cord, (int)(x2Cord - x1Cord), (int)(y2Cord - y1Cord), drawColor);
			//DrawHitbox(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord), boxType, playerIndex);
			//(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
			if (facingLeft) {
				(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x2Cord, y1Cord, (x1Cord - x2Cord), (y2Cord - y1Cord)));
			} else {
				(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
			}
		}
	}

	if (!isProjectile) {
		char cCondition1Type = 0;
		char cCondition2Type = 0;
		if (*(char*)(animDataPtr + adAnimationData_ConditionCount) > 0)
		{
			DWORD dwPointer = *(DWORD*)(animDataPtr + adAnimationData_ConditionsPointer);
			if (dwPointer != 0)
			{
				DWORD dwC1Pointer = *(DWORD*)(dwPointer + adConditions_Condition1Pointer);
				if (dwC1Pointer != 0)
				{
					cCondition1Type = *(char*)(dwC1Pointer + adCondition_Type);
				}
				if (*(char*)(animDataPtr + adAnimationData_ConditionCount) > 1)
				{
					DWORD dwC2Pointer = *(DWORD*)(dwPointer + adConditions_Condition2Pointer);
					if (dwC2Pointer != 0)
					{
						cCondition2Type = *(char*)(dwC2Pointer + adCondition_Type);
					}
				}
			}
		}
		if (cCondition1Type == 52 || cCondition2Type == 52) //Throw boxes
		{
			if (*(char*)(*(DWORD*)(animDataPtr + adAnimationData_StateDataPointer) + adStateData_Stance) == 1)
			{
	
				boxType = BoxType::Throw;

				short x1 = 0;
				short x2 = 120;
				short y1 = -120;
				short y2 = 60;

				tempFloat = (float)isRight * (windowWidth / 640.0f) * cameraZoom;
				x1Cord = ((float)x1) * (tempFloat + tempFloat) + (float)xCamTemp;
				x2Cord = ((float)x2) * (float)isRight * (windowWidth / 640.0f) * cameraZoom + (float)xCamTemp;

				y1Cord = ((float)y1) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;
				y2Cord = ((float)y2) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;

				x1Cord = ((float)x1Cord * (640.0f / windowWidth));
				x2Cord = ((float)x2Cord * (640.0f / windowWidth));
				y1Cord = ((float)y1Cord * (480.0f / windowHeight));
				y2Cord = ((float)y2Cord * (480.0f / windowHeight));

				if (facingLeft) {
					(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x2Cord, y1Cord, (x1Cord - x2Cord), (y2Cord - y1Cord)));
				}
				else {
					(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
				}
			}
			else
			{
				drawColor = 0xFFC00080;
				boxType = BoxType::Throw;

				short x1 = 0;
				short x2 = *(short*)(*(DWORD*)(objAddr + 0x33C) + 0x54) != 0 ? *(short*)(*(DWORD*)(objAddr + 0x33C) + 0x54) * 2 : 96;
				short y1 = -120;
				short y2 = 0;

				tempFloat = (float)isRight * (windowWidth / 640.0f) * cameraZoom;
				x1Cord = ((float)x1) * (tempFloat + tempFloat) + (float)xCamTemp;
				x2Cord = ((float)x2) * (float)isRight * (windowWidth / 640.0f) * cameraZoom + (float)xCamTemp;

				y1Cord = ((float)y1) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;
				y2Cord = ((float)y2) * (windowHeight / 480.0f) * cameraZoom + (float)yCamTemp;

				x1Cord = ((float)x1Cord * (640.0f / windowWidth));
				x2Cord = ((float)x2Cord * (640.0f / windowWidth));
				y1Cord = ((float)y1Cord * (480.0f / windowHeight));
				y2Cord = ((float)y2Cord * (480.0f / windowHeight));

				if (facingLeft) {
					(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x2Cord, y1Cord, (x1Cord - x2Cord), (y2Cord - y1Cord)));
				}
				else {
					(*res)[static_cast<int>(boxType)].emplace_back(BoxData(x1Cord, y1Cord, (x2Cord - x1Cord), (y2Cord - y1Cord)));
				}
			}
		}
	}
	//log("-----");

	DrawHitboxes(res);

	return true;
}

void drawColorGuide()
{
	RectDraw(0, 8, 640, 50, 0xFF000000);

	RectDraw(10, 10, 7, 10, FB_INACTIONABLE);
	TextDraw(17, 10, 10, 0xFFFFFFFF, "INACTIONABLE");
	RectDraw(120, 10, 7, 10, FB_JUMP);
	TextDraw(127, 10, 10, 0xFFFFFFFF, "JUMP STARTUP");
	RectDraw(230, 10, 7, 10, FB_HITSTUN);
	TextDraw(237, 10, 10, 0xFFFFFFFF, "HITSTUN");
	RectDraw(300, 10, 7, 10, FB_BLOCKSTUN);
	TextDraw(307, 10, 10, 0xFFFFFFFF, "BLOCKSTUN");
	RectDraw(390, 10, 7, 10, FB_ACTIVE);
	TextDraw(397, 10, 10, 0xFFFFFFFF, "ACTIVE FRAMES");
	RectDraw(500, 10, 7, 10, FB_ACTIONABLE);
	TextDraw(507, 10, 10, 0xFFFFFFFF, "FULLY ACTIONABLE");
	RectDraw(10, 22, 7, 10, FB_ADVANTAGE);
	TextDraw(17, 22, 10, 0xFFFFFFFF, "FRAME ADVANTAGE");
	RectDraw(140, 22, 7, 10, FB_NEUTRAL);
	TextDraw(147, 22, 10, 0xFFFFFFFF, "NEUTRAL FRAME");
	RectDraw(260, 22, 7, 10, FB_FREEZE);
	TextDraw(267, 22, 10, 0xFFFFFFFF, "SCREEN FREEZE");
	RectDraw(370, 22, 7, 10, FB_FREEZE_ACTIVE);
	TextDraw(377, 22, 10, 0xFFFFFFFF, "ACTIVE DURING FREEZE");
	RectDraw(530, 22, 7, 10, FB_THROWN);
	TextDraw(537, 22, 10, 0xFFFFFFFF, "BEING THROWN");
	RectDraw(10, 34, 7, 10, FB_HITSTOP);
	TextDraw(17, 34, 10, 0xFFFFFFFF, "HITSTOP");
	RectDraw(80, 34, 7, 10, FB_SHIELD);
	TextDraw(87, 34, 10, 0xFFFFFFFF, "SHIELD");
	RectDraw(140, 34, 7, 10, FB_THROW_ACTIVE);
	TextDraw(147, 34, 10, 0xFFFFFFFF, "THROW ACTIVE FRAME");

	RectDraw(10, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(14, 45, 3, 10, FB_CLASH);
	TextDraw(17, 45, 10, 0xFFFFFFFF, "CLASH");
	RectDraw(70, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(74, 45, 3, 10, FB_INVULN);
	TextDraw(77, 45, 10, 0xFFFFFFFF, "INVULN");
	RectDraw(130, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(134, 45, 3, 10, FB_COUNTER);
	TextDraw(137, 45, 10, 0xFFFFFFFF, "COUNTER");
	RectDraw(200, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(200, 44, 7, 2, FB_JUMP);
	TextDraw(207, 45, 10, 0xFFFFFFFF, "AIRBORNE");
	RectDraw(280, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(280, 54, 7, 2, FB_ACTIVE);
	TextDraw(287, 45, 10, 0xFFFFFFFF, "ACTIVE PROJECTILE");
	RectDraw(450, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(450, 54, 7, 2, FB_ASSIST_ACTIVE);
	TextDraw(457, 45, 10, 0xFFFFFFFF, "ACTIVE ASSIST");
}

int getPatternFromInput(PlayerData* PD, const char input[20])
{
	char buffer[20];
	char inputCopy[20];
	char readableInput[20] = {0};
	for (int i = 0; i < PD->cmdDataPtr->cmdFileDataPtr->maxFilePriority; i++)
	{
		if (PD->cmdDataPtr->cmdFileDataPtr->cmdPtrArray->commands[i])
		{
			int length = 0;
			snprintf(inputCopy, 20, "%s", PD->cmdDataPtr->cmdFileDataPtr->cmdPtrArray->commands[i]->input);
			for (int j = 0; j < 20; j++)
			{
				if (inputCopy[j] == '\xFF')
				{
					readableInput[j] = 0;
					length = j;
					break;
				}
				else if (inputCopy[j] >= 0x0 && inputCopy[j] <= 0x9)
				{
					readableInput[j] = inputCopy[j] + 0x30;
				}
				else
				{
					readableInput[j] = inputCopy[j];
				}
			}
			
			bool isMatch = true;
			for (int k = 0; k < length; k++)
			{
				if (readableInput[k] != input[k]) isMatch = false;
			}
			if (isMatch)
			{
				return PD->cmdDataPtr->cmdFileDataPtr->cmdPtrArray->commands[i]->pattern;
			}
		}
	}
	return -1;
}

//In-game frame bar
void drawFrameBar(int nYOverride = -1)
{
	if (!safeWrite())
		return;

	frameBarY = *(int*)(adMBAABase + adSharedFrameBarY);
	if (nYOverride != -1)
	{
		frameBarY = nYOverride;
	}

	FrameBar(P1, P2, P3, P4);

	if (!bFrameDataDisplay)
		return;

	int nBarDrawCounter = 0;

	nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);
	short sAdjustedScroll = min(min(nBarCounter - DISPLAY_RANGE, BAR_MEMORY_SIZE - DISPLAY_RANGE), nBarScrolling);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - DISPLAY_RANGE - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= DISPLAY_RANGE)
	{
		nForStart = 0;
		nForEnd = nBarCounter;
	}

	RectDraw(18, frameBarY, 603, 27, 0x99000000); //Background

	RectDraw(17, frameBarY - 2, 605, 2, 0xFFFFFFFF);
	RectDraw(16, frameBarY - 1, 2, 29, 0xFFFFFFFF);
	RectDraw(17, frameBarY + 27, 605, 2, 0xFFFFFFFF);
	RectDraw(621, frameBarY - 1, 2, 29, 0xFFFFFFFF);

	int j = 0;
	for (int i = nForStart; i < nForEnd; i++)
	{
		j = i < 0 ? i + BAR_MEMORY_SIZE : i;

		RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 2, 7, 10, Main1->dwColorBar1[j][0]);
		if (Main1->dwColorBar1[j][1] != 0)
			RectDraw(20 + 8 * nBarDrawCounter + 4, frameBarY + 2, 3, 10, Main1->dwColorBar1[j][1]);
		RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 15, 7, 10, Main2->dwColorBar1[j][0]);
		if (Main2->dwColorBar1[j][1] != 0)
			RectDraw(20 + 8 * nBarDrawCounter + 4, frameBarY + 15, 3, 10, Main2->dwColorBar1[j][1]);
		RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 1, 7, 2, Main1->dwColorBar2[j][0]);
		if (Main1->dwColorBar2[j][1] != 0)
			RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 11, 7, 2, Main1->dwColorBar2[j][1]);
		RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 14, 7, 2, Main2->dwColorBar2[j][0]);
		if (Main2->dwColorBar2[j][1] != 0)
			RectDraw(20 + 8 * nBarDrawCounter, frameBarY + 24, 7, 2, Main2->dwColorBar2[j][1]);

		static char buffer[256];

		if (Main1->nNumBar[j][0] >= 0)
		{
			int nLength = floor(log10(Main1->nNumBar[j][0]));
			snprintf(buffer, 256, "%i", Main1->nNumBar[j][0]);
			TextDraw(19 + 8 * nBarDrawCounter - 8 * nLength, frameBarY + 2, 10, 0xFFFFFFFF, buffer);
		}

		if (Main2->nNumBar[j][0] >= 0)
		{
			int nLength = floor(log10(Main2->nNumBar[j][0]));
			snprintf(buffer, 256, "%i", Main2->nNumBar[j][0]);
			TextDraw(19 + 8 * nBarDrawCounter - 8 * nLength, frameBarY + 15, 10, 0xFFFFFFFF, buffer);
		}
		nBarDrawCounter++;
	}
	static char buffer[256];
	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", Main1->nFirstActive % 1000, Main1->nInactionableMemory % 1000, nPlayerAdvantage % 1000);
	TextDraw(20, frameBarY - 12, 10, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", Main2->nFirstActive % 1000, Main2->nInactionableMemory % 1000, -nPlayerAdvantage % 1000);
	TextDraw(20, frameBarY + 29, 10, 0xFFFFFFFF, buffer);

	//DEBUG PRINTS
	//snprintf(buffer, 256, "%i", nREVERSAL_TYPE);
	//TextDraw(20, frameBarY - 23, 10, 0xFFFFFFFF, buffer);

}

void drawSimpleMeter()
{
	if (!shouldDrawHud) return;

	float fScroll = *(float*)(dwBaseAddress + adTrainingResetScroll);
	int nResetOffset = 0;
	if (fScroll > 0)
		nResetOffset = 320.0f * fScroll;

	int Y = 19;

	DWORD P1Base = adP1SubBase + (*(uint8_t*)(adMBAABase + adP1SubBase + adTagFlag) * dwPlayerStructSize * 2);
	int nP1Meter = *(int*)(adMBAABase + P1Base + adMagicCircuit);
	int nP1MeterTime = *(int*)(adMBAABase + P1Base + adMagicCircuitTime);
	uint8_t nP1MeterMode = *(uint8_t*)(adMBAABase + P1Base + adMagicCircuitMode);
	short sP1CircuitBreakTimer = *(short*)(adMBAABase + P1Base + adCircuitBreakTimer);
	short sP1CircuitBreakTotal = *(short*)(adMBAABase + P1Base + adCircuitBreakTotal);
	short sP1CircuitBreakFlag = *(short*)(adMBAABase + P1Base + adBreakOrPenalty);
	uint8_t nP1Moon = *(uint8_t*)(adMBAABase + dwP1CharMoon);

	DWORD P2Base = adP2SubBase + (*(uint8_t*)(adMBAABase + adP2SubBase + adTagFlag) * dwPlayerStructSize * 2);
	int nP2Meter = *(int*)(adMBAABase + P2Base + adMagicCircuit);
	int nP2MeterTime = *(int*)(adMBAABase + P2Base + adMagicCircuitTime);
	uint8_t nP2MeterMode = *(uint8_t*)(adMBAABase + P2Base + adMagicCircuitMode);
	short sP2CircuitBreakTimer = *(short*)(adMBAABase + P2Base + adCircuitBreakTimer);
	short sP2CircuitBreakTotal = *(short*)(adMBAABase + P2Base + adCircuitBreakTotal);
	short sP2CircuitBreakFlag = *(short*)(adMBAABase + P2Base + adBreakOrPenalty);
	uint8_t nP2Moon = *(uint8_t*)(adMBAABase + dwP2CharMoon);

	static char buffer[8];

	RectDraw(60 - nResetOffset, Y, 214, 12, 0x99000000); //BG
	switch (nP1MeterMode)
	{
	case 0: //Normal, out of 30000
	{
		float fMeterScale = nP1Moon == 2 ? 94.3396 : 141.5094;
		DWORD dwMeterColor = METER_COLOR_MAP[nP1Meter / 10000];
		if (nP1Moon == 2 && nP1Meter >= 15000) dwMeterColor = METER_COLOR_MAP[2];
		RectDraw(61 - nResetOffset, Y + 1, nP1Meter / fMeterScale, 10, dwMeterColor);
		snprintf(buffer, 8, "%3.2f", nP1Meter / 100.0);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		break;
	}
	case 1: //HEAT, out of 550
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.5943, 10, HEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 4 * 7.7777 - nResetOffset, Y + 1, 10, HEATFONT_COLOR, "HEAT");
		break;
	}
	case 2: //MAX out of 600
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.8301, 10, MAX_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 3 * 7.7777 - nResetOffset, Y + 1, 10, MAXFONT_COLOR, "MAX");
		break;
	}
	case 3: //BLOOD HEAT out of 502
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.3679, 10, BLOODHEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 10 * 7.7777 - nResetOffset, Y + 1, 10, BLOODHEATFONT_COLOR, "BLOOD HEAT");
		break;
	}
	case 5: //UNLIMITED
	{
		RectDraw(61 - nResetOffset, Y + 1, 212, 10, UNLIMITED_COLOR);
		TextDraw(61 - nResetOffset, Y + 1, 10, UNLIMITEDFONT_COLOR, "UNLIMITED");
		break;
	}
	}

	RectDraw(580 - 214 + nResetOffset, Y, 214, 12, 0x99000000); //BG
	switch (nP2MeterMode)
	{
	case 0: //NORMAL
	{
		float fMeterScale = nP2Moon == 2 ? 94.3396 : 141.5094;
		DWORD dwMeterColor = METER_COLOR_MAP[nP2Meter / 10000];
		if (nP2Moon == 2 && nP2Meter >= 15000) dwMeterColor = METER_COLOR_MAP[2];
		RectDraw(579 - nP2Meter / fMeterScale + nResetOffset, Y + 1, nP2Meter / fMeterScale, 10, dwMeterColor);
		snprintf(buffer, 8, "%6.2f", nP2Meter / 100.0);
		TextDraw(579 - 6 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		break;
	}
	case 1: //HEAT
	{
		RectDraw(579 - nP2MeterTime / 2.5943 + nResetOffset, Y + 1, nP2MeterTime / 2.5943, 10, HEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, HEATFONT_COLOR, "HEAT");
		break;
	}
	case 2: //MAX
	{
		RectDraw(579 - nP2MeterTime / 2.8301 + nResetOffset, Y + 1, nP2MeterTime / 2.8301, 10, MAX_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, MAXFONT_COLOR, "MAX");
		break;
	}
	case 3: //BLOOD HEAT
	{
		RectDraw(579 - nP2MeterTime / 2.3679 + nResetOffset, Y + 1, nP2MeterTime / 2.3679, 10, BLOODHEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, BLOODHEATFONT_COLOR, "BLOOD HEAT");
		break;
	}
	case 5: //UNLIMITED
	{
		RectDraw(579 - 212 + nResetOffset, Y + 1, 212, 10, UNLIMITED_COLOR);
		TextDraw(579 - 9 * 7.7777 + nResetOffset, Y + 1, 10, UNLIMITEDFONT_COLOR, "UNLIMITED");
		break;
	}
	}

	DWORD dwBorderColor = (sP1CircuitBreakTimer && !sP1CircuitBreakFlag) ? CIRCUITBREAK_COLOR : 0xFFFFFFFF;
	RectDraw(59 - nResetOffset, Y - 2, 216, 2, dwBorderColor);
	RectDraw(58 - nResetOffset, Y - 1, 2, 14, dwBorderColor);
	RectDraw(59 - nResetOffset, Y + 12, 216, 2, dwBorderColor);
	RectDraw(274 - nResetOffset, Y - 1, 2, 14, dwBorderColor);
	if (sP1CircuitBreakTimer && !sP1CircuitBreakFlag)
	{
		snprintf(buffer, 8, "%3i", sP1CircuitBreakTimer);
		RectDraw(61 - nResetOffset, Y + 1, sP1CircuitBreakTimer / (sP1CircuitBreakTotal / 212.0), 10, CIRCUITBREAK_COLOR & 0x60FFFFFF);
		TextDraw(61 + 106 - 1.5 * 7.7777 - nResetOffset, Y + 1, 10, CIRCUITBREAKFONT_COLOR, buffer);
	}

	dwBorderColor = (sP2CircuitBreakTimer && !sP2CircuitBreakFlag) ? CIRCUITBREAK_COLOR : 0xFFFFFFFF;
	RectDraw(59 + 306 + nResetOffset, Y - 2, 216, 2, dwBorderColor);
	RectDraw(58 + 306 + nResetOffset, Y - 1, 2, 14, dwBorderColor);
	RectDraw(59 + 306 + nResetOffset, Y + 12, 216, 2, dwBorderColor);
	RectDraw(274 + 306 + nResetOffset, Y - 1, 2, 14, dwBorderColor);
	if (sP2CircuitBreakTimer && !sP2CircuitBreakFlag)
	{
		snprintf(buffer, 8, "%3i", sP2CircuitBreakTimer);
		RectDraw(579 - sP2CircuitBreakTimer / (sP2CircuitBreakTotal / 212.0) + nResetOffset, Y + 1, sP2CircuitBreakTimer / (sP2CircuitBreakTotal / 212.0), 10, CIRCUITBREAK_COLOR & 0x60FFFFFF);
		TextDraw(579 - 106 - 1.5 * 7.7777 + nResetOffset, Y + 1, 10, CIRCUITBREAKFONT_COLOR, buffer);
	}
}

void drawStats()
{

	if (!shouldDrawHud) {
		return;
	}

	float fScroll = *(float*)(dwBaseAddress + adTrainingResetScroll);
	int nResetOffset = 0;
	if (fScroll > 0)
		nResetOffset = 320.0f * fScroll;

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
	//drawTextWithBorder(nP1RedHealthX - nResetOffset, 40, 10, 10, buffer);
	TextDraw(nP1RedHealthX - nResetOffset, 40, 10, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "%5d", *(int*)(dwBaseAddress + dwP1Health));
	//drawTextWithBorder(230 - nResetOffset, 40, 10, 10, buffer);
	TextDraw(235 - nResetOffset, 40, 10, 0xFFFFFFFF, buffer);

	int nP2RedHealth = *(int*)(dwBaseAddress + dwP2RedHealth);
	int nP2RedHealthX;
	if (nP2RedHealth >= 9200)
		nP2RedHealthX = 535;
	else if (nP2RedHealth <= 2200)
		nP2RedHealthX = 406;
	else
		nP2RedHealthX = 575.0f - (1.0f - (float)nP2RedHealth / 11400.0f) * (575.0f - 366.0f);
	snprintf(buffer, 256, "%5d", nP2RedHealth);
	//drawTextWithBorder(nP2RedHealthX + nResetOffset, 40, 10, 10, buffer);
	TextDraw(5 + nP2RedHealthX + nResetOffset, 40, 10, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "%5d", *(int*)(dwBaseAddress + dwP2Health));
	//drawTextWithBorder(366 + nResetOffset, 40, 10, 10, buffer);
	TextDraw(365 + nResetOffset, 40, 10, 0xFFFFFFFF, buffer);


	snprintf(buffer, 256, "%5.0f", *(float*)(dwBaseAddress + dwP1GuardAmount));
	//drawTextWithBorder(234 - nResetOffset, 58, 8, 9, buffer);
	TextDraw(242 - nResetOffset, 58, 8, 0xFFFFFFFF, buffer);
	snprintf(buffer, 256, "%1.3f", *(float*)(dwBaseAddress + dwP1GuardQuality));
	//drawTextWithBorder(244 - nResetOffset, 67, 6, 9, buffer);
	TextDraw(249 - nResetOffset, 67, 6, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "%5.0f", *(float*)(dwBaseAddress + dwP2GuardAmount));
	//drawTextWithBorder(368 + nResetOffset, 58, 8, 9, buffer);
	TextDraw(361 + nResetOffset, 58, 8, 0xFFFFFFFF, buffer);
	snprintf(buffer, 256, "%1.3f", *(float*)(dwBaseAddress + dwP2GuardQuality));
	//drawTextWithBorder(369 + nResetOffset, 67, 6, 9, buffer);
	TextDraw(367 + nResetOffset, 67, 6, 0xFFFFFFFF, buffer);

	
	// on p1 health bar
	drawRect(114.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(167.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(220.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);

	uint32_t nP1Health = *(uint32_t*)(dwBaseAddress + dwP1Health); // this works on maids too
	if (nP1Health >= 8550)
	{
		drawRect(61.0f - nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(61.0f - nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(62 - nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x0)).c_str());
	}
	else
	{
		drawRect(61.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);			// on vertical bar
		drawRect(61.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(62 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x0)).c_str());
	}
	if (8550 > nP1Health && nP1Health >= 5700)
	{
		drawRect(114.0f - nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(114.0f - nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(115 - nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x4)).c_str());
	}
	else
	{
		drawRect(114.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(114.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(115 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x4)).c_str());
	}
	if (5700 > nP1Health && nP1Health >= 2850)
	{
		drawRect(167.0f - nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(167.0f - nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(168 - nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x8)).c_str());
	}
	else
	{
		drawRect(167.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(167.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(168 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0x8)).c_str());
	}
	if (2850 > nP1Health)
	{
		drawRect(220.0f - nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(220.0f - nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(221 - nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0xC)).c_str());
	}
	else
	{
		drawRect(220.0f - nResetOffset, 23.0f, 1.0f, 14.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(220.0f - nResetOffset, 23.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(221 - nResetOffset, 24, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP1Guts + 0xC)).c_str());
	}


	// on p2 health bar
	drawRect(525.0f + nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(472.0f, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(419.0f, 39.0f, 1.0f, 3.0f, 0xFF000000);

	uint32_t nP2Health = *(uint32_t*)(dwBaseAddress + dwP2Health); // this works on maids too
	if (nP2Health >= 8550)
	{
		drawRect(579.0f + nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(540.0f + nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(541 + nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x0)).c_str());
	}
	else
	{
		drawRect(579.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);			// on vertical bar
		drawRect(555.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(556 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x0)).c_str());
	}
	if (8550 > nP2Health && nP2Health >= 5700)
	{
		drawRect(525.0f + nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(486.0f + nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(486 + nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x4)).c_str());
	}
	else
	{
		drawRect(525.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(501.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(501 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x4)).c_str());
	}
	if (5700 > nP2Health && nP2Health >= 2850)
	{
		drawRect(472.0f + nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(434.0f + nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(432 + nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x8)).c_str());
	}
	else
	{
		drawRect(472.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(449.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(448 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0x8)).c_str());
	}
	if (2850 > nP2Health)
	{
		drawRect(419.0f + nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(380.0f + nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(380 + nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0xC)).c_str());
	}
	else
	{
		drawRect(419.0f + nResetOffset, 23.0f, 1.0f, 14.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(395.0f + nResetOffset, 23.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(395 + nResetOffset, 24, 6, 0xFFFFFFFF, std::format("{:.3f}", *(float*)(dwBaseAddress + adSharedP2Guts + 0xC)).c_str());
	}
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

	drawObject(0x00555130 + (0xAFC * 0), false, 0); // P1
	drawObject(0x00555130 + (0xAFC * 1), false, 1); // P2
	drawObject(0x00555130 + (0xAFC * 2), false, 2); // P3
	drawObject(0x00555130 + (0xAFC * 3), false, 3); // P4

	// draw all effects

	int projectileIndex = 4;
	bool res;
	for(unsigned index = 0; index < 1000; index++) {
		if (((*(int*)(index * 0x33c + 0x67bde8) != 0) && (*(char*)(index * 0x33c + 0x67be09) == '\0'))) {
			res = drawObject(index * 0x33c + 0x67bde8, true, projectileIndex);
			if (res) {
				projectileIndex++;
			}
		}
	}
}

// highlight states

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

BYTE arrAnimHookBytesOrig[10];
BYTE arrAnimHookBytesMod[10];
void highlightHookFunc()
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

// RNG manip

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

void __stdcall legacyPauseCallback(DWORD dwMilliseconds)
{

	// windows Sleep, the func being overitten is an stdcall, which is why we have __stdcall
	// this func is legacy, but it seems like the best place for me to init my hooks, for some reason

	// i am unsure if doing this here is the best location, but it has been working
	// and weird things happen if i call it right after i grab the device
	// please never move this
	static bool isDirectXHooked = false;
	if (!isDirectXHooked) {
		isDirectXHooked = HookDirectX();
	}

	static bool isRendererHooked = false;
	if (!isRendererHooked && isDirectXHooked) {
		isRendererHooked = initRenderModifications();
	}

	Sleep(dwMilliseconds);
}

// frame start/done callbacks

void frameStartCallback() {

	//maintainFPS();

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

bool logSaveState = false;

void setFPSLimiter(bool b) {
	disableFPSLimit = b;

	// i,, i do not know what to do here.
	// i wish i designed the menu with this in mind, but not all menu elements have/need globals
	// if the menu design is changed, this will need to be changed
	// i could maybe pass an int* as the menu type instead?
	if (disableFpsMenuOption != NULL) {
		disableFpsMenuOption->optionState = b;
	}
	
}

void frameDoneCallback()
{
	profileFunction();

	static KeyState lShiftKey(VK_LSHIFT);

	//Sleep(8);

	//log("%4d %4d", __frameDoneCount, *reinterpret_cast<int*>(dwBaseAddress + adFrameCount));

	//log("combo: %d", getComboCount());

	setWind();

	if (!isPaused()) {
		static DWORD prevUnpausedFrameCount = 0;

		bool alreadyRolledReplayManager = false;

		if (prevUnpausedFrameCount != unpausedFrameCount) {
			//long long startTime = getMicroSec();

			saveStateManager.save();
			unpausedFrameCount = prevUnpausedFrameCount;
			//log("calling rollforward 1");
			//replayManager.rollForward();
			alreadyRolledReplayManager = true;

			//long long endTime = getMicroSec();
			//long long totalTime = endTime - startTime;
			//log("%3lld.%03lld", totalTime / 1000, totalTime % 1000);
		}

		static KeyState UpKey(VK_UP);
		static KeyState DownKey(VK_DOWN);

		if (UpKey.keyDownHeldFreq<4, 24>()) {
			bool needNewFrame = saveStateManager.load(1);
			if (!alreadyRolledReplayManager) {
				//log("calling rollforward 2");
				//replayManager.rollForward();
			}
			
			if (!needNewFrame) {
				rollFancyInputDisplay(1);
			}
		} else if (DownKey.keyDownHeldFreq<4, 24>()) {
			saveStateManager.load(-1);
			rollFancyInputDisplay(-1);
			replayManager.rollBack();
		}

		if (logSaveState) {
			saveStateManager.log();
		}
	}

	static KeyState rKey('R');
	if (lShiftKey.keyHeld() && rKey.keyDown()) {
		
		//log("RKEY hit");
		
		needTrainingModeReset = true;

		//replayManager.load("./ReplayVS/RED_ARCUEIDxV_SION_250203130642.rep");
		replayManager.load("./ReplayVS/RED_ARCUEIDxRED_ARCUEID_241231183621_usethis.rep");
		//replayManager.load("./ReplayVS/RED_ARCUEIDxV_SION_250203130642.rep");

		//replayManager.load("./ReplayVS/RED_ARCUEIDxRED_ARCUEID_250203161138.rep");

		replayManager.reset();


		//TASManagerObj.load("TAS.txt");
		//needTrainingModeReset = true;
	}

	static KeyState mKey('M');
	if (mKey.keyDown()) {
		needTrainingModeReset = true;
	}

	static KeyState fKey('F');
	if (lShiftKey.keyHeld() && fKey.keyDown()) {
		setFPSLimiter(!disableFPSLimit); // sorry :3
	}

	renderModificationsFrameDone();

	drawFancyMenu();
	dragManager.handleDrag();

	setAllKeys();

	shouldDrawBackground = *(uint8_t*)(dwBaseAddress + adSharedBackgroundStyle) == BG_NORMAL;
	shouldDrawHud = !*(bool*)(dwBaseAddress + adSharedDisableHUD);
	shouldDrawGroundLine = *(bool*)(dwBaseAddress + adSharedDrawGround);
	shouldDrawShadow = !*(bool*)(dwBaseAddress + adSharedDisableShadow);
	fastReversePenalty = *(bool*)(dwBaseAddress + adSharedFastReversePenalty);
	bFrameDataDisplay = *(bool*)(dwBaseAddress + adSharedFrameDataDisplay);

	switch (*(uint8_t*)(dwBaseAddress + adSharedBackgroundStyle))
	{
	case BG_NORMAL:
		shouldDrawBackground = true;
		break;
	case BG_RED:
		shouldDrawBackground = false;
		backgroundColor = 0xFFFF0000;
		break;
	case BG_GREEN:
		shouldDrawBackground = false;
		backgroundColor = 0xFF00FF00;
		break;
	case BG_BLUE:
		shouldDrawBackground = false;
		backgroundColor = 0xFF0000FF;
		break;
	case BG_WHITE:
		shouldDrawBackground = false;
		backgroundColor = 0xFFFFFFFF;
		break;
	case BG_BLACK:
		shouldDrawBackground = false;
		backgroundColor = 0xFF000000;
		break;
	case BG_GRAY:
		shouldDrawBackground = false;
		backgroundColor = 0xFF888888;
		break;
	case BG_YELLOW:
		shouldDrawBackground = false;
		backgroundColor = 0xFFFFFF00;
		break;
	case BG_PURPLE:
		shouldDrawBackground = false;
		backgroundColor = 0xFFFF00FF;
		break;
	default:
		break;
	}

	bool ok = true;
	MSG msg;
	while (*(uint8_t*)(dwBaseAddress + adSharedFreezeOverride))
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
	}

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

	if (*(bool*)(dwBaseAddress + adSharedDisplayHitboxes))
		bHitboxesDisplay = true;
	else
		bHitboxesDisplay = false;

	if (*(bool*)(dwBaseAddress + adSharedHighlight))
		bHighlightsOn = true;
	else
		bHighlightsOn = false;

	doFastReversePenalty();
	
	//drawTextWithBorder(300, 300, 36, 48	, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

	if (bFreeze && shouldDrawHud)
	{
		try
		{
			char pcFreezeKey[256];
			char pcName[19];
			UINT scanCode = MapVirtualKeyA(*(uint8_t*)(dwBaseAddress + adSharedFreezeKey), MAPVK_VK_TO_VSC);
			LONG lParamValue = (scanCode << 16);
			GetKeyNameTextA(lParamValue, pcName, 19);
			snprintf(pcFreezeKey, sizeof(pcFreezeKey), "Freeze Key: %s", pcName);
			TextDraw(100.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcFreezeKey);
		}
		catch (...)
		{
			TextDraw(100.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Freeze Key: <corrupt>");
		}

		try
		{
			char pcFrameStepKey[256];
			char pcName[19];
			UINT scanCode = MapVirtualKeyA(*(uint8_t*)(dwBaseAddress + adSharedFrameStepKey), MAPVK_VK_TO_VSC);
			LONG lParamValue = (scanCode << 16);
			GetKeyNameTextA(lParamValue, pcName, 19);
			snprintf(pcFrameStepKey, sizeof(pcFrameStepKey), "Frame Step Key: %s", pcName);
			TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcFrameStepKey);
		}
		catch (...)
		{
			TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Frame Step Key: <corrupt>");
		}
			
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
		bHitboxesDisplay = !*(bool*)(dwBaseAddress + adSharedDisplayHitboxes);
		*(bool*)(dwBaseAddress + adSharedDisplayHitboxes) = bHitboxesDisplay;
		nDrawTextTimer = TEXT_TIMER;
		if (bHitboxesDisplay)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES OFF");
	}

	if (oFrameDataDisplayKey.keyDown())
	{
		bFrameDataDisplay = !bFrameDataDisplay;
		*(bool*)(dwBaseAddress + adSharedFrameDataDisplay) = bFrameDataDisplay;
		nDrawTextTimer = TEXT_TIMER;
		if (bFrameDataDisplay)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA OFF");
	}

	if (oHighlightsOnKey.keyDown())
	{
		bHighlightsOn = !bHighlightsOn;
		*(bool*)(dwBaseAddress + adSharedHighlight) = bHighlightsOn;
		nDrawTextTimer = TEXT_TIMER;
		if (bHighlightsOn)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS OFF");
	}

	if (oReversalKey.keyHeld())
		*(bool*)(dwBaseAddress + adSharedReversalKeyHeld) = true;
	else
		*(bool*)(dwBaseAddress + adSharedReversalKeyHeld) = false;

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

		//drawTextWithBorder(243, 460, 15, 15, pcTextToDisplay);
		TextDraw(243, 460, 15, 0xFFFFFFFF, pcTextToDisplay);

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
		
		drawFrameBar();

		if (bHitboxesDisplay)
			drawFrameData();

		if (!shouldDrawMeter)
			drawSimpleMeter();
		
		/*if (bFrameDataDisplay)
		{
			drawFrameBar();
		}*/	

		if (*(bool*)(dwBaseAddress + adSharedShowStats))
			drawStats();
	}
	else if (*(bool*)(dwBaseAddress + adSharedHoveringScroll) == 1 && *(bool*)(dwBaseAddress + adSharedOnExtendedSettings))
	{
		drawFrameBar(325);
	}
	else if (*(bool*)(dwBaseAddress + adSharedHoveringScroll) == 2 && *(bool*)(dwBaseAddress + adSharedOnExtendedSettings))
	{
		drawFrameBar();
	}

	if (*(bool*)(dwBaseAddress + adSharedColorGuide))
	{
		drawColorGuide();
	}

	if (bFrameDataDisplay && frameBarY > 400)
	{
		shouldDrawMeter = 0;
	}
	else
	{
		shouldDrawMeter = 1;
	}

	*(int*)(adMBAABase + adSharedTimer) = *(int*)(adMBAABase + adTrueFrameCount);

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
		RectDraw(-0.5f, 382.5f, 640.5f, 70.0f, 0xFF101010);
		TextDraw(47.5f, 393.5f, 16.0f, 0xFFFFFFFF, pcMainInfoText);
		TextDraw(66.5f, 420.5f, 16.0f, 0xFFFFFFFF, pcSubInfoText);
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

__declspec(naked) void nakedFrameDoneCallback_RAW() {
	PUSH_ALL;
	frameDoneCallback();
	POP_ALL;

	__asm {
		ret;
	}
}

// pause funcs

int needPause = false;
void newPauseCallback2()
{

	if (oFreezeKey.keyDown()) {
		bFreeze = !bFreeze;
		if (bFreeze)
		{
			bSlow = false;
			_naked_newPauseCallback2_IsPaused = true;
		}
		else
			_naked_newPauseCallback2_IsPaused = false;
	}

	else if (!bFreeze && oFrameStepKey.keyDown())
	{
		bFreeze = true;
		bSlow = false;
		_naked_newPauseCallback2_IsPaused = true;
	}

	if (!bFreeze)
	{
		if (*(uint8_t*)(dwBaseAddress + adSharedSlowSpeed))
		{
			bSlow = true;
			_naked_newPauseCallback2_IsPaused = true;
		}
		else
		{
			// this code prevents me from being able to set needpause from anywhere in the code
			// bfreeze is legacy!, _naked_newPauseCallback2_IsPaused is what actually controls the pausing!
			// but im scared to fuck with this so im leaving it
			bSlow = false;
			needPause = false;
			_naked_newPauseCallback2_IsPaused = false;
		}
	}
	
	static uint8_t nFrameNumber = 0;
	nFrameNumber++;

	if (oFrameStepKey.keyHeld())
		oFrameStepKey.nHeldKeyCounter++;
	else
		oFrameStepKey.nHeldKeyCounter = 0;
	if (_naked_newPauseCallback2_IsPaused && (oFrameStepKey.keyDown() || oFrameStepKey.nHeldKeyCounter >= 20 || (bSlow && nFrameNumber % 4 < *(uint8_t*)(dwBaseAddress + adSharedSlowSpeed))))
	{
		needPause = true;
		_naked_newPauseCallback2_IsPaused = false;
	}
	else if (needPause == 1)
	{
		needPause = false;
		_naked_newPauseCallback2_IsPaused = true;
	}
	else if (needPause > 1) {
		needPause--;
	}
	
	if (!_naked_newPauseCallback2_IsPaused) {
		unpausedFrameCount++;
		TASManagerObj.incInputs();
	}

	if (!_naked_newPauseCallback2_IsPaused && needPause != 2) {
		
		if (!isPaused()) {
			//log("calling rollforward 3");
			replayManager.rollForward();
		}
		
	}

}

DWORD _naked_pauseInputDisplay2_FUN_004790a0 = 0x004790a0;
DWORD _naked_pauseInputDisplay2_FUN_004796a0 = 0x004796a0;
__declspec(naked) void _naked_pauseInputDisplay2() {

	__asm {

		// i need to check if the menu is open in general, freeze menu, or normal
		cmp _naked_newPauseCallback2_IsPaused, 0;
		JE _SKIP;

		// menu is open, dont do input bs
		push 004794dch;
		ret;

	_SKIP:

	}

	__asm {
		push esi;
		call[_naked_pauseInputDisplay2_FUN_004790a0];
	}
	;
	// 004794c4, MOVZX EBX,byte ptr [ESI + 0x2e7]
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
		call[_naked_pauseInputDisplay2_FUN_004796a0];
		add esp, 08h;
	};

	__asm {
		push 004794dch;
		ret;
	};
}

DWORD _naked_newPauseCallback2_UpdateBattleScene = 0x00423630;
DWORD _naked_newPauseCallback2_Func_TrainingPause = 0x0044c480;
DWORD _naked_newPauseCallback2_Func_UpdateFX = 0x00453b80;
__declspec(naked) void _naked_newPauseCallback2() {


	PUSH_ALL;
	newPauseCallback2();
	POP_ALL;

	__asm {
		cmp _naked_newPauseCallback2_IsPaused, 1;
		JE _SKIP;

		//call[_naked_newPauseCallback2_Func_Addr];
		call[_naked_newPauseCallback2_UpdateBattleScene];

		//push 0040e476h;
		//push 00432c82h;
		push 004235d6h;
		//push 0042370bh;
		//push 00423747h;
		//push 0042370bh;
		ret;

	_SKIP:

	
		// when the func is NOT called, we still need to deal with input display, attack display, and whatever the fuck controls the dummy recorder
		// i have 2/3 of those covered

		// this call, should be ok?
		call[_naked_newPauseCallback2_Func_TrainingPause];

		
		call[_naked_newPauseCallback2_Func_UpdateFX];

		//push 0040e476h;
		//push 00432c82h;
		push 004235d6h;
		//push 0042370bh;
		//push 004540beh;
		//push 004235d6h;
		//push 004235dch;
		//push 00423747h;

		// i have no idea what state the stack is in rn

		ret;
	};
}

DWORD _naked_pauseMenuProcessInput2_Func = 0x00477ce0;
__declspec(naked) void _naked_pauseMenuProcessInput2() {

	// patched at 0x0044c48e

	__asm {
		call[_naked_pauseMenuProcessInput2_Func]; // call the actual func

		// check if our pause is active
		cmp _naked_newPauseCallback2_IsPaused, 1;
		JNE _SKIP;

		// if we are here, we are in our own menu, and modify the retval

		mov eax, 0;


	_SKIP: // ret

		push 0044c493h;
		ret;
	}
}

__declspec(naked) void _naked_trigPauseHook() {
	// makes sure menu doesnt open when we are in pause

	__asm {
		cmp _naked_newPauseCallback2_IsPaused, 1;
		JNE _CONT;

		// being here means we are in our pause section. ret.
		
		ret;

	_CONT:
	}

	

	// overwritten bytes
	__asm _emit 0xA1;
	__asm _emit 0x74;
	__asm _emit 0x2A;
	__asm _emit 0x56;
	__asm _emit 0x00;

	__asm {
		push 0044c7b5h;
		ret;
	}
}

__declspec(naked) void _naked_updateEffectsPause() {

	__asm {

		cmp _naked_newPauseCallback2_IsPaused, 0;
		JE _CONT3;

		// just skip to the end of the func
		// im not even sure what the second half of the loop does.
		// it might be important tbh
		push 00453f22h;
		ret;

	_CONT3:
	}

	__asm _emit 0xBD;
	__asm _emit 0xDD;
	__asm _emit 0xC0;
	__asm _emit 0x67;
	__asm _emit 0x00;

	__asm { // contnue func exec as normal
		push 00453d5ch;
		ret;
	}
}

DWORD checkPauseEffect_Addr;
DWORD checkPauseEffect_Skip = 0;
void checkPauseEffect() {

	if (!_naked_newPauseCallback2_IsPaused) {
		return;
	}

	DWORD pattern = *(DWORD*)(checkPauseEffect_Addr + 0x10);

	if (*(DWORD*)(checkPauseEffect_Addr + 0x20) == 0x00000101) {
		// this detects if something is a heat effect. heat effects are weird
		// why 0x20? why 0x00000101? because when i was doing the effects coloring work, this worked
		return;
	}

	if (*(BYTE*)(checkPauseEffect_Addr + 0x8) == 0xFE) { // 0xFE is -2 in this case
		// this was for croa 236A, but it effects all other -2 source things
		// the meter bar flash is,, source -2, and then has 0x00FF at +9?
		// ill just check the pattern
		// also 177
		// i wish i knew where the data for all the -2 sources was
		// actually, effect.txt!!!!!!
		// this also has the effects for the bars, and well,, everything
		// perhaps preventing whatever spawns the new effects is a better approach here
		switch (pattern) {
		case 177: // roa lightning
		case 178:
		case 179:
			return;
		default:
			break;
		}

		// the meter flash seems to be,, pattern 108 or 104?
		
	}

	checkPauseEffect_Skip = 1;
}

__declspec(naked) void _naked_updateEffectsPauseLoop1() {

	__asm {
		mov checkPauseEffect_Addr, ebp;
		sub checkPauseEffect_Addr, 9; // first loop is offset by 9 from what im using
	};

	PUSH_ALL;
	checkPauseEffect();
	POP_ALL;

	__asm {
		cmp checkPauseEffect_Skip, 0;
		JE _CONT;

		mov checkPauseEffect_Skip, 0;

		// skip this iteration
		push 00453d45h;
		ret;

	_CONT:
	}


	// overwritten bytes from 00453b92

	// cmp byte ptr [EBP + -0x9]
	__asm _emit 0x80;
	__asm _emit 0x7D;
	__asm _emit 0xF7;
	__asm _emit 0x00;

	// not inlining the jz on purpose, its reljump
	// i could align my jump func to 0xF but holy shit no 
	__asm {
		JZ _gotoNextLoop;

		push 00453b9ch; // return exec to normal
		ret;

	_gotoNextLoop:
		push 00453d45h;
		ret;
	}
}

__declspec(naked) void _naked_updateEffectsPauseLoop2() {

	__asm {
		mov checkPauseEffect_Addr, ebp;
		sub checkPauseEffect_Addr, 02F5h; // second loop is offset by 2F5 from what im using. PRAY that this sub actually works
	};

	PUSH_ALL;
	checkPauseEffect();
	POP_ALL;

	__asm {
		cmp checkPauseEffect_Skip, 0;
		JE _CONT;

		mov checkPauseEffect_Skip, 0;

		// skip this iteration
		push 00453f10h;
		ret;

	_CONT:
	}


	// overwritten bytes from 00453d60

	// cmp byte ptr [fxCso->assetsIndex + 0xfffffd0b]
	__asm _emit 0x80;
	__asm _emit 0xBD;
	__asm _emit 0x0B;
	__asm _emit 0xFD;
	__asm _emit 0xFF;
	__asm _emit 0xFF;
	__asm _emit 0x00;

	// there were enough bytes in that above cmp for us to just ret
	__asm {
		push 00453d67h;
		ret;
	};
}

DWORD _naked_pauseHitEffectsHook_pauseVal = 0;
__declspec(naked) void _naked_pauseHitEffectsHook() {

	__asm {
		// backup the game's pause state
		push eax;

		//mov al, byte ptr [0055d203h]; // generates mov al, 03. how???
		mov al, ds: [0055d203h];
		mov _naked_pauseHitEffectsHook_pauseVal, eax;
		
		pop eax;
	}

	__asm {
		// overwritten asm
		push ebp;
		mov ebp, esp;
		and esp, 0fffffff8h;

		cmp _naked_newPauseCallback2_IsPaused, 0;
		JE _SKIP;

		// we are paused. make the game think so.
		//push eax;
		//mov eax, 1;
		//mov ds:[0055d203h], al; // i despise masm. shoutout to a random stackoverflow post that said "oh yea the assembler will be sane if you ds:" for some reason
		mov ds:[0055d203h], 1;
		//pop eax;

	_SKIP: // act right
		push 00458e86h;
		ret;
	}
}

void pauseHitEffectsCallbackSaftey() {
	// to prevent fire from overflowing and crashing the game, i need to either:
	// reverse the hit effects thing, we have a release in a week, no
	// do this

	//memset((void*)(0x0061e170 + (3000 * 0x60)), 0x00, 50 * 0x60);
}

__declspec(naked) void _naked_pauseHitEffectsCallback() {


	// restore the game's pause state
	__asm {
		push eax;

		mov eax, _naked_pauseHitEffectsHook_pauseVal;
		mov	ds : [0055d203h] , al;

		pop eax;
	}

	//PUSH_ALL;
	//pauseHitEffectsCallbackSaftey();
	//POP_ALL;

	__asm { // overwritten code at 0045ca27
		pop ebx;
		mov esp, ebp;
		pop ebp;
		ret;
	}
}

__declspec(naked) void _naked_preventPauseReset() {
	__asm {

		cmp _naked_newPauseCallback2_IsPaused, 1;
		JE _SKIP;

		emitCall(0x00478590);

	_SKIP:

		emitJump(0x00477eb8);
	}

}

// draw modification funcs

void drawSolidBackground() {

	if (backgroundColor == 0xFF000000) {
		return;
	}

	drawRect(0, 0, 640, 480, backgroundColor, 0x10a);

}

void drawBackgroundLine() {

	float windowWidth = *(uint32_t*)0x0054d048;
	float windowHeight = *(uint32_t*)0x0054d04c;

	int cameraX = *(int*)(0x0055dec4);
	int cameraY = *(int*)(0x0055dec8);
	float cameraZoom = *(float*)(0x0054eb70);

	int xPos = 0;
	int yPos = 0;

	float xCamTemp = ((((float)(xPos - cameraX) * cameraZoom) / 128.0f) * (windowWidth / 640.0f) + windowWidth / 2.0f);
	float yCamTemp = ((((float)(yPos - cameraY) * cameraZoom) / 128.0f - 49.0f) * (windowHeight / 480.0f) + windowHeight);

	xCamTemp = floor(xCamTemp);
	yCamTemp = floor(yCamTemp);

	float tempFloat;

	float x1Cord, x2Cord, y1Cord, y2Cord;

	x1Cord = ((float)xCamTemp - (windowWidth / 640.0f) * cameraZoom * 5.0f);
	x2Cord = ((windowWidth / 640.0f) * cameraZoom * 5.0f + (float)xCamTemp);
	y1Cord = ((float)yCamTemp - (windowWidth / 640.0f) * cameraZoom * 5.0f);
	y2Cord = yCamTemp;

	x1Cord = ((float)x1Cord * (640.0f / windowWidth));
	x2Cord = ((float)x2Cord * (640.0f / windowWidth));
	y1Cord = ((float)y1Cord * (480.0f / windowHeight));
	y2Cord = ((float)y2Cord * (480.0f / windowHeight));

	// this is one of the only justified uses of this func
	drawRect(0, (int)y2Cord, 640, 1, 0xFF42e5f4, 0x10C);

}

DWORD _naked_DrawBackground_FuncAddr = 0x004b9540;
__declspec(naked) void _naked_DrawBackground() {

	__asm {

		cmp shouldDrawBackground, 0;
		JE _SKIP;

		call[_naked_DrawBackground_FuncAddr];

	
	_SKIP:

	}

	PUSH_ALL;
	if (shouldDrawBackground == 0) {
		drawSolidBackground();
	}

	if (shouldDrawGroundLine == 1) {
		drawBackgroundLine();
	}
	POP_ALL;

	__asm {
		push 004238c5h
		ret;
	}
}

DWORD _naked_DrawResourcesHud_FuncAddr;
__declspec(naked) void _naked_DrawResourcesHud() {
	//0042485b
}

DWORD _naked_DrawHudText_FuncAddr = 0x00476c70;
__declspec(naked) void _naked_DrawHudText() {
	__asm {
		cmp shouldDrawHud, 0;
		JE _SKIP;

		call[_naked_DrawHudText_FuncAddr];

	_SKIP:

		push 004238a1h;
		ret;
	}
}

DWORD _naked_DrawHud_FuncAddr = 0x00424100;
__declspec(naked) void _naked_DrawHud() {

	// i despise msvc
	
	__asm {
		push eax;
		push ebx;

		mov ebx, shouldDrawHud;
		xor ebx, 1;

		mov eax, 5545F1h;
		mov byte ptr[eax], bl;

		pop ebx;
		pop eax;
	}
	

	__asm {
		//cmp shouldDrawHud, 0;
		//JE _SKIP;

		call[_naked_DrawHud_FuncAddr];

		push 004238abh;
		ret;

	_SKIP:

		add esp, 04h; // clean up the stack

		push 004238abh;
		ret;
	}
}

DWORD _naked_DrawHudMeter_FuncAddr = 0x004253c0;
__declspec(naked) void _naked_DrawHudMeter() {
	__asm {
		cmp shouldDrawMeter, 0;
		JE _SKIP;

		call[_naked_DrawHudMeter_FuncAddr];

	_SKIP:

		push 00425232h;
		ret;
	}
}

// MENU TESTS
MenuWindow* mainWindow;

DWORD MBAA_FUN_00429020 = 0x00429020;
DWORD MBAA_FUN_00429b00 = 0x00429b00;
DWORD MBAA_FUN_0047ce20 = 0x0047ce20;
DWORD MBAA_FUN_0047d030 = 0x0047d030;
DWORD MBAA_FUN_004804a0 = 0x004804a0;
DWORD MBAA_FUN_004d8810 = 0x004d8810;

DWORD MBAA_ReadDataFile =			0x00407c10;
DWORD MBAA_InitNormalElement =		0x00429140;
DWORD MBAA_InitMenuInfo =			0x00429400;
DWORD MBAA_EnterIntoList =			0x0042ba50;
DWORD MBAA_CompareLabel =			0x0042bdc0;
DWORD MBAA_InitItem =				0x0042f600;
DWORD MBAA_InitSelectElement =		0x0042f8f0;
DWORD MBAA_GetElementPointer =		0x0042fa90;
DWORD MBAA_GetSetting =				0x0042fb50;
DWORD MBAA_InitMenuWindow =			0x004d7b30;
DWORD MBAA_DefaultMenuFreeAll =		0x004d7c50;
DWORD MBAA_MenuDestructor =			0x004d7c70;
DWORD MBAA_operator_new =			0x004e0177;
DWORD MBAA_UnrecoveredJumptable =	0x004e0bf2;

const char* pcEXTENDED_SETTINGS = "EXTENDED SETTINGS";
const char* pcEXTENDED_SETTING = "EXTENDED_SETTING";
const char* pcHOTKEY_SETTINGS = "HOTKEY SETTINGS";
const char* pcHOTKEY_SETTING = "HOTKEY_SETTING";

Item* NEW_ITEM() {
	__asm {
		push 0x3c;
		call[MBAA_operator_new];
		add esp, 0x4;
	}
}

Element* NEW_SELECT_ELEMENT() {
	__asm {
		push 0x70;
		call[MBAA_operator_new];
		add esp, 0x4;
	}
}

Element* NEW_NORMAL_ELEMENT() {
	__asm {
		push 0x58;
		call[MBAA_operator_new];
		add esp, 0x4;
	}
}

MenuInfo* NEW_MENU_INFO() {
	__asm {
		push 0x78;
		call[MBAA_operator_new];
		add esp, 0x4;
	}
}

MenuWindow* NEW_MENU_WINDOW() {
	__asm {
		push 0xc4;
		call[MBAA_operator_new];
		add esp, 0x4;
	}
}

//wrapper for call to ReadDataFile (misleading name? pulled straight from ghidra)
void ReadDataFile(void* dest, const char* name, int nameLength) {
	//dest should be ecx
	//name should be stack[4]
	//nameLength should be stack[8]
	__asm {
		mov ecx, dest;
		push nameLength;
		push name;
		call[MBAA_ReadDataFile];
	}
}

//wrapper for call to InitNormalElement
void InitNormalElement(Element* element, const char* label, const char* tag, int elementType) {
	//element should be stack[4]
	//label should be stack[8]
	//tag should be stack[c]
	//elementType should be stack[10]
	__asm {
		push elementType;
		push tag;
		push label;
		push element;
		call[MBAA_InitNormalElement];
	}
}

//wrapper for call to InitSelectItem
void InitSelectElement(Element* element, const char* label, const char* tag, int selectItemXOffset) {
	//label should be ecx
	//element should be stack[4]
	//tag should be stack[8]
	//offset should be stack[c]
	__asm {
		mov ecx, label;
		push selectItemXOffset;
		push tag;
		push element;
		call[MBAA_InitSelectElement];
	}
}

//wrapper for call to InitItem
void InitItem(Item* item, const char* label, const char* tag, int index) {
	//item should be stack[4]
	//label should be stack[8]
	//tag should be stack[c]
	//index should be stack[10]
	__asm {
		push index;
		push tag;
		push label;
		push item;
		call[MBAA_InitItem];
	}
}

//wrapper for call to EnterIntoList
void EnterIntoList(void* list, void* entry) {
	//entry should be ebx
	//list should be esi
	__asm {
		push entry;
		mov ebx, esp;
		mov esi, list;
		call[MBAA_EnterIntoList];
		add esp, 0x4;
	}
}

MenuInfo* InitMenuInfo(MenuInfo* menuInfo, MenuWindow* menuWindow, const char* tag) {
	menuInfo->vftable = (void*)0x00535ec8;
	menuInfo->parentWindow = menuWindow;
	menuInfo->tag[0] = 0;
	menuInfo->tagLength = 0;
	menuInfo->tagMaxLength = 0xf;
	menuInfo->blank[0] = 0;
	menuInfo->blankLength = 0;
	menuInfo->blankMaxLength = 0xf;
	menuInfo->selectedElement = 0;
	menuInfo->prevSelectedElement = 0;
	menuInfo->ElementList = 0;
	menuInfo->ElementListEnd = 0;
	menuInfo->field_0x54 = 0;
	menuInfo->field_0x5c = 0;
	menuInfo->field_0x60 = 0;
	menuInfo->field_0x64 = 0;
	menuInfo->finishedDrawing = 0;
	menuInfo->timeDisplayed = 0;
	menuInfo->field_0x70 = 99999;
	menuInfo->close = 0;
	ReadDataFile(&menuInfo->tagBase, tag, strlen(tag));
	ReadDataFile(&menuInfo->blankBase, "", 0);
	return menuInfo;
}

MenuWindow* InitMenuWindow(MenuWindow* menuWindow) {
	menuWindow->MenuInfoList = 0;
	menuWindow->MenuInfoListEnd = 0;
	menuWindow->field_0x18 = 0;
	menuWindow->hoveredTagLength = 0;
	menuWindow->hoveredTagMaxLength = 0xf;
	menuWindow->hoveredTag[0] = 0;
	menuWindow->menuInfoIndex = 0;
	menuWindow->field_0x8 = 0;
	menuWindow->someYOffset = 0x10;
	menuWindow->field_0x50 = 0;
	menuWindow->didPress = 0;
	menuWindow->field_0x3c = 0;
	menuWindow->xOffset = 0x140;
	menuWindow->vftable = (void*)0x0053d3c0;
	menuWindow->labelMaxLength = 0xf;
	menuWindow->labelLength = 0;
	menuWindow->label[0] = 0;
	menuWindow->progressionRate = 0.0625;
	menuWindow->degressionRate = -0.0625;
	menuWindow->isRootMenu = 0;
	menuWindow->timeOpened = 0;
	menuWindow->closeAnimProgress = 0.0;
	menuWindow->openSubmenuIndex = 0;
	menuWindow->menuOpacity = 1.0;
	menuWindow->field_0xa0 = 0;
	menuWindow->playerInControl = 0;
	menuWindow->isBlurred = 1;
	menuWindow->isMenuBackgroundDisplayed = 1;
	menuWindow->u_layer = 0x2f0;
	menuWindow->field_0x58 = 0;
	menuWindow->field_0x40 = 0;
	menuWindow->yOffset = 0xf0;
	menuWindow->textXWidth = 0x10;
	menuWindow->textYWidth = 0x10;
	ReadDataFile(&menuWindow->labelBase, "", 0);
	menuWindow->paragraphMode = 0;
	menuWindow->field_0xb0 = 0;
	return menuWindow;
}

void AddSelectElement(MenuInfo* menuInfo, std::vector<const char*> elementVector, int pageNum, int elementNum) {
	//std::vector<const char*> elementVector = Page_Options[pageNum][elementNum];
	int vSize = size(elementVector);
	Element* element = NEW_SELECT_ELEMENT();
	char tempTag[8];
	snprintf(tempTag, 8, "%i_%i_0", pageNum, elementNum);
	InitSelectElement(element, elementVector[0], tempTag, 0xa0);
	element->vftable = (void*)0x00536654;
	for (int i = 1; i < vSize; i++) {
		Item* item = (Item*)NEW_ITEM();
		snprintf(tempTag, 8, "%i_%i_%i", pageNum, elementNum, i);
		InitItem(item, elementVector[i], tempTag, i - 1);
		EnterIntoList((void*)(&element->ListInput), (void*)(item));
	}
	EnterIntoList((void*)(&menuInfo->ListInput), (void*)(element));
}

void AddNormalElement(MenuInfo* menuInfo, std::vector<const char*> elementVector, int pageNum, int elementNum) {
	//std::vector<const char*> elementVector = Page_Options[pageNum][elementNum];
	Element* element = NEW_NORMAL_ELEMENT();
	char tempTag[8];
	snprintf(tempTag, 8, "%i_%i_0_n", pageNum, elementNum);
	InitNormalElement(element, elementVector[0], tempTag, 0);
	element->canSelect = 1;
	element->elementType = 1;
	element->vftable = (void*)0x0053604c;
	EnterIntoList((void*)(&menuInfo->ListInput), (void*)(element));
}

void AddSpaceElement(MenuInfo* menuInfo, int pageNum, int elementNum) {
	Element* element = NEW_NORMAL_ELEMENT();
	char tempTag[8];
	snprintf(tempTag, 8, "%i_%i_0_s", pageNum, elementNum);
	InitNormalElement(element, "", "", 0);
	element->canSelect = 0;
	element->elementType = 2;
	element->bottomMargin = 8;
	element->vftable = (void*)0x00536094;
	EnterIntoList((void*)(&menuInfo->ListInput), (void*)(element));
}

//wrapper for call to FUN_00429b00
void _FUN_00429b00(MenuInfo* menuInfo, const char* label) {
	//menuInfo should be ecx
	//label should be stack[4]
	__asm {
		mov ecx, menuInfo;
		push label;
		call[MBAA_FUN_00429b00];
	}
}

//wrapper for call to FUN_004804a0
void _FUN_004804a0(MenuWindow* menuWindow) {
	//menuWindow should be eax
	__asm {
		mov eax, menuWindow;
		call[MBAA_FUN_004804a0];
	}
}

void InitExtendedSettingsMenuInfo(MenuInfo* extendedInfo, MenuWindow* extendedWindow) {
	InitMenuInfo(extendedInfo, extendedWindow, "TRAINING_EXTENDEDSETTING_MENU");
	return;
}

void InitHotkeySettingsMenuInfo(MenuInfo* hotkeyInfo, MenuWindow* hotkeyWindow) {
	InitMenuInfo(hotkeyInfo, hotkeyWindow, "TRAINING_HOTKEYSETTING_MENU");
	return;
}

//wrapper for call to MBAA_GetElementPointer
Element* GetElementPointer(MenuInfo* menuInfo, const char* tag) {
	//menuInfo should be stack[4]
	//tag should be stack[8]
	Element* retElement;
	__asm {
		push tag;
		push menuInfo;
		call[MBAA_GetElementPointer];
		mov retElement, eax;
	}
	return retElement;
}

//get a single setting from persistent location
bool GetSetting(MenuInfo* menuInfo, int* setting, const char* tag) {
	Element* element = GetElementPointer(menuInfo, tag);
	if (element != 0x0 && element->GetItemListSize() != 0x0) {
		int iterator = 0;
		while (true) {
			int CurItemValue = element->GetItemValue(iterator);
			if (CurItemValue == *setting) break;
			iterator++;
			if (element->GetItemListSize() <= iterator) return false;
		}
		element->selectedItem = iterator;
		return true;
	}
	return false;
}

//get settings from persistent locations to init menu window
void GetExtendedSettings(MenuWindow* extendedWindow) {
	if (extendedWindow->MenuInfoList == 0x0 || extendedWindow->MenuInfoListEnd - extendedWindow->MenuInfoList == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}
	char tempTag[8];
	MenuInfo* extendedInfo;
	for (int pageNum = 0; pageNum < size(Page_Options); pageNum++) {
		extendedInfo = extendedWindow->MenuInfoList[pageNum];
		int settingNum = 0;
		for (int elementNum = 0; elementNum < size(Page_Options[pageNum]); elementNum++) {
			snprintf(tempTag, 8, "%i_%i_0", pageNum, elementNum);
			if (GetSetting(extendedInfo, Page_Settings[pageNum][settingNum], tempTag)) {
				settingNum++;
			};
		}
	}
	
	extendedWindow->menuInfoIndex = nEXTENDED_SETTINGS_PAGE;
	for (int i = 0; i < size(Page_Options); i++) {
		extendedWindow->MenuInfoList[i]->selectedElement = nEXTENDED_SETTINGS_CURSOR[i];
	}
}

//get settings from persistent locations to init menu window
void GetHotkeySettings(MenuWindow* hotkeyWindow) {
	if (hotkeyWindow->MenuInfoList == 0x0 || hotkeyWindow->MenuInfoListEnd - hotkeyWindow->MenuInfoList == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}
	char tempTag[8];
	MenuInfo* hotkeyInfo;
	for (int pageNum = 0; pageNum < size(HK_Page_Options); pageNum++) {
		hotkeyInfo = hotkeyWindow->MenuInfoList[pageNum];
		int settingNum = 0;
		for (int elementNum = 0; elementNum < size(HK_Page_Options[pageNum]); elementNum++) {
			snprintf(tempTag, 8, "%i_%i_0", pageNum, elementNum);
			if (GetSetting(hotkeyInfo, HK_Page_Settings[pageNum][settingNum], tempTag)) {
				settingNum++;
			};
		}
	}

	hotkeyWindow->menuInfoIndex = nHOTKEY_SETTINGS_PAGE;
	for (int i = 0; i < size(HK_Page_Options); i++) {
		hotkeyWindow->MenuInfoList[i]->selectedElement = nHOTKEY_SETTINGS_CURSOR[i];
	}
}

//init extended window, info, elements, and items
MenuWindow* InitExtendedSettingsMenu(MenuWindow* extendedWindow) {
	InitMenuWindow(extendedWindow);
	ReadDataFile(&extendedWindow->labelBase, "EXTENDED SETTINGS", 18);
	MenuInfo* extendedInfo;
	for (int pageNum = 0; pageNum < size(Page_Options); pageNum++)
	{
		MenuInfo* extendedInfo = NEW_MENU_INFO();
		if (extendedInfo != 0x0)
		{
			InitExtendedSettingsMenuInfo(extendedInfo, extendedWindow);
		}
		for (int elementNum = 0; elementNum < size(Page_Options[pageNum]); elementNum++) {
			std::vector<const char*> elementVector = Page_Options[pageNum][elementNum];
			int vSize = size(elementVector);
			switch (vSize)
			{
			case 0:
				AddSpaceElement(extendedInfo, pageNum, elementNum);
				break;
			case 1:
				AddNormalElement(extendedInfo, elementVector, pageNum, elementNum);
				break;
			default:
				AddSelectElement(extendedInfo, elementVector, pageNum, elementNum);
				break;
			}
		}
		EnterIntoList((void*)(&extendedWindow->ListInput), (void*)(extendedInfo));
		//_FUN_00429b00(extendedInfo, "EXTENDED_SETTING");
	}

	GetExtendedSettings(extendedWindow);
	extendedWindow->dimScreenPercentage = 0.0;
	extendedWindow->u_layer = 0x2f1;
	extendedWindow->isMenuLit = 1;
	extendedWindow->isBlurred = 0;
	extendedWindow->paragraphMode = 2;
	extendedWindow->xOffset = 0xfa;
	extendedWindow->textXWidth = 0xe;
	return extendedWindow;
}

//init extended window, info, elements, and items
MenuWindow* InitHotkeySettingsMenu(MenuWindow* hotkeyWindow) {
	InitMenuWindow(hotkeyWindow);
	ReadDataFile(&hotkeyWindow->labelBase, "HOTKEY SETTINGS", 18);
	MenuInfo* hotkeyInfo;
	for (int pageNum = 0; pageNum < size(HK_Page_Options); pageNum++)
	{
		MenuInfo* hotkeyInfo = NEW_MENU_INFO();
		if (hotkeyInfo != 0x0)
		{
			InitHotkeySettingsMenuInfo(hotkeyInfo, hotkeyWindow);
		}
		for (int elementNum = 0; elementNum < size(HK_Page_Options[pageNum]); elementNum++) {
			std::vector<const char*> elementVector = HK_Page_Options[pageNum][elementNum];
			int vSize = size(elementVector);
			switch (vSize)
			{
			case 0:
				AddSpaceElement(hotkeyInfo, pageNum, elementNum);
				break;
			case 1:
				AddNormalElement(hotkeyInfo, elementVector, pageNum, elementNum);
				break;
			default:
				AddSelectElement(hotkeyInfo, elementVector, pageNum, elementNum);
				break;
			}
		}
		EnterIntoList((void*)(&hotkeyWindow->ListInput), (void*)(hotkeyInfo));
	}

	GetHotkeySettings(hotkeyWindow);
	hotkeyWindow->dimScreenPercentage = 0.0;
	hotkeyWindow->u_layer = 0x2f1;
	hotkeyWindow->isMenuLit = 1;
	hotkeyWindow->isBlurred = 0;
	hotkeyWindow->paragraphMode = 2;
	hotkeyWindow->xOffset = 0xfa;
	hotkeyWindow->textXWidth = 0xe;
	return hotkeyWindow;
}

void _FUN_0047d030(const char* TRAINING_XX_MENU, MenuWindow* menuWindow) {
	//TRAINING_XX_MENU should be eax
	//menuWindow should be esi
	__asm {
		mov eax, TRAINING_XX_MENU;
		mov esi, menuWindow;
		call[MBAA_FUN_0047d030];
	}
}

void _FUN_0047ce20(const char* TRAINING_XX_MENU, void* field24) {
	//TRAINING_XX_MENU should be ecx
	//field24 should be edx
	__asm {
		mov edx, field24;
		mov ecx, TRAINING_XX_MENU;
		call[MBAA_FUN_0047ce20];
	}

}

//Sets a single setting to its persistent location
bool SetSetting(MenuInfo* menuInfo, int* setting, const char* tag) {
	Element* element = GetElementPointer(menuInfo, tag);
	if (element != 0x0) {
		int selectionIndex = element->selectedItem;
		int value = element->ItemList[selectionIndex]->value;
		*setting = value;
		return true;
	}
	return false;
}

//save extended settings to persistent locations
void SetExtendedSettings(MenuWindow* extendedWindow) {
	if (extendedWindow->MenuInfoList == 0x0 || extendedWindow->MenuInfoListEnd - extendedWindow->MenuInfoList == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}
	char tempTag[8];
	MenuInfo* extendedInfo;
	for (int pageNum = 0; pageNum < size(Page_Options); pageNum++) {
		extendedInfo = extendedWindow->MenuInfoList[pageNum];
		int settingNum = 0;
		for (int elementNum = 0; elementNum < size(Page_Options[pageNum]); elementNum++) {
			snprintf(tempTag, 8, "%i_%i_0", pageNum, elementNum);
			if (SetSetting(extendedInfo, Page_Settings[pageNum][settingNum], tempTag)) {
				settingNum++;
			};
		}
	}

	nEXTENDED_SETTINGS_PAGE = extendedWindow->menuInfoIndex;
	for (int i = 0; i < size(Page_Options); i++) {
		nEXTENDED_SETTINGS_CURSOR[i] = extendedWindow->MenuInfoList[i]->selectedElement;
	}
}

//save hotkey settings to persistent locations
void SetHotkeySettings(MenuWindow* hotkeyWindow) {
	if (hotkeyWindow->MenuInfoList == 0x0 || hotkeyWindow->MenuInfoListEnd - hotkeyWindow->MenuInfoList == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}
	char tempTag[8];
	MenuInfo* hotkeyInfo;
	for (int pageNum = 0; pageNum < size(HK_Page_Options); pageNum++) {
		hotkeyInfo = hotkeyWindow->MenuInfoList[pageNum];
		int settingNum = 0;
		for (int elementNum = 0; elementNum < size(HK_Page_Options[pageNum]); elementNum++) {
			snprintf(tempTag, 8, "%i_%i_0", pageNum, elementNum);
			if (SetSetting(hotkeyInfo, HK_Page_Settings[pageNum][settingNum], tempTag)) {
				settingNum++;
			};
		}
	}

	nHOTKEY_SETTINGS_PAGE = hotkeyWindow->menuInfoIndex;
	for (int i = 0; i < size(HK_Page_Options); i++) {
		nHOTKEY_SETTINGS_CURSOR[i] = hotkeyWindow->MenuInfoList[i]->selectedElement;
	}
}

//save extended settings and free everything
void CloseExtendedSettings(MenuWindow* extendedWindow) {
	SetExtendedSettings(extendedWindow);

	__asm {
		mov ecx, extendedWindow;
	}
	PUSH_ALL;
	__asm {
		mov eax, dword ptr[ecx];
		mov edx, dword ptr[eax];
		push 1;
		call edx;
	}
	POP_ALL;
}

//save hotkey settings and free everything
void CloseHotkeySettings(MenuWindow* hotkeyWindow) {
	SetHotkeySettings(hotkeyWindow);

	__asm {
		mov ecx, hotkeyWindow;
	}
	PUSH_ALL;
	__asm {
		mov eax, dword ptr[ecx];
		mov edx, dword ptr[eax];
		push 1;
		call edx;
	}
	POP_ALL;
}

//init extended menu if not already open, close and free if set to close
void HandleExtendedMenu() {
	if (mainWindow->ExtendedSettings == 0x0) {
		mainWindow->isMenuLit = 0;
		MenuWindow* extendedWindow = NEW_MENU_WINDOW();
		if (extendedWindow != 0x0) {
			extendedWindow = InitExtendedSettingsMenu(extendedWindow);
		}
		mainWindow->ExtendedSettings = extendedWindow;
		extendedWindow->yOffset = 0xbe;
		//_FUN_0047d030("TRAINING_XS_MENU", mainWindow->ExtendedSettings);
	}
	if (mainWindow->ExtendedSettings->openSubmenuIndex == 4) {
		//_FUN_0047ce20("TRAINING_XS_MENU", &mainWindow->ExtendedSettings->hoveredTag);
		if (mainWindow->ExtendedSettings->field_0x50 == 2) {
			mainWindow->field_0x50 = 2;
		}
		if (mainWindow->ExtendedSettings != 0x0) {
			CloseExtendedSettings(mainWindow->ExtendedSettings);
			mainWindow->ExtendedSettings = 0x0;
		}

		mainWindow->isMenuLit = 0x1;
		mainWindow->isRootMenu = 0x1;
		mainWindow->timeSubmenuOpened = 0x0;
		mainWindow->openSubmenuIndex = 0x2;
	}

}

void ExtendedMenuInputChecking() {
	MenuWindow* extendedWindow;
	__asm {
		mov extendedWindow, ecx;
	}
	switch (extendedWindow->menuInfoIndex) {
	case 0:
		break;
	case 1:
		break;
	}

	bool CurFN1Input = *(bool*)(adMBAABase + adP1FN1Input);
	if (CurFN1Input && !bOldFN1Input) {
		extendedWindow->menuInfoIndex--;
		if (extendedWindow->menuInfoIndex < 0) {
			extendedWindow->menuInfoIndex = size(Page_Options) - 1;
		}
	}
	bOldFN1Input = CurFN1Input;

	bool CurFN2Input = *(bool*)(adMBAABase + adP1FN2Input);
	if (CurFN2Input && !bOldFN2Input) {
		extendedWindow->menuInfoIndex++;
		if (extendedWindow->menuInfoIndex > size(Page_Options) - 1) {
			extendedWindow->menuInfoIndex = 0;
		}
	}
	bOldFN2Input = CurFN2Input;
}

//init hotkey menu if not already open, close and free if set to close
void HandleHotkeyMenu() {
	if (mainWindow->HotkeySettings == 0x0) {
		mainWindow->isMenuLit = 0;
		MenuWindow* hotkeyWindow = NEW_MENU_WINDOW();
		if (hotkeyWindow != 0x0) {
			hotkeyWindow = InitHotkeySettingsMenu(hotkeyWindow);
		}
		mainWindow->HotkeySettings = hotkeyWindow;
		hotkeyWindow->yOffset = 0xbe;
	}
	if (mainWindow->HotkeySettings->openSubmenuIndex == 4) {
		if (mainWindow->HotkeySettings->field_0x50 == 2) {
			mainWindow->field_0x50 = 2;
		}
		if (mainWindow->HotkeySettings != 0x0) {
			CloseHotkeySettings(mainWindow->HotkeySettings);
			mainWindow->HotkeySettings = 0x0;
		}

		mainWindow->isMenuLit = 0x1;
		mainWindow->isRootMenu = 0x1;
		mainWindow->timeSubmenuOpened = 0x0;
		mainWindow->openSubmenuIndex = 0x2;
	}

}

void HotkeyMenuInputChecking() {
	MenuWindow* hotkeyWindow;
	__asm {
		mov hotkeyWindow, ecx;
	}
	switch (hotkeyWindow->menuInfoIndex) {
	case 0:
		break;
	case 1:
		break;
	}

	bool CurFN1Input = *(bool*)(adMBAABase + adP1FN1Input);
	if (CurFN1Input && !bOldFN1Input) {
		hotkeyWindow->menuInfoIndex--;
		if (hotkeyWindow->menuInfoIndex < 0) {
			hotkeyWindow->menuInfoIndex = size(HK_Page_Options) - 1;
		}
	}
	bOldFN1Input = CurFN1Input;

	bool CurFN2Input = *(bool*)(adMBAABase + adP1FN2Input);
	if (CurFN2Input && !bOldFN2Input) {
		hotkeyWindow->menuInfoIndex++;
		if (hotkeyWindow->menuInfoIndex > size(HK_Page_Options) - 1) {
			hotkeyWindow->menuInfoIndex = 0;
		}
	}
	bOldFN2Input = CurFN2Input;
}

DWORD ExtendedSettingsMenuItem_PatchAddr = 0x0047d493;
//add extended menu element to main menu info
__declspec(naked) void _naked_ExtendedSettingsMenuItem() {
	__asm {
		push 0x58;
		call[MBAA_operator_new];
		mov edi, eax;
		add esp, 0x04;
		mov dword ptr[esp + 0x14], edi;
		test edi, edi;
		mov byte ptr[esp + 0x68], 0x04;
		je _NOINIT;

		push 0x00;
		push pcEXTENDED_SETTING;
		push pcEXTENDED_SETTINGS;
		push edi;
		call[MBAA_InitNormalElement];
		mov dword ptr[edi + 0x0c], 0x00000001;
		mov dword ptr[edi + 0x04], 0x00000001;
		mov dword ptr[edi], 0x0053604c;

		jmp _END;

	_NOINIT:
		xor edi, edi;

	_END:
		mov byte ptr[esp + 0x68], 0x01;
		lea ebx, [esp + 0x14];
		mov esi, ebp;
		mov[esp + 0x14], edi;
		call[MBAA_EnterIntoList];

		push 0x58;
		call[MBAA_operator_new];
		push 0x0047d49a;
		ret;
	}
}

DWORD HotkeySettingsMenuItem_PatchAddr = 0x0047db08;
//add hotkey menu element to main menu info
__declspec(naked) void _naked_HotkeySettingsMenuItem() {
	__asm {
		push 0x58;
		call[MBAA_operator_new];
		mov edi, eax;
		add esp, 0x04;
		mov dword ptr[esp + 0x14], edi;
		test edi, edi;
		mov byte ptr[esp + 0x68], 0x04;
		je _NOINIT;

		push 0x00;
		push pcHOTKEY_SETTING;
		push pcHOTKEY_SETTINGS;
		push edi;
		call[MBAA_InitNormalElement];
		mov dword ptr[edi + 0x0c], 0x00000001;
		mov dword ptr[edi + 0x04], 0x00000001;
		mov dword ptr[edi], 0x0053604c;

		jmp _END;

	_NOINIT:
		xor edi, edi;

	_END:
		mov byte ptr[esp + 0x68], 0x01;
		lea ebx, [esp + 0x14];
		mov esi, ebp;
		mov[esp + 0x14], edi;
		call[MBAA_EnterIntoList];

		push 0x58;
		call[MBAA_operator_new];
		push 0x0047db0f;
		ret;
	}
}

DWORD TrainingMenuSwitch_PatchAddr = 0x0047ee54;
//Add check for extended menu to default menu switch case
__declspec(naked) void _naked_TrainingMenuSwitch() {
	__asm {
		cmp eax, 0xe;
		je _DOEXTENDEDSETTINGS;
		cmp eax, 0xf;
		je _DOHOTKEYSETTINGS;
		mov ecx, ebp;
		call[MBAA_FUN_004d8810];
		jmp _BREAKSWITCH;

	_DOEXTENDEDSETTINGS:
		mov mainWindow, ebp;
	}

	PUSH_ALL;
	HandleExtendedMenu();
	POP_ALL;

	__asm {
		jmp _BREAKSWITCH;

	_DOHOTKEYSETTINGS:
		mov mainWindow, ebp;
	}

	PUSH_ALL;
	HandleHotkeyMenu();
	POP_ALL;

	__asm {
	_BREAKSWITCH:
		push 0x0047ee5b;
		ret;
	}
}

DWORD AddExtendedSettingToList_PatchAddr = 0x0047d1ae;
//enables the menu option to be selected
__declspec(naked) void _naked_AddExtendedSettingToList() {
	__asm {
		push esi;
		mov edx, pcEXTENDED_SETTING;
		call[MBAA_FUN_00429020];

		push esi;
		mov edx, 0x005384ac;

		push 0x0047d1b4;
		ret;
	}
}

DWORD AddHotkeySettingToList_PatchAddr = 0x0047d1f0;
//enables the menu option to be selected
__declspec(naked) void _naked_AddHotkeySettingToList() {
	__asm {
		push esi;
		mov edx, pcHOTKEY_SETTING;
		call[MBAA_FUN_00429020];

		push esi;
		mov edx, 0x00537718;

		push 0x0047d1f6;
		ret;
	}
}

DWORD CheckOpenNewSettings_PatchAddr = 0x0047ee5b;
//add extended menu to open check if-chain
__declspec(naked) void _naked_CheckOpenNewSettings() {
	__asm {
		cmp dword ptr[ebp + 0x1c], ebx;
		je _NO_PRESS;
		lea edi, [ebp + 0x20];
		mov ecx, pcEXTENDED_SETTING;
		call[MBAA_CompareLabel];
		test al, al;
		je _HOTKEY_CHECK;
		mov dword ptr[ebp + 0x7c], ebx;
		mov dword ptr[ebp + 0x88], ebx;
		mov dword ptr[ebp + 0x84], 0x11;
		jmp _SET_HOVERED;

	_HOTKEY_CHECK:
		mov ecx, pcHOTKEY_SETTING;
		call[MBAA_CompareLabel];
		test al, al;
		je _BATTLE_CHECK;
		mov dword ptr[ebp + 0x7c], ebx;
		mov dword ptr[ebp + 0x88], ebx;
		mov dword ptr[ebp + 0x84], 0x12;
		jmp _SET_HOVERED;

	_BATTLE_CHECK:
		push 0x0047ee67;
		ret;

	_SET_HOVERED:
		push 0x0047ef96;
		ret;

	_NO_PRESS:
		push 0x0047f062;
		ret;
	}
}

DWORD UpdateMenus_PatchAddr = 0x0047e1da;
//add menus to update if-chain
__declspec(naked) void _naked_UpdateMenus() {
	__asm {
		cmp dword ptr[edi + 0xe0], ebx;
		jz _CHECK_HOTKEY;
		mov eax, dword ptr[esp + 0x64];
		mov ecx, dword ptr[edi + 0xe0];
		mov edx, dword ptr[ecx];
		mov edx, dword ptr[edx + 0x8];
		push eax;
		mov eax, dword ptr[esp + 0x64];
		push eax;
		call edx; //u_InitSubmenus
	}

	PUSH_ALL;
	ExtendedMenuInputChecking();
	POP_ALL;

	__asm {
		jmp _EXIT_CHAIN;

	_CHECK_HOTKEY:
		cmp dword ptr[edi + 0xe4], ebx;
		jz _CHECK_BATTLE;
		mov eax, dword ptr[esp + 0x64];
		mov ecx, dword ptr[edi + 0xe4];
		mov edx, dword ptr[ecx];
		mov edx, dword ptr[edx + 0x8];
		push eax;
		mov eax, dword ptr[esp + 0x64];
		push eax;
		call edx; //u_InitSubmenus
	}

	PUSH_ALL;
	HotkeyMenuInputChecking();
	POP_ALL;

	__asm {
	_CHECK_BATTLE:
		cmp dword ptr[edi + 0xc8], ebx;
		jz _CHECK_ENEMY;
		push 0x0047e1e2;
		ret;

	_CHECK_ENEMY:
		push 0x0047e1fb;
		ret;

	_EXIT_CHAIN:
		push 0x0047e27a;
		ret;
	}
}

DWORD RenderSettings_PatchAddr = 0x0047e52a;
//add menus to render if-chain
__declspec(naked) void _naked_RenderSettings() {
	__asm {
		cmp dword ptr[edi + 0xe0], 0x0;
		jz _CHECK_HOTKEY;
		mov ecx, dword ptr[edi + 0xe0];
		mov edx, dword ptr[ecx];
		mov eax, dword ptr[edx + 0x14];
		call eax;

	_CHECK_HOTKEY:
		cmp dword ptr[edi + 0xe4], 0x0;
		jz _EXIT;
		mov ecx, dword ptr[edi + 0xe4];
		mov edx, dword ptr[ecx];
		mov eax, dword ptr[edx + 0x14];
		call eax;

	_EXIT:
		pop edi;
		add esp, 0xc;
		ret;

	}
}

DWORD ZeroMenuPointers_PatchAddr = 0x0047dd50;
//need to zero extended menu pointer on menu close
__declspec(naked) void _naked_ZeroMenuPointers() {
	__asm {
		mov[edi + 0xc4], ebx;
		mov[edi + 0xc8], ebx;
		mov[edi + 0xcc], ebx;
		mov[edi + 0xd0], ebx;
		mov[edi + 0xd4], ebx;
		mov[edi + 0xd8], ebx;
		mov[edi + 0xe0], ebx;
		mov[edi + 0xe4], ebx;
		push 0x0047dd74;
		ret;
	}
}

DWORD NewExtMainMenuWindow_PatchAddr = 0x0047815b;
//allocate space for extended menu pointer (size from 0xe0 -> 0xe8)
__declspec(naked) void _naked_NewExtMainMenuWindow() {
	__asm {
		push 0xe8;
		call[MBAA_operator_new];

		push 0x00478165;
		ret;
	}
}

//END MENU TESTS

DWORD _naked_DisableShadows_FuncAddr = 0x0041a390;
__declspec(naked) void _naked_DisableShadows() {
	__asm {

		//cmp shouldDrawBackground, 0;
		cmp shouldDrawShadow, 0;
		JE _SKIP;

		call[_naked_DisableShadows_FuncAddr];
	
	_SKIP:

		push 0041b481h;
		ret;
	}
}

// attack display funcs

void doFastReversePenalty() {

	if (fastReversePenalty == 0) {
		return;
	}

	static DWORD prevComboStates[4];

	for (int i = 0; i < 4; i++) {
		DWORD temp = *(DWORD*)(0x00555130 + 0x64 + (i * 0xAFC));
		if (temp == 1 && prevComboStates[i] == 0) {
			// reset reverse penalty to 0 on all chars
			// a DWORD is used to reset both the counter and the actual value
			if (i & 1) { // P2, reset P1
				*(DWORD*)(0x0055531A + (0 * 0xAFC)) = 0;
				*(DWORD*)(0x0055531A + (2 * 0xAFC)) = 0;
			}
			else { // P1, reset P2
				*(DWORD*)(0x0055531A + (1 * 0xAFC)) = 0;
				*(DWORD*)(0x0055531A + (3 * 0xAFC)) = 0;
			}
		}
		prevComboStates[i] = temp;
	}
}

int nTempP1MeterGain = 0;
int nTempP2MeterGain = 0;
int nP1MeterGain = 0;
int nP2MeterGain = 0;
DWORD prevComboPtr = 0;
void attackMeterDisplayCallback()
{


	/*
	static char buffer[256];

	drawTextWithBorder(420, 186, 10, 14, "P1 METER GAIN");
	snprintf(buffer, 256, "%4d.%02d%%", nP1MeterGain / 100, nP1MeterGain % 100);
	drawTextWithBorder(420 + 130, 186, 10, 14, buffer);

	drawTextWithBorder(420, 186 + 14, 10, 14, "P2 METER GAIN");
	snprintf(buffer, 256, "%4d.%02d%%", nP2MeterGain / 100, nP2MeterGain % 100);
	drawTextWithBorder(420 + 130, 186 + 14, 10, 14, buffer);
	*/
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

DWORD newAttackDisplay_local_14c;
void newAttackDisplay() {

	// recreate local_154
	DWORD local_154 = 0;
	local_154 = *(BYTE*)0x0055df0f; // 00478bda
	local_154 *= 0x20C; // 00478be7
	local_154 += 0x00557db8; // 00478bed

	// recreate local_150
	DWORD local_150 = 0x00555134 + ((*(DWORD*)(local_154)) * 0xAFC);

	unsigned invalidComboVal = *(DWORD*)(newAttackDisplay_local_14c + 0);
	unsigned validComboVal = *(DWORD*)(newAttackDisplay_local_14c + 4);

	unsigned scaledDamageVal = *(DWORD*)(newAttackDisplay_local_14c + 8);
	unsigned unscaledDamageVal = *(DWORD*)(newAttackDisplay_local_14c + 12);

	unsigned correctionValue = *(DWORD*)(local_154 + 0x20);
	float reversePenalty = (*(short*)(local_150 + 0x1E6) * 55.0f) / 10000.0f;

	// taken back from attackMeterDisplayCallback
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

	if (correctionValue == 0) {
		correctionValue = 100;
	}

	float xVal = 440.0f;

	if (*(BYTE*)(0x05585f8)) { // input display is active
		xVal -= 100.0f;
	}

	TextDraw(xVal, 122,      12, 0xFFFFFFFF, "COMBO%9d(%5d)", invalidComboVal, validComboVal);
	TextDraw(xVal, 122 + 12, 12, 0xFFFFFFFF, "DAMAGE%8d(%5d)", scaledDamageVal, unscaledDamageVal);
			 
	TextDraw(xVal, 152,      12, 0xFFFFFFFF, "CORRECTION VALUE%4d%%", correctionValue);
	TextDraw(xVal, 152 + 12, 12, 0xFFFFFFFF, "REVERSE PENALTY%5.1f%%", reversePenalty);
			 
	TextDraw(xVal, 182,      12, 0xFFFFFFFF, "P1 METER GAIN%4d.%02d%%", nP1MeterGain / 100, nP1MeterGain % 100);
	TextDraw(xVal, 182 + 12, 12, 0xFFFFFFFF, "P2 METER GAIN%4d.%02d%%", nP2MeterGain / 100, nP2MeterGain % 100);

}

__declspec(naked) void _naked_newAttackDisplay() {
	// there are some last minute things that need to be done here before we can move on to draw code
	// for reasons unknown, they are also interlaied with random pushes for the next call. why? idek 
	// recreating the code without the pushes here
	// and while i am aware, that i could trust the assembler to do it for me, i dont, so its raw bytes again (maybe);
	
	__asm {
		add esp, 014h; // cleanup the prev stack
		mov newAttackDisplay_local_14c, esi;
	}

	PUSH_ALL;
	newAttackDisplay();
	POP_ALL;

	// overwritten code
	/*
	
	*/

	__asm {
		//push 00478c3eh;
		push 00478fcfh;
		ret;
	}

}

// input display funcs

__declspec(naked) void _naked_dualInputDisplay() {

	//PUSH_ALL;
	//dualInputDisplay();
	//POP_ALL;

	emitJump(0x00477f25);
}

// reset funcs

void __stdcall battleResetCallback()
{
	nTempP1MeterGain = 0;
	nTempP2MeterGain = 0;
	nP1MeterGain = 0;
	nP2MeterGain = 0;
	prevComboPtr = 0;

	dualInputDisplayReset();

	TASManagerObj.load("TAS.txt");

	loadCustomShader();

}

__declspec(naked) void _naked_battleResetCallback() {
	PUSH_ALL;
	battleResetCallback();
	POP_ALL;
	__asm {
		ret;
	}
}

// input funcs
bool needTrainingModeReset = false;
void inputCallback() {

	// does melty update controllers in a thread? and is this inside said thread?

	profileFunction();

	KeyState::updateControllers(); // this call is taking half a ms, and wtf why am i even caring
		
	TASManagerObj.setInputs();
	replayManager.setInputs();

	if (needTrainingModeReset) {
		needTrainingModeReset = false;

		PUSH_ALL;
		emitCall(0x00478590);
		POP_ALL;
	}

}

__declspec(naked) void _naked_inputCallback() {

	PUSH_ALL;
	inputCallback();
	POP_ALL;

	__asm {
		add esp, 0090h;
		ret;
	}

}

__declspec(naked) void _naked_showCssHook() {

	__asm {

		cmp showCSS, 1;

		JE continueToShowCSS;

		ret 4;

	continueToShowCSS:
		// overwritten asm
		push ebp;
		mov ebp, esp;
		and esp, 0fffffff8h;

	}

	emitJump(0x0048bb86);

}

// init funcs

void initFrameDoneCallback()
{
	// caster hooks the start of this func. ill have to hook the end of it 
	void* patchAddr = (void*)0x00432e2b;
	patchJump(patchAddr, nakedFrameDoneCallback);

}

void initHighlightHook() 
{
	void* funcAddress = (void*)0x0045f650;
	// backup
	patchMemcpy(arrAnimHookBytesOrig, funcAddress, 10);
	// new bytes
	patchFunction(funcAddress, highlightHookFunc);
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

	/*
	
	for reasons only known to madsci, caster does some wack things in regards to fps, busy waiting, the whole 9 yards of ugh
	i do not want to fuck with this shit,, hell if i were to release a caster update this is what i would fix
	but like,,, omfg
	the fps value is so fucking weird

	*/

	// make presentFrameEnd just ret
	//patchByte(dwCasterBaseAddress + 0x6638c020, 0xC3);

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
	patchFunction(funcAddr, _naked_battleResetCallback);

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

	patchFunction(addr, legacyPauseCallback);

	// the call being patched is 6 bytes long, patch the extra byte with a nop
	patchByte((BYTE*)addr + 5, 0x90);

}

void initNewPauseCallback() {
	
	patchJump(0x004794c4, _naked_pauseInputDisplay2);

	patchJump(0x004235d1, _naked_newPauseCallback2);

	patchJump(0x0044c48e, _naked_pauseMenuProcessInput2);

	// ppl wanted to open the menu while paused, so not doing this anymore	
	//patchJump(0x0044c7b0, _naked_trigPauseHook);

	//patchJump(0x00477eb3, _naked_preventPauseReset);

	//patchJump(0x00453d57, _naked_updateEffectsPause);
	patchJump(0x00453b92, _naked_updateEffectsPauseLoop1);
	patchJump(0x00453d60, _naked_updateEffectsPauseLoop2);

	// IT IS CRITICAL THAT WE HOOK THE START, AND NOT OVERWRITE THE CALL OF THIS FUNC, IN ORDER TO PRESERVE RET ADDRS PROPERLY!
	//patchJump(0x00458e80, _naked_pauseHitEffectsHook);
	//patchJump(0x0045ca27, _naked_pauseHitEffectsCallback);

}

void initDrawBackground() {
	patchJump(0x004238c0, _naked_DrawBackground);
	//patchJump(0x0042389c, _naked_DrawHudText);
	patchJump(0x004238a6, _naked_DrawHud);
	patchJump(0x0042522d, _naked_DrawHudMeter);
	patchJump(0x0041b47c, _naked_DisableShadows);
}

void initNewAttackDisplay() {

	patchJump(0x00478c74, _naked_newAttackDisplay);

}

void initDualInputDisplay() {

	patchJump(0x00477f17, _naked_dualInputDisplay);
	patchMemset(0x00477f20, 0x90, 5); 
}

void initInputCallback() {

	// called after controller is read, but not processed. i think?

	patchJump(0x0041f1a6, _naked_inputCallback);

}

void initTrainingMenu() {
	patchJump(ExtendedSettingsMenuItem_PatchAddr, _naked_ExtendedSettingsMenuItem);
	patchJump(HotkeySettingsMenuItem_PatchAddr, _naked_HotkeySettingsMenuItem);

	patchJump(AddExtendedSettingToList_PatchAddr, _naked_AddExtendedSettingToList);
	patchJump(AddHotkeySettingToList_PatchAddr, _naked_AddHotkeySettingToList);

	patchJump(RenderSettings_PatchAddr, _naked_RenderSettings);
	patchJump(UpdateMenus_PatchAddr, _naked_UpdateMenus);
	patchJump(CheckOpenNewSettings_PatchAddr, _naked_CheckOpenNewSettings);
	patchJump(NewExtMainMenuWindow_PatchAddr, _naked_NewExtMainMenuWindow);
	patchJump(TrainingMenuSwitch_PatchAddr, _naked_TrainingMenuSwitch);
	patchJump(ZeroMenuPointers_PatchAddr, _naked_ZeroMenuPointers);
}

void initCustomHealthRecover() {
	BYTE tempCode[6] = { 0x8b, 0x15, 0x00, 0x13, 0x78, 0x00 }; //put address of desired custom health amount as last 4 bytes here
	patchMemcpy(0x004242fa, tempCode, 6); //currently using adShareBase + 0x300 = 0x00781300
}

void initShowCssHook() {
	patchJump(0x0048bb80, _naked_showCssHook);
}

// dll thread func

void threadFunc() 
{
	srand(time(NULL));

	// make sure that caster has time to hook at the start
	Sleep(32);

	initLogFile();

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
	initHighlightHook();
	InitializeCharacterMaps();
	// when running with caster, the prints to this area are disabled
	// when not running with caster, they arent even there, so this is fine to run regardless of caster 
	initAttackMeterDisplay();
	initMeterGainHook();
	initBattleResetCallback();
	initNewAttackDisplay();

	initNewPauseCallback();
	initDrawBackground();

	initDualInputDisplay();

	initInputCallback();

	initCustomHealthRecover();

	initShowCssHook();

	//initTrainingMenu(); //uncomment for experimental new menu

	ReadFromRegistry(L"ShowDebugMenu", &showDebugMenu);

	//timeMeltyCall(0x0040d350, "GoesToGameLoop0");

	//timeMeltyCall(0x0040e410, "FUN_0048e0a0");
	//timeMeltyCall(0x0040e438, "linkedListAppend_MAYBE???");
	//timeMeltyCall(0x0040e471, "GoesToGameLoop1");
	//timeMeltyCall(0x0040e483, "FUN_0043b8d0");
	//timeMeltyCall(0x0040e494, "callsTheFpsAndEverything");
	//timeMeltyCall(0x0040e499, "callsImportantDraw1");
	//timeMeltyCall(0x0040e49e, "callsDirectXPresent2");
	//timeMeltyCall(0x0040e4a3, "somethingTimeRelated");
	//timeMeltyCall(0x0040e4a8, "FUN_004bf970");
	//timeMeltyCall(0x0040e4bd, "FUN_004151f0");
	//timeMeltyCall(0x0040e4fb, "FUN_00406680");
	//timeMeltyCall(0x0040e500, "FUN_004be8b0");
	//timeMeltyCall(0x0040e505, "FUN_0040e220");

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
