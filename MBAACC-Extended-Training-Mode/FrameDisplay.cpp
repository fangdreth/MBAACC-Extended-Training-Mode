
#include "FrameDisplay.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <format>

char cGameState = 0; // 1:In-Game 2:Title 3:Logos 8:Loading 9:Arcade Cutscene 10:Next Stage 12:Options 20:CSS 25:Main Menu
char cP1Freeze = 0; //Used for EXFlashes where initiator still moves (ex. Satsuki 214C winds up during flash)
char cP2Freeze = 0;
char cP3Freeze = 0;
char cP4Freeze = 0;
int nFrameCount = 0; //Counts slower during slowdown
int nLastFrameCount = 0;
int nTrueFrameCount = 0; //Counts all frames during slowdown
int nLastTrueFrameCount = 0;
char cGlobalFreeze = 0; //Used for EXFlashes where neither char moves
char cP1FN2Input = 0;
char cDummyState = 0; // Same as Common.h "Enemy Status" except -1 for recording
int nTimer = 0;

int nBarCounter = 0;
short nBarScrolling = 0;
short nLastBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;
int nBarDisplayRange = 0;
int nLastBarDisplayRange = 0;
bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;

bool bLockInput = false;

static bool bEnableFN2Load = false;
static bool bInExtendedSettings = false;

bool bAdvancedFrameData = false;
bool bSimpleFrameInfo = true;
bool bDisplayFreeze = false; //Whether to show global ex flashes and frames where both chars are in hitstop
bool bDisplayInputs = false;
bool bDisplayCancels = false;
bool bPrintColorGuide = false;
char cDisplayOptions = 0;
char cLastDisplayOptions = 0;

std::string sColumnHeader = "";

int nPlayerAdvantage;
int nSharedHitstop;

int outBufferIndex = 0;
char outBuffer[outBufferSize];

void writeBuffer(const char* fmt, ...) {
	// this will add a written string to the buffer, which is to be printed at the end of each frame.
	// combining all prints into one is very helpful in this case

	if (outBufferIndex > outBufferSize) {
		//netlog("writeBuffer size was not large enough???");
		return;
	}

	if (fmt == NULL) {
		return;
	}

	va_list args;
	va_start(args, fmt);
	int res = vsnprintf(&outBuffer[outBufferIndex], outBufferSize - outBufferIndex, fmt, args);
	va_end(args);

	outBufferIndex += res;
}

void displayBuffer() {

	printf("%s", outBuffer);

	outBufferIndex = 0;
}

void CheckGameState(HANDLE hMBAAHandle)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adGameState), &cGameState, 1, 0);
}

void UpdateGlobals(HANDLE hMBAAHandle)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adP1Freeze), &cP1Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adP2Freeze), &cP2Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adP3Freeze), &cP3Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adP4Freeze), &cP4Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adFrameCount), &nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adTrueFrameCount), &nTrueFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adGlobalFreeze), &cGlobalFreeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adDummyState), &cDummyState, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adP1FN2Input), &cP1FN2Input, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adSharedTimer), &nTimer, 4, 0);
}

// stupid bs msvc compiler made this take wayYYYY to long to compile for no damn reason
//Player P1{ .cPlayerNumber = 0, .adPlayerBase = adMBAABase + adP1Base, .adInactionable = adMBAABase + adP1Inaction };
//Player P2{ .cPlayerNumber = 1, .adPlayerBase = adMBAABase + adP2Base, .adInactionable = adMBAABase + adP2Inaction };
//Player P3{ .cPlayerNumber = 2, .adPlayerBase = adMBAABase + adP3Base, .adInactionable = adMBAABase + adP1Inaction };
//Player P4{ .cPlayerNumber = 3, .adPlayerBase = adMBAABase + adP4Base, .adInactionable = adMBAABase + adP2Inaction };

Player P1(0, adMBAABase + adP1Base, adMBAABase + adP1Inaction);
Player P2(0, adMBAABase + adP2Base, adMBAABase + adP2Inaction);
Player P3(0, adMBAABase + adP3Base, adMBAABase + adP1Inaction);
Player P4(0, adMBAABase + adP4Base, adMBAABase + adP2Inaction);

Player* paPlayerArray[4] = { &P1, &P2, &P3, &P4 };

Player* Main1 = &P1;
Player* Main2 = &P2;
Player* Assist1 = &P3;
Player* Assist2 = &P4;

void CheckProjectiles(HANDLE hMBAAHandle)
{
	char cBlankEffectCount = 0;
	bool bProjectileExists = 0;
	uint8_t cProjectileStatus = 0;
	char cProjectileOwner = 0;
	int nProjectilePattern = 0;
	DWORD dwProjectileAttackDataPointer = 0;
	P1.nActiveProjectileCount = 0;
	P2.nActiveProjectileCount = 0;
	P3.nActiveProjectileCount = 0;
	P4.nActiveProjectileCount = 0;
	for (int i = 0; i < 200; i++)
	{
		if (cBlankEffectCount > 16) break;
		cBlankEffectCount++;
		ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adEffectBase + dwEffectStructSize * i), &bProjectileExists, 1, 0);
		if (!bProjectileExists) continue;
		cBlankEffectCount = 0;
		ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adEffectSubBase + dwEffectStructSize * i + adEffectStatus), &cProjectileStatus, 1, 0);
		if (cProjectileStatus != 0xFF) continue;
		ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adEffectSubBase + dwEffectStructSize * i + adEffectOwner), &cProjectileOwner, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adEffectSubBase + dwEffectStructSize * i + adAttackDataPointer), &dwProjectileAttackDataPointer, 4, 0);
		if (dwProjectileAttackDataPointer) (*paPlayerArray[cProjectileOwner]).nActiveProjectileCount++;
	}
}

void UpdatePlayers(HANDLE hMBAAHandle)
{
	for (int i = 0; i < 4; i++)
	{
		Player& P = *paPlayerArray[i];
		P.nLastInactionableFrames = P.nInactionableFrames;
		P.nLastFrameCount = P.nFrameCount;
		P.bLastOnRight = P.bIsOnRight;
		P.cLastStance = P.cState_Stance;
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.adPlayerBase), &P, 1, 0);
		if (!P.cExists) continue;
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.adPlayerBase), &P, 0x328, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_StateDataPointer), &P.dwAnimation_StateDataPointer, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_ConditionCount), &P.cAnimation_ConditionCount, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_BoxIndex), &P.cAnimation_BoxIndex, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_StateDataPointer + adStateData_Stance), &P.cState_Stance, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_StateDataPointer + adStateData_Invuln), &P.cState_Invuln, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_StateDataPointer + adStateData_NormalCancel), &P.cState_NormalCancel, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_StateDataPointer + adStateData_SpecialCancel), &P.cState_SpecialCancel, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_StateDataPointer + adStateData_Flagset2), &P.nState_Flagset2, 4, 0);
		if (P.cAnimation_ConditionCount > 0)
		{
			ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_ConditionsPointer), &P.dwAnimation_ConditionsPointer, 4, 0);
			ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_ConditionsPointer + adConditions_Condition1Pointer), &P.dwConditions_Condition1Pointer, 4, 0);
			ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwConditions_Condition1Pointer + adCondition_Type), &P.dwCondition1_ConditionType, 4, 0);
			ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwConditions_Condition1Pointer + adCondition_Param1), &P.dwCondition1_ConditionParam1, 4, 0);
			if (P.cAnimation_ConditionCount > 1)
			{
				ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimation_ConditionsPointer + adConditions_Condition2Pointer), &P.dwConditions_Condition2Pointer, 4, 0);
				ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwConditions_Condition2Pointer + adCondition_Type), &P.dwCondition2_ConditionType, 4, 0);
				ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwConditions_Condition2Pointer + adCondition_Param1), &P.dwCondition2_ConditionParam1, 4, 0);
			}
			else
			{
				P.dwCondition2_ConditionType = 0;
			}
		}
		else
		{
			P.dwCondition1_ConditionType = 0;
			P.dwCondition2_ConditionType = 0;
		}
		ReadProcessMemory(hMBAAHandle, (LPVOID)(P.adInactionable), &P.nInactionableFrames, 4, 0);
	}
}

void PrintColorGuide()
{

	std::cout << FD_INACTIONABLE << "00" << FD_CLEAR << " INACTIONABLE\t\t\t";
	std::cout << FD_JUMP << "00" << FD_CLEAR << " JUMP STARTUP\n";
	std::cout << FD_HITSTUN << "00" << FD_CLEAR << " HITSTUN\t\t\t";
	std::cout << FD_BLOCKSTUN << "00" << FD_CLEAR << " BLOCKSTUN\n";
	std::cout << FD_ACTIONABLE << "00" << FD_CLEAR << " FULLY ACTIONABLE\t\t";
	std::cout << FD_ADVANTAGE << "00" << FD_CLEAR << " FRAME ADVANTAGE\n";
	std::cout << FD_NEUTRAL << "00" << FD_CLEAR << " NEUTRAL FRAME\t\t";
	std::cout << FD_THROWN << " t" << FD_CLEAR << " BEING THROWN\n";
	std::cout << FD_CLASH << "00" << FD_CLEAR << " CLASH\t\t\t";
	std::cout << FD_SHIELD << "00" << FD_CLEAR << " SHIELD\n";
	std::cout << FD_INACTIONABLE_INVULN << "00" << FD_CLEAR << " INVULN\t\t\t";
	std::cout << FD_ACTIONABLE_INVULN << "00" << FD_CLEAR << " INVULN AND FULLY ACTIONABLE\n";
	std::cout << FD_FREEZE << "00" << FD_CLEAR << " SCREEN FREEZE\t\t";
	std::cout << FD_HITSTOP << "00" << FD_CLEAR << " HITSTOP\n";
	std::cout << FD_ACTIVE << "00" << FD_CLEAR << " ACTIVE FRAMES\t\t";
	std::cout << FD_ASSIST_ACTIVE << "00" << FD_CLEAR << " ASSIST ACTIVE FRAMES\n";
	std::cout << FD_THROW_ACTIVE << "00" << FD_CLEAR << " THROW ACTIVE FRAME\t\t";
	std::cout << FD_BUTTON_PRESSED << "00" << FD_CLEAR << " BUTTON PRESSED\n";
	std::cout << FD_A_PRESSED << "00" << FD_CLEAR << " A PRESSED\t\t\t";
	std::cout << FD_B_PRESSED << "00" << FD_CLEAR << " B PRESSED\n";
	std::cout << FD_C_PRESSED << "00" << FD_CLEAR << " C PRESSED\t\t\t";
	std::cout << FD_D_PRESSED << "00" << FD_CLEAR << " D PRESSED\n\n";

	std::cout << "WHEN \"SHOW CANCEL WINDOWS\" IS TURNED ON:\n";
	std::cout << NORMAL_CANCEL_FONTS[0] + FD_NORMAL_GUIDE_FG << "00" << FD_CLEAR << " NO NORMAL CANCEL\t\t";
	std::cout << NORMAL_CANCEL_FONTS[1] + FD_NORMAL_GUIDE_FG << "00" << FD_CLEAR << " NORMAL CANCEL ON (SUCCESSFUL) HIT\n";
	std::cout << NORMAL_CANCEL_FONTS[2] + FD_NORMAL_GUIDE_FG << "00" << FD_CLEAR << " NORMAL CANCEL ALWAYS\n";
	std::cout << SPECIAL_CANCEL_FONTS[0] + FD_SPECIAL_GUIDE_BG << "00" << FD_CLEAR << " NO SPECIAL CANCEL\t\t";
	std::cout << SPECIAL_CANCEL_FONTS[1] + FD_SPECIAL_GUIDE_BG << "00" << FD_CLEAR << " SPECIAL/REKKA CANCEL ON (SUCCESSFUL) HIT\n";
	std::cout << SPECIAL_CANCEL_FONTS[2] + FD_SPECIAL_GUIDE_BG << "00" << FD_CLEAR << " SPECIAL/REKKA CANCEL ALWAYS\t";
	std::cout << FD_INACTIONABLE + FD_UNDERLINE << "00" << FD_CLEAR << " EX CANCEL\n";
}

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (P1.nInactionableFrames == 0 && P2.nInactionableFrames == 0)
	{
		bDoAdvantage = false;
	}
	else if (P1.nInactionableFrames != 0 && P2.nInactionableFrames != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && nFrameCount != nLastFrameCount &&
		cP1Freeze == 0 &&
		cP2Freeze == 0 &&
		cGlobalFreeze == 0)
	{
		if (P1.nInactionableFrames == 0 && P2.nInactionableFrames != 0)
		{
			P1.nAdvantageCounter += nFrameCount - nLastFrameCount;
		}
		else if (P2.nInactionableFrames == 0 && P1.nInactionableFrames != 0)
		{
			P2.nAdvantageCounter += nFrameCount - nLastFrameCount;
		}
	}
}

void ResetBars(HANDLE hMBAAHandle)
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	nBarScrolling = 0;
	bDoBarReset = false;
	nBarIntervalMax = nBarDisplayRange;
	for (int i = 0; i < 4; i++)
	{
		Player& P = *paPlayerArray[i];
		for (int j = 0; j < BAR_MEMORY_SIZE; j++)
		{
			P.sBar1[j][1] = "";
			P.sBar1[j][2] = "";
			P.sBar2[j][1] = "";
			P.sBar2[j][2] = "";
			P.sBar3[j][1] = "";
			P.sBar3[j][2] = "";
			P.sBar4[j][1] = "";
			P.sBar4[j][2] = "";
			P.sBar4[j][3] = "";
			P.sBar4[j][4] = "";
			P.sBar5[j][1] = "";
			P.sBar5[j][2] = "";
			P.sBar5[j][3] = "";
			P.sBar5[j][4] = "";
		}
	}
	WriteProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_frameScroll), &nBarScrolling, 2, 0);
	std::cout << "\x1b[J";
}

void UpdateBars(Player& P, Player& Assist)
{
	std::string sFont = FD_CLEAR;
	std::string sBarValue = "  ";
	std::string sLeftFont = FD_CLEAR;
	std::string sRightFont = FD_CLEAR;
	std::string sRightValue = " ";
	bool bIsButtonPressed = P.cButtonInput != 0 || P.cMacroInput != 0;
	bool bHasInvuln = P.cAnimation_BoxIndex <= 1 || P.sStrikeInvuln != 0 || P.cState_Invuln == 3;

	//Bar 1 - General action information
	sBarValue = P.nInactionableFrames >= 100 ? std::format("{:02}", P.nInactionableFrames % 100) : std::format("{:2}", P.nInactionableFrames);
	if (P.nInactionableFrames != 0) //Doing something with limited actionability
	{
		if (P.nPattern >= 35 && P.nPattern <= 37) //Jump Startup
		{
			sFont = FD_JUMP;
		}
		else if (P.nHitstunRemaining != 0 && P.bBlockstunFlag == 0) //Hitstun
		{
			sFont = FD_HITSTUN;
			if (P.cState_Stance == 1) //Airborne
			{
				if (P.sUntechCounter < P.sUntechTotal) //Still has untech remaining
				{
					sBarValue = std::format("{:2}", (P.sUntechTotal - P.sUntechCounter) % 100);
				}
			}
			else //Grounded
			{
				if (P.nHitstunRemaining > 1) //Still has hitstun remaining
				{
					sBarValue = std::format("{:2}", (P.nHitstunRemaining - 1) % 100);
				}
			}
		}
		else if (P.bBlockstunFlag) //Blockstun
		{
			sFont = FD_BLOCKSTUN;
			if (P.cState_Stance != 1 && P.nHitstunRemaining > 1)
			{
				sBarValue = std::format("{:2}", (P.nHitstunRemaining - 1) % 100);
			}
		}
		else if (bHasInvuln)
		{
			sFont = FD_INACTIONABLE_INVULN;
		}
		else
		{
			sFont = FD_INACTIONABLE;
			if (bDisplayCancels)
			{
				char SpecialCancel = P.cState_SpecialCancel;
				if (P.dwCondition1_ConditionType == 35 || P.dwCondition2_ConditionType == 35)
				{
					if (P.dwCondition1_ConditionParam1 == 1 || P.dwCondition2_ConditionParam1 == 1)
					{
						SpecialCancel = 1;
					}
					else
					{
						SpecialCancel = 2;
					}
				}
				sFont = SPECIAL_CANCEL_FONTS[SpecialCancel] + NORMAL_CANCEL_FONTS[P.cState_NormalCancel];
			}
		}
	}
	else //Fully actionable
	{
		sFont = FD_ACTIONABLE;
		sBarValue = std::format("{:2}", P.nPattern % 100);
		if (bDoAdvantage) //Has advantage
		{
			sBarValue = std::format("{:2}", P.nAdvantageCounter % 100);
			sFont = FD_ADVANTAGE;
		}
		if (P.nLastInactionableFrames != 0 || (P.cLastStance == 1 && P.cState_Stance != 1)) //Neutral frame
		{
			sFont = FD_NEUTRAL;
		}
		else if (bHasInvuln)
		{
			sFont = FD_ACTIONABLE_INVULN;
		}
	}

	if (bDisplayCancels && P.nState_Flagset2 & 1)
	{
		sFont = FD_UNDERLINE + sFont;
	}
	else
	{
		sFont = FD_CLEAR + sFont;
	}

	if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0 || cP3Freeze != 0 || cP4Freeze != 0) //Screen is frozen
	{
		sFont = FD_FREEZE;
	}
	else if (P.bThrowFlag != 0) //Being thrown
	{
		sFont = FD_THROWN;
		sBarValue = " t";
	}
	else if (P.cHitstop != 0) //in hitstop
	{
		sFont = FD_HITSTOP;
	}
	else if (P.cAnimation_BoxIndex == 12) //Clash
	{
		sFont = FD_CLASH;
	}
	else if (P.dwCondition1_ConditionType == 51) //Shield
	{
		sFont = FD_SHIELD;
	}
	else if (P.dwCondition1_ConditionType == 52 || P.dwCondition2_ConditionType == 52)
	{
		sFont = FD_THROW_ACTIVE;
	}

	P.sBar1[nBarCounter % BAR_MEMORY_SIZE][0] = sFont;
	P.sBar1[nBarCounter % BAR_MEMORY_SIZE][1] = sBarValue;

	//Bar 2 - Active frames
	if (P.dwAttackDataPointer != 0)
	{
		sBarValue = std::format("{:2}", P.nActiveCounter % 100);
		if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0)
		{
			sFont = FD_FREEZE;
		}
		else if (P.cHitstop != 0)
		{
			sFont = FD_HITSTOP;
		}
		else
		{
			sFont = FD_ACTIVE;
		}

		if (P.cState_Stance == 1)
		{
			sFont += FD_UNDERLINE;
		}
	}
	else if (P.cState_Stance == 1)
	{
		sFont = FD_CLEAR;
		sBarValue = " ^";
	}
	else
	{
		sFont = FD_CLEAR;
		sBarValue = "  ";
	}

	P.sBar2[nBarCounter % BAR_MEMORY_SIZE][0] = sFont;
	P.sBar2[nBarCounter % BAR_MEMORY_SIZE][1] = sBarValue;

	//Bar 3 - Projectile and assist active frames
	if (Assist.cExists && Assist.dwAttackDataPointer != 0)
	{
		sBarValue = std::format("{:2}", Assist.nActiveCounter % 100);
		if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0)
		{
			sFont = FD_FREEZE;
		}
		else if (Assist.cHitstop != 0)
		{
			sFont = FD_HITSTOP;
		}
		else
		{
			sFont = FD_ASSIST_ACTIVE;
		}

		if (Assist.cState_Stance == 1)
		{
			sFont += FD_UNDERLINE;
		}
	}
	else if (P.nActiveProjectileCount != 0 || Assist.nActiveProjectileCount != 0)
	{
		sFont = FD_ACTIVE;
		sBarValue = std::format("{:2}", (P.nActiveProjectileCount + Assist.nActiveProjectileCount) % 100);
	}
	else
	{
		sFont = FD_CLEAR;
		sBarValue = "  ";
	}

	P.sBar3[nBarCounter % BAR_MEMORY_SIZE][0] = sFont;
	P.sBar3[nBarCounter % BAR_MEMORY_SIZE][1] = sBarValue;

	if (bDisplayInputs)
	{
		//Bar 4
		if (P.cMacroInput != 0)
		{
			sRightValue = "e";
		}
		else
		{
			sRightValue = ".";
		}

		if (P.cButtonInput & 0x10)
		{
			sLeftFont = FD_A_PRESSED;
		}
		else if (bIsButtonPressed)
		{
			sLeftFont = FD_BUTTON_PRESSED;
		}
		else
		{
			sLeftFont = FD_CLEAR;
		}

		if (P.cButtonInput & 0x20)
		{
			sRightFont = FD_B_PRESSED;
		}
		else if (bIsButtonPressed)
		{
			sRightFont = FD_BUTTON_PRESSED;
		}
		else
		{
			sRightFont = FD_CLEAR;
		}

		if (P.bLastOnRight != P.bIsOnRight)
		{
			sLeftFont += FD_UNDERLINE;
			sRightFont += FD_UNDERLINE;
		}

		P.sBar4[nBarCounter % BAR_MEMORY_SIZE][0] = sLeftFont;
		P.sBar4[nBarCounter % BAR_MEMORY_SIZE][1] = " ";
		P.sBar4[nBarCounter % BAR_MEMORY_SIZE][2] = sRightFont;
		P.sBar4[nBarCounter % BAR_MEMORY_SIZE][3] = sRightValue;

		//Bar 5
		if (P.cRawDirectionalInput == 0)
		{
			sRightValue = ".";
		}
		else if (P.cState_Stance == 1)
		{
			sRightValue = std::to_string(P.cAirDirectionalInput);
		}
		else if (P.bIsOnRight)
		{
			sRightValue = std::to_string(REVERSE_INPUT_MAP[P.cRawDirectionalInput]);
		}
		else
		{
			sRightValue = std::to_string(P.cRawDirectionalInput);
		}

		if (P.cButtonInput & 0x40)
		{
			sLeftFont = FD_C_PRESSED;
		}
		else if (bIsButtonPressed)
		{
			sLeftFont = FD_BUTTON_PRESSED;
		}
		else
		{
			sLeftFont = FD_CLEAR;
		}

		if (P.cButtonInput & 0x80)
		{
			sRightFont = FD_D_PRESSED;
		}
		else if (bIsButtonPressed)
		{
			sRightFont = FD_BUTTON_PRESSED;
		}
		else
		{
			sRightFont = FD_CLEAR;
		}

		if (P.bLastOnRight != P.bIsOnRight)
		{
			sLeftFont += FD_UNDERLINE;
			sRightFont += FD_UNDERLINE;
		}

		P.sBar5[nBarCounter % BAR_MEMORY_SIZE][0] = sLeftFont;
		P.sBar5[nBarCounter % BAR_MEMORY_SIZE][1] = " ";
		P.sBar5[nBarCounter % BAR_MEMORY_SIZE][2] = sRightFont;
		P.sBar5[nBarCounter % BAR_MEMORY_SIZE][3] = sRightValue;
	}
}

void IncrementActive(Player& P)
{
	if (P.dwAttackDataPointer != 0 && P.cHitstop == 0 && nFrameCount != nLastFrameCount)
	{
		P.nActiveCounter += nFrameCount - nLastFrameCount;
	}
	else if (P.dwAttackDataPointer == 0)
	{
		P.nActiveCounter = 0;
	}
}

void HandleInactive(Player& P)
{
	if (P.nInactionableFrames != 0)
	{
		P.nInactionableMemory = P.nInactionableFrames;
	}
}

void BarHandling(HANDLE hMBAAHandle, Player& P1, Player& P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool DoBarUpdate = (
		P1.nInactionableFrames != 0 || P2.nInactionableFrames ||
		P1.nState_Flagset2 != 0 || P2.nState_Flagset2 != 0 ||
		P1.nActiveProjectileCount != 0 || P2.nActiveProjectileCount ||
		(P1Assist.cExists && (P1Assist.dwAttackDataPointer != 0 || P1Assist.nActiveProjectileCount != 0)) ||
		(P2Assist.cExists && (P2Assist.dwAttackDataPointer || P2Assist.nActiveProjectileCount != 0))
		); //True if either char is inactionable, can't block, has an active projectile, has an active assist, or has an active assist projectile

	if (DoBarUpdate)
	{
		bIsBarReset = false;
		bUpdateBar = true;
		nBarIntervalCounter = 0;
	}
	else
	{
		bUpdateBar = false;
		nBarIntervalCounter++;
	}

	if (nBarCounter >= nBarDisplayRange)
	{
		nBarIntervalMax = BAR_INTERVAL;
	}

	if (nBarIntervalCounter > nBarIntervalMax)
	{
		bDoBarReset = true;
	}

	if (bDoBarReset && bUpdateBar)
	{
		ResetBars(hMBAAHandle);
	}

	if (nBarIntervalCounter < nBarIntervalMax && !bIsBarReset)
	{
		bUpdateBar = true;
	}

	if (bUpdateBar)
	{
		if (P1.cHitstop != 0 && P2.cHitstop != 0) //Player hitstop values count down but we need it to count up
		{
			nSharedHitstop++;
		}
		else
		{
			nSharedHitstop = 0;
		}

		bool bIsFreeze = (
			cGlobalFreeze != 0 ||
			nSharedHitstop > 1
			);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
			if (P1Assist.cExists)
			{
				IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);
			}
			if (P2Assist.cExists)
			{
				IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);
			}
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			nBarCounter += nTrueFrameCount - nLastTrueFrameCount;
			if (nBarCounter < 0) {
				nBarCounter = 0;
			}
		}
	}
}

void PrintFrameDisplay(HANDLE hMBAAHandle, Player& P1, Player& P2, Player& P3, Player& P4)
{
	if (nBarDisplayRange != nLastBarDisplayRange || sColumnHeader == "")
	{
		sColumnHeader = "\x1b[0m\x1b[4m";
		for (int i = 1; i <= nBarDisplayRange; i++)
		{
			if (i % 10 != 0)
			{
				sColumnHeader += std::format("{:2}", i % 10);
			}
			else
			{
				sColumnHeader += std::format("\x1b[7m\x1b[4m{:2}\x1b[0m\x1b[4m", i % 100);
			}
		}
		sColumnHeader += "\x1b[0m\x1b[K\n";
	}

	nLastBarScrolling = nBarScrolling;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_frameScroll), &nBarScrolling, 4, 0);
	short sAdjustedScroll = min(min(nBarCounter - nBarDisplayRange, BAR_MEMORY_SIZE - nBarDisplayRange), nBarScrolling);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - nBarDisplayRange - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= nBarDisplayRange)
	{
		nForStart = 0;
		nForEnd = nBarCounter;
	}

	std::string sLastFont1 = "";
	std::string sLastFont2 = "";
	std::string sLastFont3 = "";
	std::string sLastFont4Left = "";
	std::string sLastFont4Right = "";
	std::string sLastFont5Left = "";
	std::string sLastFont5Right = "";

	P1.sBarString1 = "";
	P1.sBarString2 = "";
	P1.sBarString3 = "";
	P1.sBarString4 = "";
	P1.sBarString5 = "";
	P2.sBarString1 = "";
	P2.sBarString2 = "";
	P2.sBarString3 = "";
	P2.sBarString4 = "";
	P2.sBarString5 = "";
	bool bFirstFrameInDisplay = true;
	for (int i = nForStart; i < nForEnd; i++)
	{
		int c = i < 0 ? i + BAR_MEMORY_SIZE : i;
		int l = i - 1 < 0 ? i - 1 + BAR_MEMORY_SIZE : i - 1;
		if (bFirstFrameInDisplay)
		{
			P1.sBarString1 += P1.sBar1[c][0];
			P1.sBarString1 += P1.sBar1[c][1];
			P1.sBarString2 += P1.sBar2[c][0];
			P1.sBarString2 += P1.sBar2[c][1];
			P1.sBarString3 += P1.sBar3[c][0];
			P1.sBarString3 += P1.sBar3[c][1];
			P1.sBarString4 += P1.sBar4[c][0];
			P1.sBarString4 += P1.sBar4[c][1];
			if (P1.sBar4[c][2] != P1.sBar4[c][0]) P1.sBarString4 += P1.sBar4[c][2];
			P1.sBarString4 += P1.sBar4[c][3];
			P1.sBarString5 += P1.sBar5[c][0];
			P1.sBarString5 += P1.sBar5[c][1];
			if (P1.sBar5[c][2] != P1.sBar5[c][0]) P1.sBarString5 += P1.sBar5[c][2];
			P1.sBarString5 += P1.sBar5[c][3];

			P2.sBarString1 += P2.sBar1[c][0];
			P2.sBarString1 += P2.sBar1[c][1];
			P2.sBarString2 += P2.sBar2[c][0];
			P2.sBarString2 += P2.sBar2[c][1];
			P2.sBarString3 += P2.sBar3[c][0];
			P2.sBarString3 += P2.sBar3[c][1];
			P2.sBarString4 += P2.sBar4[c][0];
			P2.sBarString4 += P2.sBar4[c][1];
			if (P2.sBar4[c][2] != P2.sBar4[c][0]) P2.sBarString4 += P2.sBar4[c][2];
			P2.sBarString4 += P2.sBar4[c][3];
			P2.sBarString5 += P2.sBar5[c][0];
			P2.sBarString5 += P2.sBar5[c][1];
			if (P2.sBar5[c][2] != P2.sBar5[c][0]) P2.sBarString5 += P2.sBar5[c][2];
			P2.sBarString5 += P2.sBar5[c][3];
		}
		else
		{
			if (P1.sBar1[c][0] != P1.sBar1[l][0]) P1.sBarString1 += P1.sBar1[c][0];
			P1.sBarString1 += P1.sBar1[c][1];
			if (P1.sBar2[c][0] != P1.sBar2[l][0]) P1.sBarString2 += P1.sBar2[c][0];
			P1.sBarString2 += P1.sBar2[c][1];
			if (P1.sBar3[c][0] != P1.sBar3[l][0]) P1.sBarString3 += P1.sBar3[c][0];
			P1.sBarString3 += P1.sBar3[c][1];
			if (P1.sBar4[c][0] != P1.sBar4[l][2]) P1.sBarString4 += P1.sBar4[c][0];
			P1.sBarString4 += P1.sBar4[c][1];
			if (P1.sBar4[c][2] != P1.sBar4[c][0]) P1.sBarString4 += P1.sBar4[c][2];
			P1.sBarString4 += P1.sBar4[c][3];
			if (P1.sBar5[c][0] != P1.sBar5[l][2]) P1.sBarString5 += P1.sBar5[c][0];
			P1.sBarString5 += P1.sBar5[c][1];
			if (P1.sBar5[c][2] != P1.sBar5[c][0]) P1.sBarString5 += P1.sBar5[c][2];
			P1.sBarString5 += P1.sBar5[c][3];

			if (P2.sBar1[c][0] != P2.sBar1[l][0]) P2.sBarString1 += P2.sBar1[c][0];
			P2.sBarString1 += P2.sBar1[c][1];
			if (P2.sBar2[c][0] != P2.sBar2[l][0]) P2.sBarString2 += P2.sBar2[c][0];
			P2.sBarString2 += P2.sBar2[c][1];
			if (P2.sBar3[c][0] != P2.sBar3[l][0]) P2.sBarString3 += P2.sBar3[c][0];
			P2.sBarString3 += P2.sBar3[c][1];
			if (P2.sBar4[c][0] != P2.sBar4[l][2]) P2.sBarString4 += P2.sBar4[c][0];
			P2.sBarString4 += P2.sBar4[c][1];
			if (P2.sBar4[c][2] != P2.sBar4[c][0]) P2.sBarString4 += P2.sBar4[c][2];
			P2.sBarString4 += P2.sBar4[c][3];
			if (P2.sBar5[c][0] != P2.sBar5[l][2]) P2.sBarString5 += P2.sBar5[c][0];
			P2.sBarString5 += P2.sBar5[c][1];
			if (P2.sBar5[c][2] != P2.sBar5[c][0]) P2.sBarString5 += P2.sBar5[c][2];
			P2.sBarString5 += P2.sBar5[c][3];
		}
		bFirstFrameInDisplay = false;
	}
	P1.sBarString1 += "\x1b[0m\x1b[K\n";
	P1.sBarString2 += "\x1b[0m\x1b[K\n";
	P1.sBarString3 += "\x1b[0m\x1b[K\n";
	P1.sBarString4 += "\x1b[0m\x1b[K\n";
	P1.sBarString5 += "\x1b[0m\x1b[K\n";
	P2.sBarString1 += "\x1b[0m\x1b[K\n";
	P2.sBarString2 += "\x1b[0m\x1b[K\n";
	P2.sBarString3 += "\x1b[0m\x1b[K\n";
	P2.sBarString4 += "\x1b[0m\x1b[K\n";
	P2.sBarString5 += "\x1b[0m\x1b[K\n";


	int nP1XPixelPosition = (int)floor(P1.nXPosition / 128.0);
	int nP1YPixelPosition = (int)floor(P1.nYPosition / 128.0);
	char cP1CounterHit = CH_MAP[P1.cCounterHit];
	int nP1GuardGauge = (int)P1.fGuardGauge;
	int nP1Gravity = max(0, (int)100 - floor(100 - (P1.fGravity - 0.072f) / 0.928f * 19));
	int nP1GravityHits = (int)round(P1.fGravity / 0.008);

	int nP2XPixelPosition = (int)floor(P2.nXPosition / 128.0);
	int nP2YPixelPosition = (int)floor(P2.nYPosition / 128.0);
	char cP2CounterHit = CH_MAP[P2.cCounterHit];
	int nP2GuardGauge = (int)P2.fGuardGauge;
	int nP2Gravity = max(0, (int)100 - floor(100 - (P2.fGravity - 0.072f) / 0.928f * 19));
	int nP2GravityHits = (int)round(P2.fGravity / 0.008);

	int nXDistance = (int)abs(P1.nXPosition - P2.nXPosition);
	int nYDistance = (int)abs(P1.nYPosition - P2.nYPosition);
	int nXPixelDistance = (int)abs(floor(P1.nXPosition / 128.0) - floor(P2.nXPosition / 128.0));
	int nYPixelDistance = (int)abs(floor(P1.nYPosition / 128.0) - floor(P2.nYPosition / 128.0));

	if (!bAdvancedFrameData)
	{
		writeBuffer("\x1b[0m" "Total %3i / Advantage %3i / Distance %3i" "\n", P1.nInactionableMemory, nPlayerAdvantage, nXPixelDistance);
	}

	if (bAdvancedFrameData)
	{
		writeBuffer("\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "pat %3i [%2i]" "\x1b[7m"
			"x-spd %5i" "\x1b[0m" "x-acc %5i" "\x1b[7m" "y-spd %5i" "\x1b[0m" "y-acc %5i" "\x1b[7m" "hp %5i" "\x1b[0m" "mc %5i" "\x1b[0m\x1b[K\n"
			"\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "pat %3i [%2i]" "\x1b[7m"
			"x-spd %5i" "\x1b[0m" "x-acc %5i" "\x1b[7m" "y-spd %5i" "\x1b[0m" "y-acc %5i" "\x1b[7m" "hp %5i" "\x1b[0m" "mc %5i" "\x1b[0m\x1b[K\n"
			"\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "adv %3i" "\x1b[K\n",
			P1.nXPosition, P1.nYPosition, nP1XPixelPosition, nP1YPixelPosition, P1.nPattern, P1.nState,
			P1.nXSpeed + P1.nMomentum, P1.sXAcceleration, P1.nYSpeed, P1.sYAcceleration, P1.nHealth, P1.nMagicCircuit,
			P2.nXPosition, P2.nYPosition, nP2XPixelPosition, nP2YPixelPosition, P2.nPattern, P2.nState,
			P2.nXSpeed + P2.nMomentum, P2.sXAcceleration, P2.nYSpeed, P2.sYAcceleration, P2.nHealth, P2.nMagicCircuit,
			nXDistance, nYDistance, nXPixelDistance, nYPixelDistance, nPlayerAdvantage);
	}

	writeBuffer("%s%s%s%s%s%s%s", sColumnHeader.c_str(), P1.sBarString1.c_str(), P1.sBarString2.c_str(), P1.sBarString3.c_str(), P2.sBarString1.c_str(), P2.sBarString2.c_str(), P2.sBarString3.c_str());

	if (bAdvancedFrameData)
	{
		writeBuffer("\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %3i [%2i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %5i [%.3f]" "\x1b[0m" "total %3i" "\x1b[0m\x1b[K\n"
			"\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %3i [%2i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %5i [%.3f]" "\x1b[0m" "total %3i"  "\x1b[0m\x1b[K\n",
			cP1Freeze, cP1CounterHit, P3.nPattern, P3.nState, nP1GravityHits, nP1Gravity % 100, P1.sUntechPenalty % 10, P1.nRedHealth, nP1GuardGauge, P1.fGuardQuality, P1.nInactionableMemory % 1000,
			cP2Freeze, cP2CounterHit, P4.nPattern, P4.nState, nP2GravityHits, nP2Gravity % 100, P2.sUntechPenalty % 10, P2.nRedHealth, nP2GuardGauge, P2.fGuardQuality, P2.nInactionableMemory % 1000);
	}

	if (bDisplayInputs)
	{
		writeBuffer("%s%s%s%s%s", sColumnHeader.c_str(), P1.sBarString4.c_str(), P1.sBarString5.c_str(), P2.sBarString4.c_str(), P2.sBarString5.c_str());
	}

	displayBuffer();
}

void FrameDisplay(HANDLE hMBAAHandle)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_frameData), &bAdvancedFrameData, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_showHitstopAndFreeze), &bDisplayFreeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_showInputs), &bDisplayInputs, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_showCancel), &bDisplayCancels, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_frameScroll), &nBarScrolling, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(adMBAABase + adXS_colorGuide), &bPrintColorGuide, 1, 0);

	cLastDisplayOptions = cDisplayOptions;
	cDisplayOptions = 8 * bAdvancedFrameData + 4 * bDisplayFreeze + 2 * bDisplayInputs + 1 * bPrintColorGuide;

	CheckGameState(hMBAAHandle);

	if (cGameState != 1) //If not in game (any gamemode)
	{
		std::cout << "\x1b[J";
		return;
	}

	nLastBarDisplayRange = nBarDisplayRange;
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo);
	nBarDisplayRange = (screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left) / 2;
	if (nBarDisplayRange != nLastBarDisplayRange || cDisplayOptions != cLastDisplayOptions)
	{
		std::cout << "\x1b[J";
	}

	UpdateGlobals(hMBAAHandle);

	Main1 = &P1;
	Main2 = &P2;
	Assist1 = &P3;
	Assist2 = &P4;
	if (P1.bTagFlag)
	{
		Main1 = &P3;
		Assist1 = &P1;
	}
	if (P2.bTagFlag)
	{
		Main2 = &P4;
		Assist2 = &P2;
	}

	if (nTrueFrameCount == 0)
	{
		ResetBars(hMBAAHandle);
	}

	if (nLastTrueFrameCount != nTrueFrameCount && nTrueFrameCount == nTimer)
	{
		UpdatePlayers(hMBAAHandle);
		CheckProjectiles(hMBAAHandle);

		BarHandling(hMBAAHandle, *Main1, *Main2, *Assist1, *Assist2);
		nLastFrameCount = nFrameCount;
		nLastTrueFrameCount = nTrueFrameCount;

		if (bPrintColorGuide)
		{
			PrintColorGuide();
		}
		else
		{
			PrintFrameDisplay(hMBAAHandle, *Main1, *Main2, *Assist1, *Assist2);
		}
	}

	if (cDisplayOptions != cLastDisplayOptions || nBarScrolling != nLastBarScrolling || nBarDisplayRange != nLastBarDisplayRange)
	{
		if (bPrintColorGuide)
		{
			PrintColorGuide();
		}
		else
		{
			PrintFrameDisplay(hMBAAHandle, *Main1, *Main2, *Assist1, *Assist2);
		}
	}

	nPlayerAdvantage = ((*Main1).nAdvantageCounter - (*Main2).nAdvantageCounter) % 100;

}

