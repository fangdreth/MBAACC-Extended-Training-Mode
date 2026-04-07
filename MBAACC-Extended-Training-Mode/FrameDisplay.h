#pragma once

#include <string>
#include "Logger.h"
#include "..\Common\Common.h"

extern char cGameState; // 1:In-Game 2:Title 3:Logos 8:Loading 9:Arcade Cutscene 10:Next Stage 12:Options 20:CSS 25:Main Menu
extern char cP1Freeze; //Used for EXFlashes where initiator still moves (ex. Satsuki 214C winds up during flash)
extern char cP2Freeze;
extern char cP3Freeze;
extern char cP4Freeze;
extern int nFrameCount; //Counts slower during slowdown
extern int nLastFrameCount;
extern int nTrueFrameCount; //Counts all frames during slowdown
extern int nLastTrueFrameCount;
extern char cGlobalFreeze; //Used for EXFlashes where neither char moves
extern char cP1FN2Input;
extern char cDummyState; // Same as Common.h "Enemy Status" except -1 for recording
extern int nTimer;

extern int nBarCounter;
extern short nBarScrolling;
extern short nLastBarScrolling;
extern int nBarIntervalCounter;
extern int nBarIntervalMax;
extern int nBarDisplayRange;
extern int nLastBarDisplayRange;
extern bool bIsBarReset;
extern bool bDoBarReset;
extern bool bUpdateBar;
extern bool bDoAdvantage;

extern bool bLockInput;

extern bool bEnableFN2Load;
extern bool bInExtendedSettings;

extern bool bAdvancedFrameData;
extern bool bSimpleFrameInfo;
extern bool bDisplayFreeze; //Whether to show global ex flashes and frames where both chars are in hitstop
extern bool bDisplayInputs;
extern bool bDisplayCancels;
extern bool bPrintColorGuide;
extern char cDisplayOptions;
extern char cLastDisplayOptions;

extern std::string sColumnHeader;

extern int nPlayerAdvantage;
extern int nSharedHitstop;

const int outBufferSize = 16384 * 2;
extern int outBufferIndex;
extern char outBuffer[outBufferSize];
void writeBuffer(const char* fmt, ...);

void displayBuffer();

void CheckGameState(HANDLE hMBAAHandle);

void UpdateGlobals(HANDLE hMBAAHandle);

struct FrameDisplayPlayerData
{
	char cExists = 0; //0x0
	char padding0[0xF];
	int nPattern = 0; //0x10
	int nState = 0; //0x14
	char padding1[0xA4];
	int nHealth = 0; //0xBC
	int nRedHealth = 0; //0xC0
	float fGuardGauge = 0.0f; //0xC4
	char padding2[0x10];
	float fGuardQuality = 0.0f; //0xD8
	char padding3[0x4];
	int nMagicCircuit = 0; //0xE0
	char padding4[0xC];
	int nFrameCount = 0; //0xF0
	char padding5[0x14];
	int nXPosition = 0; //0x108
	int nYPosition = 0; //0x10C
	char padding6[0xC];
	int nXSpeed = 0; //0x11C
	int nYSpeed = 0; //0x120
	short sXAcceleration = 0; //0x124
	short sYAcceleration = 0; //0x126
	char padding7[0x10];
	int nMomentum = 0; //0x138
	char padding8[0x36];
	char cHitstop = 0; //0x172
	char padding9[0x3];
	bool bThrowFlag = false; //0x176
	char padding10[0x1];
	bool bTagFlag = false; //0x178
	char padding11[0x2];
	bool bBlockstunFlag = false; //0x17B
	char padding12[0x9];
	char sStrikeInvuln = 0; //0x185
	char sThrowInvuln = 0; //0x186
	char padding13[0x7];
	short sUntechTotal = 0; //0x18E
	short sUntechCounter = 0; //0x190
	char padding14[0x1A];
	int nHitstunRemaining = 0; //0x1AC
	char padding15[0x4A];
	char cCounterHit = 0; //1FA
	char padding16[0xE9];
	float fGravity = 0.0f; //2E4
	short sUntechPenalty = 0; //0x2E8
	char cAirDirectionalInput = 0; //0x2EA
	char cRawDirectionalInput = 0; //0x2EB
	char padding17[0x1];
	char cButtonInput = 0; //0x2ED
	char cMacroInput = 0; //0x2EE
	char padding18[0x26];
	bool bIsOnRight = false; //0x315
	char padding19[0xA];
	DWORD dwAnimationDataPointer = 0x0; //0x320
	DWORD dwAttackDataPointer = 0x0; //0x324

	DWORD dwAnimation_StateDataPointer = 0x0; //0x320 : 0x38
	char cState_Stance = 0; //0x320 : 0x38 : 0xC
	char cState_Invuln = 0; //0x320 : 0x38 : 0xD
	char cState_NormalCancel = 0; //0x320 : 0x38 : 0xE
	char cState_SpecialCancel = 0; //0x320 : 0x38 : 0xF
	int nState_Flagset2 = 0; //0x320 : 0x38 : 0x18

	char cAnimation_ConditionCount = 0; //0x320 : 0x40
	char cAnimation_BoxIndex = 0; //0x320 : 0x42
	DWORD dwAnimation_ConditionsPointer = 0x0; //0x320 : 0x44
	DWORD dwConditions_Condition1Pointer = 0x0; //0x320 : 0x44 : 0x0
	DWORD dwConditions_Condition2Pointer = 0x0; //0x320 : 0x44 : 0x4
	DWORD dwCondition1_ConditionType = 0; //0x320 : 0x44 : 0x0 : 0x0
	DWORD dwCondition2_ConditionType = 0; //0x320 : 0x44 : 0x4 : 0x0
	DWORD dwCondition1_ConditionParam1 = 0; //0x320 : 0x44 : 0x0 : 0x4
	DWORD dwCondition2_ConditionParam1 = 0; //0x320 : 0x44 : 0x4 : 0x4

	char cPlayerNumber = 0;
	DWORD adPlayerBase = 0x0;
	DWORD adInactionable = 0x0;

	int nInactionableFrames = 0;
	int nLastInactionableFrames = 0;

	std::string sBar1[BAR_MEMORY_SIZE][2];
	std::string sBar2[BAR_MEMORY_SIZE][2];
	std::string sBar3[BAR_MEMORY_SIZE][2];
	std::string sBar4[BAR_MEMORY_SIZE][4];
	std::string sBar5[BAR_MEMORY_SIZE][4];
	std::string sBarString1;
	std::string sBarString2;
	std::string sBarString3;
	std::string sBarString4;
	std::string sBarString5;

	int nActiveCounter = 0;
	int nInactionableMemory = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
	bool bLastOnRight = false;
	char cLastStance = 0;
	bool bBoxInvuln = false;

	FrameDisplayPlayerData(char _cPlayerNumber, DWORD _adPlayerBase, DWORD _adInactionable) {
		cPlayerNumber = _cPlayerNumber;
		adPlayerBase = _adPlayerBase;
		adInactionable = _adInactionable;
	}

};

extern FrameDisplayPlayerData FD_P1;
extern FrameDisplayPlayerData FD_P2;
extern FrameDisplayPlayerData FD_P3;
extern FrameDisplayPlayerData FD_P4;

extern FrameDisplayPlayerData* FD_PlayerArray[4];

extern FrameDisplayPlayerData* FD_Main1;
extern FrameDisplayPlayerData* FD_Main2;
extern FrameDisplayPlayerData* FD_Assist1;
extern FrameDisplayPlayerData* FD_Assist2;

void CheckProjectiles(HANDLE hMBAAHandle);

void UpdatePlayers(HANDLE hMBAAHandle);

void PrintColorGuide();

void CalculateAdvantage(FrameDisplayPlayerData& P1, FrameDisplayPlayerData& P2);

void ResetBars(HANDLE hMBAAHandle);

void UpdateBars(FrameDisplayPlayerData& P, FrameDisplayPlayerData& Assist);

void IncrementActive(FrameDisplayPlayerData& P);

void HandleInactive(FrameDisplayPlayerData& P);

void BarHandling(HANDLE hMBAAHandle, FrameDisplayPlayerData& P1, FrameDisplayPlayerData& P2, FrameDisplayPlayerData& P1Assist, FrameDisplayPlayerData& P2Assist);

void PrintFrameDisplay(HANDLE hMBAAHandle, FrameDisplayPlayerData& P1, FrameDisplayPlayerData& P2, FrameDisplayPlayerData& P3, FrameDisplayPlayerData& P4);

void FrameDisplay(HANDLE hMBAAHandle);

// Foreground color -> \x1b[38;2;R;G;Bm
// Background color -> \x1b[48;2;R;G;Bm
const std::string FD_CLEAR = "\x1b[0m";
const std::string FD_ITALICS = "\x1b[3m";
const std::string FD_UNDERLINE = "\x1b[4m";
const std::string FD_INACTIONABLE = "\x1b[38;2;255;255;255m\x1b[48;2;65;200;0m";
const std::string FD_JUMP = "\x1b[38;2;177;177;255m\x1b[48;2;241;224;132m";
const std::string FD_HITSTUN = "\x1b[38;2;255;255;255m\x1b[48;2;140;140;140m";
const std::string FD_BLOCKSTUN = "\x1b[38;2;255;255;255m\x1b[48;2;180;180;180m";
const std::string FD_ACTIONABLE = "\x1b[38;2;92;92;92m\x1b[48;2;0;0;0m";
const std::string FD_ADVANTAGE = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m";
const std::string FD_NEUTRAL = "\x1b[38;2;255;255;255m\x1b[48;2;32;90;0m";
const std::string FD_THROW_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;192;0;128m";
const std::string FD_THROWN = "\x1b[38;2;255;255;255m\x1b[48;2;110;110;110m";
const std::string FD_CLASH = "\x1b[38;2;255;255;255m\x1b[48;2;225;184;0m";
const std::string FD_SHIELD = "\x1b[38;2;255;255;255m\x1b[48;2;145;194;255m";
const std::string FD_INACTIONABLE_INVULN = "\x1b[38;2;160;160;160m\x1b[48;2;255;255;255m";
const std::string FD_ACTIONABLE_INVULN = "\x1b[38;2;100;100;100m\x1b[48;2;255;255;255m";
const std::string FD_FREEZE = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
const std::string FD_HITSTOP = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
const std::string FD_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
const std::string FD_ASSIST_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;255;128;0m";
const std::string FD_BUTTON_PRESSED = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
const std::string FD_A_PRESSED = "\x1b[38;2;255;143;169m\x1b[48;2;170;27;58m";
const std::string FD_B_PRESSED = "\x1b[38;2;255;255;137m\x1b[48;2;169;91;7m";
const std::string FD_C_PRESSED = "\x1b[38;2;143;255;195m\x1b[48;2;18;132;62m";
const std::string FD_D_PRESSED = "\x1b[38;2;137;255;255m\x1b[48;2;21;66;161m";

const std::string FD_SPECIAL_GUIDE_BG = "\x1b[48;2;65;200;0m";
const std::string SPECIAL_CANCEL_FONTS[4] = { "\x1b[38;2;255;255;255m", "\x1b[38;2;128;80;128m", "\x1b[38;2;0;0;0m", "\x1b[38;2;128;80;128m" };
const std::string FD_NORMAL_GUIDE_FG = "\x1b[38;2;255;255;255m";
const std::string NORMAL_CANCEL_FONTS[4] = { "\x1b[48;2;65;200;0m", "\x1b[48;2;65;150;30m", "\x1b[48;2;65;80;65m", "\x1b[48;2;65;150;30m" };