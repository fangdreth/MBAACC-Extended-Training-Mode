#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "..\Common\Common.h"

const ADDRESS adBaseAddress = (0x00400000);

int nLastFrameCount = 0; //Counts slower during slowdown
int nLastTrueFrameCount = 0; //Counts all frames during slowdown

int nBarCounter = 0;
int nBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;

bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;
bool bAddPlayerFreeze = false;

bool bDisplayFreeze = false; //Whether to show global ex flashes and frames where both chars are in hitstop
bool bDisplayInputs = false;

int nPlayerAdvantage;
int nSharedHitstop;

static int nFrameBarY = 400;

char pcTextToDisplay[256];
char pcTextPattern[256];
int nDrawTextTimer = 0;
int nClearSaveTimer = 0;

struct Player
{
	char cPlayerNumber = 0;

	DWORD adPlayerBase = 0x0;
	DWORD adInaction = 0x0;

	int nLastInactionableFrames = 0;

	DWORD dwColorBar1[BAR_MEMORY_SIZE][2]; //[0] = main color, [1] = sub color
	DWORD dwColorBar2[BAR_MEMORY_SIZE][2]; //[0] = jump, [1] = effect/assist active

	int nNumBar[BAR_MEMORY_SIZE][2]; //[0] = number, [1] = flag for keeping number

	int nActiveCounter = 0;
	int nInactionableMemory = 0;
	int nAdvantageCounter = 0;
	int nFirstActiveCounter = 0;
	int nLastFrameCount = 0;
	bool bLastOnRight = false;
	DWORD dwLastActivePointer = 0x0;
	char cLastHitstop = 0;
	int nFirstActive = 0;
	bool bAlreadyGotFirstActive = false;
	bool bProjectileActive = false;
	bool bLastProjectileActive = false;
};

Player P1{ 0, adMBAABase + adP1Base, adMBAABase + adP1Inaction };
Player P2{ 1, adMBAABase + adP2Base, adMBAABase + adP2Inaction };
Player P3{ 2, adMBAABase + adP3Base, adMBAABase + adP1Inaction };
Player P4{ 3, adMBAABase + adP4Base, adMBAABase + adP2Inaction };

Player* paPlayerArray[4] = { &P1, &P2, &P3, &P4 };

Player* Main1 = &P1;
Player* Main2 = &P2;
Player* Assist1 = &P3;
Player* Assist2 = &P4;

void UpdatePlayers() //Called after bar handling
{
	for (int i = 0; i < 4; i++)
	{
		Player& P = *paPlayerArray[i];
		P.nLastInactionableFrames = *(int*)(P.adInaction);
		P.nLastFrameCount = *(int*)(P.adPlayerBase + adPlayerFrameCount);
		P.bLastOnRight = *(int*)(P.adPlayerBase + adOnRightFlag);
		P.dwLastActivePointer = *(DWORD*)(P.adPlayerBase + adAttackDataPointer);
		P.cLastHitstop = *(char*)(P.adPlayerBase + adHitstop);
		if (*(short*)(P.adPlayerBase + adInputEvent) != -1)
		{
			P.bAlreadyGotFirstActive = false;
			P.nFirstActiveCounter = 0;
		}
	}
}

void CheckProjectiles()
{
	P1.bLastProjectileActive = P1.bProjectileActive;
	P1.bProjectileActive = false;
	P2.bLastProjectileActive = P2.bProjectileActive;
	P2.bProjectileActive = false;
	P3.bLastProjectileActive = P3.bProjectileActive;
	P3.bProjectileActive = false;
	P4.bLastProjectileActive = P4.bProjectileActive;
	P4.bProjectileActive = false;
	char cBlankEffectCount = 0;
	for (int i = 0; i < 200; i++) //Check Projectiles for active
	{
		if (cBlankEffectCount > 16) break;
		cBlankEffectCount++;
		if (*(char*)((adMBAABase + adEffectBase) + dwEffectStructSize * i) != 0 &&
			*(DWORD*)((adMBAABase + adEffectBase) + dwEffectStructSize * i + adAttackDataPointer) != 0 &&
			*(int*)((adMBAABase + adEffectBase) + dwEffectStructSize * i + adEffectStatus) == 0xFF)
		{
			cBlankEffectCount = 0;
			Player& P = *paPlayerArray[*(char*)((adMBAABase + adEffectBase) + dwEffectStructSize * i + adEffectOwner)];
			P.bProjectileActive = true;
		}
	}
}

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (*(int*)(P1.adInaction) == 0 && *(int*)(P2.adInaction) == 0)
	{
		bDoAdvantage = false;
	}
	else if (*(int*)(P1.adInaction) != 0 && *(int*)(P2.adInaction) != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && *(int*)(adMBAABase + adFrameCount) != nLastFrameCount && *(char*)(adMBAABase + adGlobalFreeze) == 0)
	{
		if (*(int*)(P1.adInaction) == 0 && *(int*)(P2.adInaction) != 0)
		{
			P1.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		}
		else if (*(int*)(P2.adInaction) == 0 && *(int*)(P1.adInaction) != 0)
		{
			P2.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		}
	}
}

void ResetBars()
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	nBarScrolling = 0;
	bDoBarReset = false;
	nBarIntervalMax = DISPLAY_RANGE;
	for (int i = 0; i < 4; i++)
	{
		Player& P = *paPlayerArray[i];
		for (int i = 0; i < BAR_MEMORY_SIZE; i++)
		{
			P.dwColorBar1[i][0] = 0x00000000;
			P.dwColorBar1[i][1] = 0x00000000;
			P.dwColorBar2[i][0] = 0x00000000;
			P.dwColorBar2[i][1] = 0x00000000;

			P.nNumBar[i][0] = -1;
			P.nNumBar[i][1] = 0;
		}
	}
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)(adBaseAddress + adSharedScrolling), &nBarScrolling, 2, 0);
}

void UpdateBars(Player& P, Player& Assist)
{
	DWORD dwColor = 0x00000000;
	DWORD dwColor2 = 0x00000000;
	DWORD dwBar2Color0 = 0x00000000;
	DWORD dwBar2Color1 = 0x00000000;
	int nNumber = -1;
	int nNumFlag = 0; //0 = default, go away on next info; 1 = persist always; 2 = persist and delete prior 2s; 3 = get deleted and pass it on if followed by 2
	bool bIsButtonPressed = *(char*)(P.adPlayerBase + adButtonInput) != 0 || *(char*)(P.adPlayerBase + adMacroInput) != 0;

	//Bar 1 - General action information
	if (*(int*)(P.adInaction) != 0) //Doing something with limited actionability
	{
		dwColor = FB_INACTIONABLE;
		nNumber = *(int*)P.adInaction;
		if (*(int*)(P.adPlayerBase + adPattern) >= 35 && *(int*)(P.adPlayerBase + adPattern) <= 37) //Jump Startup
		{
			dwColor = FB_JUMP;
		}
		else if (*(int*)(P.adPlayerBase + adHitstunRemaining) != 0 && *(char*)(P.adPlayerBase + adBlockstunFlag) == 0) //Hitstun
		{
			dwColor = FB_HITSTUN;
			if (*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Stance) == 1) //Airborne
			{
				if (*(short*)(P.adPlayerBase + adUntechCounter) < *(short*)(P.adPlayerBase + adUntechTotal)) //Still has untech remaining
				{
					nNumber = *(short*)(P.adPlayerBase + adUntechTotal) - *(short*)(P.adPlayerBase + adUntechCounter);
				}
			}
			else //Grounded
			{
				if (*(int*)(P.adPlayerBase + adHitstunRemaining) > 2) //Still has hitstun remaining
				{
					nNumber = *(short*)(P.adPlayerBase + adHitstunRemaining) - 1;
				}
			}
		}
		else if (*(char*)(P.adPlayerBase + adBlockstunFlag)) //Blockstun
		{
			dwColor = FB_BLOCKSTUN;
			if (*(int*)(P.adPlayerBase + adHitstunRemaining) > 2 && *(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Stance) != 1)
			{
				nNumber = *(short*)(P.adPlayerBase + adHitstunRemaining) - 1;
			}
		}
		else if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0) //Attacking
		{
			dwColor = FB_ACTIVE;
			nNumber = P.nActiveCounter;
			nNumFlag = 2;
			if (P.dwLastActivePointer == 0 && !P.bAlreadyGotFirstActive)
			{
				P.nFirstActive = P.nFirstActiveCounter;
				P.bAlreadyGotFirstActive = true;
			}
		}
	}
	else //Fully actionable
	{
		dwColor = FB_ACTIONABLE;

		if (P.nLastInactionableFrames != 0) //Neutral frame
		{
			dwColor = FB_NEUTRAL;
		}
		else if (bDoAdvantage) //Has advantage
		{
			dwColor = FB_ADVANTAGE;
		}
	}

	char cCondition1Type = 0;
	char cCondition2Type = 0;
	if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_ConditionCount) > 0)
	{
		DWORD dwPointer = *(DWORD*)(P.adPlayerBase + adAnimationDataPointer);
		if (dwPointer != 0)
		{
			dwPointer = *(DWORD*)(dwPointer + adAnimationData_ConditionsPointer);
			if (dwPointer != 0)
			{
				DWORD dwC1Pointer = *(DWORD*)(dwPointer + adConditions_Condition1Pointer);
				if (dwC1Pointer != 0)
				{
					cCondition1Type = *(char*)(dwC1Pointer + adCondition_Type);
				}
				if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_ConditionCount) > 1)
				{
					DWORD dwC2Pointer = *(DWORD*)(dwPointer + adConditions_Condition2Pointer);
					if (dwC2Pointer != 0)
					{
						cCondition2Type = *(char*)(dwC2Pointer + adCondition_Type);
					}
				}
			}
		}
	}

	if (*(char*)(adMBAABase + adGlobalFreeze) != 0 || //Screen is frozen
		*(int*)(adMBAABase + adP1Freeze) != 0 ||
		*(int*)(adMBAABase + adP2Freeze) != 0)
	{
		dwColor = FB_FREEZE;
		if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0) //Attacking
		{
			dwColor = FB_FREEZE_ACTIVE;
		}
	}
	else if (*(char*)(P.adPlayerBase + adThrowFlag) != 0) //Being thrown
	{
		dwColor = FB_THROWN;
	}
	else if (*(char*)(P.adPlayerBase + adHitstop) != 0) //in hitstop
	{
		dwColor = FB_HITSTOP;
		if (*(char*)(P.adPlayerBase + adNotInCombo) == 0 && P.cLastHitstop == 0)
		{
			nNumFlag = 1;
		}
		else if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0)
		{
			nNumFlag = 3;
			nNumber = -1;
		}
	}
	else if (cCondition1Type == 51) //Shield
	{
		dwColor = FB_SHIELD;
	}
	else if (cCondition1Type == 52 || cCondition2Type == 52) //Throw
	{
		if (!P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
		}
		dwColor = FB_THROW_ACTIVE;
	}

	if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_BoxIndex) == 12) //Clash
	{
		dwColor2 = FB_CLASH;
	}
	else if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_BoxIndex) <= 1 || //Various forms of invuln
		*(char*)(P.adPlayerBase + adEFStrikeInvuln) != 0 ||
		*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Invuln) == 3)
	{
		dwColor2 = FB_INVULN;
	}

	if (*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Stance) == 1) //Airborne
	{
		dwBar2Color0 = FB_JUMP;
	}

	if (P.bProjectileActive || Assist.bProjectileActive)
	{
		dwBar2Color1 = FB_ACTIVE;
		if (!P.bLastProjectileActive && !P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
			P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] = 1;
		}
	}

	if (*(DWORD*)(Assist.adPlayerBase + adAttackDataPointer) != 0) //Check Assist for active
	{
		dwBar2Color1 = FB_ASSIST_ACTIVE;
		if (Assist.dwLastActivePointer == 0 && !P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
			P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] = 1;
		}
	}

	P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][0] = dwColor;
	P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][1] = dwColor2;

	P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][0] = dwBar2Color0;
	P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][1] = dwBar2Color1;

	P.nNumBar[nBarCounter % BAR_MEMORY_SIZE][0] = nNumber;
	P.nNumBar[nBarCounter % BAR_MEMORY_SIZE][1] = nNumFlag;
	if (nNumFlag != 2 &&
		nNumber >= 0 && 
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] == 0) //clear last frame if current frame has info and last frame numFlag = 0
	{
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][0] = -1;
	}
	else if (nNumFlag == 2 &&
		nNumber >= 0 &&
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] == 2) //clear last frame if current and last frame numFlag = 2
	{
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][0] = -1;
	}
	else if (nNumFlag == 2 &&
		nNumber >= 0 &&
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] == 3) //continuous clear if numFlag = 2 and previous = 3
	{
		int i = 1;
		while (P.nNumBar[(nBarCounter - i) % BAR_MEMORY_SIZE][1] == 3)
		{
			P.nNumBar[(nBarCounter - ++i) % BAR_MEMORY_SIZE][0] = -1;
		}
	}
}

void IncrementActive(Player& P)
{
	if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0 && *(char*)(P.adPlayerBase + adHitstop) == 0 && *(int*)(P.adPlayerBase + adPlayerFrameCount) != P.nLastFrameCount)
	{
		P.nActiveCounter += *(int*)(P.adPlayerBase + adPlayerFrameCount) - P.nLastFrameCount;
	}
	else if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) == 0)
	{
		P.nActiveCounter = 0;
	}
}

void IncrementFirstActive(Player& P)
{
	if (*(char*)(P.adPlayerBase + adHitstop) == 0 &&
		*(int*)(adMBAABase + adP1Freeze) == 0 &&
		*(int*)(adMBAABase + adP2Freeze) == 0 &&
		*(char*)(adMBAABase + adGlobalFreeze) == 0 &&
		*(int*)(P.adPlayerBase + adPlayerFrameCount) != P.nLastFrameCount)
	{
		P.nFirstActiveCounter += *(int*)(P.adPlayerBase + adPlayerFrameCount) - P.nLastFrameCount;
		bAddPlayerFreeze = true;
	}
	if (bAddPlayerFreeze &&
		(*(int*)(adMBAABase + adP1Freeze) != 0 ||
		*(int*)(adMBAABase + adP2Freeze) != 0) &&
		*(int*)(P.adPlayerBase + adPlayerFrameCount) != P.nLastFrameCount)
	{
		P.nFirstActiveCounter += *(int*)(P.adPlayerBase + adPlayerFrameCount) - P.nLastFrameCount;
		bAddPlayerFreeze = false;
	}
}

void HandleInactive(Player& P)
{
	if (*(int*)(P.adInaction) != 0)
	{
		P.nInactionableMemory = *(int*)(P.adInaction);
	}
}

void BarHandling(Player& P1, Player& P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool IsInput = (
		*(DWORD*)(P1.adPlayerBase + adRawDirectionalInput) != 0 ||
		*(DWORD*)(P2.adPlayerBase + adRawDirectionalInput) != 0
		); //True if player or dummy has any input
	bool DoBarUpdate = (
		*(int*)(P1.adInaction) != 0 ||
		*(int*)(P2.adInaction) ||
		*(DWORD*)(*(DWORD*)(*(DWORD*)(P1.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Flagset2) != 0 ||
		*(DWORD*)(*(DWORD*)(*(DWORD*)(P2.adPlayerBase + adAnimationDataPointer) + adAnimationData_StateDataPointer) + adStateData_Flagset2) != 0 ||
		P1.bProjectileActive != 0 ||
		P2.bProjectileActive != 0 ||
		*(DWORD*)(P1Assist.adPlayerBase + adAttackDataPointer) != 0 ||
		*(DWORD*)(P2Assist.adPlayerBase + adAttackDataPointer) != 0 ||
		P1Assist.bProjectileActive != 0 ||
		P2Assist.bProjectileActive != 0
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

	if (nBarCounter >= DISPLAY_RANGE)
	{
		nBarIntervalMax = BAR_INTERVAL;
	}

	if (nBarIntervalCounter > nBarIntervalMax)
	{
		bDoBarReset = true;
	}

	if (bDoBarReset && bUpdateBar)
	{
		ResetBars();
	}

	if (nBarIntervalCounter < nBarIntervalMax && !bIsBarReset)
	{
		bUpdateBar = true;
	}

	if (bUpdateBar)
	{
		if (*(char*)(P1.adPlayerBase + adHitstop) != 0 &&
			*(char*)(P2.adPlayerBase + adHitstop) != 0) //Player hitstop values count down but we need it to count up
		{
			nSharedHitstop++;
		}
		else
		{
			nSharedHitstop = 0;
		}

		bool bIsFreeze = (
			*(char*)(adMBAABase + adGlobalFreeze) != 0 ||
			nSharedHitstop > 1
			);

		IncrementFirstActive(P1);
		IncrementFirstActive(P2);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			if (*(char*)(P1Assist.adPlayerBase))
			{
				//IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);
			}
			if (*(char*)(P2Assist.adPlayerBase))
			{
				//IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);
			}
			nBarCounter += *(int*)(adMBAABase + adTrueFrameCount) - nLastTrueFrameCount;
		}
	}
}

void FrameBar(Player& P1, Player& P2, Player& P3, Player& P4)
{
	bDisplayFreeze = *(char*)(adMBAABase + adSharedDisplayFreeze);
	bDisplayInputs = *(char*)(adMBAABase + adSharedDisplayInputs);
	nBarScrolling = *(short*)(adMBAABase + adSharedScrolling);

	Main1 = &P1;
	Main2 = &P2;
	Assist1 = &P3;
	Assist2 = &P4;
	if (*(char*)(P1.adPlayerBase + adTagFlag))
	{
		Main1 = &P3;
		Assist1 = &P1;
	}
	if (*(char*)(P2.adPlayerBase + adTagFlag))
	{
		Main2 = &P4;
		Assist2 = &P2;
	}
	
	CheckProjectiles();

	if (*(int*)(adMBAABase + adTrueFrameCount) != nLastTrueFrameCount)
	{
		BarHandling(*Main1, *Main2, *Assist1, *Assist2);
	}

	if (*(int*)(adMBAABase + adTrueFrameCount) == 1)
	{
		ResetBars();
	}

	nPlayerAdvantage = ((*Main1).nAdvantageCounter - (*Main2).nAdvantageCounter) % 100;

	UpdatePlayers();

	nLastFrameCount = *(int*)(adMBAABase + adFrameCount);
	nLastTrueFrameCount = *(int*)(adMBAABase + adTrueFrameCount);
}
