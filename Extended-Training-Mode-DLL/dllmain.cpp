


#include "..\Common\CharacterData.h"
#include "dllmain.h"
#include "FrameBar.h"
#include "SaveState.h"
#include "ReplayManager.h"
#include "TASManager.h"
#include "FancyMenu.h"
#include "TrainingMenu.h"
#include <filesystem>



#pragma push_macro("optimize")
#pragma optimize("t", on) 

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

TASManager TASManagerObj[4];

bool fn1Press2v2[4] = { false, false, false, false };
void doWeird2v2Fixes() {
	// FN1 needs to be written at a different point than it is. this is a horrid fix, that may result in the button press being 1f late, or more, but i just want this done
	 
	for (int i = 0; i < 4; i++) {
		if (fn1Press2v2[i]) {
			*(BYTE*)((*(DWORD*)0x76E6AC) + 0x25 + (i * 0x14)) = 1;
			fn1Press2v2[i] = false;
		}
	}
}

// have all pointers as DWORDS, or a goofy object type, fangs way of doing things was right as to not have pointers get incremented by sizeof(unsigned)
// or i could make all pointers u8*, but that defeats half the point of what i did
// or,, making address a class which allowed for CONST ONLY derefing 
// or i could allow for writing and integrate the virtualprotect stuff into it?

DWORD addrEndScene = 0x663fb900;
DWORD addrEndScenePatch = 0x663fb996;

bool bCasterInit = false;
DWORD dwCasterBaseAddress = 0;

LONG isWine;

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
int showReplayMenu = 0;
bool bFreeze = false;
bool bSlow = false;
int nInputHeldFrameAdvanceCounter = 0;
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
bool bShowFrameBarPreview = false;
bool bShowFrameBarYPreview = false;
bool bForceGuard = false;
int dummyDelayTechFramesElapsed = 0;
bool showFrameScrubber = false;
int comboTimer = 0;

bool initLoadChars = false;

struct TrueComboDamageData {
	int startingHealth = 11400;
	int damage = 0;
	ActorData* defender = nullptr;
};

TrueComboDamageData trueComboData[2][8];

ActorData* pDefPlayer = nullptr;
ActorData* pAttPlayer = nullptr;

bool nLastCustomInputDisplay = false;
bool nLastVanillaInputDisplay = false;

std::vector<std::string> vPatternNames = GetEmptyPatternList();
std::vector<int> vAirReversals;
std::vector<int> vGroundReversals;

int nP2CharacterID = 0;

int nSavedP1ActiveChar = 0;
int nSavedP2ActiveChar = 1;

DWORD _naked_newPauseCallback2_IsPaused = 0;

std::array<uint8_t, 4> arrDefaultHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrIdleHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrBlockingHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrHitHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrArmorHighlightSetting({ 255, 255, 255, 0 });
std::array<uint8_t, 4> arrThrowProtectionHighlightSetting({ 255, 255, 255, 0 });

PlayerData* pP1 = (PlayerData*)(adMBAABase + adP1Base);
PlayerData* pP2 = (PlayerData*)(adMBAABase + adP2Base);
PlayerData* pP3 = (PlayerData*)(adMBAABase + adP3Base);
PlayerData* pP4 = (PlayerData*)(adMBAABase + adP4Base);

PlayerAuxData* pdP1Data = (PlayerAuxData*)(adMBAABase + adP1DataBase);
PlayerAuxData* pdP2Data = (PlayerAuxData*)(adMBAABase + adP2DataBase);
PlayerAuxData* pdP3Data = (PlayerAuxData*)(adMBAABase + adP3DataBase);
PlayerAuxData* pdP4Data = (PlayerAuxData*)(adMBAABase + adP4DataBase);

PlayerData* pActiveP1 = pP1;
PlayerData* pActiveP2 = pP2;
bool isP1Controlled = 1;
PlayerData* pPlayer = pP1;
PlayerData* pDummy = pP2;

void initRegistryValues()
{
	ReadFromRegistry(sP1_LIST_INPUT_X, &fP1_LIST_INPUT_X);
	ReadFromRegistry(sP1_LIST_INPUT_Y, &fP1_LIST_INPUT_Y);
	ReadFromRegistry(sP2_LIST_INPUT_X, &fP2_LIST_INPUT_X);
	ReadFromRegistry(sP2_LIST_INPUT_Y, &fP2_LIST_INPUT_Y);
	ReadFromRegistry(sP1_ARCADE_INPUT_X, &fP1_ARCADE_INPUT_X);
	ReadFromRegistry(sP1_ARCADE_INPUT_Y, &fP1_ARCADE_INPUT_Y);
	ReadFromRegistry(sP2_ARCADE_INPUT_X, &fP2_ARCADE_INPUT_X);
	ReadFromRegistry(sP2_ARCADE_INPUT_Y, &fP2_ARCADE_INPUT_Y);

	ReadFromRegistry(sFRAME_BAR_X, &frameBar.x);
	ReadFromRegistry(sFRAME_BAR_Y, &frameBar.y);
	int backCompY;
	ReadFromRegistry(sFRAME_BAR_Y, &backCompY);
	if (backCompY > 0 && backCompY < 480) {
		frameBar.y = 410.0f;
	}
	ReadFromRegistry(sFRAME_BAR_W, &frameBar.w);
	ReadFromRegistry(sFRAME_BAR_H, &frameBar.h);
	ReadFromRegistry(sFRAME_BAR_NUMCELLS, &frameBar.numCells);
	ReadFromRegistry(sFRAME_BAR_CELLWIDTH, &frameBar.cellWidth);

	ReadFromRegistry(sDISPLAY_CURSOR, &enableCursor);
	ReadFromRegistry(sDEBUG_MENU_COMPACT_VIEW, &compactView);
}

void initSharedValues()
{
	*(byte*)(adMBAABase + adXS_frameData) = XS_consoleData;
	*(byte*)(adMBAABase + adXS_showHitstopAndFreeze) = XS_showHitstopAndFreeze;
	*(byte*)(adMBAABase + adXS_showInputs) = XS_showInputs;
	*(byte*)(adMBAABase + adXS_showCancel) = XS_showCancelWindows;
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

void QueueTrainingReset()
{
	*(byte*)(0x0055dec3) = 0xFF;
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
#include "DirectXHook.h"
//#include "RendererModifications.h"

void initPaletteLoadPatches();

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

	int xPos = effect->subObj.xPos + effect->subObj.thrownXOffset;
	int yPos = effect->subObj.yPos + effect->subObj.thrownYOffset;
	bool facingLeft = effect->subObj.facingLeft;
	
	AnimationData* animationData = effect->subObj.animationDataPtr;
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
	if (XS_originStyle != 0) {
		if (playerIndex % 2 == 0) boxType = BoxType::ExtendedP1Origin;
		else boxType = BoxType::ExtendedP2Origin;
	}

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

	bool bIsThrow = false;
	if (!isProjectile && animationData->highestIFIndex > 0) {
		for (int i = 0; i < animationData->highestIFIndex; i++) {
			if (animationData->IFs[i] != 0 && animationData->IFs[i]->IFTP == 52) {
				bIsThrow = true;
			}
		}

		if (bIsThrow) //Throw boxes
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

int getPatternFromInput(PlayerData* PD, const char input[20])
{
	char buffer[20];
	char inputCopy[20];
	char readableInput[20] = {0};
	for (int i = 0; i < PD->cmdFileDataPtr->cmdDataPtr->count; i++)
	{
		if (PD->cmdFileDataPtr->cmdDataPtr->array[i] != 0)
		{
			int length = 0;
			snprintf(inputCopy, 20, "%s", PD->cmdFileDataPtr->cmdDataPtr->array[i]->input);
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
				return PD->cmdFileDataPtr->cmdDataPtr->array[i]->pattern;
			}
		}
	}
	return -1;
}

CommandData* getCommandDataFromInput(PlayerData* PD, const char input[20]) {
	char buffer[20];
	char inputCopy[20];
	char readableInput[20] = { 0 };
	for (int i = 0; i < PD->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (PD->cmdFileDataPtr->cmdDataPtr->array[i] != 0) {
			int length = 0;
			snprintf(inputCopy, 20, "%s", PD->cmdFileDataPtr->cmdDataPtr->array[i]->input);
			for (int j = 0; j < 20; j++) {
				if (inputCopy[j] == '\xFF') {
					readableInput[j] = 0;
					length = j;
					break;
				} else if (inputCopy[j] >= 0x0 && inputCopy[j] <= 0x9) {
					readableInput[j] = inputCopy[j] + 0x30;
				} else {
					readableInput[j] = inputCopy[j];
				}
			}

			bool isMatch = true;
			for (int k = 0; k < length; k++) {
				if (readableInput[k] != input[k]) isMatch = false;
			}
			if (isMatch) {
				return PD->cmdFileDataPtr->cmdDataPtr->array[i];
			}
		}
	}
	return NULL;
}

int getIDFromPattern(PlayerData* playerData, int nPattern, int nthMatch = 1)
{
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern == nPattern) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

int getIDFromCmd(PlayerData* playerData, const char* cmd, int nthMatch = 1) {
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (strcmp(cmd, playerData->cmdFileDataPtr->cmdDataPtr->array[i]->input) == 0) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

int getPatternFromCmd(PlayerData* playerData, const char* cmd, int stance = 7, int nthMatch = 1) {
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (strcmp(cmd, playerData->cmdFileDataPtr->cmdDataPtr->array[i]->input) == 0 &&
				playerData->cmdFileDataPtr->cmdDataPtr->array[i]->flagsets[0] & stance) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

DWORD MBAA_CheckCmdVars = 0x0046d430;
bool checkCmdVars(PlayerData* playerData, int ID) {
	__asm {
		mov ecx, ID;
		mov eax, playerData;
		call[MBAA_CheckCmdVars];
	}
	return;
}

DWORD MBAA_CheckValidCommandConditions = 0x0046cea0;
bool tryCmdID(PlayerData* playerData, int ID) {
	__asm {
		push ID;
		mov eax, playerData;
		call[MBAA_CheckValidCommandConditions];
		add esp, 0x4;
	}
	return;
}

bool tryCmdPattern(PlayerData* playerData, int nPattern) {
	if (nPattern < 41) {
		return false;
	}
	else if (nPattern == 260 || nPattern == playerData->cmdFileDataPtr->groundThrowPat) {
		playerData->subObj.targetPattern = nPattern;
	}
	int id = -1;
	int meterMem = 0;
	bool didCmd = false;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern == nPattern) {
				id = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				if (checkCmdVars(playerData, id)) {
					meterMem = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend;
					playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend = 0;
					didCmd = tryCmdID(playerData, id);
					playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend = meterMem;
					if (didCmd) return true;
				}
			};
		}
	}
	return false;
}

DWORD MBAA_UniversalCommands = 0x004666b0;
void tryUnivCmd(PlayerData* playerData, byte buttons, byte directions) {
	__asm {
		mov ecx, playerData;
		add ecx, 0x4;
		push directions;
		push buttons;
		call[MBAA_UniversalCommands];
		add esp, 0x8;
	}
}

DWORD MBAA_SetBurstFlags = 0x00464390;
void SetBurstFlags(PlayerData* playerData) {
	__asm {
		mov ecx, playerData;
		add ecx, 0x4;
		call[MBAA_SetBurstFlags];
	}
}

bool tryBurst(PlayerData* playerData) {
	if (playerData->subObj.hitstunTimeRemaining != 0 && playerData->subObj.burstLock == 0 && playerData->subObj.hitstop == 0) {
		int stance = playerData->subObj.animationDataPtr->stateData->stance;
		if (stance == 0 || stance == 2) {
			if (playerData->subObj.hitstunTimeRemaining < 0) {
				return 0;
			}
		}
		else if (stance == 1) {
			if (9 < playerData->subObj.untechTimeElapsed) {
				return 0;
			}
			if (playerData->subObj.completedHitVectors != 0) {
				return 0;
			}
		}
		SetBurstFlags(playerData);
		return 1;
	}
	return 0;
}

//dir : input as numeric e.g. 236
//buttons : logical and, A = 1, B = 2, C = 4, D = 8
void setBufferCmd(PlayerData* playerData, WORD dir, WORD buttons) {
	WORD dirCount = (WORD)log10(dir);
	playerData->dirInputs[0] = dirCount;
	int i = dirCount * 2 + 1;
	while (dir > 0) {
		playerData->dirInputs[i] = dir % 10;
		playerData->dirInputs[i + 1] = 1;
		dir = dir / 10;
		i -= 2;
	}
	//WORD dirCount = dir[0] * 2 + 3;
	//memcpy(playerData->dirInputs, dir, dirCount * 0x2);
	playerData->aInputs[0] = 0;
	playerData->aInputs[1] = buttons & 1;
	playerData->aInputs[2] = 1;

	playerData->bInputs[0] = 0;
	playerData->bInputs[1] = buttons & 2;
	playerData->bInputs[2] = 1;

	playerData->cInputs[0] = 0;
	playerData->cInputs[1] = buttons & 4;
	playerData->cInputs[2] = 1;

	playerData->dInputs[0] = 0;
	playerData->dInputs[1] = buttons & 8;
	playerData->dInputs[2] = 1;
}

DWORD MBAA_GetHighestPriorityValidCommand = 0x0046d510;
bool tryBufferCmd(PlayerData* playerData) {
	__asm {
		mov eax, playerData;
		call[MBAA_GetHighestPriorityValidCommand];
	}
	return;
}

DWORD MBAA_ManageNormalAttacksAndMovements = 0x004666b0;
void tryNormalsAndMovement(ActorData* actorData, byte inputButton, byte inputDirection) {
	__asm {
		mov ecx, actorData;
		push inputDirection;
		push inputButton;
		call[MBAA_ManageNormalAttacksAndMovements];
	}
	return;
}

//returns AND of 1 for held shield and 2 for ex shield
byte getShieldCancel(PlayerData* playerData, int pat) {
	byte exOnly = playerData->subObj.moon == 0 ? 0x2 : 0x0;
	byte retVal = 0x0;
	if (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground ||
		pat == playerData->cmdFileDataPtr->ShieldCounter_Air ||
		pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch) {
		return 0x3;
	}
	if (pat <= 40 || //universal normals and movement
		pat == 260 || //heat activation
		pat == playerData->cmdFileDataPtr->groundThrowPat || //throws
		pat == playerData->cmdFileDataPtr->airThrowPat) return exOnly;
	int ID = getIDFromPattern(playerData, pat, 1);
	if (ID == -1) return 0x0;
	WORD flagsets = *(WORD*)playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->flagsets;
	int specialFlag = playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->specialFlag;
	if (flagsets & 0x8) { // if no cancel
		if (flagsets & 0x40 && !(flagsets & 0x1000)) { //if guard/shield cancel and not guard cancel only (covers D > D)
			retVal = 0x3;
		}
		else {
			retVal = 0x0;
		}
	}
	else if (specialFlag == 1 || specialFlag == 2) { //if special or ex
		retVal = 0x3;
	}
	else if (specialFlag == 0) { //if normal or movement
		retVal = exOnly;
	}
	
	ID = getIDFromPattern(playerData, pat, 2);
	if (ID == -1) return retVal;
	flagsets = *(WORD*)playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->flagsets;
	specialFlag = playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->specialFlag;
	if (flagsets & 0x8) {
		if (flagsets & 0x40 && !(flagsets & 0x1000)) {
			retVal = 0x3;
		}
		else {
			retVal = 0x0;
		}
	}
	else if (specialFlag == 1 || specialFlag == 2) {
		retVal |= 0x3;
	}
	else if (specialFlag == 0) {
		retVal |= exOnly;
	}
	return retVal;
}

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getCmdStance(PlayerData* playerData, int ID) {
	return playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->flagsets[0] & 0x7;
}

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getPatStance(PlayerData* playerData, int pat) {
	if ((0 <= pat && pat <= 6) || (35 <= pat && pat <= 37) || pat == 260 || pat == playerData->cmdFileDataPtr->groundThrowPat) { //ground universal normals, normal jumps, heat activation, ground throw
		return 0x5;
	}
	else if ((7 <= pat && pat <= 9) || (38 <= pat && pat <= 40) || pat == playerData->cmdFileDataPtr->airThrowPat) { //air universal normals, normal double jumps, air throw
		return 0x2;
	}
	else if (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground || pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch) {
		int retVal = 0x0;
		retVal += 0x1 * (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground);
		retVal += 0x4 * (pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch);
		return retVal;
	}
	else if (pat == playerData->cmdFileDataPtr->ShieldCounter_Air) {
		return 0x2;
	}
	int ID = getIDFromPattern(playerData, pat);
	return getCmdStance(playerData, ID);
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
	
	int nP1Health = pP1->subObj.health; // this works on maids too
	int nP1RedHealth = pP1->subObj.redHealth;
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

	snprintf(buffer, 256, "%5d", nP1Health);
	//drawTextWithBorder(230 - nResetOffset, 40, 10, 10, buffer);
	TextDraw(235 - nResetOffset, 40, 10, 0xFFFFFFFF, buffer);

	int nP2Health = pP2->subObj.health; // this works on maids too
	int nP2RedHealth = pP2->subObj.redHealth;
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

	snprintf(buffer, 256, "%5d", nP2Health);
	//drawTextWithBorder(366 + nResetOffset, 40, 10, 10, buffer);
	TextDraw(365 + nResetOffset, 40, 10, 0xFFFFFFFF, buffer);


	snprintf(buffer, 256, "%5.0f", pP1->subObj.guardGauge);
	//drawTextWithBorder(234 - nResetOffset, 58, 8, 9, buffer);
	TextDraw(242 - nResetOffset, 58, 8, 0xFFFFFFFF, buffer);
	snprintf(buffer, 256, "%1.3f", pP1->subObj.quardQuality);
	//drawTextWithBorder(244 - nResetOffset, 67, 6, 9, buffer);
	TextDraw(249 - nResetOffset, 67, 6, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "%5.0f", pP2->subObj.guardGauge);
	//drawTextWithBorder(368 + nResetOffset, 58, 8, 9, buffer);
	TextDraw(361 + nResetOffset, 58, 8, 0xFFFFFFFF, buffer);
	snprintf(buffer, 256, "%1.3f", pP2->subObj.quardQuality);
	//drawTextWithBorder(369 + nResetOffset, 67, 6, 9, buffer);
	TextDraw(367 + nResetOffset, 67, 6, 0xFFFFFFFF, buffer);

	
	// on p1 health bar
	drawRect(114.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(167.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(220.0f - nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);

	const float gutsMod[4] = { 32.0f / 32.0f, 31.0f / 32.0f, 30.0f / 32.0f, 29.0f / 32.0f };
	float extraMod = 1.0f;
	if (pP1->subObj.charID == (int)eCharID::MAIDS) { //if maids
		extraMod = 1.035f;
	}

	float* P1Guts = pActiveP1->cmdFileDataPtr->guts;

	float P1AdjGuts[4];

	for (int i = 0; i < 4; i++) {
		float curGuts = (int)(P1Guts[i] * 1000) / 1000.0f;
		curGuts = (int)(curGuts * extraMod * 1000) / 1000.0f;
		curGuts = (int)(curGuts * gutsMod[i] * 1000) / 1000.0f;
		P1AdjGuts[i] = curGuts;
	}

	if (nP1Health >= 8550)
	{
		drawRect(61.0f - nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(61.0f - nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(62 - nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[0]).c_str());
	}
	else
	{
		drawRect(61.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);			// on vertical bar
		drawRect(61.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(62 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[0]).c_str());
	}
	if (8550 > nP1Health && nP1Health >= 5700)
	{
		drawRect(114.0f - nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(114.0f - nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(115 - nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[1]).c_str());
	}
	else
	{
		drawRect(114.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(114.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(115 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[1]).c_str());
	}
	if (5700 > nP1Health && nP1Health >= 2850)
	{
		drawRect(167.0f - nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(167.0f - nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(168 - nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[2]).c_str());
	}
	else
	{
		drawRect(167.0f - nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(167.0f - nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(168 - nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[2]).c_str());
	}
	if (2850 > nP1Health)
	{
		drawRect(220.0f - nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(220.0f - nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(221 - nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[3]).c_str());
	}
	else
	{
		drawRect(220.0f - nResetOffset, 23.0f, 1.0f, 14.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(220.0f - nResetOffset, 23.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(221 - nResetOffset, 24, 6, 0xFFFFFFFF, std::format("{:.3f}", P1AdjGuts[3]).c_str());
	}


	// on p2 health bar
	drawRect(525.0f + nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(472.0f + nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);
	drawRect(419.0f + nResetOffset, 39.0f, 1.0f, 3.0f, 0xFF000000);

	extraMod = 1.0f;
	if (pP2->subObj.charID == (int)eCharID::MAIDS) { //if maids
		extraMod = 1.035f;
	}

	float* P2Guts = pActiveP2->cmdFileDataPtr->guts;

	float P2AdjGuts[4];

	for (int i = 0; i < 4; i++) {
		float curGuts = (int)(P2Guts[i] * 1000) / 1000.0f;
		curGuts = (int)(curGuts * extraMod * 1000) / 1000.0f;
		curGuts = (int)(curGuts * gutsMod[i] * 1000) / 1000.0f;
		P2AdjGuts[i] = curGuts;
	}

	if (nP2Health >= 8550)
	{
		drawRect(579.0f + nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(540.0f + nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(541 + nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[0]).c_str());
	}
	else
	{
		drawRect(579.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);			// on vertical bar
		drawRect(555.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(556 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[0]).c_str());
	}
	if (8550 > nP2Health && nP2Health >= 5700)
	{
		drawRect(525.0f + nResetOffset, 25.0f, 1.0f, 12.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(486.0f + nResetOffset, 25.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(486 + nResetOffset, 26, 10, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[1]).c_str());
	}
	else
	{
		drawRect(525.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(501.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(501 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[1]).c_str());
	}
	if (5700 > nP2Health && nP2Health >= 2850)
	{
		drawRect(472.0f + nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(434.0f + nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(432 + nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[2]).c_str());
	}
	else
	{
		drawRect(472.0f + nResetOffset, 29.0f, 1.0f, 8.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(449.0f + nResetOffset, 29.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(448 + nResetOffset, 30, 6, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[2]).c_str());
	}
	if (2850 > nP2Health)
	{
		drawRect(419.0f + nResetOffset, 19.0f, 1.0f, 18.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(380.0f + nResetOffset, 19.0f, 39.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(380 + nResetOffset, 20, 10, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[3]).c_str());
	}
	else
	{
		drawRect(419.0f + nResetOffset, 23.0f, 1.0f, 14.0f, 0xFFFFFFFF);		// vertical bar
		drawRect(395.0f + nResetOffset, 23.0f, 24.0f, 1.0f, 0xFFFFFFFF);		// horizontal bar
		TextDraw(395 + nResetOffset, 24, 6, 0xFFFFFFFF, std::format("{:.3f}", P2AdjGuts[3]).c_str());
	}

	//meter multipliers
	if (shouldDrawMeter) {
		nResetOffset = 0;
		if (fScroll > 0)
			nResetOffset = 48.0f * fScroll;

		int exGuardMultTimer = pActiveP1->subObj.exGuardMeterPenaltyTimer;
		float otgMeterMult = playerAuxDataArr[0].comboCalcData[playerAuxDataArr[0].comboCalcIndex].otgMeterMult / 100.0f;
		int meterMultTimer = pActiveP1->subObj.meterMultTimer;
		float y = 464.0f + nResetOffset;
		if (exGuardMultTimer > 0) {
			TextDraw(240, y, 10, 0xFFFFFFFF, "x0.10");
			RectDraw(240, y + 11, 38.0f * exGuardMultTimer / 120.0f, 2, 0xFF20A020);
		}
		y -= 12.0f;
		if (otgMeterMult > 0 && pActiveP2->subObj.hitstunTimeRemaining != 0) {
			TextDraw(240, y, 10, 0xFFFFFFFF, "x%0.2f", otgMeterMult);
		}
		y -= 12.0f;
		if (meterMultTimer > 0) {
			TextDraw(240, y, 10, 0xFFFFFFFF, "x%0.2f", pActiveP1->subObj.meterGainMultiplier / 255.0f);
			RectDraw(240, y + 11, 38.0f * (float)meterMultTimer / pActiveP1->subObj.meterMultTimerTotal, 2, 0xFF20A020);
		}
		
		exGuardMultTimer = pActiveP2->subObj.exGuardMeterPenaltyTimer;
		otgMeterMult = playerAuxDataArr[1].comboCalcData[playerAuxDataArr[1].comboCalcIndex].otgMeterMult / 100.0f;
		meterMultTimer = pActiveP2->subObj.meterMultTimer;
		y = 464.0f + nResetOffset;
		if (exGuardMultTimer > 0) {
			TextDraw(362, y, 10, 0xFFFFFFFF, "x0.10");
			RectDraw(362, y + 11, 38.0f * exGuardMultTimer / 120.0f, 2, 0xFF20A020);
		}
		y -= 12.0f;
		if (otgMeterMult > 0 && pActiveP1->subObj.hitstunTimeRemaining != 0) {
			TextDraw(362, y, 10, 0xFFFFFFFF, "x%0.2f", otgMeterMult);
		}
		y -= 12.0f;
		if (meterMultTimer > 0) {
			TextDraw(362, y, 10, 0xFFFFFFFF, "x%0.2f", pActiveP2->subObj.meterGainMultiplier / 255.0f);
			RectDraw(362, y + 11, 38.0f * (float)meterMultTimer / pActiveP2->subObj.meterMultTimerTotal, 2, 0xFF20A020);
		}
		
	}
		
}

void drawComboTimer()
{
	if (!shouldDrawHud) {
		return;
	}

	bool showTimer = false;
	for (int i = 0; i < 4; i++) {
		PlayerData* P = &playerDataArr[i];
		if (!P->exists) continue;
		if (!P->subObj.notInCombo ||
			P->subObj.throwFlag ||
			P->subObj.inBlockstun) {
			showTimer = true;
		}
	}

	if (showTimer) {
		if (*(int*)(adMBAABase + 0x0015df00) == 0 && *(int*)(adMBAABase + adGlobalFreeze) == 0 && !_naked_newPauseCallback2_IsPaused) {
			bool incrementTimer = true;
			for (int i = 0; i < 4; i++) {
				PlayerData* P = &playerDataArr[i];
				if (!P->exists) continue;
				if (P->subObj.magicCircuitState == 1 ||
					P->subObj.magicCircuitState == 3) {
					incrementTimer = false;
				}
			}
			if (incrementTimer) comboTimer++;
		}

		float length = floor(log10(max(comboTimer / 48.0f, 1.0f))) + 1.5f;
		if (*(int*)(adMBAABase + 0x0015df00) == 0 && *(int*)(adMBAABase + adGlobalFreeze) == 0) {
			TextDraw(320 - (7.2 * length), 15, 10, 0xFFFFFFFF, "%.02f", comboTimer / 48.0f);
		}
		else {
			TextDraw(320 - (7.2 * length), 15, 10, 0x80FFFFFF, "%.02f", comboTimer / 48.0f);
		}

	}
	else {
		comboTimer = 0;
	}
}

void drawAccurateComboDamage()
{
	if (!shouldDrawHud) {
		return;
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			PlayerAuxData* playerData = &playerAuxDataArr[i];
			if (trueComboData[i][j].damage != 0 &&
				playerData->comboCalcData[j].someFlag != -1 &&
				trueComboData[i][j].damage != playerData->comboCalcData[j].damage) {
				float xPos = playerData->comboCalcData[j].xPos;
				float yPos = playerData->comboCalcData[j].yPos;
				byte alpha = playerData->comboCalcData[j].alpha;
				DWORD ARGB = (alpha << 24) | 0x00FFFFFF;
				TextDraw(xPos + 137.0f, yPos + 62.0f, 10, ARGB, "%i", trueComboData[i][j].damage);
			}
		}
	}
}

void drawHitstunBar()
{
	if (!shouldDrawHud) {
		return;
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			PlayerAuxData* playerData = &playerAuxDataArr[i];
			if (playerData->comboCalcData[j].damage != 0 &&
				playerData->comboCalcData[j].numHits > 1 &&
				playerData->comboCalcData[j].someFlag != -1) {
				float xPos = playerData->comboCalcData[j].xPos;
				float yPos = playerData->comboCalcData[j].yPos;
				byte alpha = playerData->comboCalcData[j].alpha;
				DWORD ARGB = (alpha << 24) | 0x00FFFFFF;
				DWORD bgARGB = (alpha << 24) | 0x0046A0E6;
				PlayerData* opponent = i == 0 ? pActiveP2 : pActiveP1;
				float width = 0.0f;
				float bgWidth = 0.0f;
				if (opponent->subObj.hitstunTimeRemaining == -2) { //airborne
					width = (float)(opponent->subObj.totalUntechTime - opponent->subObj.untechTimeElapsed) / 30.0f;
					bgWidth = (float)(opponent->subObj.totalUntechTime - 1) / 30.0f;
				}
				else if (opponent->subObj.hitstunTimeRemaining > 0) { //grounded
					width = (float)(opponent->subObj.hitstunTimeRemaining - 1) / 30.0f;
					bgWidth = (float)(opponent->subObj.hitstunTimeRemaining + opponent->subObj.hitstunBlockstunTimeElapsed - 2) / 30.0f;
				}
				else if (opponent->subObj.hitstunTimeRemaining == -3) { //knockdown
					width = 1.0f;
					bgWidth = 1.0f;	
				}

				int untechDecay = 0;
				if (opponent->subObj.gravity > 0.072) untechDecay = max(0, (int)256 - floor(256 - (opponent->subObj.gravity - 0.072f) / 0.928f * 19)); //(opponent->subObj.gravity - 0.072f) / 0.928f * 19.0f;

				if (opponent->subObj.bounceCount > 2) {
					ARGB = (alpha << 24) | 0x00BF6716;
					bgARGB = (alpha << 24) | 0x00000000;
				}
				else if (untechDecay + opponent->subObj.untechPenalty > 2) {
					bgARGB = (alpha << 24) | 0x00000000;
				}
				else if (untechDecay + opponent->subObj.untechPenalty > 1) {
					bgARGB = (alpha << 24) | 0x00808080;
				}
				else if (untechDecay + opponent->subObj.untechPenalty > 0) {
					bgARGB = (alpha << 24) | 0x0078F0F0;
				}

				RectDraw(xPos - 1, yPos + 42.0f, 278.0f, 4, 0xFF000000);
				if (i == 0) {
					bgWidth = CLAMP(bgWidth, 0.0f, 1.0f);
					bgWidth *= 276.0f;
					RectDraw(xPos + 276.0f - bgWidth, yPos + 43.0f, bgWidth, 2, bgARGB);
					width = CLAMP(width, 0.0f, 1.0f);
					width *= 276.0f;
					RectDraw(xPos + 276.0f - width, yPos + 43.0f, width, 2, ARGB);
				}
				else {
					bgWidth = CLAMP(bgWidth, 0.0f, 1.0f);
					bgWidth *= 276.0f;
					RectDraw(xPos, yPos + 43.0f, bgWidth, 2, bgARGB);
					width = CLAMP(width, 0.0f, 1.0f);
					width *= 276.0f;
					RectDraw(xPos, yPos + 43.0f, width, 2, ARGB);
				}
				
			}
		}
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

	PlayerData** drawOrderArray = (PlayerData**)(adMBAABase + 0x00346048);
	for (int i = 3; i >= 0; i--) {
		if (drawOrderArray[i]->exists) drawObject((DWORD)drawOrderArray[i], false, drawOrderArray[i]->subObj.index);
	}

	//drawObject(0x00555130 + (0xAFC * 0), false, 0); // P1
	//drawObject(0x00555130 + (0xAFC * 1), false, 1); // P2
	//if (*(bool*)(0x00555130 + (0xAFC * 2))) drawObject(0x00555130 + (0xAFC * 2), false, 2); // P3
	//if (*(bool*)(0x00555130 + (0xAFC * 3))) drawObject(0x00555130 + (0xAFC * 3), false, 3); // P4

	// draw all effects

	int projectileIndex = 4;
	bool res;
	for(unsigned index = 0; index < 1000; index++) {
		if ((*(int*)(index * 0x33c + 0x67bde8) != 0) && (*(char*)(index * 0x33c + 0x67be09) == '\0') && (*(char*)(index * 0x33c + 0x67c0dd) == '\0')) {
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

	auto updateAnimation = [](DWORD animDataAddr, BYTE blockState, DWORD patternState, DWORD notInCombo, BYTE armorTimer, DWORD throwInvuln) -> void
		{
			// the order of this if block denotes the priority for each highlight
			if (XS_highlights == 0)
				patchMemcpy(animDataAddr + 0x18, arrDefaultHighlightSetting.data(), 3);
			else if (arrBlockingHighlightSetting[3] == 1 && blockState == 1)	// BLOCKING
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
	if (XS_rate == RNG_EVERY_FRAME)
	{
		*(uint32_t*)(dwBaseAddress + adRNGIndex) = 55;
		uint8_t nOffset = 1;
		do
		{
			*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * nOffset++) = (nOffset < 22) ? nRN : 0;
		} while (nOffset < 56);
	}
	else if (XS_rate == RNG_EVERY_RESET)
	{
		SetSeed(nRN);
		*(uint32_t*)(dwBaseAddress + adRNGIndex) = 55;
		uint32_t nRand = std::rand() + std::rand() * 0x10000;
		*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * 1) = nRand + nRN;
		*(uint32_t*)(dwBaseAddress + adRNGArray + 4 * 22) = nRand;
	}
}

//Handle extended training gameplay effects
bool bDoReversal = false;
int nReversalDelayFramesLeft = 0;
byte nHoldButtons = 0;
bool bHoldShield = false;
bool bDidShield = false;
int nSaveShieldRevIndex = 0;
int validIndex = -1;

void HandleReversalsPage() {
	if (pdP2Data->inactionableFrames == 0) {
		nHoldButtons = 0;
		bHoldShield = false;
	}
	if ((XS_reversalType == 0 && !bDoReversal) || pActiveP2->subObj.doTrainingAction != 1 || pActiveP2->subObj.targetPatternPriority == 30000) return;
	std::vector<int> vValidReversals = (pActiveP2->subObj.yPos == 0 && pActiveP2->subObj.prevYPos == 0 ? vGroundReversals : vAirReversals);
	int pat;
	if (vValidReversals.size() != 0 && bDoReversal && (pdP2Data->inactionableFrames == 0 || pActiveP2->subObj.shieldSuccessType != 0)) {
		if (nReversalDelayFramesLeft == 0) {
			int totalWeight = 0;
			for (int i = 0; i < 4; i++) {
				if (pActiveP2->subObj.shieldSuccessType != 0) {
					pat = vValidReversals[i] % 1000;
					byte shieldCancel = getShieldCancel(pActiveP2, pat);
					if (*XS_reversalSlots[i] != 0 && vValidReversals[i] != 0 && (pActiveP2->subObj.shieldSuccessType & shieldCancel) != 0) totalWeight += *XS_reversalWeights[i];
				}
				else {
					if (*XS_reversalSlots[i] != 0 && vValidReversals[i] != 0) totalWeight += *XS_reversalWeights[i];
				}
			}
			totalWeight += XS_noReversalWeight;
			if (totalWeight == 0) return;
			int randomWeight = rand() % totalWeight + 1;
			validIndex = -1;
			for (int i = 0; i < 4; i++) {
				if (pActiveP2->subObj.shieldSuccessType != 0) {
					pat = vValidReversals[i] % 1000;
					byte shieldCancel = getShieldCancel(pActiveP2, pat);
					if (*XS_reversalSlots[i] == 0 || vValidReversals[i] == 0 || (pActiveP2->subObj.shieldSuccessType & shieldCancel) == 0) continue;
				}
				else {
					if (*XS_reversalSlots[i] == 0 || vValidReversals[i] == 0) continue;
				}
				randomWeight -= *XS_reversalWeights[i];
				if (randomWeight <= 0) {
					validIndex = i;
					break;
				}
			}

			if (validIndex > -1) {
				pat = vValidReversals[validIndex] % 1000;
				if (*XS_reversalSlots[validIndex] >> 16 != 0) {
					nSaveShieldRevIndex = validIndex;
					int p2CharacterNumber = *(int*)(adMBAABase + dwP2CharNumber);
					int p2Moon = *(int*)(adMBAABase + dwP2CharMoon);
					int p2CharacterID = 10 * p2CharacterNumber + p2Moon;
					pat = -1;
					switch (*XS_reversalSlots[validIndex] >> 16) {
					case 1:
						//pat = GetPattern(p2CharacterID, "5D");
						pat = getPatternFromCmd(pActiveP2, "D\xff", 5);
						break;
					case 2:
						//pat = GetPattern(p2CharacterID, "5D");
						pat = getPatternFromCmd(pActiveP2, "D\xff", 5);
						bHoldShield = true;
						break;
					case 3:
						//pat = GetPattern(p2CharacterID, "2D");
						pat = getPatternFromCmd(pActiveP2, "V+D\xff", 5);
						break;
					case 4:
						//pat = GetPattern(p2CharacterID, "2D");
						pat = getPatternFromCmd(pActiveP2, "V+D\xff", 5);
						bHoldShield = true;
						break;
					case 5:
						//pat = GetPattern(p2CharacterID, "j.D");
						pat = getPatternFromCmd(pActiveP2, "D\xff", 4);
						break;
					case 6:
						//pat = GetPattern(p2CharacterID, "j.D");
						pat = getPatternFromCmd(pActiveP2, "D\xff", 4);
						bHoldShield = true;
						break;
					}
					if (tryCmdPattern(pActiveP2, pat)) {
						bDidShield = true;
						pActiveP2->subObj.targetPatternPriority = 30000;
					}
				}
				else if (pat == 999) {
					enableTAS = true;
					enableRevTAS = true;
					TASManagerObj[pActiveP2->subObj.index].load("TAS_REV.txt");
					TASManagerObj[pActiveP2->subObj.index].setInputs(pActiveP2->subObj.index);
					revTasDoTrainingActionMem = pActiveP2->subObj.doTrainingAction;
					pActiveP2->subObj.doTrainingAction = 0;
				}
				else if (pat > 40) {
					tryCmdPattern(pActiveP2, vValidReversals[validIndex] % 1000);
					pActiveP2->subObj.targetPatternPriority = 30000;
				}
				else {
					pActiveP2->subObj.targetPattern = vValidReversals[validIndex] % 1000;
					pActiveP2->subObj.targetPatternPriority = 30000;
				}

				if (vValidReversals[validIndex] > 999) {
					nHoldButtons = vValidReversals[validIndex] / 1000;
				}
			}
			bDoReversal = false;
		}
		else {
			nReversalDelayFramesLeft--;
		}
	}

	if (pActiveP2->subObj.shieldSuccessType != 0 && bDidShield) { // D > X reversals
		nHoldButtons = 0;
		bHoldShield = false;
		pat = vValidReversals[nSaveShieldRevIndex] % 1000;
		if (pat > 40 &&
			!(pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Ground ||
			pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Air ||
			pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Crouch)) {
			tryCmdPattern(pActiveP2, vValidReversals[nSaveShieldRevIndex] % 1000);
			pActiveP2->subObj.targetPatternPriority = 30000;
		}
		else {
			pActiveP2->subObj.targetPattern = vValidReversals[nSaveShieldRevIndex] % 1000;
			pActiveP2->subObj.targetPatternPriority = 30000;
		}
		if (vValidReversals[nSaveShieldRevIndex] > 999) {
			nHoldButtons = vValidReversals[nSaveShieldRevIndex] / 1000;
		}
		bDidShield = false;
		bDoReversal = false;
	}

	if (pActiveP2->subObj.targetPatternPriority == 30000) return;

	switch (XS_reversalType) {
	case 1:
		if (pActiveP2->subObj.hitstunTimeRemaining != 0 || (pActiveP2->subObj.receivedHitVector != 0xFF && (pActiveP2->subObj.receivedHitVector < 90 || pActiveP2->subObj.receivedHitVector > 95)) || pActiveP2->subObj.shieldSuccessType != 0) {
			bDoReversal = true;
			nReversalDelayFramesLeft = XS_reversalDelay;
			nHoldButtons = 0;
			bDidShield = false;
		}
		break;
	case 2:
		if (pActiveP2->subObj.inBlockstun != 0) {
			bDoReversal = true;
			nReversalDelayFramesLeft = XS_reversalDelay;
			nHoldButtons = 0;
			bDidShield = false;
		}
		break;
	case 3:
		if ((pActiveP2->subObj.hitstunTimeRemaining != 0 || (pActiveP2->subObj.receivedHitVector != 0xFF && (pActiveP2->subObj.receivedHitVector < 90 || pActiveP2->subObj.receivedHitVector > 95))) && pActiveP2->subObj.inBlockstun == 0) {
			bDoReversal = true;
			nReversalDelayFramesLeft = XS_reversalDelay;
			nHoldButtons = 0;
			bDidShield = false;
		}
		break;
	case 4:
		if (pActiveP2->subObj.hitstunTimeRemaining == -3 || pActiveP2->subObj.pattern == 352 || pActiveP2->subObj.isGroundTech) {
			bDoReversal = true;
			nReversalDelayFramesLeft = XS_reversalDelay;
			nHoldButtons = 0;
			bDidShield = false;
		}
		break;
	case 5:
		if (pActiveP2->subObj.shieldSuccessType != 0) {
			bDoReversal = true;
			nReversalDelayFramesLeft = XS_reversalDelay;
			nHoldButtons = 0;
			bDidShield = false;
		}
	}

	if (XS_reversalSlot1 == 0 && XS_reversalSlot2 == 0 && XS_reversalSlot3 == 0 && XS_reversalSlot4 == 0) {
		bDoReversal = false;
	}
}

void HandleTrainingPage() {
	if (XS_penaltyReset == 1) {
		doFastReversePenalty();
	}

	if (XS_guardBarReset == 1) {
		float mults[5] = { 1.0, 0.75, 0.5, 0.25, 0.0 };
		int guardSetting = *(short*)(adMBAABase + adBS_GUARD_GAUGE);
		int gauges[3] = { 8000 * mults[guardSetting], 7000 * mults[guardSetting], 10500 * mults[guardSetting] };
		if (pP1->subObj.inBlockstun == 0) pP1->subObj.guardGauge = gauges[pP1->subObj.moon];
		if (pP2->subObj.inBlockstun == 0) pP2->subObj.guardGauge = gauges[pP2->subObj.moon];
		if (pP3->exists && pP3->subObj.inBlockstun == 0) pP3->subObj.guardGauge = gauges[pP3->subObj.moon];
		if (pP4->exists && pP4->subObj.inBlockstun == 0) pP4->subObj.guardGauge = gauges[pP4->subObj.moon];

		if (pP1->subObj.guardGaugeState == 2) pP1->subObj.guardGaugeState = 1;
		if (pP2->subObj.guardGaugeState == 2) pP2->subObj.guardGaugeState = 1;
		if (pP3->exists && pP3->subObj.guardGaugeState == 2) pP3->subObj.guardGaugeState = 1;
		if (pP4->exists && pP4->subObj.guardGaugeState == 2) pP4->subObj.guardGaugeState = 1;
	}

	if (XS_exGuard == 1 || (XS_exGuard == 2 && rand() % 2 == 0)) {
		if (pDummy->subObj.doTrainingAction) {
			pDummy->subObj.exGuard = 10;
		}
	}

	static bool doBurst = true;
	static int burstFailHitCount = -1;
	static int queuedBurstHitCount = -1;
	if (XS_hitsUntilBurst != 0) {
		int hitcount = max(pActiveP2->subObj.onBlockComboCount, pActiveP2->subObj.onHitComboCount);
		if (pActiveP2->subObj.hitstop == 0) {
			if (doBurst &&
				rand() % 100 < dummyBurstChance) {
				if (hitcount >= XS_hitsUntilBurst &&
					hitcount != burstFailHitCount) {
					if (dummyDelayBunkerChance < 200) {
						doBurst = false;
						burstFailHitCount = -1;
						queuedBurstHitCount = hitcount;
					}
				}
			}
			else {
				burstFailHitCount = hitcount;
			}
		}

		if (queuedBurstHitCount == hitcount && rand() % 100 < dummyDelayBunkerChance) {
			if (tryBurst(pActiveP2)) {
				doBurst = false;
				burstFailHitCount = -1;
				queuedBurstHitCount = -1;
			}
		}

		if (!doBurst && pActiveP2->subObj.onBlockComboCount == 0 && pActiveP2->subObj.onHitComboCount == 0) {
			doBurst = true;
			burstFailHitCount = -1;
			queuedBurstHitCount = -1;
		}
	}

	static bool doBunker = true;
	static int bunkerFailHitCount = -1;
	static int queuedBunkerHitCount = -1;
	if (XS_hitsUntilBunker != 0) {
		if (pActiveP2->subObj.hitstop < 2) {
			if (doBunker &&
				rand() % 100 < dummyBunkerChance) {
				if (pActiveP2->subObj.onBlockComboCount >= XS_hitsUntilBunker &&
					pActiveP2->subObj.onBlockComboCount != bunkerFailHitCount &&
					pActiveP2->subObj.animationDataPtr->stateData->stance != 1) {
					if (dummyDelayBunkerChance < 200) {
						doBunker = false;
						bunkerFailHitCount = -1;
						queuedBunkerHitCount = pActiveP2->subObj.onBlockComboCount;
					}
				}
			}
			else {
				bunkerFailHitCount = pActiveP2->subObj.onBlockComboCount;
			}
		}

		if (queuedBunkerHitCount == pActiveP2->subObj.onBlockComboCount && rand() % 100 < dummyDelayBunkerChance && pActiveP2->subObj.untechTimeElapsed < 11) {
			int bunkerPat = getPatternFromCmd(pActiveP2, "\2\1\4D\xff");
			pActiveP2->subObj.targetPattern = bunkerPat;
			DWORD bunkerFlags[7] = { 4, 0, 0, 0, 0, 0, 0 };
			memcpy(&pActiveP2->subObj.defensiveStateQueue, bunkerFlags, 7 * 0x4);
			pActiveP2->subObj.hitstunTimeRemaining = 0;
			doBunker = false;
			bunkerFailHitCount = -1;
			queuedBunkerHitCount = -1;
		}

		if (!doBunker && pActiveP2->subObj.onBlockComboCount == 0) {
			doBunker = true;
			bunkerFailHitCount = -1;
			queuedBunkerHitCount = -1;
		}
	}

	if (XS_hitsUntilForceGuard != 0 &&
		pActiveP2->subObj.onBlockComboCount >= XS_hitsUntilForceGuard &&
		pActiveP2->subObj.animationDataPtr->stateData->stance != 1 &&
		pActiveP2->subObj.defensiveStateFlag == 5) {
		bForceGuard = true;
	}
	else {
		bForceGuard = false;
	}
}

void HandleCharacterPage() {
	switch (pActiveP1->subObj.charID) {
	case 0: //sion
		if (XS_sionBullets == 0 && pActiveP1->subObj.animationDataPtr->stateData->canMove) {
			pActiveP1->subObj.extraVariables[1] = 0;
		}
		break;
	case 4: //maids
		if (XS_fMaidsHearts == 0 && pActiveP1->subObj.animationDataPtr->stateData->canMove) {
			pP1->subObj.extraVariables[4] = 0;
			pP3->subObj.extraVariables[5] = 0;
		}
		break;
	case 31: //roa
		if (XS_roaHiddenCharges == 0 && pActiveP1->subObj.animationDataPtr->stateData->canMove) {
			pActiveP1->subObj.extraVariables[6] = 9;
		}
		if (XS_roaVisibleCharges == 0 && pActiveP1->subObj.animationDataPtr->stateData->canMove) {
			pActiveP1->subObj.extraVariables[7] = 9;
		}
		break;
	case 33: //ryougi
		if (XS_ryougiKnife == 0 && pActiveP1->subObj.animationDataPtr->stateData->canMove) {
			pActiveP1->subObj.specialVariables[0] = 0;
		}
		break;
	}

	switch (pActiveP2->subObj.charID) {
	case 0: //sion
		if (XS_sionBullets == 0 && pActiveP2->subObj.animationDataPtr->stateData->canMove) {
			pActiveP2->subObj.extraVariables[1] = 0;
		}
		break;
	case 4: //maids
		if (XS_fMaidsHearts == 0 && pActiveP2->subObj.animationDataPtr->stateData->canMove) {
			pP2->subObj.extraVariables[4] = 0;
			pP4->subObj.extraVariables[5] = 0;
		}
		break;
	case 31: //roa
		if (XS_roaHiddenCharges == 0 && pActiveP2->subObj.animationDataPtr->stateData->canMove) {
			pActiveP2->subObj.extraVariables[6] = 9;
		}
		if (XS_roaVisibleCharges == 0 && pActiveP2->subObj.animationDataPtr->stateData->canMove) {
			pActiveP2->subObj.extraVariables[7] = 9;
		}
		break;
	case 33: //ryougi
		if (XS_ryougiKnife == 0 && pActiveP2->subObj.animationDataPtr->stateData->canMove) {
			pActiveP2->subObj.specialVariables[0] = 0;
		}
		break;
	}
}

void HandleExtendedTrainingEffects() {
	pActiveP1 = (PlayerData*)(adMBAABase + adP1Base + pdP1Data->activeCharacter * dwPlayerStructSize);
	pActiveP2 = (PlayerData*)(adMBAABase + adP1Base + pdP2Data->activeCharacter * dwPlayerStructSize);
	isP1Controlled = pP1->subObj.doTrainingAction == 0;
	pPlayer = isP1Controlled ? pActiveP1 : pActiveP2;
	pDummy = isP1Controlled ? pActiveP2 : pActiveP1;
	HandleReversalsPage();			//page 1
	HandleTrainingPage();				//page 2
	HandleCharacterPage();		//page 5
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
        if (isWine == ERROR_SUCCESS) {
            isDirectXHooked = true;
            memcpy(pTable, *reinterpret_cast<void***>(device), sizeof(pTable));
            oPresent = (tPresent)trampolineHook((char*)pTable[17], (char*)hkPresent, 7);
	    } else {
			isDirectXHooked = HookDirectX();
		}
	}

	static bool isRendererHooked = false;
	if (!isRendererHooked && isDirectXHooked) {
		isRendererHooked = initRenderModifications();
	}

	Sleep(dwMilliseconds);
}

// frame start/done callbacks

void frameStartCallback() {

}

bool logSaveState = false;

void (*setDesiredFPS)(double) = NULL;

void setFPSLimiter(bool b) {
	disableFPSLimit = b;

	static bool isFirstRun = true;
	if (isFirstRun) {
		isFirstRun = false;



	}

	if (setDesiredFPS != NULL) {
		//log("setting fps");
		//setDesiredFPS(b ? 60.0 : 1000.0);
	}

	// i,, i do not know what to do here.
	// i wish i designed the menu with this in mind, but not all menu elements have/need globals
	// if the menu design is changed, this will need to be changed
	// i could maybe pass an int* as the menu type instead?
	if (disableFpsMenuOption != NULL) {
		disableFpsMenuOption->optionState = b;
	}
	
}

void ChangeVolume() {
	const DWORD MBAA_Change_Volume = 0x00418030;
	PUSH_ALL;
	__asm {
		call[MBAA_Change_Volume];
	}
	POP_ALL;
}

void SaveGameSettings() {
	const DWORD MBAA_Save_Game_Settings = 0x00401540;
	PUSH_ALL;
	__asm {
		call[MBAA_Save_Game_Settings];
	}
	POP_ALL;
}

void MenuSound() {
	*(byte*)(adMBAABase + 0x0036e0a0) = 1;
}

void SelectSound() {
	*(byte*)(adMBAABase + 0x0036e09f) = 1;
}

bool MenuControlsMouseInBounds = false;
bool MenuControlsClickIsPress = false;
bool MenuControlsScrollUp = false;
bool MenuControlsScrollDown = false;
void DoSingleMenuMouseControls(MenuWindow* menu) {
	MenuControlsScrollUp = false;
	MenuControlsScrollDown = false;
	if (menu != 0) {
		int middle = menu->yOffset;
		MenuInfo* menuInfo = menu->menuInfoList.listStart[menu->menuInfoIndex];
		int numElements = (menuInfo->elementList.listEnd - menuInfo->elementList.listStart);
		int totalHeight = 0;
		for (int i = 0; i < numElements; i++) {
			totalHeight += menuInfo->elementList.listStart[i]->drawHeight;
		}
		int top = middle - (totalHeight / 2);
		int bottom = middle + (totalHeight / 2);
		if (mousePos.y < top - 5 || mousePos.y > bottom + 5) {
			MenuControlsMouseInBounds = false;
		}
		else {
			MenuControlsMouseInBounds = true;
		}
		MenuControlsClickIsPress = true;
		if (MenuControlsMouseInBounds) {
			int curPos = top;
			int selection = 0;
			bool canSelect = true;
			for (int i = 0; i < numElements; i++) {
				if (curPos < mousePos.y) {
					selection = i;
					canSelect = menuInfo->elementList.listStart[i]->canSelect;
				}
				curPos += menuInfo->elementList.listStart[i]->drawHeight;
			}

			if ((DWORD)menuInfo->elementList.listStart[selection]->vftable == 0x00536654 && mousePos.x > menu->elementsXOffset) {
				MenuControlsClickIsPress = false;
			}

			if (canSelect) {
				if (menuInfo->selectedElement != selection) MenuSound();
				menuInfo->selectedElement = selection;
			}
		}
	}
}

void DoMenuMouseControls() {
	static Point prevMousePos;

	bool idleMouse = false;
	if (prevMousePos == mousePos && !lClick && !rClick) {
		idleMouse = true;
	}
	prevMousePos = mousePos;
	if (idleMouse) return;

	MenuWindow* training = *(MenuWindow**)(adMBAABase + adTrainingMenu);
	if (training != 0) {
		switch (training->openSubmenuIndex) {
		case 2:
			DoSingleMenuMouseControls(training);
			break;
		case 6:
			if (training->BattleSettings != 0) DoSingleMenuMouseControls(training->BattleSettings);
			break;
		case 7:
			if (training->EnemySettings != 0) DoSingleMenuMouseControls(training->EnemySettings);
			break;
		case 8:
			if (training->TrainingDisplay != 0) DoSingleMenuMouseControls(training->TrainingDisplay);
			break;
		case 9:
			if (training->DummySettings != 0) DoSingleMenuMouseControls(training->DummySettings);
			break;
		case 10:
		case 11:
		case 14:
			if (training->YesNoMenu != 0) DoSingleMenuMouseControls(training->YesNoMenu);
			break;
		case 17:
			if (training->ExtendedSettings != 0) DoSingleMenuMouseControls(training->ExtendedSettings);
		case 18:
			if (training->HotkeySettings != 0) DoSingleMenuMouseControls(training->HotkeySettings);
		}
	}
}

bool CheckLoadedGameFile(LPCSTR filePath)
{
	const DWORD MBAA_FUN_004db9b0 = 0x004db9b0;
	__asm {
		push filePath;
		mov esi, 0x0076e9c4; //this is the game's list of loaded files
		call[MBAA_FUN_004db9b0];
	}
}

void frameDoneCallback()
{
	profileFunction();

	static KeyState lShiftKey(VK_LSHIFT);

	static uint8_t nFrameCount = 0;
	nFrameCount = (nFrameCount + 1) % 255;

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

			//HandleExtendedTrainingEffects();
		}

		static KeyState UpKey(VK_UP);
		static KeyState DownKey(VK_DOWN);

		if (bFreeze)
		{
			if (oNextFrameHotkey.keyDownHeldFreq<4, 24>()) {
				bool needNewFrame = saveStateManager.load(1);
				if (!alreadyRolledReplayManager) {
					//log("calling rollforward 2");
					//replayManager.rollForward();
				}

				if (!needNewFrame) {
					rollFancyInputDisplay(1);
				}
			}
			else if (oPrevFrameHotkey.keyDownHeldFreq<4, 24>()) {
				if (saveStateManager.currentState > 0) {
					saveStateManager.load(-1);
					rollFancyInputDisplay(-1);
					replayManager.rollBack();
				}
				/*
				saveStateManager.load(-1);
				rollFancyInputDisplay(-1);
				replayManager.rollBack();
				*/
			}
		}

		if (logSaveState) {
			saveStateManager.log();
		}

	}

	static KeyState rKey('R');
	if (lShiftKey.keyHeld() && rKey.keyDown()) {
		
		//log("RKEY hit");
		
		QueueTrainingReset();
		
		///replayManager.load("./ReplayVS/RED_ARCUEIDxSATSUKI_251128130201.rep");
		replayManager.reset();
		

		//TASManagerObj.load("TAS.txt");
		//needTrainingModeReset = true;
	}

	static KeyState mKey('M');
	if (lShiftKey.keyHeld() && mKey.keyDown())
	//if (oResetKey.keyDown())
	{
		QueueTrainingReset();
	}

	static KeyState nKey('N');
	if ((lShiftKey.keyHeld() && nKey.keyDown()) || doCharacterReload)
	{
		doCharacterReload = false;
		ArrayContainer<CSSData*> cssArray = **(ArrayContainer<CSSData*>**)(0x0055df18);
		if (cssArray.array[p1LoadChar] != 0x0)
		{
			char buffer[256];
			char* charName = cssArray.array[p1LoadChar]->File1;
			snprintf(buffer, 256, "%s%s_%0d.txt", ".\\data\\", charName, p1LoadMoon);
			if (!CheckLoadedGameFile(buffer)) return;

			if (cssArray.array[p1LoadChar]->File2[0] != '0')
			{
				charName = cssArray.array[p1LoadChar]->File2;
				snprintf(buffer, 256, "%s%s_%0d.txt", ".\\data\\", charName, p1LoadMoon);
				if (!CheckLoadedGameFile(buffer)) return;
			}
		}

		if (cssArray.array[p2LoadChar] != 0x0)
		{
			char buffer[256];
			char* charName = cssArray.array[p2LoadChar]->File1;
			snprintf(buffer, 256, "%s%s_%0d.txt", ".\\data\\", charName, p2LoadMoon);
			if (!CheckLoadedGameFile(buffer)) return;

			if (cssArray.array[p2LoadChar]->File2[0] != '0')
			{
				charName = cssArray.array[p2LoadChar]->File2;
				snprintf(buffer, 256, "%s%s_%0d.txt", ".\\data\\", charName, p2LoadMoon);
				if (!CheckLoadedGameFile(buffer)) return;
			}
		}

		pP1->exists = 0;
		pP2->exists = 0;
		pP3->exists = 0;
		pP4->exists = 0;

		*(int*)(0x0074d83c) = p1LoadPal - 1;
		*(int*)(0x0074d840) = p1LoadChar;
		*(int*)(0x0074d84C) = p1LoadMoon;

		*(int*)(0x0074d868) = p2LoadPal - 1;
		*(int*)(0x0074d86C) = p2LoadChar;
		*(int*)(0x0074d878) = p2LoadMoon;

		FullCharacterReload();

		pdP1Data->activeCharacter = 0;
		pdP2Data->activeCharacter = 1;

		UpdateCharPointers(&(pP1->subObj));
		UpdateCharPointers(&(pP2->subObj));
		if (pP3->exists) UpdateCharPointers(&(pP3->subObj));
		if (pP4->exists) UpdateCharPointers(&(pP4->subObj));

		for (int i = 0; i < 3; i++)
		{
			saveStateManager.FullSaves[i]->unsave();
		}

		vPatternNames = GetEmptyPatternList();
		XS_reversalSlot1 = 0;
		XS_reversalSlot2 = 0;
		XS_reversalSlot3 = 0;
		XS_reversalSlot4 = 0;

		initLoadChars = true;

		QueueTrainingReset();
	}

	static KeyState fKey('F');
	if (lShiftKey.keyHeld() && fKey.keyDown()) {
		setFPSLimiter(!disableFPSLimit); // sorry :3
	}
	////setFPSLimiter(bDisableFPSLimit);

	static KeyState zKey('Z');
	if (lShiftKey.keyHeld() && zKey.keyDown()) {
		showFrameScrubber = !showFrameScrubber;
	}

	renderModificationsFrameDone();

	if (device != NULL) {
		drawFancyMenu();
		dragManager.handleDrag();
	}
	
	static bool etmMenuInit = false;
	if (!etmMenuInit) {
		initExtendedMenu();
		initHotkeyMenu();
		initMenuFromRegistry();
		initSharedValues();
		etmMenuInit = true;
	}

	shouldDrawBackground = true;
	shouldDrawHud = !XS_hideHUD;
	shouldDrawGroundLine = XS_drawGround;
	shouldDrawShadow = !XS_hideShadows;

	switch (XS_background)
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
	
	//drawTextWithBorder(300, 300, 36, 48	, " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~");
	if (bFreeze && shouldDrawHud)
	{
		try
		{
			char pcFreezeKey[256];
			char pcName[19];
			oFreezeHotkey.getKeyName(pcName);
			snprintf(pcFreezeKey, sizeof(pcFreezeKey), "Freeze Key: %s", pcName);
			TextDraw(100.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcFreezeKey);
		}
		catch (...)
		{
			TextDraw(100.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Freeze Key: <corrupt>");
		}

		if (nFrameCount > 170)
		{
			try
			{
				char pcFrameStepKey[256];
				char pcName[19];
				oNextFrameHotkey.getKeyName(pcName);
				snprintf(pcFrameStepKey, sizeof(pcFrameStepKey), "Next Frame: %s", pcName);
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcFrameStepKey);
			}
			catch (...)
			{
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Next Frame: <corrupt>");
			}
		}
		else if (nFrameCount > 85)
		{
			try
			{
				char pcPrevFrameKey[256];
				char pcName[19];
				oPrevFrameHotkey.getKeyName(pcName);
				snprintf(pcPrevFrameKey, sizeof(pcPrevFrameKey), "Prev Frame: %s", pcName);
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcPrevFrameKey);
			}
			catch (...)
			{
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Prev Frame: <corrupt>");
			}
		}
		else
		{
			try
			{
				char pcPrevFrameKey[256];
				char pcName[19];
				oAdvanceFrameHotkey.getKeyName(pcName);
				snprintf(pcPrevFrameKey, sizeof(pcPrevFrameKey), "Advance Frame: %s", pcName);
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, pcPrevFrameKey);
			}
			catch (...)
			{
				TextDraw(375.0f, 3.5f, 16.0f, 0xFFFFFFFF, "Advance Frame: <corrupt>");
			}
		}

		if (showFrameScrubber) {
			float x = 10.0f;
			float y = 390.0f;
			float unitW = (640.0f - 2 * x) / (float)(saveStateManager.states.size() - 1);
			float maxW = (640.0f - 2 * x);
			float h = 10.0f;
			RectDraw(x, y, maxW, h, 0x99000000); //BG
			for (int i = 0; i < saveStateManager.currentState; i++) {
				float x1 = x + unitW * i;
				RectDraw(x1, y, unitW, h, 0xFF00FF00);
			}

			if (lHeld && mousePos.y > 380.0f && mousePos.y < 410.0f) {
				float usedX = mousePos.x;
				float ratio = (mousePos.x - 10.0f) / 620.0f;
				int newState = floor(ratio * saveStateManager.states.size());
				if (newState < 0) newState = 0;
				else if (newState >= saveStateManager.states.size()) newState = saveStateManager.states.size() - 1;
				saveStateManager.currentState = newState;
				saveStateManager.states[newState]->load();
			}
		}
	}

	if (oFrameBarLeftHotkey.keyHeld())
		oFrameBarLeftHotkey.nHeldKeyCounter++;
	else
		oFrameBarLeftHotkey.nHeldKeyCounter = 0;
	if (oFrameBarLeftHotkey.keyDown() || oFrameBarLeftHotkey.nHeldKeyCounter >= 20)
	{
		XS_scrollDisplay--;
		if (XS_scrollDisplay <= -400) {
			XS_scrollDisplay = -400;
			//nSCROLL_DISPLAY = 0;
		}
		else {
			//nSCROLL_DISPLAY = 1;
		}
		*(short*)(adMBAABase + adXS_frameScroll) = -XS_scrollDisplay;
	}

	if (oFrameBarRightHotkey.keyHeld())
		oFrameBarRightHotkey.nHeldKeyCounter++;
	else
		oFrameBarRightHotkey.nHeldKeyCounter = 0;
	if (oFrameBarRightHotkey.keyDown() || oFrameBarRightHotkey.nHeldKeyCounter >= 20)
	{
		XS_scrollDisplay++;
		if (XS_scrollDisplay >= 0) {
			XS_scrollDisplay = 0;
			//nSCROLL_DISPLAY = 2;
		}
		else {
			//nSCROLL_DISPLAY = 1;
		}
		*(short*)(adMBAABase + adXS_frameScroll) = -XS_scrollDisplay;
	}

	if (oToggleHitboxesHotkey.keyDown())
	{
		XS_displayHitboxes = !XS_displayHitboxes;
		nDrawTextTimer = TEXT_TIMER;
		if (XS_displayHitboxes)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HITBOXES OFF");
	}

	if (oToggleFrameBarHotkey.keyDown())
	{
		XS_inGameFrameDisplay = !XS_inGameFrameDisplay;
		SetRegistryValue(sFRAME_DISPLAY, XS_inGameFrameDisplay);
		nDrawTextTimer = TEXT_TIMER;
		if (XS_inGameFrameDisplay)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "FRAME DATA OFF");
	}

	if (oToggleHighlightsHotkey.keyDown())
	{
		XS_highlights = !XS_highlights;
		SetRegistryValue(sHIGHLIGHTS, XS_highlights);
		nDrawTextTimer = TEXT_TIMER;
		if (XS_highlights)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS ON");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "HIGHLIGHTS OFF");
	}

	if (oQueueReversalHotkey.keyDown() &&
		(XS_reversalSlot1 != 0 || XS_reversalSlot2 != 0 || XS_reversalSlot3 != 0 || XS_reversalSlot4 != 0))
	{
		bDoReversal = true;
	}

	if (oSaveStateHotkey.keyDown() && safeWrite())
	{
		if (XS_saveStateSlot > 0) {
			saveStateManager.FullSaves[XS_saveStateSlot - 1]->save();
			if (XS_syncSavesWithFiles) saveStateManager.SaveToFile();
		}
		nDrawTextTimer = TEXT_TIMER;
		if (XS_saveStateSlot == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SAVED SLOT", XS_saveStateSlot);
	}

	if (oSaveStateHotkey.keyHeld() && safeWrite() && XS_saveStateSlot != 0)
	{
		nClearSaveTimer++;
		if (nClearSaveTimer == SAVE_RESET_TIME)
		{
			saveStateManager.FullSaves[XS_saveStateSlot - 1]->unsave();
			if (XS_syncSavesWithFiles) saveStateManager.SaveToFile();
			nDrawTextTimer = TEXT_TIMER;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "CLEARED SAVE", XS_saveStateSlot);
		}
	}
	else
	{
		nClearSaveTimer = 0;
	}

	if (oPrevSaveSlotHotkey.keyDown())
	{
		XS_saveStateSlot = max(0, XS_saveStateSlot - 1);
		nDrawTextTimer = TEXT_TIMER;
		if (XS_saveStateSlot == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SELECTED SAVE", XS_saveStateSlot);
	}
	if (oNextSaveSlotHotkey.keyDown())
	{
		XS_saveStateSlot = min(XS_saveStateSlot + 1, MAX_SAVES);
		nDrawTextTimer = TEXT_TIMER;
		if (XS_saveStateSlot == 0)
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s", "NO SLOT SELECTED");
		else
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "%s %i", "SELECTED SAVE", XS_saveStateSlot);
	}

	if (oIncrementRNGHotkey.keyHeld())
		oIncrementRNGHotkey.nHeldKeyCounter++;
	else
		oIncrementRNGHotkey.nHeldKeyCounter = 0;
	if (XS_customRNG != 0 && oIncrementRNGHotkey.keyDown() || oIncrementRNGHotkey.nHeldKeyCounter >= 20)
	{
		nDrawTextTimer = TEXT_TIMER;
		if (XS_customRNG == RNG_SEED)
		{
			XS_seed++;
			if (XS_seed > 0xffff) XS_seed = 0;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RNG SEED: %i", XS_seed);
		}
		else if (XS_customRNG == RNG_RN)
		{
			XS_seed++;
			if (XS_seed > 0xffff) XS_seed = 0;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RNG VALUE: %i", XS_seed);
		}
	}

	if (oDecrementRNGHotkey.keyHeld())
		oDecrementRNGHotkey.nHeldKeyCounter++;
	else
		oDecrementRNGHotkey.nHeldKeyCounter = 0;
	if (XS_customRNG != 0 && oDecrementRNGHotkey.keyDown() || oDecrementRNGHotkey.nHeldKeyCounter >= 20)
	{
		nDrawTextTimer = TEXT_TIMER;
		if (XS_customRNG == RNG_SEED)
		{
			XS_seed--;
			if (XS_seed < 0) XS_seed = 0xffff;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RNG SEED: %i", XS_seed);
		}
		else if (XS_customRNG == RNG_RN)
		{
			XS_seed--;
			if (XS_seed < 0) XS_seed = 0xffff;
			snprintf(pcTextToDisplay, sizeof(pcTextToDisplay), "RNG VALUE: %i", XS_seed);
		}
	}

	if (*(byte*)(adMBAABase + adP1FN2Input) &&
		(*(byte*)(adMBAABase + adDummyState) == 0xFF || *(byte*)(adMBAABase + adDummyState) == 0x5) &&
		!isPaused() &&
		safeWrite())
	{
		*(byte*)(adMBAABase + adNewSceneFlag) = 0xFF;
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

	// don't draw on the pause menu, but do on VIEW SCREEN
	MenuWindow* trainingMenu = *(MenuWindow**)(adMBAABase + adTrainingMenu);
	bool bVIEW_SCREEN = false;
	if (trainingMenu != 0x0 && trainingMenu->openSubmenuIndex == 12) bVIEW_SCREEN = true;
	//int nSubMenu;
	//ReadProcessMemory(GetCurrentProcess(), (LPVOID)(nSubMenuPointer), &nSubMenu, 4, 0);
	if ((safeWrite() && !isPaused()) || (isPaused() && bVIEW_SCREEN)) {

		if (XS_displayHitboxes)
			drawFrameData();

		if (!shouldDrawMeter && shouldDrawHud)
			drawSimpleMeter();

		if (XS_showStats)
			drawStats();

		if (XS_accurateComboDamage)
			drawAccurateComboDamage();

		if (displayComboTimer)
			drawComboTimer();

		if (displayHitstunBar)
			drawHitstunBar();
	}

	bool doDraw = false;
	if ((safeWrite() && !isPaused() && XS_inGameFrameDisplay) || (isPaused() && ((bVIEW_SCREEN && XS_inGameFrameDisplay) || bShowFrameBarPreview))) doDraw = true;
	drawFrameBar(doDraw);

	if (safeWrite() && !isPaused()) UpdateFrameBar();

	if (XS_colorGuide) drawColorGuide();

	if (doDraw && frameBar.y + (frameBar.h / 2) > 436)
	{
		shouldDrawMeter = 0;
	}
	else
	{
		shouldDrawMeter = 1;
	}

	*(int*)(adMBAABase + adSharedTimer) = *(int*)(adMBAABase + adTrueFrameCount);

	if (XS_rate == RNG_EVERY_FRAME)
	{
		if (XS_customRNG == RNG_SEED)
			SetSeed(XS_seed);
		if (XS_customRNG == RNG_RN)
			SetRN(XS_seed);
	}

	if (freezeCamera)
	{
		static Point customCameraMouseTracker;
		if (lClick) {
			customCameraMouseTracker = mousePos;
		}
		else if (lHeld) {
			customCameraX += (customCameraMouseTracker.x - mousePos.x) * 128 / customCameraZoom;
			customCameraY += (customCameraMouseTracker.y - mousePos.y) * 128 / customCameraZoom;
			customCameraMouseTracker = mousePos;
		}

		static Point customCameraMouseTracker2;
		if (rClick) {
			customCameraMouseTracker2 = mousePos;
		}
		else if (rHeld) {
			float xDist = 320.0f - customCameraMouseTracker2.x;
			float yDist = 240.0f - customCameraMouseTracker2.y;
			float trackerCenterDist = sqrt(xDist * xDist + yDist * yDist);
			xDist = 320.0f - mousePos.x;
			yDist = 240.0f - mousePos.y;
			float mouseCenterDist = sqrt(xDist * xDist + yDist * yDist);
			customCameraZoom += (mouseCenterDist - trackerCenterDist) / 100.0f;
			customCameraMouseTracker2 = mousePos;
		}

		*(byte*)(adMBAABase + 0x00157d2b) = 0x1;
		*(float*)(adMBAABase + 0x0014eb70) = customCameraZoom;
		*(int*)(adMBAABase + 0x00155124) = customCameraX;
		*(int*)(adMBAABase + 0x00155128) = customCameraY;
	}
	else {
		*(byte*)(adMBAABase + 0x00157d2b) = 0x0;
	}

	int inputsPt1 = *(int*)(adMBAABase + 0x00371398);
	int inputsPt2 = *(int*)(adMBAABase + 0x0037139C);
	if (inputsPt1 != 0 || inputsPt2 != 0) {
		nInputHeldFrameAdvanceCounter++;
	}
	else {
		nInputHeldFrameAdvanceCounter = 0;
	}

	//some janky linking of the extended input display options to the vanilla one
	bool nInputDisplaySettings = XS_p1InputDisplay || XS_p2InputDisplay;
	if (nInputDisplaySettings != nLastCustomInputDisplay) {
		*(bool*)(INPUTDISPLAYTOGGLE) = nInputDisplaySettings;
		nLastVanillaInputDisplay = nInputDisplaySettings;
	}

	if (*(bool*)(INPUTDISPLAYTOGGLE) != nLastVanillaInputDisplay) {
		if (*(bool*)(INPUTDISPLAYTOGGLE)) {
			XS_p1InputDisplay = 1;
			nLastCustomInputDisplay = true;
			SetRegistryValue(sP1_INPUT_DISPLAY, 1);
		}
		else {
			XS_p1InputDisplay = 0;
			XS_p2InputDisplay = 0;
			nLastCustomInputDisplay = false;
			SetRegistryValue(sP1_INPUT_DISPLAY, 0);
			SetRegistryValue(sP2_INPUT_DISPLAY, 0);
		}
		nInputDisplaySettings = XS_p1InputDisplay || XS_p2InputDisplay;
	}

	if (safeWrite()) {
		byte curComboData;
		if (pActiveP2->subObj.notInCombo) {
			curComboData = playerAuxDataArr[0].comboCalcData[0].index;
			trueComboData[0][curComboData].startingHealth = pActiveP2->subObj.health;
			trueComboData[0][curComboData].defender = &(pActiveP2->subObj);
		}
		if (pActiveP1->subObj.notInCombo) {
			curComboData = playerAuxDataArr[1].comboCalcData[0].index;
			trueComboData[1][curComboData].startingHealth = pActiveP1->subObj.health;
			trueComboData[1][curComboData].defender = &(pActiveP1->subObj);
		}
	}

	nLastCustomInputDisplay = nInputDisplaySettings;
	nLastVanillaInputDisplay = *(bool*)(INPUTDISPLAYTOGGLE);

	nSavedP1ActiveChar = pdP1Data->activeCharacter;
	nSavedP2ActiveChar = pdP2Data->activeCharacter;

	if (enableMouseControls && !showDebugMenu) {
		DoMenuMouseControls();
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

// reset funcs

int nTempP1MeterGain = 0;
int nTempP2MeterGain = 0;
int nP1MeterGain = 0;
int nP2MeterGain = 0;
DWORD prevComboPtr = 0;

void ResetCallback() {
	if (XS_saveStateSlot == 0 || !(saveStateManager.FullSaves[XS_saveStateSlot - 1]->IsSaved)) { // if not loading a save
		if (*(int*)(adMBAABase + adBS_MAGIC_CIRCUIT) == 0) { // if Magic Circuit is set to Normal
			for (int i = 0; i < 4; i++) {
				PlayerData* curPlayer = &playerDataArr[i];
				if (!curPlayer->exists) continue;
				int USE_METER = i % 2 == 0 ? XS_p1Meter : XS_p2Meter;
				switch (curPlayer->subObj.moon) {
				case 0:
					switch (USE_METER) {
					case 30000:
						curPlayer->subObj.magicCircuit = 20000;
						curPlayer->subObj.heatTimeLeft = 600;
						curPlayer->subObj.magicCircuitState = 2;
						curPlayer->subObj.maxHeatTime = 600;
						curPlayer->subObj.magicCircuitPause = 10;
						break;
					case 30001:
						curPlayer->subObj.magicCircuit = 0;
						curPlayer->subObj.heatTimeLeft = 550;
						curPlayer->subObj.magicCircuitState = 1;
						curPlayer->subObj.maxHeatTime = 550;
						curPlayer->subObj.magicCircuitPause = 10;
						break;
					case 30002:
						curPlayer->subObj.magicCircuit = 0;
						curPlayer->subObj.heatTimeLeft = 500;
						curPlayer->subObj.magicCircuitState = 3;
						curPlayer->subObj.maxHeatTime = 500;
						curPlayer->subObj.magicCircuitPause = 60;
						break;
					default:
						curPlayer->subObj.magicCircuit = USE_METER;
						break;
					}
					break;
				case 1:
					switch (USE_METER) {
					case 30000:
						curPlayer->subObj.magicCircuit = 20000;
						curPlayer->subObj.heatTimeLeft = 600;
						curPlayer->subObj.magicCircuitState = 2;
						curPlayer->subObj.maxHeatTime = 600;
						curPlayer->subObj.magicCircuitPause = 10;
						break;
					case 30001:
						curPlayer->subObj.magicCircuit = 0;
						curPlayer->subObj.heatTimeLeft = 550;
						curPlayer->subObj.magicCircuitState = 1;
						curPlayer->subObj.maxHeatTime = 550;
						curPlayer->subObj.magicCircuitPause = 10;
						break;
					case 30002:
						curPlayer->subObj.magicCircuit = 0;
						curPlayer->subObj.heatTimeLeft = 750;
						curPlayer->subObj.magicCircuitState = 3;
						curPlayer->subObj.maxHeatTime = 750;
						curPlayer->subObj.magicCircuitPause = 60;
						break;
					default:
						curPlayer->subObj.magicCircuit = USE_METER;
						break;
					}
					break;
				case 2:
					if (USE_METER == 20000) {
						curPlayer->subObj.magicCircuit = 0;
						curPlayer->subObj.heatTimeLeft = 550;
						curPlayer->subObj.magicCircuitState = 1;
						curPlayer->subObj.maxHeatTime = 550;
						curPlayer->subObj.magicCircuitPause = 10;
					}
					else
					{
						curPlayer->subObj.magicCircuit = USE_METER;
					}
					break;
				}
			}
		}

		if (XS_resetToPositions) {
			pP1->subObj.xPos = XS_p1Position;
			pP2->subObj.xPos = XS_p2Position;
			bool p1LookLeft = pP1->subObj.xPos > pP2->subObj.xPos;
			pP1->subObj.facingLeft = p1LookLeft;
			pP1->subObj.isOpponentToLeft = p1LookLeft;
			pP2->subObj.facingLeft = !p1LookLeft;
			pP2->subObj.isOpponentToLeft = !p1LookLeft;

			if (pP3->exists && pP3->subObj.charID != 51) {
				pP3->subObj.xPos = XS_p1AssistPosition;

				bool p3LookLeft = pP3->subObj.xPos > pP2->subObj.xPos;
				pP3->subObj.facingLeft = p3LookLeft;
				pP3->subObj.isOpponentToLeft = p3LookLeft;
			}

			if (pP4->exists && pP4->subObj.charID != 51) {
				pP4->subObj.xPos = XS_p2AssistPosition;

				bool p4LookLeft = pP4->subObj.xPos > pP1->subObj.xPos;
				pP4->subObj.facingLeft = p4LookLeft;
				pP4->subObj.isOpponentToLeft = p4LookLeft;
			}
		}

		switch (pActiveP1->subObj.charID) {
		case 0: //sion
			if (XS_sionBullets > 1) {
				pActiveP1->subObj.extraVariables[1] = XS_sionBullets - 1;
			}
			break;
		case 4: //maids
			if (XS_fMaidsHearts > 1) { //maids
				pP1->subObj.extraVariables[4] = XS_fMaidsHearts - 1;
				pP3->subObj.extraVariables[5] = XS_fMaidsHearts - 1;
			}
			break;
		case 31: //roa
			if (XS_roaHiddenCharges > 1) {
				pActiveP1->subObj.extraVariables[6] = XS_roaHiddenCharges - 1;
			}
			if (XS_roaVisibleCharges > 1) {
				pActiveP1->subObj.extraVariables[7] = XS_roaVisibleCharges - 1;
			}
			break;
		}

		switch (pActiveP2->subObj.charID) {
		case 0: //sion
			if (XS_sionBullets > 1) {
				pActiveP2->subObj.extraVariables[1] = XS_sionBullets - 1;
			}
			break;
		case 4: //maids
			if (XS_fMaidsHearts > 1) { //maids
				pP2->subObj.extraVariables[4] = XS_fMaidsHearts - 1;
				pP4->subObj.extraVariables[5] = XS_fMaidsHearts - 1;
			}
			break;
		case 31: //roa
			if (XS_roaHiddenCharges > 1) {
				pActiveP2->subObj.extraVariables[6] = XS_roaHiddenCharges - 1;
			}
			if (XS_roaVisibleCharges > 1) {
				pActiveP2->subObj.extraVariables[7] = XS_roaVisibleCharges - 1;
			}
			break;
		}

		pP1->subObj.guardGaugeHeal = 50000.0f;
		pP2->subObj.guardGaugeHeal = 50000.0f;
		if (pP3->exists) pP3->subObj.guardGaugeHeal = 50000.0f;
		if (pP4->exists) pP4->subObj.guardGaugeHeal = 50000.0f;

		pP1->subObj.guardGaugeState = 1;
		pP2->subObj.guardGaugeState = 1;
		if (pP3->exists) pP3->subObj.guardGaugeState = 1;
		if (pP4->exists) pP4->subObj.guardGaugeState = 1;

		int tempX;
		if (nSavedP1ActiveChar != 0) {
			pdP1Data->activeCharacter = 2;
			pP1->subObj.tagFlag = 1;
			pP3->subObj.tagFlag = 0;
			tempX = pP1->subObj.xPos;
			pP1->subObj.xPos = pP3->subObj.xPos;
			pP3->subObj.xPos = tempX;
		}

		if (nSavedP2ActiveChar != 1) {
			pdP2Data->activeCharacter = 3;
			pP2->subObj.tagFlag = 1;
			pP4->subObj.tagFlag = 0;
			tempX = pP2->subObj.xPos;
			pP2->subObj.xPos = pP4->subObj.xPos;
			pP4->subObj.xPos = tempX;
		}

	}

	if (XS_rate == RNG_EVERY_RESET)
	{
		if (XS_customRNG == RNG_SEED)
			SetSeed(XS_seed);
		if (XS_customRNG == RNG_RN)
			SetRN(XS_seed);
	}

	bDoReversal = false;
	nHoldButtons = 0;
	bHoldShield = false;
	bDidShield = false;
	if (enableRevTAS) {
		enableRevTAS = false;
		enableTAS = false;
	}

	ResetBars();

	nTempP1MeterGain = 0;
	nTempP2MeterGain = 0;
	nP1MeterGain = 0;
	nP2MeterGain = 0;
	prevComboPtr = 0;

	dualInputDisplayReset();

	// 2v2 support. for some reason. i suppose. i need a nap
	TASManagerObj[0].load("TAS.txt");
	TASManagerObj[1].load("TAS2.txt");
	TASManagerObj[2].load("TAS3.txt");
	TASManagerObj[3].load("TAS4.txt");

	didHitboxConnect = 0;

	loadCustomShader();
}

DWORD MBAA_ResetBattleMode = 0x00423380;
DWORD ResetCallback_PatchAddr = 0x0042357c;
__declspec(naked) void _naked_ResetCallback() {
	__asm {
		push esi;
		call[MBAA_ResetBattleMode];
	}

	PUSH_ALL;
	ResetCallback();
	POP_ALL;

	__asm {
		push 0x00423582;
		ret;
	}
}

// roundcall funcs

bool doLoad = false;
void RoundcallCallback() {
	if (XS_syncSavesWithFiles) saveStateManager.LoadFromFile();

	if (XS_saveStateSlot > 0 && saveStateManager.FullSaves[XS_saveStateSlot - 1]->IsSaved)
	{
		doLoad = true;
	}

	if (initLoadChars) {
		p1LoadChar = pP1->subObj.charID;
		p1LoadMoon = pP1->subObj.moon;
		p1LoadPal = pP1->subObj.palette + 1;

		p2LoadChar = pP2->subObj.charID;
		p2LoadMoon = pP2->subObj.moon;
		p2LoadPal = pP2->subObj.palette + 1;

		initLoadChars = false;
	}
}

DWORD RoundcallCallback_PatchAddr = 0x00472964;
__declspec(naked) void _naked_RoundcallCallback() {
	
	PUSH_ALL;
	RoundcallCallback();
	POP_ALL;

	__asm {
		ret;
	}
}

// char input funcs

void CharInputCallback() {
	if (nHoldButtons != 0) {
		pActiveP2->subObj.buttonInputs |= (0x1000 * nHoldButtons);
	}
	if (bHoldShield) {
		pActiveP2->subObj.buttonInputs |= 0x8000;
	}
}

DWORD CharInputCallback_PatchAddr = 0x0046dc1a;
DWORD MBAA_ReadCharacterInputs = 0x0046d7a0;
DWORD MBAA_FUN_004412c0 = 0x004412c0;
__declspec(naked) void _naked_CharInputCallback() {
	__asm {
		call[MBAA_ReadCharacterInputs];
	}
	PUSH_ALL;
	CharInputCallback();
	POP_ALL;

	__asm {
		call[MBAA_FUN_004412c0];
		push 0x0046dc24;
		ret;
	}
}

// pause funcs

int needPause = false;
void newPauseCallback2()
{
	if (oFreezeHotkey.keyDown()) {
		bFreeze = !bFreeze;
		if (bFreeze)
		{
			bSlow = false;
			_naked_newPauseCallback2_IsPaused = true;
		}
		else
			_naked_newPauseCallback2_IsPaused = false;
	}

	else if (!bFreeze && (oAdvanceFrameHotkey.keyDown() || oNextFrameHotkey.keyDown() || oPrevFrameHotkey.keyDown()))
	{
		bFreeze = true;
		bSlow = false;
		_naked_newPauseCallback2_IsPaused = true;
	}

	if (!bFreeze)
	{
		if (XS_gameSpeed)
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

	if (oAdvanceFrameHotkey.keyHeld())
		oAdvanceFrameHotkey.nHeldKeyCounter++;
	else
		oAdvanceFrameHotkey.nHeldKeyCounter = 0;

	if (_naked_newPauseCallback2_IsPaused &&
		(oAdvanceFrameHotkey.keyDown() ||
		oAdvanceFrameHotkey.nHeldKeyCounter >= 20 ||
		(bSlow && nFrameNumber % 4 >= XS_gameSpeed)) ||
		(doAutoAdvance && nInputHeldFrameAdvanceCounter > autoAdvanceFrames))
	{
		needPause = true;
		_naked_newPauseCallback2_IsPaused = false;
		nInputHeldFrameAdvanceCounter = 0;
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
		for (int i = 0; i < 4; i++) {
			TASManagerObj[i].incInputs();
		}
	}

	if (!_naked_newPauseCallback2_IsPaused && needPause != 2) {
		
		if (!isPaused()) {
			//log("calling rollforward 3");
			replayManager.rollForward();
		}
		
	}

	if (!_naked_newPauseCallback2_IsPaused)
		HandleExtendedTrainingEffects();

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

// training menu funcs
MenuWindow* mainWindow;

DWORD MBAA_FUN_00429020 = 0x00429020;
DWORD MBAA_FUN_00429b00 = 0x00429b00;
DWORD MBAA_FUN_0047ce20 = 0x0047ce20;
DWORD MBAA_FUN_0047d030 = 0x0047d030;
DWORD MBAA_FUN_004804a0 = 0x004804a0;
DWORD MBAA_FUN_004d8810 = 0x004d8810;

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

MenuInfo* InitMenuInfo(MenuInfo* menuInfo, MenuWindow* menuWindow, const char* label) {
	menuInfo->vftable = (void*)0x00535ec8;
	menuInfo->parentWindow = menuWindow;
	(menuInfo->label).maxLength = 0;
	(menuInfo->label).length = 0;
	(menuInfo->label).shortString[0] = 0;
	(menuInfo->blank).maxLength = 0xf;
	(menuInfo->blank).length = 0;
	(menuInfo->blank).shortString[0] = 0;
	(menuInfo->elementList).listStart = 0;
	(menuInfo->elementList).listEnd = 0;
	(menuInfo->elementList).listMaxEnd = 0;
	(menuInfo->selectableList).listStart = 0;
	(menuInfo->selectableList).listEnd = 0;
	(menuInfo->selectableList).listMaxEnd = 0;
	menuInfo->parentWindow = menuWindow;
	ReadDataFile(&menuInfo->label, label, strlen(label));
	ReadDataFile(&menuInfo->blank, "", 0);
	menuInfo->selectedElement = 0;
	menuInfo->prevSelectedElement = 0;
	menuInfo->finishedDrawing = 0;
	menuInfo->timeDisplayed = 0;
	menuInfo->field_0x70 = 99999;
	menuInfo->close = 0;
	return menuInfo;
}

MenuWindow* InitMenuWindow(MenuWindow* menuWindow) {
	(menuWindow->menuInfoList).listStart = 0;
	(menuWindow->menuInfoList).listEnd = 0;
	(menuWindow->menuInfoList).listMaxEnd = 0;
	(menuWindow->hoveredTag).length = 0;
	(menuWindow->hoveredTag).maxLength = 0xf;
	(menuWindow->hoveredTag).shortString[0] = 0;
	menuWindow->menuInfoIndex = 0;
	menuWindow->field_0x8 = 0;
	menuWindow->someYOffset = 0x10;
	menuWindow->field_0x50 = 0;
	menuWindow->didPress = 0;
	menuWindow->field_0x3c = 0;
	menuWindow->elementsXOffset = 0x140;
	menuWindow->vftable = (void*)0x0053d3c0;
	(menuWindow->label).maxLength = 0xf;
	(menuWindow->label).length = 0;
	(menuWindow->label).shortString[0] = 0;
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
	menuWindow->bgXOffset = 0;
	menuWindow->yOffset = 0xf0;
	menuWindow->textXWidth = 0x10;
	menuWindow->textYWidth = 0x10;
	ReadDataFile(&menuWindow->label, "", 0);
	menuWindow->paragraphMode = 0;
	menuWindow->field_0xb0 = 0;
	return menuWindow;
}

Element* AddSelectElement(MenuInfo* menuInfo, std::string label, std::vector<std::string> itemLabels, int pageNum, int elementNum) {
	Element* element = NEW_SELECT_ELEMENT();
	char tempTag[16];
	snprintf(tempTag, 15, "%i_%i", pageNum, elementNum);
	InitSelectElement(element, label.c_str(), tempTag, 0xa0);
	element->vftable = (void*)0x00536654;
	int itemNum = 0;
	for (auto& itemLabel : itemLabels) {
		Item* item = (Item*)NEW_ITEM();
		snprintf(tempTag, 15, "%i_%i_%i", pageNum, elementNum, itemNum);
		InitItem(item, itemLabel.c_str(), tempTag, itemNum);
		EnterIntoList((void*)(&element->itemList), (void*)(item));
		itemNum++;
	}
	if (label == " ") {
		element->selectItemLabelXOffset = 64;
	}
	EnterIntoList((void*)(&menuInfo->elementList), (void*)(element));
	return element;
}

Element* AddNormalElement(MenuInfo* menuInfo, std::string label, int pageNum, int elementNum) {
	Element* element = NEW_NORMAL_ELEMENT();
	char tempTag[16];
	snprintf(tempTag, 15, "%i_%i", pageNum, elementNum);
	InitNormalElement(element, label.c_str(), tempTag, 0);
	element->canSelect = 1;
	element->elementType = 1;
	element->vftable = (void*)0x0053604c;
	EnterIntoList((void*)(&menuInfo->elementList), (void*)(element));
	return element;
}

Element* AddSpaceElement(MenuInfo* menuInfo, int pageNum, int elementNum, int height = 8) {
	Element* element = NEW_NORMAL_ELEMENT();
	char tempTag[16];
	snprintf(tempTag, 15, "%i_%i", pageNum, elementNum);
	InitNormalElement(element, "", "", 0);
	element->canSelect = 0;
	element->elementType = 2;
	element->drawHeight = height;
	element->vftable = (void*)0x00536094;
	EnterIntoList((void*)(&menuInfo->elementList), (void*)(element));
	return element;
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

bool GetSetting(Setting& setting) {
	if (setting.element != nullptr && setting.element->GetItemListSize() != 0x0) {
		if (setting.settingType == 0) {
			setting.element->selectedItem = *setting.valuePtr;
			return true;
		}
		int iterator = 0;
		while (true) {
			int CurItemValue = setting.element->GetItemValue(iterator);
			if (CurItemValue == setting.storage) break;
			iterator++;
			if (setting.element->GetItemListSize() <= iterator) return false;
		}
		setting.element->selectedItem = iterator;
		return true;
	}
	return false;
}

void LoadMenuSettings(MenuWindow* window, MenuContainer& container) {
	if ((window->menuInfoList).listStart == 0x0 || (window->menuInfoList).listEnd - (window->menuInfoList).listStart == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}

	for (auto& page : container.pages) {
		for (auto& setting : page.settings) {
			GetSetting(setting);
		}
	}

	window->menuInfoIndex = container.savedSelection;
	for (int i = 0; i < container.pages.size(); i++) {
		(window->menuInfoList).listStart[i]->selectedElement = container.get(i)->savedSelection;
	}
}

//init extended window, info, elements, and items
MenuWindow* InitExtendedSettingsMenu(MenuWindow* extendedWindow) {
	InitMenuWindow(extendedWindow);
	ReadDataFile(&extendedWindow->label, "EXTENDED SETTINGS", 18);
	int pageNum = 0;
	for (auto& page : XS_Menu.pages)
	{
		MenuInfo* extendedInfo = NEW_MENU_INFO();
		if (extendedInfo != 0x0)
		{
			InitExtendedSettingsMenuInfo(extendedInfo, extendedWindow);
		}
		int elementNum = 0;
		for (auto& setting : page.settings) {
			if (setting.label == "") {
				AddSpaceElement(extendedInfo, pageNum, elementNum);
			}
			else if (setting.itemLabels.size() == 0) {
				AddNormalElement(extendedInfo, setting.label, pageNum, elementNum);
			}
			else {
				setting.element = AddSelectElement(extendedInfo, setting.label, setting.itemLabels, pageNum, elementNum);
			}
			elementNum++;
		}
		EnterIntoList((void*)(&extendedWindow->menuInfoList), (void*)(extendedInfo));
		pageNum++;
		//_FUN_00429b00(extendedInfo, "EXTENDED_SETTING");
	}

	LoadMenuSettings(extendedWindow, XS_Menu);
	extendedWindow->dimScreenPercentage = 0.0;
	extendedWindow->u_layer = 0x2f1;
	extendedWindow->isMenuLit = 1;
	extendedWindow->isBlurred = 0;
	extendedWindow->paragraphMode = 2;
	extendedWindow->elementsXOffset = 0xfa;
	extendedWindow->textXWidth = 0xe;
	return extendedWindow;
}

//init extended window, info, elements, and items
MenuWindow* InitHotkeySettingsMenu(MenuWindow* hotkeyWindow) {
	InitMenuWindow(hotkeyWindow);
	ReadDataFile(&hotkeyWindow->label, "HOTKEY SETTINGS", 18);
	int pageNum = 0;
	for (auto& page : HK_Menu.pages)
	{
		MenuInfo* hotkeyInfo = NEW_MENU_INFO();
		if (hotkeyInfo != 0x0)
		{
			InitHotkeySettingsMenuInfo(hotkeyInfo, hotkeyWindow);
		}
		int elementNum = 0;
		for (auto& setting : page.settings) {
			if (setting.label == "") {
				AddSpaceElement(hotkeyInfo, pageNum, elementNum);
			}
			else if (setting.itemLabels.size() == 0) {
				AddNormalElement(hotkeyInfo, setting.label, pageNum, elementNum);
			}
			else {
				setting.element = AddSelectElement(hotkeyInfo, setting.label, setting.itemLabels, pageNum, elementNum);
			}
			elementNum++;
		}
		EnterIntoList((void*)(&hotkeyWindow->menuInfoList), (void*)(hotkeyInfo));
		pageNum++;
	}

	LoadMenuSettings(hotkeyWindow, HK_Menu);
	hotkeyWindow->dimScreenPercentage = 0.0;
	hotkeyWindow->u_layer = 0x2f1;
	hotkeyWindow->isMenuLit = 1;
	hotkeyWindow->isBlurred = 0;
	hotkeyWindow->paragraphMode = 2;
	hotkeyWindow->elementsXOffset = 0xfa;
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

bool SetSetting(Setting& setting) {
	if (setting.element != nullptr && setting.element->GetItemListSize() != 0x0) {
		int selectionIndex = setting.element->selectedItem;
		int elementValue = (setting.element->itemList).listStart[selectionIndex]->value;
		setting.storage = elementValue;
		if (setting.settingType == 0) {
			*setting.valuePtr = elementValue;
		}
		return true;
	}
	return false;
}

void SaveMenuSettings(MenuWindow* window, MenuContainer& container) {
	if ((window->menuInfoList).listStart == 0x0 || (window->menuInfoList).listEnd - (window->menuInfoList).listStart == 0) {
		__asm {
			call[MBAA_UnrecoveredJumptable];
		}
	}

	for (auto& page : container.pages) {
		for (auto& setting : page.settings) {
			SetSetting(setting);
		}
	}

	container.savedSelection = window->menuInfoIndex;
	for (int i = 0; i < container.pages.size(); i++) {
		container.get(i)->savedSelection = (window->menuInfoList).listStart[i]->selectedElement;
	}
}

//save extended settings and free everything
void CloseExtendedSettings(MenuWindow* extendedWindow) {
	SaveMenuSettings(extendedWindow, XS_Menu);

	XS_colorGuide = false;
	*(bool*)(adMBAABase + adXS_colorGuide) = XS_colorGuide;
	bShowFrameBarPreview = false;
	bShowFrameBarYPreview = false;

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
	SaveMenuSettings(hotkeyWindow, HK_Menu);

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

int ScrollAccelTimer = 0;
const int ScrollAccelThreshold = 50;
bool LoopingScrolling(Element* element, int& storage, int min, int max, int interval = 1, int accelInterval = 0) {
	bool retVal = false;
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;

	if (accelInterval != 0) {
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (item == LEFT) { //left
		retVal = true;
		if (storage - interval < min) {
			interval -= max - min + 1;
		}
		storage -= interval;
		
	}
	else if (item == RIGHT) { //right
		retVal = true;
		if (storage + interval > max) {
			interval -= max - min + 1;
		}
		storage += interval;
	}

	if (accelInterval != 0 &&
		(*(byte*)(adMBAABase + adP1MenuDirInput) == 4 || *(byte*)(adMBAABase + adP1MenuDirInput) == 6)) {
		ScrollAccelTimer++;
	}
	else {
		ScrollAccelTimer = 0;
	}

	element->selectedItem = MIDDLE;
	return retVal;
}


bool NormalScrolling(Element* element, int& storage, int min, int max, int interval = 1, int accelInterval = 0) {
	bool retVal = false;
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;
	int targetIndex = MIDDLE;

	if (accelInterval != 0) {
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (storage == max)
	{
		if (item == MIDDLE) { //left
			retVal = true;
			storage -= interval;
		}
	}
	else if (storage == min)
	{
		if (item == MIDDLE) { //right
			retVal = true;
			storage += interval;
		}
	}
	else
	{
		if (item == LEFT) { //left
			retVal = true;
			storage = max(storage - interval, min);
		}
		else if (item == RIGHT) { //right
			retVal = true;
			storage = min(storage + interval, max);
		}
	}

	if (accelInterval != 0 && 
		(*(byte*)(adMBAABase + adP1MenuDirInput) == 4 || *(byte*)(adMBAABase + adP1MenuDirInput) == 6)) {
		ScrollAccelTimer++;
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (storage == max) targetIndex = RIGHT;
	if (storage == min) targetIndex = LEFT;
	element->selectedItem = targetIndex;
	return retVal;
}

void CFMeterScrolling(Element* element, int& storage, bool toggle) {
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;
	int targetIndex = MIDDLE;
	int interval = 100;
	int accelInterval = 1000;

	if (!toggle) {
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 30000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}

		switch (storage) {
		case 30002: //BLOOD HEAT
			if (item == MIDDLE) storage--;
			break;
		case 30001: //HEAT
			if (item == LEFT) storage--;
			else if (item == RIGHT) storage++;
			break;
		case 30000: //MAX
			if (item == LEFT) storage -= 100;
			else if (item == RIGHT) storage++;
			break;
		case 0:
			if (item == MIDDLE) storage += 100;
			break;
		default:
			if (item == LEFT) {
				storage = max(storage - interval, 0);
			}
			else if (item == RIGHT) {
				storage = min(storage + interval, 30000);
			}
			break;
		}
	}
	else
	{
		interval = 1;
		accelInterval = 10;
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 30000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}

		switch (storage) {
		case 30002: //BLOOD HEAT
			if (item == MIDDLE) storage--;
			break;
		case 30001: //HEAT
			if (item == LEFT) storage--;
			else if (item == RIGHT) storage++;
			break;
		case 30000: //MAX
			if (item == LEFT) storage--;
			else if (item == RIGHT) storage++;
			break;
		case 0:
			if (item == MIDDLE) storage++;
			break;
		default:
			if (item == LEFT) {
				if (storage % 100 - interval < 0) {
					interval -= 100;
				}
				storage -= interval;
			}
			if (item == RIGHT) {
				if (storage % 100 + interval > 99) {
					interval -= 100;
				}
				storage += interval;
			}
			break;
		}
	}

	if (*(byte*)(adMBAABase + adP1MenuDirInput) == 4 || *(byte*)(adMBAABase + adP1MenuDirInput) == 6) {
		ScrollAccelTimer++;
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (storage == 30002) targetIndex = RIGHT;
	if (storage == 0) targetIndex = LEFT;
	element->selectedItem = targetIndex;
}

void HMeterScrolling(Element* element, int& storage, bool toggle) {
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;
	int targetIndex = MIDDLE;
	int interval = 100;
	int accelInterval = 1000;

	if (!toggle) {
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 20000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}

		switch (storage) {
		case 20000: //HEAT
			if (item == MIDDLE) storage--;
			break;
		case 0:
			if (item == MIDDLE) storage += 100;
			break;
		default:
			if (item == LEFT) storage = max(storage - interval, 0);
			if (item == RIGHT) storage = min(storage + interval, 20000);
			break;
		}
	}
	else
	{
		interval = 1;
		accelInterval = 10;
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 20000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}

		switch (storage) {
		case 20000: //HEAT
			if (item == MIDDLE) storage--;
			break;
		case 0:
			if (item == MIDDLE) storage += 1;
			break;
		default:
			if (item == LEFT) {
				if (storage % 100 - interval < 0) {
					interval -= 100;
				}
				storage -= interval;
			}
			if (item == RIGHT) {
				if (storage % 100 + interval > 99) {
					interval -= 100;
				}
				storage += interval;
			}
			break;
		}
	}

	if (*(byte*)(adMBAABase + adP1MenuDirInput) == 4 || *(byte*)(adMBAABase + adP1MenuDirInput) == 6) {
		ScrollAccelTimer++;
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (storage == 20000) targetIndex = RIGHT;
	if (storage == 0) targetIndex = LEFT;
	element->selectedItem = targetIndex;

}

void HighlightSwitch(int option, std::array<uint8_t, 4>& array) {
	switch (option) {
	default:
	case NO_HIGHLIGHT:
		array = { 255, 255, 255, 0 };
		break;
	case RED_HIGHLIGHT:
		array = { 255, 90, 90, 1 };
		break;
	case YELLOW_HIGHLIGHT:
		array = { 255, 255, 0, 1 };
		break;
	case GREEN_HIGHLIGHT:
		array = { 60, 255, 60, 1 };
		break;
	case BLUE_HIGHLIGHT:
		array = { 90, 90, 255, 1 };
		break;
	case PURPLE_HIGHLIGHT:
		array = { 255, 90, 255, 1 };
		break;
	case BLACK_HIGHLIGHT:
		array = { 60, 60, 60, 1 };
		break;
	}
}

void PositionScrolling(Element* element, int& storage, bool toggle) {
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;
	int targetIndex = MIDDLE;
	int interval = 128;
	int accelInterval = 1024;
	storage += 0x10000; //change range from -0x10000 - 0x10000 to 0x0 - 0x20000, undone at end of func
	if (!toggle) {
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 0x20000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}

		switch (storage) {
		case 0x20000:
			if (item == MIDDLE) storage -= interval;
			break;
		case 0x0:
			if (item == MIDDLE) storage += interval;
			break;
		default:
			if (item == LEFT) storage = max(storage - interval, 0x0);
			if (item == RIGHT) storage = min(storage + interval, 0x20000);
			break;
		}
	}
	else
	{
		interval = 1;
		accelInterval = 8;
		if (ScrollAccelTimer >= ScrollAccelThreshold) {
			interval = accelInterval;
			if (storage < 0x20000 && storage % accelInterval != 0)
			{
				if (item == LEFT) {
					interval = (storage % accelInterval);
				}
				else if (item == RIGHT) {
					interval = (accelInterval - storage % accelInterval);
				}
			}
		}
		switch (storage) {
		case 0x20000:
			if (item == MIDDLE) storage--;
			break;
		case 0x0:
			if (item == MIDDLE) storage++;
			break;
		default:
			if (item == LEFT) {
				if (storage % 128 - interval < 0) {
					interval -= 128;
				}
				storage -= interval;
			}
			if (item == RIGHT) {
				if (storage % 128 + interval > 127) {
					interval -= 128;
				}
				storage += interval;
			}
			break;
		}
	}

	if (*(byte*)(adMBAABase + adP1MenuDirInput) == 4 || *(byte*)(adMBAABase + adP1MenuDirInput) == 6) {
		ScrollAccelTimer++;
	}
	else {
		ScrollAccelTimer = 0;
	}

	if (storage == 0x20000) targetIndex = RIGHT;
	if (storage == 0x0) targetIndex = LEFT;
	element->selectedItem = targetIndex;
	storage -= 0x10000; //undoing de-negativing at beginning of func
}

bool PageScrolling(Element* element, MenuWindow* window, int numPages) {
	bool retVal = false;
	enum index {
		LEFT, MIDDLE, RIGHT
	};
	int item = element->selectedItem;
	int curInfo = window->menuInfoIndex;
	int destInfo = curInfo;

	if (item == LEFT) { //left
		retVal = true;
		destInfo = window->menuInfoIndex - 1;
		if (destInfo < 0) destInfo = numPages;
		window->menuInfoIndex = destInfo;
	}
	else if (item == RIGHT) { //right
		retVal = true;
		destInfo = window->menuInfoIndex + 1;
		if (destInfo > numPages) destInfo = 0;
		window->menuInfoIndex = destInfo;
	}

	element->selectedItem = MIDDLE;
	if (retVal)
	{
		MenuList elements = (window->menuInfoList).listStart[destInfo]->elementList;
		int numElements = (elements.listEnd - elements.listStart);
		if (numElements != 0)
		{
			(window->menuInfoList).listStart[destInfo]->selectedElement = numElements - 1;
		}
	}
	return retVal;
}

void XS_Reversal_Slots(Element* element, int &reversal_value, bool APos, bool DPos) {
	int temp = reversal_value % 0x00010000;
	if (LoopingScrolling(element, temp, 0, vPatternNames.size() - 1)) reversal_value = temp;
	int pat = GetPattern(nP2CharacterID, vPatternNames[reversal_value % 0x00010000]);
	bool isSC = (pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Ground ||
		pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Air ||
		pat == pActiveP2->cmdFileDataPtr->ShieldCounter_Crouch);
	if (isSC && reversal_value >> 16 == 0) DPos = true;
	if (APos) {
		reversal_value = 0;
	}
	if (DPos && reversal_value != 0) {
		reversal_value = (reversal_value + 0x00010000) % 0x00070000;
		byte stance = getPatStance(pActiveP2, pat);
		byte shieldCancel = getShieldCancel(pActiveP2, pat);
		if (pActiveP2->subObj.moon == 2) shieldCancel &= 0x2;
		byte exShield = shieldCancel & 0x2;
		byte heldShield = shieldCancel & 0x1;
		byte validShields[7] = { !isSC, (stance & 0x1) * exShield, (stance & 0x1) * heldShield,
			(stance & 0x4) * exShield, (stance & 0x4) * heldShield,
			(stance & 0x2) * exShield, (stance & 0x2) * heldShield };
		while (!validShields[reversal_value >> 16]) {
			reversal_value = (reversal_value + 0x00010000) % 0x00070000;
		}
	}
}

void ExtendedMenuInputChecking() {
	MenuWindow* extendedWindow;
	MenuInfo* curMenuInfo;
	Element* curElement;
	__asm {
		mov extendedWindow, ecx;
	}
	char labelBuf[32];
	static bool aPrev = false;
	static bool dPrev = false;
	bool mouseAPress = lClick && MenuControlsClickIsPress;
	bool aHeld = (*(bool*)(adMBAABase + adP1AInput) || mouseAPress) && extendedWindow->openSubmenuIndex == 2;
	bool aPressed = aHeld && !aPrev;
	bool dHeld = *(bool*)(adMBAABase + adP1DInput) && extendedWindow->openSubmenuIndex == 2;
	bool dPressed = dHeld && !dPrev;
	bShowFrameBarPreview = false;
	bShowFrameBarYPreview = false;
	curMenuInfo = extendedWindow->menuInfoList.listStart[extendedWindow->menuInfoIndex];
	curElement = curMenuInfo->elementList.listStart[curMenuInfo->selectedElement];
	switch ((eXS_PAGES)extendedWindow->menuInfoIndex) {
	case eXS_PAGES::REVERSALS:
	{
		if (vPatternNames.size() == 1)
		{
			int nP2CharacterNumber = *(int*)(adMBAABase + dwP2CharNumber);
			int nP2Moon = *(int*)(adMBAABase + dwP2CharMoon);
			nP2CharacterID = 10 * nP2CharacterNumber + nP2Moon;
			vPatternNames = GetPatternList(nP2CharacterID);
		}
		//clear maids reversal on switch if no longer valid
		if (nP2CharacterID >= 190 && nP2CharacterID <= 192) {
			vPatternNames = GetMaidsPatternList(nP2CharacterID, pP2->subObj.tagFlag);
			if ((XS_reversalSlot1 & 0xFFFF) % 1000 > vPatternNames.size() - 1) {
				XS_reversalSlot1 = 0;
			}
			if ((XS_reversalSlot2 & 0xFFFF) % 1000 > vPatternNames.size() - 1) {
				XS_reversalSlot2 = 0;
			}
			if ((XS_reversalSlot3 & 0xFFFF) % 1000 > vPatternNames.size() - 1) {
				XS_reversalSlot3 = 0;
			}
			if ((XS_reversalSlot4 & 0xFFFF) % 1000 > vPatternNames.size() - 1) {
				XS_reversalSlot4 = 0;
			}
		}

		switch ((eREVERSALS)curMenuInfo->selectedElement) {
		case eREVERSALS::REVERSAL_SLOT_1:
			XS_Reversal_Slots(curElement, XS_reversalSlot1, aPressed, dPressed);
			break;
		case eREVERSALS::REVERSAL_SLOT_2:
			XS_Reversal_Slots(curElement, XS_reversalSlot2, aPressed, dPressed);
			break;
		case eREVERSALS::REVERSAL_SLOT_3:
			XS_Reversal_Slots(curElement, XS_reversalSlot3, aPressed, dPressed);
			break;
		case eREVERSALS::REVERSAL_SLOT_4:
			XS_Reversal_Slots(curElement, XS_reversalSlot4, aPressed, dPressed);
			break;
		case eREVERSALS::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eREVERSALS::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eREVERSALS::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		snprintf(labelBuf, 31, "%s%s", REV_SHIELD_PREFIX[XS_reversalSlot1 >> 16], vPatternNames[XS_reversalSlot1 % 0x00010000].c_str());
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_1]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_1]->textOpacity = XS_reversalSlot1 == 0 ? 0.5f : 1.0f;

		snprintf(labelBuf, 31, "%s%s", REV_SHIELD_PREFIX[XS_reversalSlot2 >> 16], vPatternNames[XS_reversalSlot2 % 0x00010000].c_str());
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_2]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_2]->textOpacity = XS_reversalSlot2 == 0 ? 0.5f : 1.0f;

		snprintf(labelBuf, 31, "%s%s", REV_SHIELD_PREFIX[XS_reversalSlot3 >> 16], vPatternNames[XS_reversalSlot3 % 0x00010000].c_str());
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_3]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_3]->textOpacity = XS_reversalSlot3 == 0 ? 0.5f : 1.0f;

		snprintf(labelBuf, 31, "%s%s", REV_SHIELD_PREFIX[XS_reversalSlot4 >> 16], vPatternNames[XS_reversalSlot4 % 0x00010000].c_str());
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::REVERSAL_SLOT_4]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)eREVERSALS::WEIGHT_4]->textOpacity = XS_reversalSlot4 == 0 ? 0.5f : 1.0f;

		PopulateAirAndGroundReversals(&vAirReversals, &vGroundReversals, nP2CharacterID, &vPatternNames,
			XS_reversalSlot1 % 0x00010000, XS_reversalSlot2 % 0x00010000, XS_reversalSlot3 % 0x00010000, XS_reversalSlot4 % 0x00010000);

		break;
	}
	case eXS_PAGES::TRAINING:
	{
		int healthInterval = aHeld ? 1 : 114;
		int healthAccel = aHeld ? 10 : 570;
		switch ((eTRAINING)curMenuInfo->selectedElement) {
		case eTRAINING::P1_METER:
			if (pP1->subObj.moon != 2) {
				CFMeterScrolling(curElement, XS_p1Meter, aHeld);
			}
			else {
				HMeterScrolling(curElement, XS_p1Meter, aHeld);
			}
			break;
		case eTRAINING::P2_METER:
			if (pP2->subObj.moon != 2) {
				CFMeterScrolling(curElement, XS_p2Meter, aHeld);
			}
			else {
				HMeterScrolling(curElement, XS_p2Meter, aHeld);
			}
			break;
		case eTRAINING::P1_HEALTH:
			NormalScrolling(curElement, XS_p1Health, 0, 11400, healthInterval, healthAccel);
			break;
		case eTRAINING::P2_HEALTH:
			NormalScrolling(curElement, XS_p2Health, 0, 11400, healthInterval, healthAccel);
			break;
		case eTRAINING::HITS_UNTIL_BURST:
			NormalScrolling(curElement, XS_hitsUntilBurst, 0, 101);
			break;
		case eTRAINING::HITS_UNTIL_BUNKER:
			NormalScrolling(curElement, XS_hitsUntilBunker, 0, 101);
			break;
		case eTRAINING::HITS_UNTIL_FORCE_GUARD:
			NormalScrolling(curElement, XS_hitsUntilForceGuard, 0, 101);
			break;
		case eTRAINING::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eTRAINING::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eTRAINING::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		if (pP1->subObj.moon != 2) {
			switch (XS_p1Meter) {
			case 30000:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel("MAX");
				break;
			case 30001:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel("HEAT");
				break;
			case 30002:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel("BLOOD HEAT");
				break;
			default:
				snprintf(labelBuf, 31, "%i.%02i%%", XS_p1Meter / 100, XS_p1Meter % 100);
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel(labelBuf);
			}
		}
		else {
			switch (XS_p1Meter) {
			case 20000:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel("HEAT");
				break;
			default:
				snprintf(labelBuf, 31, "%i.%02i%%", XS_p1Meter / 100, XS_p1Meter % 100);
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_METER]->SetCurItemLabel(labelBuf);
			}
		}
		
		if (pP2->subObj.moon != 2) {
			switch (XS_p2Meter) {
			case 30000:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel("MAX");
				break;
			case 30001:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel("HEAT");
				break;
			case 30002:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel("BLOOD HEAT");
				break;
			default:
				snprintf(labelBuf, 31, "%i.%02i%%", XS_p2Meter / 100, XS_p2Meter % 100);
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel(labelBuf);
			}
		}
		else {
			switch (XS_p2Meter) {
			case 20000:
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel("HEAT");
				break;
			default:
				snprintf(labelBuf, 31, "%i.%02i%%", XS_p2Meter / 100, XS_p2Meter % 100);
				(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_METER]->SetCurItemLabel(labelBuf);
			}
		}

		snprintf(labelBuf, 31, "%i (%.1f%%)", XS_p1Health, XS_p1Health / 114.0f);
		(curMenuInfo->elementList).listStart[(int)eTRAINING::P1_HEALTH]->SetCurItemLabel(labelBuf);
		snprintf(labelBuf, 31, "%i (%.1f%%)", XS_p2Health, XS_p2Health / 114.0f);
		(curMenuInfo->elementList).listStart[(int)eTRAINING::P2_HEALTH]->SetCurItemLabel(labelBuf);

		snprintf(labelBuf, 31, "%i", XS_hitsUntilBurst);
		(curMenuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_BURST]->SetCurItemLabel(labelBuf);
		snprintf(labelBuf, 31, "%i", XS_hitsUntilBunker);
		(curMenuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_BUNKER]->SetCurItemLabel(labelBuf);
		snprintf(labelBuf, 31, "%i", XS_hitsUntilForceGuard);
		(curMenuInfo->elementList).listStart[(int)eTRAINING::HITS_UNTIL_FORCE_GUARD]->SetCurItemLabel(labelBuf);

		break;
	}
	case eXS_PAGES::HIGHLIGHTS:
	{
		switch ((eHIGHLIGHTS)curMenuInfo->selectedElement) {
		case eHIGHLIGHTS::HIGHLIGHTS:
			SetRegistryValue(sHIGHLIGHTS, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::GUARD:
			HighlightSwitch(curElement->selectedItem, arrBlockingHighlightSetting);
			SetRegistryValue(sBLOCKING_HIGHLIGHT, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::HIT:
			HighlightSwitch(curElement->selectedItem, arrHitHighlightSetting);
			SetRegistryValue(sHIT_HIGHLIGHT, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::ARMOR:
			HighlightSwitch(curElement->selectedItem, arrArmorHighlightSetting);
			SetRegistryValue(sARMOR_HIGHLIGHT, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::THROW_PROTECTION:
			HighlightSwitch(curElement->selectedItem, arrThrowProtectionHighlightSetting);
			SetRegistryValue(sTHROW_PROTECTION_HIGHLIGHT, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::IDLE:
			HighlightSwitch(curElement->selectedItem, arrIdleHighlightSetting);
			SetRegistryValue(sIDLE_HIGHLIGHT, curElement->selectedItem);
			break;
		case eHIGHLIGHTS::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eHIGHLIGHTS::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eHIGHLIGHTS::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		break;
	}
	case eXS_PAGES::POSITIONS:
	{
		switch ((ePOSITIONS)curMenuInfo->selectedElement) {
		case ePOSITIONS::P1_POSITION:
			PositionScrolling(curElement, XS_p1Position, aHeld);
			break;
		case ePOSITIONS::P1_ASSIST_POSITION:
			PositionScrolling(curElement, XS_p1AssistPosition, aHeld);
			break;
		case ePOSITIONS::P2_POSITION:
			PositionScrolling(curElement, XS_p2Position, aHeld);
			break;
		case ePOSITIONS::P2_ASSIST_POSITION:
			PositionScrolling(curElement, XS_p2AssistPosition, aHeld);
			break;
		case ePOSITIONS::MOVE_TO_POSITIONS:
			if (aPressed) {
				pP1->subObj.xPos = XS_p1Position;
				pP2->subObj.xPos = XS_p2Position;
				bool p1LookLeft = pP1->subObj.xPos > pP2->subObj.xPos;
				pP1->subObj.facingLeft = p1LookLeft;
				pP1->subObj.isOpponentToLeft = p1LookLeft;
				pP2->subObj.facingLeft = !p1LookLeft;
				pP2->subObj.isOpponentToLeft = !p1LookLeft;

				if (pP3->exists && pP3->subObj.charID != (BYTE)eCharID::HIME) {
					pP3->subObj.xPos = XS_p1AssistPosition;

					bool p3LookLeft = pP3->subObj.xPos > pP2->subObj.xPos;
					pP3->subObj.facingLeft = p3LookLeft;
					pP3->subObj.isOpponentToLeft = p3LookLeft;
				}

				if (pP4->exists && pP4->subObj.charID != (BYTE)eCharID::HIME) {
					pP4->subObj.xPos = XS_p2AssistPosition;

					bool p4LookLeft = pP4->subObj.xPos > pP1->subObj.xPos;
					pP4->subObj.facingLeft = p4LookLeft;
					pP4->subObj.isOpponentToLeft = p4LookLeft;
				}
			}
			break;
		case ePOSITIONS::SET_CURRENT_POSITIONS:
			if (aPressed) {
				XS_p1Position = pP1->subObj.xPos;
				XS_p2Position = pP2->subObj.xPos;

				if (pP3->exists && pP3->subObj.charID != (BYTE)eCharID::HIME) {
					XS_p1AssistPosition = pP3->subObj.xPos;
				}
				if (pP4->exists && pP4->subObj.charID != (BYTE)eCharID::HIME) {
					XS_p2AssistPosition = pP4->subObj.xPos;
				}
			}
			break;
		case ePOSITIONS::INVERT:
			if (aPressed) {
				int temp = XS_p1Position;
				XS_p1Position = XS_p2Position;
				XS_p2Position = temp;

				temp = XS_p1AssistPosition;
				XS_p1AssistPosition = XS_p2AssistPosition;
				XS_p2AssistPosition = temp;
			}
			break;
		case ePOSITIONS::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case ePOSITIONS::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case ePOSITIONS::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		snprintf(labelBuf, 31, "%i %i", XS_p1Position >> 7, (XS_p1Position + 0x10000) % 0x80);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P1_POSITION]->SetCurItemLabel(labelBuf);
		snprintf(labelBuf, 31, "%i %i", XS_p1AssistPosition >> 7, (XS_p1AssistPosition + 0x10000) % 0x80);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P1_ASSIST_POSITION]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P1_ASSIST_POSITION]->textOpacity = pP3->exists ? 1.0f : 0.5f;

		snprintf(labelBuf, 31, "%i %i", XS_p2Position >> 7, (XS_p2Position + 0x10000) % 0x80);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P2_POSITION]->SetCurItemLabel(labelBuf);
		snprintf(labelBuf, 31, "%i %i", XS_p2AssistPosition >> 7, (XS_p2AssistPosition + 0x10000) % 0x80);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P2_ASSIST_POSITION]->SetCurItemLabel(labelBuf);
		(curMenuInfo->elementList).listStart[(int)ePOSITIONS::P2_ASSIST_POSITION]->textOpacity = pP4->exists ? 1.0f : 0.5f;

		break;
	}
	case eXS_PAGES::CHARACTER:
	{
		switch ((eCHARACTER)curMenuInfo->selectedElement) {
		case eCHARACTER::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eCHARACTER::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eCHARACTER::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		break;
	}
	case eXS_PAGES::HITBOXES:
	{
		switch ((eHITBOXES)curMenuInfo->selectedElement) {
		case eHITBOXES::HITBOX_STYLE:
			SetRegistryValue(sHITBOX_STYLE, curElement->selectedItem);
			break;
		case eHITBOXES::COLOR_BLIND_MODE:
			SetRegistryValue(sCOLOR_BLIND_MODE, curElement->selectedItem);
			break;
		case eHITBOXES::ORIGIN_STYLE:
			SetRegistryValue(sORIGIN_STYLE, curElement->selectedItem);
			break;
		case eHITBOXES::DRAW_GROUND:
			SetRegistryValue(sDRAW_GROUND, curElement->selectedItem);
			break;
		case eHITBOXES::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eHITBOXES::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eHITBOXES::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		break;
	}
	case eXS_PAGES::SAVE_STATES:
	{
		switch ((eSAVE_STATES)curMenuInfo->selectedElement) {
		case eSAVE_STATES::SAVE_STATE_SLOT:
			XS_saveStateSlot = curElement->selectedItem;
			if (aPressed) {
				saveStateManager.FullSaves[XS_saveStateSlot - 1]->unsave();
				if (XS_syncSavesWithFiles) saveStateManager.SaveToFile();
			}
			break;
		case eSAVE_STATES::SAVE_STATE:
			if (aPressed && XS_saveStateSlot > 0) {
				saveStateManager.FullSaves[XS_saveStateSlot - 1]->save();
				if (XS_syncSavesWithFiles) saveStateManager.SaveToFile();
			}
			break;
		case eSAVE_STATES::CLEAR_ALL_SAVES:
			if (aPressed) {
				for (int i = 0; i < MAX_SAVES; i++) {
					saveStateManager.FullSaves[i]->unsave();
				}
				if (XS_syncSavesWithFiles) saveStateManager.SaveToFile();
			}
			break;
		case eSAVE_STATES::IMPORT_SAVE:
			if (aPressed) {
				saveStateManager.FullSaves[XS_saveStateSlot - 1]->nport();
			}
			break;
		case eSAVE_STATES::EXPORT_SAVE:
			if (aPressed) {
				saveStateManager.FullSaves[XS_saveStateSlot - 1]->xport();
			}
			break;
		case eSAVE_STATES::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eSAVE_STATES::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eSAVE_STATES::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		for (int i = 0; i < MAX_SAVES; i++) {
			if (saveStateManager.FullSaves[i]->IsSaved) {
				snprintf(labelBuf, 31, "%s %02i (%s)", "SLOT", i + 1, "SAVED");
			}
			else {
				snprintf(labelBuf, 31, "%s %02i (%s)", "SLOT", i + 1, "NOT SAVED");
			}
			(curMenuInfo->elementList).listStart[(int)eSAVE_STATES::SAVE_STATE_SLOT]->SetItemLabel(labelBuf, i + 1);
		}

		break;
	}
	case eXS_PAGES::FRAME_DATA:
	{
		switch ((eFRAME_DATA)curMenuInfo->selectedElement) {
		case eFRAME_DATA::CONSOLE_DATA:
			*(byte*)(adMBAABase + adXS_frameData) = curElement->selectedItem;
			SetRegistryValue(sCONSOLE_DATA, curElement->selectedItem);
			break;
		case eFRAME_DATA::IN_GAME_FRAME_DISPLAY:
			SetRegistryValue(sFRAME_DISPLAY, curElement->selectedItem);
			break;
		case eFRAME_DATA::SHOW_HITSTOP_AND_FREEZE:
			*(byte*)(adMBAABase + adXS_showHitstopAndFreeze) = curElement->selectedItem;
			SetRegistryValue(sDISPLAY_FREEZE, curElement->selectedItem);
			if (curElement->selectedItem == 0) {
				(curMenuInfo->elementList).listStart[(int)eFRAME_DATA::SHOW_INPUTS]->selectedItem = 0;
				*(byte*)(adMBAABase + adXS_showInputs) = 0;
				SetRegistryValue(sDISPLAY_INPUTS, 0);
			}
			break;
		case eFRAME_DATA::SHOW_COUNTERHIT:
			SetRegistryValue(sDISPLAY_COUNTERHIT, curElement->selectedItem);
			break;
		case eFRAME_DATA::SHOW_INPUTS:
			*(byte*)(adMBAABase + adXS_showInputs) = curElement->selectedItem;
			SetRegistryValue(sDISPLAY_INPUTS, curElement->selectedItem);
			if (curElement->selectedItem == 1) {
				(curMenuInfo->elementList).listStart[(int)eFRAME_DATA::SHOW_HITSTOP_AND_FREEZE]->selectedItem = 1;
				*(byte*)(adMBAABase + adXS_showHitstopAndFreeze) = 1;
				SetRegistryValue(sDISPLAY_FREEZE, 1);
			}
			break;
		case eFRAME_DATA::SHOW_CANCEL_WINDOWS:
			*(byte*)(adMBAABase + adXS_showCancel) = curElement->selectedItem;
			SetRegistryValue(sDISPLAY_CANCELS, curElement->selectedItem);
			break;
		case eFRAME_DATA::SCROLL_DISPLAY:
		{
			int scrollMax;
			scrollMax = nBarCounter > DISPLAY_RANGE ? min(nBarCounter - DISPLAY_RANGE, BAR_MEMORY_SIZE - DISPLAY_RANGE) : 1;
			NormalScrolling(curElement, XS_scrollDisplay, -scrollMax, 0);
			*(short*)(adMBAABase + adXS_frameScroll) = -XS_scrollDisplay;
			bShowFrameBarPreview = true;
			break;
		}
		case eFRAME_DATA::COLOR_GUIDE:
			if (aPressed) {
				XS_colorGuide = !XS_colorGuide;
				*(bool*)(adMBAABase + adXS_colorGuide) = XS_colorGuide;
			}
			break;
		case eFRAME_DATA::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eFRAME_DATA::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eFRAME_DATA::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		snprintf(labelBuf, 31, "%i", XS_scrollDisplay);
		(curMenuInfo->elementList).listStart[(int)eFRAME_DATA::SCROLL_DISPLAY]->SetCurItemLabel(labelBuf);

		break;
	}
	case eXS_PAGES::RNG:
	{
		switch ((eRNG)curMenuInfo->selectedElement) {
		case eRNG::CUSTOM_RNG:
			XS_customRNG = curElement->selectedItem;
			break;
		case eRNG::SEED:
			LoopingScrolling(curElement, XS_seed, 0, 0x0000ffff, 1, 10);
			break;
		case eRNG::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eRNG::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eRNG::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		snprintf(labelBuf, 31, "%i", XS_seed);
		(curMenuInfo->elementList).listStart[(int)eRNG::SEED]->SetCurItemLabel(labelBuf);

		(curMenuInfo->elementList).listStart[(int)eRNG::RATE]->textOpacity = XS_customRNG ? 1.0f : 0.5f;
		(curMenuInfo->elementList).listStart[(int)eRNG::SEED]->textOpacity = XS_customRNG ? 1.0f : 0.5f;

		break;
	}
	case eXS_PAGES::UI:
	{
		switch ((eUI)curMenuInfo->selectedElement) {
		case eUI::SHOW_STATS:
			SetRegistryValue(sDISPLAY_STATS, curElement->selectedItem);
			break;
		case eUI::ACCURATE_COMBO_DAMAGE:
			SetRegistryValue(sACCURATE_COMBO_DAMAGE, curElement->selectedItem);
			break;
		case eUI::P1_INPUT_DISPLAY:
			SetRegistryValue(sP1_INPUT_DISPLAY, curElement->selectedItem);
			break;
		case eUI::P2_INPUT_DISPLAY:
			SetRegistryValue(sP2_INPUT_DISPLAY, curElement->selectedItem);
			break;
		case eUI::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eUI::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eUI::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		break;
	}
	case eXS_PAGES::SYSTEM:
	{
		switch ((eSYSTEM)curMenuInfo->selectedElement) {
		case eSYSTEM::HIDE_HUD: // set these options in-menu for live preview
			SetRegistryValue(sHIDE_HUD, curElement->selectedItem);
			XS_hideHUD = curElement->selectedItem;
			break;
		case eSYSTEM::HIDE_SHADOWS:
			SetRegistryValue(sHIDE_SHADOWS, curElement->selectedItem);
			XS_hideShadows = curElement->selectedItem;
			break;
		case eSYSTEM::HIDE_EXTRAS:
			SetRegistryValue(sHIDE_EXTRAS, curElement->selectedItem);
			XS_hideExtras = curElement->selectedItem;
			break;
		case eSYSTEM::BACKGROUND:
			XS_background = curElement->selectedItem;
			break;
		case eSYSTEM::DEFAULT:
			if (aPressed) XS_Menu.pages[extendedWindow->menuInfoIndex]._default();
			break;
		case eSYSTEM::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eSYSTEM::PAGE:
			PageScrolling(curElement, extendedWindow, XS_Menu.pages.size() - 1);
			break;
		}

		(curMenuInfo->elementList).listStart[(int)eSYSTEM::HIDE_EXTRAS]->textOpacity = XS_hideHUD ? 0.5f : 1.0f;


		break;
	}
	}

	extendedWindow->SetLabel(XS_Menu.pages[extendedWindow->menuInfoIndex].label.c_str());

	static bool oldFN1Input = false;
	bool curFN1Input = *(bool*)(adMBAABase + adP1FN1Input);
	if (curFN1Input && !oldFN1Input) {
		extendedWindow->menuInfoIndex--;
		if (extendedWindow->menuInfoIndex < 0) {
			extendedWindow->menuInfoIndex = XS_Menu.pages.size() - 1;
		}
	}
	oldFN1Input = curFN1Input;

	static bool oldFN2Input = false;
	bool curFN2Input = *(bool*)(adMBAABase + adP1FN2Input);
	if (curFN2Input && !oldFN2Input) {
		extendedWindow->menuInfoIndex++;
		if (extendedWindow->menuInfoIndex > XS_Menu.pages.size() - 1) {
			extendedWindow->menuInfoIndex = 0;
		}
	}
	oldFN2Input = curFN2Input;

	aPrev = aHeld;
	dPrev = dHeld;
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
	MenuInfo* curMenuInfo;
	Element* curElement;
	__asm {
		mov hotkeyWindow, ecx;
	}
	char labelBuf[32];
	static bool aPrev = false;
	bool mouseAPress = lClick && MenuControlsClickIsPress;
	bool aHeld = (*(bool*)(adMBAABase + adP1AInput) || mouseAPress) && hotkeyWindow->openSubmenuIndex == 2;
	bool aPressed = aHeld && !aPrev;
	curMenuInfo = (hotkeyWindow->menuInfoList).listStart[hotkeyWindow->menuInfoIndex];
	curElement = (curMenuInfo->elementList).listStart[curMenuInfo->selectedElement];
	switch (hotkeyWindow->menuInfoIndex) {
	case 0:
		switch ((eHK_PAGE1)curMenuInfo->selectedElement) {
		case eHK_PAGE1::FREEZE:
			CheckNewHotkey(aPressed, oFreezeHotkey, sFREEZE_KEY_REG);
			break;
		case eHK_PAGE1::ADVANCE_FRAME:
			CheckNewHotkey(aPressed, oAdvanceFrameHotkey, sADVANCE_FRAME_KEY_REG);
			break;
		case eHK_PAGE1::NEXT_FRAME:
			CheckNewHotkey(aPressed, oNextFrameHotkey, sNEXT_FRAME_KEY_REG);
			break;
		case eHK_PAGE1::PREV_FRAME:
			CheckNewHotkey(aPressed, oPrevFrameHotkey, sPREV_FRAME_KEY_REG);
			break;
		case eHK_PAGE1::TOGGLE_HITBOXES:
			CheckNewHotkey(aPressed, oToggleHitboxesHotkey, sTOGGLE_HITBOXES_KEY_REG);
			break;
		case eHK_PAGE1::TOGGLE_FRAME_BAR:
			CheckNewHotkey(aPressed, oToggleFrameBarHotkey, sTOGGLE_FRAME_BAR_KEY_REG);
			break;
		case eHK_PAGE1::TOGGLE_HIGHLIGHTS:
			CheckNewHotkey(aPressed, oToggleHighlightsHotkey, sTOGGLE_HIGHLIGHTS_KEY_REG);
			break;
		case eHK_PAGE1::QUEUE_REVERSAL:
			CheckNewHotkey(aPressed, oQueueReversalHotkey, sQUEUE_REVERSAL_KEY_REG);
			break;
		case eHK_PAGE1::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eHK_PAGE1::PAGE:
			PageScrolling(curElement, hotkeyWindow, HK_Menu.pages.size() - 1);
			break;
		}

		GetKeyStateMenuLabel(labelBuf, oFreezeHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::FREEZE]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oAdvanceFrameHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::ADVANCE_FRAME]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oNextFrameHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::NEXT_FRAME]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oPrevFrameHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::PREV_FRAME]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oToggleHitboxesHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::TOGGLE_HITBOXES]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oToggleFrameBarHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::TOGGLE_FRAME_BAR]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oToggleHighlightsHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::TOGGLE_HIGHLIGHTS]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oQueueReversalHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE1::QUEUE_REVERSAL]->SetCurItemLabel(labelBuf);

		break;
	case 1:
		switch ((eHK_PAGE2)curMenuInfo->selectedElement) {
		case eHK_PAGE2::SAVE_STATE:
			CheckNewHotkey(aPressed, oSaveStateHotkey, sSAVE_STATE_KEY_REG);
			break;
		case eHK_PAGE2::PREV_SAVE_SLOT:
			CheckNewHotkey(aPressed, oPrevSaveSlotHotkey, sPREV_SAVE_SLOT_KEY_REG);
			break;
		case eHK_PAGE2::NEXT_SAVE_SLOT:
			CheckNewHotkey(aPressed, oNextSaveSlotHotkey, sNEXT_SAVE_SLOT_KEY_REG);
			break;
		case eHK_PAGE2::FRAME_BAR_LEFT:
			CheckNewHotkey(aPressed, oFrameBarLeftHotkey, sFRAME_BAR_LEFT_KEY_REG);
			break;
		case eHK_PAGE2::FRAME_BAR_RIGHT:
			CheckNewHotkey(aPressed, oFrameBarRightHotkey, sFRAME_BAR_RIGHT_KEY_REG);
			break;
		case eHK_PAGE2::INCREMENT_RNG:
			CheckNewHotkey(aPressed, oIncrementRNGHotkey, sINCREMENT_RNG_KEY_REG);
			break;
		case eHK_PAGE2::DECREMENT_RNG:
			CheckNewHotkey(aPressed, oDecrementRNGHotkey, sDECREMENT_RNG_KEY_REG);
			break;
		case eHK_PAGE2::RETURN:
			if (aPressed) curMenuInfo->close = 1;
			break;
		case eHK_PAGE2::PAGE:
			PageScrolling(curElement, hotkeyWindow, HK_Menu.pages.size() - 1);
			break;
		}

		GetKeyStateMenuLabel(labelBuf, oSaveStateHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::SAVE_STATE]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oPrevSaveSlotHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::PREV_SAVE_SLOT]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oNextSaveSlotHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::NEXT_SAVE_SLOT]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oFrameBarLeftHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::FRAME_BAR_LEFT]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oFrameBarRightHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::FRAME_BAR_RIGHT]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oIncrementRNGHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::INCREMENT_RNG]->SetCurItemLabel(labelBuf);
		GetKeyStateMenuLabel(labelBuf, oDecrementRNGHotkey);
		(curMenuInfo->elementList).listStart[(int)eHK_PAGE2::DECREMENT_RNG]->SetCurItemLabel(labelBuf);

		break;
	}

	static bool oldFN1Input = false;
	bool curFN1Input = *(bool*)(adMBAABase + adP1FN1Input);
	if (curFN1Input && !oldFN1Input) {
		hotkeyWindow->menuInfoIndex--;
		if (hotkeyWindow->menuInfoIndex < 0) {
			hotkeyWindow->menuInfoIndex = HK_Menu.pages.size() - 1;
		}
	}
	oldFN1Input = curFN1Input;

	static bool oldFN2Input = false;
	bool curFN2Input = *(bool*)(adMBAABase + adP1FN2Input);
	if (curFN2Input && !oldFN2Input) {
		hotkeyWindow->menuInfoIndex++;
		if (hotkeyWindow->menuInfoIndex > HK_Menu.pages.size() - 1) {
			hotkeyWindow->menuInfoIndex = 0;
		}
	}
	oldFN2Input = curFN2Input;

	if (nHOTKEY_CD_TIMER > 0) nHOTKEY_CD_TIMER--;

	aPrev = aHeld;
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

DWORD UpdateMenus_PatchAddr = 0x0047e1b6;
//add menus to update if-chain
__declspec(naked) void _naked_UpdateMenus() {
	__asm {
		cmp enableMouseControls, 0;
		je _EXIT;
		cmp showDebugMenu, 0;
		jne _EXIT;
		cmp MenuControlsScrollUp, 0;
		jne _SETUP;
		cmp MenuControlsScrollDown, 0;
		jne _SETDOWN;
		cmp MenuControlsClickIsPress, 0;
		je _CHECKDIRECTION;
		cmp lClick, 0;
		je _EXIT;
		cmp MenuControlsMouseInBounds, 0;
		je _EXIT;
	_SETPRESS:
		mov dword ptr[esp + 0x64], 1;
		jmp _EXIT;
	_CHECKDIRECTION:
		cmp lClick, 0;
		jne _SETLEFT;
		cmp rClick, 0;
		jne _SETRIGHT;
		jmp _EXIT;
	_SETRIGHT:
		mov dword ptr[esp + 0x60], 6;
		jmp _EXIT;
	_SETLEFT:
		mov dword ptr[esp + 0x60], 4;
		jmp _EXIT;
	_SETUP:
		mov dword ptr[esp + 0x60], 8;
		jmp _EXIT;
	_SETDOWN:
		mov dword ptr[esp + 0x60], 2;
	_EXIT:

	}

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
		jz _CHECK_YESNO;
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
		jmp _EXIT_CHAIN;

	_CHECK_YESNO:
		cmp dword ptr[edi + 0xc4], ebx;
		jz _CHECK_BATTLE;
		push 0x0047e1be;
		ret;

	_CHECK_BATTLE:
		push 0x0047e1da;
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

void Handle_REV(char* buffer) {
	if (vPatternNames.size() == 1)
	{
		int nP2CharacterNumber = *(int*)(adMBAABase + dwP2CharNumber);
		int nP2Moon = *(int*)(adMBAABase + dwP2CharMoon);
		nP2CharacterID = 10 * nP2CharacterNumber + nP2Moon;
		vPatternNames = GetPatternList(nP2CharacterID);
	}
	if (strcmp(vPatternNames[XS_reversalSlot1 % 0x00010000].c_str(), "OFF") == 0) {
		snprintf(buffer, 128, "%sNo reversal.", SUB_INFO_PREFIX);
		return;
	}
	else if (strcmp(vPatternNames[XS_reversalSlot1 % 0x00010000].c_str(), "CUSTOM") == 0) {
		snprintf(buffer, 128, "%sReversal using TAS_REV.txt (must be in MBAA.exe directory).", SUB_INFO_PREFIX);
		return;
	}
	snprintf(buffer, 128, "%sReversal with %s%s (Press D).", SUB_INFO_PREFIX, REV_SHIELD_PREFIX[XS_reversalSlot1 >> 16], vPatternNames[XS_reversalSlot1 % 0x00010000].c_str());
}

void Handle_METER1(char* buffer) {
	char meterStr[32];
	switch (XS_p1Meter) {
	case 30000:
		snprintf(meterStr, 32, "MAX");
		break;
	case 30001:
		snprintf(meterStr, 32, "HEAT");
		break;
	case 30002:
		snprintf(meterStr, 32, "BLOOD HEAT");
		break;
	case 20000:
		if (pP1->subObj.moon == 2) {
			snprintf(meterStr, 32, "HEAT");
			break;
		}
	default:
		snprintf(meterStr, 32, "%i.%02i%%", XS_p1Meter / 100, XS_p1Meter % 100);
	}
	snprintf(buffer, 128, "%sReset meter to %s (Hold A).", SUB_INFO_PREFIX, meterStr);
}

void Handle_METER2(char* buffer) {
	char meterStr[32];
	switch (XS_p2Meter) {
	case 30000:
		snprintf(meterStr, 32, "MAX");
		break;
	case 30001:
		snprintf(meterStr, 32, "HEAT");
		break;
	case 30002:
		snprintf(meterStr, 32, "BLOOD HEAT");
		break;
	case 20000:
		if (pP1->subObj.moon == 2) {
			snprintf(meterStr, 32, "HEAT");
			break;
		}
	default:
		snprintf(meterStr, 32, "%i.%02i%%", XS_p2Meter / 100, XS_p2Meter % 100);
	}
	snprintf(buffer, 128, "%sReset meter to %s (Hold A).", SUB_INFO_PREFIX, meterStr);
}

void Handle_HEALTH1(char* buffer) {
	snprintf(buffer, 128, "%sReset health to %i (%.1f%%) (Hold A).", SUB_INFO_PREFIX, XS_p1Health, XS_p1Health / 114.0f);
}

void Handle_HEALTH2(char* buffer) {
	snprintf(buffer, 128, "%sReset health to %i (%.1f%%) (Hold A).", SUB_INFO_PREFIX, XS_p2Health, XS_p2Health / 114.0f);
}

void Handle_BURST(char* buffer) {
	snprintf(buffer, 128, "%sBurst after %i hit(s).", SUB_INFO_PREFIX, XS_hitsUntilBurst);
}

void Handle_BUNKER(char* buffer) {
	snprintf(buffer, 128, "%sBunker after %i hit(s).", SUB_INFO_PREFIX, XS_hitsUntilBunker);
}

void Handle_FORCEGUARD(char* buffer) {
	snprintf(buffer, 128, "%sForce guard after %i hit(s).", SUB_INFO_PREFIX, XS_hitsUntilForceGuard);
}

void Handle_POS1(char* buffer) {
	snprintf(buffer, 128, "%sReset position to %i pixels %i sub pixels (Hold A).", SUB_INFO_PREFIX, XS_p1Position >> 7, (XS_p1Position + 0x10000) % 0x80);
}

void Handle_POS1A(char* buffer) {
	snprintf(buffer, 128, "%sReset position to %i pixels %i sub pixels (Hold A).", SUB_INFO_PREFIX, XS_p1AssistPosition >> 7, (XS_p1AssistPosition + 0x10000) % 0x80);
}

void Handle_POS2(char* buffer) {
	snprintf(buffer, 128, "%sReset position to %i pixels %i sub pixels (Hold A).", SUB_INFO_PREFIX, XS_p2Position >> 7, (XS_p2Position + 0x10000) % 0x80);
}

void Handle_POS2A(char* buffer) {
	snprintf(buffer, 128, "%sReset position to %i pixels %i sub pixels (Hold A).", SUB_INFO_PREFIX, XS_p2AssistPosition >> 7, (XS_p2AssistPosition + 0x10000) % 0x80);
}

void Handle_SCROLL(char* buffer) {
	snprintf(buffer, 128, "%sScroll display by %i frames.", SUB_INFO_PREFIX, XS_scrollDisplay);
}

void Handle_RNG(char* buffer) {
	snprintf(buffer, 128, "%sUse a seed / value of %i", SUB_INFO_PREFIX, XS_seed);
}

void HandleInformationMenu() {
	MenuWindow* trainingMenu = *(MenuWindow**)(adMBAABase + adTrainingMenu);
	if (trainingMenu == 0 || trainingMenu->InformationMenu == 0) return;

	MenuWindow* extendedWindow = trainingMenu->ExtendedSettings;
	InfoWindow* infoWindow = trainingMenu->InformationMenu;
	char buffer[128];

	char* checkTag;
	if (infoWindow->elementTag.maxLength < 0x10) {
		checkTag = (char*)&(infoWindow->elementTag.shortString);
	}
	else {
		checkTag = infoWindow->elementTag.pLongString;
	}

	std::string checkStr(checkTag);

	if (MAIN_INFORMATION_MAP.find(checkStr) != MAIN_INFORMATION_MAP.end()) {
		snprintf(buffer, 128, "%s%s", MAIN_INFO_PREFIX, MAIN_INFORMATION_MAP.at(checkStr));
		SetInfoWindowText(buffer, trainingMenu->InformationMenu->mainInfo);
	}

	if (infoWindow->itemTag.maxLength < 0x10) {
		checkTag = (char*)&(infoWindow->itemTag.shortString);
	}
	else {
		checkTag = infoWindow->itemTag.pLongString;
	}

	checkStr = std::string(checkTag);

	if (SUB_INFORMATION_MAP.find(checkStr) != SUB_INFORMATION_MAP.end()) {
		const char* subInfo = SUB_INFORMATION_MAP.at(checkStr);
		if (subInfo[0] == '_') {
			if (strcmp(subInfo, "_REV") == 0) {
				Handle_REV(buffer);
			}
			else if (strcmp(subInfo, "_METER1") == 0) {
				Handle_METER1(buffer);
			}
			else if (strcmp(subInfo, "_METER2") == 0) {
				Handle_METER2(buffer);
			}
			else if (strcmp(subInfo, "_HEALTH1") == 0) {
				Handle_HEALTH1(buffer);
			}
			else if (strcmp(subInfo, "_HEALTH2") == 0) {
				Handle_HEALTH2(buffer);
			}
			else if (strcmp(subInfo, "_BURST") == 0) {
				Handle_BURST(buffer);
			}
			else if (strcmp(subInfo, "_BUNKER") == 0) {
				Handle_BUNKER(buffer);
			}
			else if (strcmp(subInfo, "_FORCEGUARD") == 0) {
				Handle_FORCEGUARD(buffer);
			}
			else if (strcmp(subInfo, "_POS1") == 0) {
				Handle_POS1(buffer);
			}
			else if (strcmp(subInfo, "_POS1A") == 0) {
				Handle_POS1A(buffer);
			}
			else if (strcmp(subInfo, "_POS2") == 0) {
				Handle_POS2(buffer);
			}
			else if (strcmp(subInfo, "_POS2A") == 0) {
				Handle_POS2A(buffer);
			}
			else if (strcmp(subInfo, "_SCROLL") == 0) {
				Handle_SCROLL(buffer);
			}
			else if (strcmp(subInfo, "_RNG") == 0) {
				Handle_RNG(buffer);
			}
		}
		else {
			snprintf(buffer, 128, "%s%s", SUB_INFO_PREFIX, SUB_INFORMATION_MAP.at(checkStr));
		}
		SetInfoWindowText(buffer, trainingMenu->InformationMenu->subInfo);
	}
}

DWORD HandleInformationWindow_PatchAddr = 0x004da725;
__declspec(naked) void _naked_HandleInformationMenu() {

	PUSH_ALL;
	HandleInformationMenu();
	POP_ALL;

	__asm {
		pop edi;
		pop ebx;
		mov esp, ebp;
		pop ebp;
		ret;
	}
}


const char* const extendedPrefix = "XS_";
const char* const hotkeyPrefix = "HK_";
DWORD InformationWindowSetTargetWindow_PatchAddr = 0x0047e33c;
__declspec(naked) void _naked_InformationWindowSetTargetWindow() {
	__asm {
		mov eax, dword ptr[edi + 0xe0];
		cmp eax, ebx;
		jz __NEXT;
		lea ecx, [esp + 0x14];
		push ecx;
		mov ecx, extendedPrefix;
		mov ebx, eax;
		push 0x0047e345;
		ret;

	__NEXT:
		mov eax, dword ptr[edi + 0xe4];
		cmp eax, ebx;
		jz __END;
		lea ecx, [esp + 0x14];
		push ecx;
		mov ecx, hotkeyPrefix;
		mov ebx, eax;
		push 0x0047e345;
		ret;

	__END:
		lea eax, [esp + 0x14];
		push eax;
		xor ecx, ecx;
		mov ebx, edi;
		push 0x0047e345;
		ret;
	}
}

//CSS funcs

int CSSGetRandomChar() {
	const DWORD MBAA_CSSGetRandomChar = 0x0048c4f0;
	__asm {
		call[MBAA_CSSGetRandomChar];
	}
}

int CSSCheckMoonAvailable(int port, int moonID) {
	const DWORD MBAA_CheckMoon = 0x0048c310;
	__asm {
		push port;
		mov ebx, moonID;
		call[MBAA_CheckMoon];
		add esp, 0x4;
	}
}

void CSSDoPalette(int port, byte directionalInput, int buttonInput) {
	const DWORD MBAA_DoPalette = 0x0048ab00;
	__asm {
		mov esi, port;
		push buttonInput;
		push directionalInput;
		call[MBAA_DoPalette];
	}
}

int ChangeStage(int movement, int stageID) {
	const DWORD MBAA_ChangeStage = 0x00488760;
	__asm {
		mov eax, movement;
		mov ecx, stageID;
		call[MBAA_ChangeStage];
	}
}

int ResetCSSState() {
	const DWORD MBAA_ResetCSSState = 0x0048c880;
	__asm {
		push 0x1;
		call[MBAA_ResetCSSState];
		add esp, 0x4;
	}
}

void MouseControlsDoCharSelect() {
	struct CSSInfo {
		int base;
		int state;
		int port;
		int x0c;
		int gridPos;
		int charID;
		int moon;
		int pal;
		int x20;
	};

	int portSelecting = 0;
	CSSInfo* cssInfo = (CSSInfo*)(adMBAABase + 0x0034d8e8);
	if (cssInfo->state == 5) {
		cssInfo = (CSSInfo*)(adMBAABase + 0x0034d90c);
		portSelecting = 1;
	}

	struct CSSMoonSelect {
		int queueShowMoonSelection;
		int showMoonSelection;
		int selectedMoon;
		int timer;
		float rotationDegrees;
		float rotationProgress;
		int x18;
		int x1c;
		float fadeProgress;
		int x24;
		int x28;
		int x2c;
		int x30;
		int x34;
	};

	CSSMoonSelect* moonSelectArr = *(CSSMoonSelect**)(adMBAABase + 0x003717dc);
	CSSMoonSelect* moonSelect = &moonSelectArr[portSelecting];

	switch (cssInfo->state) {
	case 0: //character
	{
		struct Portrait {
			int id;
			int idCopy;
			int charID;
			int x;
			int y;
			float turnAnim;
		};
		//portrait textures are 48 x 64
		//portrait visuals are 34 x 42
		int target = -1;
		Portrait* portraitPtr = *(Portrait**)(adMBAABase + 0x0037181C);
		if (portraitPtr == 0x0) return;
		for (int i = 0; i < 63; i++) {
			Point center = Point(portraitPtr[i].x + 24, portraitPtr[i].y + 32);
			if (abs(mousePos.x - center.x) < 17 && abs(mousePos.y - center.y) < 21) {
				target = i;
			}
		}

		if (target != -1 && portraitPtr[target].charID != -1) {
			if (cssInfo->gridPos != target) MenuSound();
			cssInfo->gridPos = target;
			int* charIDMap = (int*)(adMBAABase + 0x00151d28);
			cssInfo->charID = charIDMap[target];
			if (lClick) {
				if (cssInfo->charID == 99) {
					cssInfo->charID = CSSGetRandomChar();
				}
				cssInfo->state = 1;
				moonSelect->x2c = 0;
				moonSelect->queueShowMoonSelection = 1;
				moonSelect->x24 = 0;
				moonSelect->x28 = 0;
				SelectSound();
			}
		}
		break;
	}
	case 1: //moon
	{
		if (rClick) {
			moonSelect->showMoonSelection = 0;
			cssInfo->state = 0;
		}
		else if (lClick) {
			bool inXRange = portSelecting == 0 ? (mousePos.x < 275) : (mousePos.x > 365);
			if (mousePos.y > 225 && inXRange) {
				if (mousePos.y < 400 && mousePos.y > 300) {
					cssInfo->state = 2;
					moonSelect->showMoonSelection = 100;
					moonSelect->fadeProgress = 1.0f;
					SelectSound();
				}
				else {
					int movement = mousePos.y > 350 ? 1 : -1;
					int avail = 0;
					do {
						moonSelect->selectedMoon += movement;
						if (moonSelect->selectedMoon > 4) {
							moonSelect->selectedMoon = 0;
						}
						else if (moonSelect->selectedMoon < 0) {
							moonSelect->selectedMoon = 4;
						}
						int* moonMap2 = (int*)(adMBAABase + 0x0014d3f4);
						int moonID = moonMap2[moonSelect->selectedMoon];
						avail = CSSCheckMoonAvailable(portSelecting, moonID);
					} while (avail == 0);
					moonSelect->rotationProgress = 1.0f * movement;
					moonSelect->rotationDegrees = moonSelect->rotationDegrees + 60.0f * movement;
					int* moonMap = (int*)(adMBAABase + 0x0014d3cc);
					cssInfo->moon = moonMap[moonSelect->selectedMoon];
					MenuSound();
				}
			}
		}
		break;
	}
	case 2: //palette
	{
		bool inYRange = mousePos.y > 288 && mousePos.y < 400;
		bool inSelectRange = portSelecting == 0 ? (mousePos.x > 110 && mousePos.x < 220) : (mousePos.x > 420 && mousePos.x < 530);
		bool inGeneralRange = portSelecting == 0 ? (mousePos.x < 275) : (mousePos.x > 365);
		int movementCheck = portSelecting == 0 ? 165 : 475;
		int movement = mousePos.x > movementCheck ? 1 : -1;
		if (inYRange && inSelectRange) {
			struct SomeCSSStruct {
				byte x0[0x34];
				int paletteSelectedTime;
				int isRandomPaletteSelected;
				byte x3c[0x1a0];
			};
			SomeCSSStruct* someStruct = *(SomeCSSStruct**)(adMBAABase + 0x0034d808);
			if (mousePos.y < 384) {
				int basePal = (cssInfo->pal / 6) * 6;
				int newPal = basePal + (mousePos.y - 288.0f) / 16;
				if (cssInfo->pal != newPal) MenuSound();
				cssInfo->pal = newPal;
				someStruct[portSelecting].isRandomPaletteSelected = 0;
			}
			else {
				if (someStruct[portSelecting].isRandomPaletteSelected != 1) {
					someStruct[portSelecting].paletteSelectedTime = 0;
					MenuSound();
				}
				someStruct[portSelecting].isRandomPaletteSelected = 1;
			}
		}

		if (rClick) {
			cssInfo->state = 0;
		}
		else if (lClick && inYRange) {
			if (inSelectRange) {
				CSSDoPalette(portSelecting, 0, 1);
				cssInfo->state = 4;
			}
			else if (inGeneralRange) {
				CSSDoPalette(portSelecting, 6 * movement, 0);
			}

		}
		break;
	}
	}
}

void MouseControlsDoStageSelect() {
	struct cssGamestate {
		byte x00[0x3c];
		int state;
	};
	cssGamestate* gs = (cssGamestate*)(adMBAABase + 0x00373158);

	if (rClick) {
		ResetCSSState();
		gs->state = 0;
	}
	if (lClick) {
		if (mousePos.y > 224 && mousePos.y < 248) {
			gs->state = 4;
			SelectSound();
		}
		else {
			int movement = mousePos.y > 236 ? 1 : -1;
			int stageID = *(int*)(adMBAABase + 0x0034fd98);
			*(int*)(adMBAABase + 0x0036e988) = stageID;
			int newStageID = ChangeStage(movement, stageID);
			*(int*)(adMBAABase + 0x0034fd98) = newStageID;
			*(float*)(adMBAABase + 0x0036e980) = -1.0f * movement;
			MenuSound();
		}
	}
}

void DoCSSMouseControls() {
	static Point prevMousePos;

	bool idleMouse = false;
	if (prevMousePos == mousePos && !lClick && !rClick) {
		idleMouse = true;
	}
	prevMousePos = mousePos;
	if (idleMouse) return;


	struct cssGamestate {
		byte x00[0x3c];
		int state;
	};
	cssGamestate* gs = (cssGamestate*)(adMBAABase + 0x00373158);

	switch (gs->state) {
	case 0: //char select
		MouseControlsDoCharSelect();
		break;
	case 1: //stage select
		MouseControlsDoStageSelect();
	}
}

void CSSCallback() {
	for (int i = 0; i < MAX_SAVES; i++)
		saveStateManager.FullSaves[i]->unsave();

	vPatternNames = GetEmptyPatternList();
	XS_reversalSlot1 = 0;
	XS_reversalSlot2 = 0;
	XS_reversalSlot3 = 0;
	XS_reversalSlot4 = 0;

	initLoadChars = true;

	if (enableMouseControls && !showDebugMenu) {
		DoCSSMouseControls();
	}
		
}

DWORD CSSCallback_PatchAddr = 0x004271e0;
__declspec(naked) void _naked_CSSCallback() {

	PUSH_ALL;
	CSSCallback();
	POP_ALL;

	__asm {
		ret;
	}
}

// ---

DWORD _naked_DisableShadows_FuncAddr = 0x0041a390;
__declspec(naked) void _naked_DisableShadows() { 

	// patched at 0x0041b47c and 0041b58a
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

__declspec(naked) void _naked_DisableShadows2() {

	// patched at 0x0041b47c and 0041b58a
	__asm {

		//cmp shouldDrawBackground, 0;
		cmp shouldDrawShadow, 0;
		JE _SKIP;

		call[_naked_DisableShadows_FuncAddr];

	_SKIP:

		push 0041b58fh;
		ret;
	}
}

// attack display funcs

void doFastReversePenalty() {

	//if (fastReversePenalty == 0) {
	//	return;
	//}

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

// input funcs
void inputCallback() {

	// does melty update controllers in a thread? and is this inside said thread?

	profileFunction();

	KeyState::updateControllers(); // this call is taking half a ms, and wtf why am i even caring
	
	if (enableRevTAS) {
		TASManagerObj[pActiveP2->subObj.index].setInputs(pActiveP2->subObj.index);
	}
	else {
		for (int i = 0; i < 4; i++) {
			TASManagerObj[i].setInputs(i);
		}
	}
	
	replayManager.setInputs();

}

void inputCallbackMidGameUpdate() {

	for (int i = 0; i < 4; i++) {
		//TASManagerObj[i].setInputs(i);
	}

}

__declspec(naked) void _naked_inputCallback() {

	// patched at 0x0041f1a6

	PUSH_ALL;
	inputCallback();
	POP_ALL;

	__asm {
		add esp, 0090h;
		ret;
	}

}

__declspec(naked, noinline) void _naked_inputCallbackMidGameUpdate() {
	
	// patched at 0x0042373d

	// overwritten asm 
	emitCall(0x004745e0); // maybeReadControlsOrSomething?

	PUSH_ALL;
	inputCallbackMidGameUpdate();
	POP_ALL;

	emitCall(0x0046db40); // ControlCharacter

	emitJump(0x00423747);

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

DWORD CustomHealthRegen_PatchAddr = 0x004242fa;
//overwrites 100% health regen
__declspec(naked) void _naked_CustomHealthRegen() {
	__asm {
		push 0x00424302;
		cmp esi, 0x555c2c;
		jl __P1;
		cmp esi, 0x556728;
		jl __P2;
		cmp esi, 0x557224;
		jl __P3;

	__P4:
		mov edx, XS_p2Health;
		mov[esi], edx;
		ret;
	__P1:
		mov edx, XS_p1Health;
		mov[esi], edx;
		ret;
	__P2:
		mov edx, XS_p2Health;
		mov[esi], edx;
		ret;
	__P3:
		mov edx, XS_p1Health;
		mov[esi], edx;
		ret;
	}
}

__declspec(naked) void _naked_init2v2Hack() {

	// patched at 0040e3ab

	PUSH_ALL;
	doWeird2v2Fixes();
	POP_ALL;

	// overwritten asm from 0040e3ab. i dont trust the compiler
	emitByte(0x8B);
	emitByte(0x0D);
	emitByte(0xB0);
	emitByte(0xE6);
	emitByte(0x76);
	emitByte(0x00);

	emitJump(0x0040e3b1);

}

DWORD ForceDummyGuard_PatchAddr = 0x004710a3;
DWORD MBAA_GetTrainingDummyBlockType = 0x004703e0;
__declspec(naked) void _naked_ForceDummyGuard() {
	__asm {
		mov edx, edi;
		mov eax, ebp;
		call[MBAA_GetTrainingDummyBlockType];
	}
	if (bForceGuard) {
		__asm {
			mov eax, XS_forceGuardStance;
			add eax, 0x1;
		}
	}
	__asm {
		push 0x004710ac;
		ret;
	}
}

void GetInitialHealthForTrueComboDamage() {
	if (pAttPlayer->OwnerSubObjPtr != 0) {
		pAttPlayer = pAttPlayer->OwnerSubObjPtr;
	}
	byte curComboData = playerAuxDataArr[pAttPlayer->ownerIndex].comboCalcData[0].index;
	if (playerAuxDataArr[pAttPlayer->ownerIndex].comboCalcData[curComboData].numHits == 0) {
		trueComboData[pAttPlayer->ownerIndex][curComboData].startingHealth = pDefPlayer->health;
		trueComboData[pAttPlayer->ownerIndex][curComboData].defender = pDefPlayer;
	}
}

DWORD GetInitialHealthForTrueComboDamage_PatchAddr = 0x0047155f;
__declspec(naked) void _naked_GetInitialHealthForTrueComboDamage() {
	__asm {
		mov esi, dword ptr[esp + 0xe0];
		mov ebx, 0x1;
		push ebx;
		push esi;
		push ebp;
		mov pDefPlayer, ebp;
		mov pAttPlayer, edi;
	}

	PUSH_ALL;
	GetInitialHealthForTrueComboDamage();
	POP_ALL;

	__asm {
		mov eax, edi;
		push 0x00471570;
		ret;
	}
}

void SetTrueComboDamage() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			PlayerAuxData* playerData = &playerAuxDataArr[i];
			if (playerData->comboCalcData[j].someFlag == -1) {
				trueComboData[i][j].damage = 0;
				trueComboData[i][j].defender = nullptr;
			}
			else if (playerData->comboCalcData[j].someFlag <= 100 &&
				trueComboData[i][j].defender != nullptr &&
				trueComboData[i][j].defender->notInCombo == false) {
				//playerData->comboCalcData[j].damage = trueComboData[i][j].startingHealth - trueComboData[i][j].defender->health;
				trueComboData[i][j].damage = trueComboData[i][j].startingHealth - trueComboData[i][j].defender->health;
			}
		}
	}
}

//DWORD DrawTrueComboDamage_PatchAddr = 0x0047698a;
DWORD SetTrueComboDamage_PatchAddr = 0x00476a81;
__declspec(naked) void _naked_SetTrueComboDamage() {
	PUSH_ALL;
	SetTrueComboDamage();
	POP_ALL;

	__asm {
		pop edi;
		pop esi;
		pop ebp;
		pop ebx;
		add esp, 0xc;
		ret;
	}
}

byte randomTechDelay = 0xff;
DWORD MBAA_IsInGroundTechWindow = 0x00463760;
DWORD DummyDelayTech_PatchAddr = 0x00451346;
__declspec(naked) void _naked_DummyDelayTech() {
	ActorData* cso;
	__asm {
		mov cso, esi;
	}
	if (randomTechDelay == 0xff) randomTechDelay = rand() % 3;
	if ((dummyTechDelay == 3 && cso->untechTimeElapsed - 2 < randomTechDelay) ||
		(dummyTechDelay < 3 && cso->untechTimeElapsed - 2 < dummyTechDelay))
	{
		__asm {
			push 0x0045137b; //skip ground tech
			ret;
		}
	}
	randomTechDelay = 0xff;
	__asm {
		mov eax, 0x00557d20;
		mov eax, [eax];
		cmp eax, 0x4;
		mov byte ptr[esi + 0x17b], bl;
		push 0x00451354;
		ret;
	}
}

void DummyTechGuardFix() {
	if (pDummy->subObj.doTrainingAction != 0 && pDummy->subObj.onHitComboCount != 0) {
		pDummy->subObj.willBlock = 0;
	}
}

void DummyFirstHitGuard() {
	if (pDummy->subObj.doTrainingAction != 0 && dummyGuardFirstHitOnly != 0) {
		static int numGuards = 0;
		static int framesOutOfGuard = 0;
		static bool inGuardLastFrame = false;
		if (pDummy->subObj.onBlockComboCount == 0) {
			framesOutOfGuard += 1;
		}
		else {
			framesOutOfGuard = 0;
		}

		if (pDummy->subObj.animationDataPtr->stateData->canMove && numGuards < dummyGuardFirstHitNumGaps) {
			pDummy->subObj.willBlock = 1;
		}
		else {
			if (rand() % 100 < dummyGuardFirstHitDropChance) {
				pDummy->subObj.willBlock = 0;
			}
			else {
				pDummy->subObj.willBlock = 1;
			}
		}

		if (framesOutOfGuard > 30) {
			numGuards = 0;
			framesOutOfGuard = 31;
		}
		else if (inGuardLastFrame && framesOutOfGuard != 0) {
			numGuards += 1;
		}

		inGuardLastFrame = framesOutOfGuard == 0;
	}
}

void DummyCrossUpNoGuard() {
	if (pDummy->subObj.doTrainingAction != 0 && dummyGuardUntilCrossUp != 0) {
		static int crossUpTimer = 0;
		bool dummyIsOppLeft = pDummy->subObj.isOpponentToLeft;
		static bool lastDummyIsOppLeft = false;
		if (dummyIsOppLeft != lastDummyIsOppLeft) {
			crossUpTimer = 1;
		}
		if (crossUpTimer != 0) {
			pDummy->subObj.willBlock = 0;
			crossUpTimer += 1;
			if (crossUpTimer > 30) {
				crossUpTimer = 0;
			}
		}
		lastDummyIsOppLeft = pDummy->subObj.isOpponentToLeft;
	}
}

void LoadSave() {
	if (doLoad && XS_saveStateSlot > 0 && saveStateManager.FullSaves[XS_saveStateSlot - 1]->IsSaved)
	{
		//save recording status
		byte p1DoTraining = pP1->subObj.doTrainingAction;
		byte p2DoTraining = pP2->subObj.doTrainingAction;
		byte p3DoTraining = pP3->subObj.doTrainingAction;
		byte p4DoTraining = pP4->subObj.doTrainingAction;

		CommandFileData* cmdPtrs[4] = { pP1->cmdFileDataPtr, pP2->cmdFileDataPtr, pP3->cmdFileDataPtr, pP4->cmdFileDataPtr };
		saveStateManager.FullSaves[XS_saveStateSlot - 1]->load(XS_loadRNG);
		PlayerData* curPlayer;
		for (int i = 0; i < 4; i++) {
			curPlayer = &playerDataArr[i];
			if (curPlayer->exists)
			{
				UpdateCharPointers(&(curPlayer->subObj));
				curPlayer->cmdFileDataPtr = cmdPtrs[i];
				for (int j = 0; j < 8; j++) {
					if (curPlayer->subObj.attackingSubObjPtrArr[j] != 0) {
						ActorData* attackingSubObj = curPlayer->subObj.attackingSubObjPtrArr[j];
						curPlayer->subObj.receivingAttackDataPtrArr[j] = attackingSubObj->attackDataPtr;
					}
				}
			}

		}

		EffectData* curEffect;
		for (int i = 0; i < 1000; i++) {
			curEffect = &effectDataArr[i];
			if (curEffect->exists)
			{
				UpdateCharPointers(&(curEffect->subObj));
			}

		}

		//load recording status
		pP1->subObj.doTrainingAction = p1DoTraining;
		pP2->subObj.doTrainingAction = p2DoTraining;
		pP3->subObj.doTrainingAction = p3DoTraining;
		pP4->subObj.doTrainingAction = p4DoTraining;

		doLoad = false;
	}
}

//called once per game frame after all other gameplay processing
void EndUpdateBattleScene() {
	LoadSave();
	if (*(byte*)(adMBAABase + 0x0015d203) == 0) { //is not paused
		DummyFirstHitGuard();
		DummyCrossUpNoGuard();
		DummyTechGuardFix();
	}
}

DWORD EndUpdateBattleScene_PatchAddr = 0x004540b8;
__declspec(naked) void _naked_EndUpdateBattleScene() {
	PUSH_ALL;
	EndUpdateBattleScene();
	POP_ALL;

	__asm {
		mov eax, 0x1;
		pop esi;
		ret;
	}
}

DWORD didHitboxConnect = 0;
__declspec(naked) void _naked_HitboxOnConnect() {
	// patched at 0x0046f8d0

	__asm {
		mov didHitboxConnect, 1;
	}

	// overwritten asm 
	emitByte(0x80);
	emitByte(0x83);
	emitByte(0x76);
	emitByte(0x01);
	emitByte(0x00);
	emitByte(0x00);
	emitByte(0xFF);

	emitJump(0x0046f8d7);

}

DWORD MBAA_GetHoveredReplayPath = 0x0043d340;
DWORD CustomLoadReplay_PatchAddr = 0x00437d66;
__declspec(naked) void _naked_CustomLoadReplay() {
	__asm {
		cmp customLoadReplay, 1;
		JE _CUSTOM;
		call[MBAA_GetHoveredReplayPath];
		jmp _END;

	_CUSTOM:
		mov eax, customLoadReplayPathPtr;
		mov customLoadReplay, 0;
	
	_END:
		push 0x00437d6b;
		ret;
	}
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


	// the following is for ... seeing if i can call caster funcs from here

	HMODULE casterHook = GetModuleHandleA("hook.dll");
	//log("casterhook was %08X", (DWORD)casterHook);
	setDesiredFPS = (setDesiredFPSType)GetProcAddress(casterHook, "setDesiredFPS");
	//log("setDesiredFPS was %08X", (DWORD)setDesiredFPS);

	// ok to be honest, wanting to have draws in both caster and training mode is greed beyond human comprehension
	// but having 2v2, etm, and shock collar be able to share code (mostly the debug info here) and all use the same draw code instead of overwriting each other 
	// would be soso nice
	// but also having everything in one project would be,, difficult
	// i wanted to make the next shock collar revision in caster, but maybe i should keep it as a mod.
	// idk
	// msvc also has some things, mainly the use of __try and nicer asm syntax.. idk
	// ig for now, ill just keep things as is.
	// putting shock collar and 2v2 into melty just,, thats for another day



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
	patchJump(0x0041b58a, _naked_DisableShadows2);
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

	patchJump(0x0042373d, _naked_inputCallbackMidGameUpdate);

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

	patchJump(HandleInformationWindow_PatchAddr, _naked_HandleInformationMenu);
	patchJump(InformationWindowSetTargetWindow_PatchAddr, _naked_InformationWindowSetTargetWindow);
}

void initCustomHealthRecover() {
	//BYTE tempCode[6] = { 0x8b, 0x15, 0x00, 0x13, 0x78, 0x00 }; //put address of desired custom health amount as last 4 bytes here
	//patchMemcpy(0x004242fa, tempCode, 6); //currently using adShareBase + 0x300 = 0x00781300
	patchJump(CustomHealthRegen_PatchAddr, _naked_CustomHealthRegen);
}

void initShowCssHook() {
	patchJump(0x0048bb80, _naked_showCssHook);
}

void initResetCallback() {
	patchJump(ResetCallback_PatchAddr, _naked_ResetCallback);
}

void initRoundcallCallback() {
	patchJump(RoundcallCallback_PatchAddr, _naked_RoundcallCallback);
}

void initCharInputCallback() {
	patchJump(CharInputCallback_PatchAddr, _naked_CharInputCallback);
}

void initCSSCallback() {
	patchJump(CSSCallback_PatchAddr, _naked_CSSCallback);
}

void initForceDummyGuard() {
	patchJump(ForceDummyGuard_PatchAddr, _naked_ForceDummyGuard);
}

void initDisabledExit() {
	patchMemset(0x0047d21c, 0x90, 11);
}

void initTrueComboDamage() {
	//patchJump(GetInitialHealthForTrueComboDamage_PatchAddr, _naked_GetInitialHealthForTrueComboDamage);
	patchJump(SetTrueComboDamage_PatchAddr, _naked_SetTrueComboDamage);
}

void initDummyDelayTech() {
	patchJump(DummyDelayTech_PatchAddr, _naked_DummyDelayTech);
}

void init2v2Hack() {
	patchJump(0x0040e3ab, _naked_init2v2Hack);
}

void initEndUpdateBattleScene() {
	patchJump(EndUpdateBattleScene_PatchAddr, _naked_EndUpdateBattleScene);
}

void initHitboxOnConnect() {
	patchJump(0x0046f8d0, _naked_HitboxOnConnect);
}

void initCustomLoadReplay() {
	patchJump(CustomLoadReplay_PatchAddr, _naked_CustomLoadReplay);
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


	// detect if we are running through wine using the registry
	LPCTSTR subKey = L"Software\\Wine";
	HKEY res = nullptr;
	isWine = RegOpenKeyEx(HKEY_CURRENT_USER,subKey,0,KEY_READ,&res);
	RegCloseKey(res);

	initPauseCallback();
	initFrameDoneCallback();
	initHighlightHook();
	InitializeCharacterMaps();
	// when running with caster, the prints to this area are disabled
	// when not running with caster, they arent even there, so this is fine to run regardless of caster 
	//initAttackMeterDisplay();
	initMeterGainHook();
	//initNewAttackDisplay();

	initNewPauseCallback();
	initDrawBackground();

	initDualInputDisplay();

	initInputCallback();

	initCustomHealthRecover();

	initShowCssHook();

	initTrainingMenu();

	initResetCallback();

	initRoundcallCallback();

	initCharInputCallback();

	initCSSCallback();

	initForceDummyGuard();

	initDisabledExit();

	initTrueComboDamage();

	initDummyDelayTech();

	initRegistryValues();
	init2v2Hack();

	initEndUpdateBattleScene();

	initHitboxOnConnect();

	initCustomLoadReplay();

	initPaletteLoadPatches();

	ReadFromRegistry(L"ShowDebugMenu", &showDebugMenu);

	ReadFromRegistry(L"showReplayMenuf", &showReplayMenu);

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
