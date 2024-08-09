#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <map>
#include <iostream>

enum eMenu { MAIN = 2, BATTLE_SETTINGS = 6, ENEMY_SETTINGS = 7, VIEW_SCREEN = 12, COMMAND_LIST = 13 };
enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eMagicCircuit { UNLIMITED = 3 };
enum eRecover {};
enum eEnemyOffOnRandom { OFF, ON, RANDOM };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, BLOCKSTRING, HEATOS, FUZZYMASH, FUZZYJUMP, CUSTOM };
enum eEnemyGuardLevelSettings { INF, ONEHUNDRED, SEVENTYFIVE, FIFTY, TWENTYFIVE, ZERO };
enum ePages { REVERSALS_PAGE = 1, STATS_PAGE = 2, POSITIONS_PAGE = 3, FRAME_TOOL = 4, CHARACTER_SPECIFICS = 5, HIGHLIGHT_PAGE = 6 };
enum eReversalType { REVERSAL_NORMAL, REVERSAL_RANDOM, /*REVERSAL_SEQUENCE,*/ REVERSAL_REPEAT };
enum eFrameDataDisplay { FRAMEDISPLAY_NORMAL, FRAMEDISPLAY_ADVANCED };
enum eHighlightSettings { NO_HIGHLIGHT, RED_HIGHLIGHT, GREEN_HIGHLIGHT, BLUE_HIGHLIGHT };

const std::string GITHUB_LATEST = "https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/latest";
const std::string GITHUB_RELEASE = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases";
const std::string GITHUB_README = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/README.md";
const std::string VERSION = "v2.0";

const DWORD dwP2Offset = 0xAFC;
const DWORD dwRoundTime = 0x162A40; //0-inf
const DWORD dwMot = 0x1581CC;   // this one is mysterious.  I think it's an animation counter
const DWORD dwPlayerState = 0x155140;  //0:STAND 13:CROUCH 17:STANDGUARDING 12:STAND->CROUCH ETC
const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE
const DWORD dwEnemyDefenseCorrected = 0x155DA8; // set this after setting dwEnemyDefense
const DWORD dwEnemyDefenseType = 0x37C1F4; //0:OFF 1:COMBO 2:RANDOM
const DWORD dwP1GuardAmount = 0x1551F4; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwGuardSetting = 0x37C200; //0:100 1:75 2:50 3:25 4:0
const DWORD dwAirRecovery = 0x37C208;
const DWORD dwDownRecovery = 0x37C20C;
const DWORD dwThrowRecovery = 0x37C214;
const DWORD dwReduceRecovery = 0x37C218;
const DWORD dwExGuard = 0x1551E0; //10:ExGuard
const DWORD dwP1Meter = 0x155210;   // abc.xy truncated Ex. 12345 = 123.4%
const DWORD dwP1Health = 0x1551EC;
const DWORD dwP1RedHealth = 0x1551F0;
const DWORD dwP1SionBullets = 0x1552F6; // counts shots used i.e. 0=13 in mag, 4=9 in mag, etc
const DWORD dwP1RoaVisibleCharge = 0x155302;    // 0-9
const DWORD dwP1RoaHiddenCharge = 0x155300; // 0-9
const DWORD dwP2Y = 0x155248;
const DWORD dwP1X = 0x155238;
const DWORD dwP1Exists = 0x155130;
const DWORD dwP2PatternSet = 0x155F38;
const DWORD dwP1PatternRead = 0x155140;
const DWORD dwP2PatternRead = dwP1PatternRead + 0xAFC;
const DWORD dwCSSFlag = 0x155130;
const DWORD dwP2CharNumber = 0x34D91C;
const DWORD dwP2CharMoon = 0x34D924;
const DWORD dwP1CharMoon = 0x34D900;
const DWORD dwP1CharNumber = 0x34D8F8;
const DWORD dwP1ButtonPress = 0x154654;
const DWORD dwP1APressed = 0x371399;
const DWORD dwP1F1Pressed = 0x37144C;
const DWORD dwP1F2Pressed = 0x37144D;
const DWORD dwP1DPressed = 0x37139C;
const DWORD dwP1DirectionPress = 0x371448;
const DWORD dwPausedFlag = 0x162A64;
const DWORD dwEnemyAction = 0x37C1EC;
const DWORD dwLifeRecover = 0x37C1F8;
const DWORD dwP1HitstunRemaining = 0x1552DC;
const DWORD dwP1NotInCombo = 0x155194; //1:InCombo 0:NotInCombo
const DWORD dwP1HitstopRemaining = 0x1552D4; //0:not in hitstop
const DWORD dwComboCount = 0x157E00; // there's another address that tracks this
const DWORD dwP2E = 0x3713C9;
const DWORD dwMagicCircuitSetting = 0x37C1FC;
const DWORD dwBasePointer = 0x34D7FC;
const DWORD dwBurstCooldown = 0x155DBE;
const DWORD dwP2RecievedHitstop = 0x155DD0;
const DWORD dwGameMode = 0x162A74; // 16:training
const DWORD dwFPS = 0x374A70;
const DWORD dwP1Color = 0x34D83C;
const DWORD dwP2Color = 0x34D868;
const DWORD dwP1Struct = 0x155130;
const DWORD dwP2Struct = dwP1Struct + 0xAFC;
const DWORD dwP1RyougiKnife = 0x1552E8; //0=knife 1=no knife
const DWORD dwP1FMaidsHearts = 0x1552FC; //0=full 5=empty
const DWORD dwP1Blocking = 0x1552AB;
const DWORD dwP2Blocking = dwP1Blocking + 0xAFC;
const DWORD dwGlobalEXFlash = 0x162A48;
const DWORD dwFrameTimer = 0x15D1CC;
const DWORD dwGameState = 0x14EEE8; //1=training

// SharedMemory
enum eSharedOffsets {	SHARE_IDLEHIGHLIGHT, SHARE_BLOCKINGHIGHLIGHT, SHARE_TEMP1HIGHLIGHT, SHARE_TEMP2HIGHLIGHT, SHARE_TEMP3HIGHLIGHT, SHARE_TEMP4HIGHLIGHT,
						SHARE_REVERSALINDEX1, SHARE_REVERSALINDEX2, SHARE_REVERSALINDEX3, SHARE_REVERSALINDEX4, SHARE_REVERSALDELAYFRAMES };
const LPCWSTR sSharedName = L"MBAACCExtendedTrainingMode";
const int nSharedSize = 11 * sizeof(int);

// DLL Constants
const DWORD dwCameraX = 0x0055dec4;
const DWORD dwCameraY = 0x0055dec8;
const DWORD dwCameraZoom = 0x0054eb70;
const DWORD dwWindowWidth = 0x0054d048;
const DWORD dwWindowHeight = 0x0054d04c;
const DWORD dwP1AnimationPtr = dwP1Struct + 0x320;
const DWORD dwP2AnimationPtr = dwP2Struct + 0x320;

//FrameDisplay Constants
typedef DWORD ADDRESS; //I think doing this + adXxxxYyyy looks nice

const int BAR_MEMORY_SIZE = 400; //Number of frames stored before overriding (FIFO). For use when scrolling is implemented
const int BAR_INTERVAL = 40; //Number of frames of no new bars before the bar resets on the next bar
const int MAX_SAVES = 3;

const char REVERSE_INPUT_MAP[10] = { 0, 3, 2, 1, 6, 0, 4, 9, 8, 7 };
const char CH_MAP[3] = { ' ', 'H', 'L' };

const DWORD dwPlayerStructSize = 0xAFC;
const ADDRESS adP1Base = 0x155130; //0x155130
const ADDRESS adP2Base = 0x155130 + dwPlayerStructSize; //0x155C2C
const ADDRESS adP3Base = 0x155130 + 2 * dwPlayerStructSize; //0x156728
const ADDRESS adP4Base = 0x155130 + 3 * dwPlayerStructSize; //0x157224

const ADDRESS adP1ControlledCharacter = 0x157DB8;
const ADDRESS adP2ControlledCharacter = 0x157FC4;
const ADDRESS adP1TagFlag = 0x155130 + 0x178; //0x1552A8
const ADDRESS adP2TagFlag = 0x155130 + 0x178 + dwPlayerStructSize; //0x155DA4
const ADDRESS adP3TagFlag = 0x155130 + 0x178 + 2 * dwPlayerStructSize; //0x1568A0
const ADDRESS adP4TagFlag = 0x155130 + 0x178 + 3 * dwPlayerStructSize; //0x15739C

const DWORD dwProjectileStructSize = 0x33C;
const ADDRESS adProjectileBase = 0x27BDE8;

// addresses for fonts
const ADDRESS adFont0 = 0x55D680;
const ADDRESS adFont1 = 0x55D260;
const ADDRESS adFont2 = 0x55DAA0;

// integer representations of raw float values
// not interested in messing with converting them when a table is good enough
const std::vector<int> vGuardLevelLookupTable =
{
    1174011904/*C 100*/, 1169915904/*C 75*/, 1165623296/*C 50*/, 1157234688/*C 25*/, 0/*C 0*/,
    1171963904/*F 100*/, 1168379904/*F 75*/, 1163575296/*F 50*/, 1155186688/*F 25*/, 0/*F 0*/,
    1176768512/*H 100*/, 1173755904/*H 75*/, 1168379904/*H 50*/, 1159991296/*H 25*/, 0/*H 0*/
};

const int MAX_REVERSAL_DELAY = 99;
const int MAX_HEALTH = 11400;
const int MAX_METER = 30000;
const int MAX_PAGES = 6;
const int MAX_BULLETS = 13; //14:normal 15:infinite
const int MAX_CHARGE = 9;
const int MAX_HEARTS = 5; //6:normal 7:infinite
const int MIN_X = -65536;
const int MAX_X = 65536;
const int P1_DEFAULT_X = -16384;
const int P2_DEFAULT_X = 16384;
const int P3_DEFAULT_X = -29184;
const int P4_DEFAULT_X = 29184;
const int OFFSCREEN_LOCATION = 0x10000000;
const int ONSCREEN_LOCATION = 0x000000A0;
const int TOO_HIGH_TO_BURST = 10000;
const int MAX_BURST = 99;

const char pcSionBullets_13[13] = "SION BULLETS";
const char pcRoaVisibleCharge_19[19] = "ROA VISIBLE CHARGE";
const char pcRoaHiddenCharge_19[19] = "ROA HIDDEN CHARGE";
const char pcFMaidsHearts_15[15] = "F-MAIDS HEARTS";
const char pcRyougiKnife_13[13] = "RYOUGI KNIFE";
const char pcP1ControlledChar_19[19] = "P1 CONTROLLED CHAR";
const char pcP2ControlledChar_19[19] = "P2 CONTROLLED CHAR";
const char pcHitsUntilBurst_17[17] = "HITS UNTIL BURST";
const char pcNormal_7[7] = "NORMAL";
const char pcInfinite_10[10] = "INFINITE";
const char pcOne_2[2] = "1";
const char pcMain_5[5] = "MAIN";
const char pcAssist_7[7] = "ASSIST";
const char pcHisui_6[6] = "HISUI";
const char pcKohaku_8[8] = "KOHAKU";

const char pcASlow_12[12] = "[A]->SLOWER";
const char pcP1XLoc_11[11] = "P1 X-LOC";
const char pcP2XLoc_11[11] = "P2 X-LOC";
const char pcAssistLoc_13[13] = "ASSIST X-LOC";
const char pcInvert_7[7] = "INVERT";
const char pcUnstable_11[11] = "(UNSTABLE)";

const char pcReversalSlot1_16[16] = "REVERSAL SLOT 1";
const char pcReversalSlot2_16[16] = "REVERSAL SLOT 2";
const char pcReversalSlot3_16[16] = "REVERSAL SLOT 3";
const char pcReversalSlot4_16[16] = "REVERSAL SLOT 4";
const char pcReversalType_14[14] = "REVERSAL TYPE";
const char pcExGuard_9[9] = "EX GUARD";
const char pcReversalDelay_15[15] = "REVERSAL DELAY";
const char pcMeter_18[18] = "METER [A]->SLOWER";
const char pcHealth_19[19] = "HEALTH [A]->SLOWER";
const char pcGuardBar_10[10] = "GUARD BAR";
const char pcBlank_1[1] = "";

const char pcEnemyReversal_15[15] = "ENEMY REVERSAL";

const char pcRecover_8[8] = "RECOVER";
const char pcRecover_11[11] = "NO RECOVER";

const char pcTrainingPreset_17[17] = "training preset.";
const char pcExtendedSettings_18[18] = "EXTENDED SETTINGS";

const char pcLatestVersion_19[19] = "LATEST VERSION";
const char pcOffline_8[8] = "OFFLINE";

const char pcFrameData_11[11] = "FRAME DATA";
const char pcSaveStateSlot_16[16] = "SAVE STATE SLOT";
const char pcSaveState_11[11] = "SAVE STATE";
//const char pcClearSaveStates_18[18] = "CLEAR SAVE STATES";
//const char pcNoSaveStates_15[15] = "NO SAVE STATES";
const char pcDisplayFreeze_16[16] = "DISPLAY FREEZE";
const char pcDisplayInputs_15[15] = "DISPLAY INPUTS";
const char pcScrollDisplay_15[15] = "SCROLL DISPLAY";
const char pcAdvanced_9[9] = "ADVANCED";
const char pcOff_4[4] = "OFF";
const char pcOn_3[3] = "ON";
const char pcNone_5[5] = "NONE";

const char pcBlocking_9[9] = "BLOCKING";
const char pcIdle_5[5] = "IDLE";
const char pcRed_4[4] = "RED";
const char pcGreen_6[6] = "GREEN";
const char pcBlue_5[5] = "BLUE";

void SetSharedMemory(	int* nIdleHighlightSetting,
						int* nBlockingHighlightSetting,
						int* nTemp1HighlightSetting,
						int* nTemp2HighlightSetting,
						int* nTemp3HighlightSetting,
						int* nTemp4HighlightSetting,
						int* nReversalIndex1,
						int* nReversalIndex2,
						int* nReversalIndex3,
						int* nReversalIndex4,
						int* nReversalDelayFrames,
						int* nReversalType)
{
	static bool bSharedMemoryInit = false;
    static HANDLE hMapFile = NULL;
    static char* pBuf = NULL;

    if (!bSharedMemoryInit) {
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSharedSize, sSharedName);
        if (hMapFile == NULL) {
            std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
            return;
        }

        pBuf = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, nSharedSize);
        if (pBuf == NULL) {
            std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
            CloseHandle(hMapFile);
            return;
        }
        bSharedMemoryInit = true;
    }

    //BYTE arrSendBuffer[nSharedSize];

    //*(DWORD*)&(arrSendBuffer[0 * sizeof(int)]) = nIdleHighlightSetting;
    //*(DWORD*)&(arrSendBuffer[1 * sizeof(int)]) = nBlockingHighlightSetting;

    //CopyMemory(pBuf, &arrSendBuffer, nSharedSize);

	if (nIdleHighlightSetting)
		CopyMemory(pBuf + 0 * sizeof(int), nIdleHighlightSetting, sizeof(int));

	if (nBlockingHighlightSetting)
		CopyMemory(pBuf + 1 * sizeof(int), nBlockingHighlightSetting, sizeof(int));

	if (nTemp1HighlightSetting)
		CopyMemory(pBuf + 2 * sizeof(int), nTemp1HighlightSetting, sizeof(int));

	if (nTemp2HighlightSetting)
		CopyMemory(pBuf + 3 * sizeof(int), nTemp2HighlightSetting, sizeof(int));

	if (nTemp3HighlightSetting)
		CopyMemory(pBuf + 4 * sizeof(int), nTemp3HighlightSetting, sizeof(int));

	if (nTemp4HighlightSetting)
		CopyMemory(pBuf + 5 * sizeof(int), nTemp4HighlightSetting, sizeof(int));

	if (nReversalIndex1)
		CopyMemory(pBuf + 6 * sizeof(int), nReversalIndex1, sizeof(int));

	if (nReversalIndex2)
		CopyMemory(pBuf + 7 * sizeof(int), nReversalIndex2, sizeof(int));

	if (nReversalIndex3)
		CopyMemory(pBuf + 8 * sizeof(int), nReversalIndex3, sizeof(int));

	if (nReversalIndex4)
		CopyMemory(pBuf + 9 * sizeof(int), nReversalIndex4, sizeof(int));

	if (nReversalDelayFrames)
		CopyMemory(pBuf + 10 * sizeof(int), nReversalDelayFrames, sizeof(int));

	if (nReversalType)
		CopyMemory(pBuf + 11 * sizeof(int), nReversalType, sizeof(int));
}

void GetSharedMemory(	std::array<BYTE, 3>* parrIdleHighlightSetting,
						std::array<BYTE, 3>* parrBlockingHighlightSetting, 
						std::array<BYTE, 3>* parrTemp1HighlightSetting, 
						std::array<BYTE, 3>* parrTemp2HighlightSetting, 
						std::array<BYTE, 3>* parrTemp3HighlightSetting, 
						std::array<BYTE, 3>* parrTemp4HighlightSetting, 
						int* pnReversalIndex1,
						int* pnReversalIndex2,
						int* pnReversalIndex3,
						int* pnReversalIndex4,
						int* pnReversalDelayFrames,
						int* pnReversalType)
{
	static bool bSharedMemoryInit = false;
	static HANDLE hMapFile = NULL;
	static LPVOID pBuf = NULL;

	if (!bSharedMemoryInit)
	{
		hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, sSharedName);
		if (hMapFile == NULL)
		{
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

	if (parrIdleHighlightSetting)
	{
		int nIdleHighlightSetting = ((int*)pBuf)[SHARE_IDLEHIGHLIGHT];
		switch (nIdleHighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrIdleHighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrIdleHighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrIdleHighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrIdleHighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrBlockingHighlightSetting)
	{
		int nBlockingHighlightSetting = ((int*)pBuf)[SHARE_BLOCKINGHIGHLIGHT];
		switch (nBlockingHighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp1HighlightSetting)
	{
		int nTemp1HighlightSetting = ((int*)pBuf)[SHARE_TEMP1HIGHLIGHT];
		switch (nTemp1HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp2HighlightSetting)
	{
		int nTemp2HighlightSetting = ((int*)pBuf)[SHARE_TEMP2HIGHLIGHT];
		switch (nTemp2HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp3HighlightSetting)
	{
		int nTemp3HighlightSetting = ((int*)pBuf)[SHARE_TEMP3HIGHLIGHT];
		switch (nTemp3HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp4HighlightSetting)
	{
		int nTemp4HighlightSetting = ((int*)pBuf)[SHARE_TEMP4HIGHLIGHT];
		switch (nTemp4HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (pnReversalIndex1)
		*pnReversalIndex1 = ((int*)pBuf)[SHARE_REVERSALINDEX1];

	if (pnReversalIndex2)
		*pnReversalIndex2 = ((int*)pBuf)[SHARE_REVERSALINDEX2];

	if (pnReversalIndex3)
		*pnReversalIndex3 = ((int*)pBuf)[SHARE_REVERSALINDEX3];

	if (pnReversalIndex4)
		*pnReversalIndex4 = ((int*)pBuf)[SHARE_REVERSALINDEX4];

	if (pnReversalDelayFrames)
		*pnReversalDelayFrames = ((int*)pBuf)[SHARE_REVERSALDELAYFRAMES];

	if (pnReversalType)
		*pnReversalType = ((int*)pBuf)[SHARE_REVERSALDELAYFRAMES];
}