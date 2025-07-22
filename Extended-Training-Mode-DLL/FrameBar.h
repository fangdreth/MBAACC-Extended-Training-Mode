#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "DebugInfo.h"
#include "TrainingMenu.h"

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

	PlayerData* PlayerData = 0x0;
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
	//bool bAddPlayerFreeze = false;
};

Player P1{ 0, (PlayerData*)(adMBAABase + adP1Base), adMBAABase + adP1Inaction };
Player P2{ 1, (PlayerData*)(adMBAABase + adP2Base), adMBAABase + adP2Inaction };
Player P3{ 2, (PlayerData*)(adMBAABase + adP3Base), adMBAABase + adP1Inaction };
Player P4{ 3, (PlayerData*)(adMBAABase + adP4Base), adMBAABase + adP2Inaction };

Player* paPlayerArray[4] = { &P1, &P2, &P3, &P4 };

Player* Main1 = &P1;
Player* Main2 = &P2;
Player* Assist1 = &P3;
Player* Assist2 = &P4;

void UpdatePlayers() //Called after bar handling
{
	for (int i = 0; i < 4; i++) {
		Player& P = *paPlayerArray[i];
		P.nLastInactionableFrames = *(int*)(P.adInaction);
		P.nLastFrameCount = P.PlayerData->heatTimeCounter;
		P.bLastOnRight = P.PlayerData->isOpponentToLeft;
		P.dwLastActivePointer = (DWORD)P.PlayerData->attackDataPtr;
		P.cLastHitstop = P.PlayerData->hitstop;
		if (P.PlayerData->targetPattern != 0xFFFF) {
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
	EffectData* curFX = (EffectData*)(adMBAABase + adEffectBase);
	for (int i = 0; i < 200; i++) //Check Projectiles for active
	{
		if (cBlankEffectCount > 16) break;
		cBlankEffectCount++;
		if (curFX->exists)
		{
			cBlankEffectCount = 0;
			if (curFX->attackDataPtr != 0 &&
				curFX->someFlag == 0xFF)
			{
				Player& P = *paPlayerArray[curFX->index];
				P.bProjectileActive = true;
			}
		}
		curFX = (EffectData*)(adMBAABase + adEffectBase + dwEffectStructSize * i);
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

	if (bDoAdvantage && *(int*)(adMBAABase + adFrameCount) != nLastFrameCount &&
		*(int*)(adMBAABase + adP1Freeze) == 0 &&
		*(int*)(adMBAABase + adP2Freeze) == 0 &&
		*(char*)(adMBAABase + adGlobalFreeze) == 0)
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
	nTRUE_SCROLL_DISPLAY = 0;
	nSCROLL_DISPLAY = 2;
	*(short*)(adMBAABase + adXS_frameScroll) = -nTRUE_SCROLL_DISPLAY;
}

void UpdateBars(Player& P, Player& Assist)
{
	DWORD dwColor = 0x00000000;
	DWORD dwColor2 = 0x00000000;
	DWORD dwBar2Color0 = 0x00000000;
	DWORD dwBar2Color1 = 0x00000000;
	int nNumber = -1;
	int nNumFlag = 0; //0 = default, go away on next info; 1 = persist always; 2 = persist and delete prior 2s; 3 = get deleted and pass it on if followed by 2

	//Bar 1 - General action information
	if (*(int*)(P.adInaction) != 0) //Doing something with limited actionability
	{
		dwColor = FB_INACTIONABLE;
		nNumber = *(int*)P.adInaction;
		if (P.PlayerData->pattern >= 35 && P.PlayerData->pattern <= 37) //Jump Startup
		{
			dwColor = FB_JUMP;
		}
		else if (P.PlayerData->hitstunTimeRemaining != 0 && !P.PlayerData->inBlockstun) //Hitstun
		{
			dwColor = FB_HITSTUN;
			if (P.PlayerData->animationDataPtr->stateData->stance == 1) //Airborne
			{
				if (P.PlayerData->untechTimeElapsed < P.PlayerData->totalUntechTime) //Still has untech remaining
				{
					nNumber = P.PlayerData->totalUntechTime - P.PlayerData->untechTimeElapsed;
				}
			}
			else //Grounded
			{
				if (P.PlayerData->hitstunTimeRemaining > 2) //Still has hitstun remaining
				{
					nNumber = P.PlayerData->hitstunTimeRemaining - 1;
				}
			}
		}
		else if (P.PlayerData->inBlockstun) //Blockstun
		{
			dwColor = FB_BLOCKSTUN;
			if (P.PlayerData->hitstunTimeRemaining > 2 && P.PlayerData->animationDataPtr->stateData->stance != 1)
			{
				nNumber = P.PlayerData->hitstunTimeRemaining - 1;
			}
		}
		else if (P.PlayerData->attackDataPtr) //Attacking
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

	bool bIsShield = false;
	bool bIsThrow = false;
	if (P.PlayerData->animationDataPtr->highestIFIndex > 0)
	{
		AnimationData* animData = P.PlayerData->animationDataPtr;
		for (int i = 0; i < animData->highestIFIndex; i++) {
			if (animData->IFs[i] != 0) {
				if (animData->IFs[i]->IFTP == 52) {
					bIsThrow = true;
				}
				else if (animData->IFs[i]->IFTP == 51) {
					bIsShield = true;
				}
			}
		}
	}

	if (*(char*)(adMBAABase + adGlobalFreeze) != 0 || //Screen is frozen
		*(int*)(adMBAABase + adP1Freeze) != 0 ||
		*(int*)(adMBAABase + adP2Freeze) != 0)
	{
		dwColor = FB_FREEZE;
		if (P.PlayerData->attackDataPtr != 0) //Attacking
		{
			dwColor = FB_FREEZE_ACTIVE;
		}
	}
	else if (P.PlayerData->throwFlag) //Being thrown
	{
		dwColor = FB_THROWN;
	}
	else if (P.PlayerData->hitstop != 0) //in hitstop
	{
		dwColor = FB_HITSTOP;
		if (!P.PlayerData->notInCombo && P.cLastHitstop == 0)
		{
			nNumFlag = 1;
		}
		else if (P.PlayerData->attackDataPtr != 0)
		{
			nNumFlag = 3;
			nNumber = -1;
		}
	}
	else if (bIsShield)
	{
		dwColor = FB_SHIELD;
	}
	else if (bIsThrow)
	{
		if (!P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
		}
		dwColor = FB_THROW_ACTIVE;
	}

	if (P.PlayerData->animationDataPtr->highestNonHitboxIndex == 12) //Clash
	{
		dwColor2 = FB_CLASH;
	}
	else if (P.PlayerData->animationDataPtr->highestNonHitboxIndex <= 1 || //Various forms of invuln
		P.PlayerData->strikeInvuln != 0 ||
		P.PlayerData->animationDataPtr->stateData->invincibility == 3)
	{
		dwColor2 = FB_INVULN;
	}
	else if (P.PlayerData->animationDataPtr->highestNonHitboxIndex == 10) //Special Box 1
	{
		dwColor2 = FB_COUNTER;
	}

	if (P.PlayerData->animationDataPtr->stateData->stance == 1) //Airborne
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

	if (Assist.PlayerData->attackDataPtr != 0) //Check Assist for active
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
	if (P.PlayerData->attackDataPtr && P.PlayerData->hitstop == 0 && P.PlayerData->heatTimeCounter != P.nLastFrameCount)
	{
		P.nActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
	}
	else if (P.PlayerData->attackDataPtr == 0)
	{
		P.nActiveCounter = 0;
	}
}

void IncrementFirstActive(Player& P1, Player& P2)
{
	if (P1.PlayerData->hitstop == 0 &&
		//*(int*)(adMBAABase + adP1Freeze) == 0 &&
		//*(int*)(adMBAABase + adP2Freeze) == 0 &&
		//*(char*)(adMBAABase + adGlobalFreeze) == 0 &&
		P1.PlayerData->heatTimeCounter != P1.nLastFrameCount &&
		P2.PlayerData->heatTimeCounter != P2.nLastFrameCount)
	{
		P1.nFirstActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		//P1.bAddPlayerFreeze = true;
	}
	//if (P1.bAddPlayerFreeze &&
	//	(*(int*)(adMBAABase + adP1Freeze) != 0 ||
	//	*(int*)(adMBAABase + adP2Freeze) != 0) &&
	//	P1.PlayerData->heatTimeThisHeat != P.nLastFrameCount)
	//{
	//	P1.nFirstActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
	//	P1.bAddPlayerFreeze = false;
	//}
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

	bool DoBarUpdate = (
		*(int*)(P1.adInaction) != 0 ||
		*(int*)(P2.adInaction) != 0||
		P1.PlayerData->animationDataPtr->stateData->flagset2 != 0||
		P2.PlayerData->animationDataPtr->stateData->flagset2 != 0||
		P1.bProjectileActive != 0 ||
		P2.bProjectileActive != 0 ||
		P1Assist.PlayerData->attackDataPtr != 0 ||
		P2Assist.PlayerData->attackDataPtr != 0 ||
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
		if (P1.PlayerData->hitstop != 0 &&
			P2.PlayerData->hitstop != 0) //Player hitstop values count down but we need it to count up
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

		IncrementFirstActive(P1, P2);
		IncrementFirstActive(P2, P1);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			if (P1Assist.PlayerData->exists)
			{
				//IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);
			}
			if (P2Assist.PlayerData->exists)
			{
				//IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);
			}
			nBarCounter += *(int*)(adMBAABase + adTrueFrameCount) - nLastTrueFrameCount;
			if (nBarCounter < 0) {
				nBarCounter = 0;
			}
		}
	}
}

void FrameBar(Player& P1, Player& P2, Player& P3, Player& P4)
{
	bDisplayFreeze = nSHOW_FREEZE_AND_INPUTS;
	bDisplayInputs = nSHOW_FREEZE_AND_INPUTS;
	nBarScrolling = nTRUE_SCROLL_DISPLAY;

	Main1 = &P1;
	Main2 = &P2;
	Assist1 = &P3;
	Assist2 = &P4;
	if (P1.PlayerData->tagFlag)
	{
		Main1 = &P3;
		Assist1 = &P1;
	}
	if (P2.PlayerData->tagFlag)
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

	nPlayerAdvantage = (Main1->nAdvantageCounter - Main2->nAdvantageCounter) % 100;

	UpdatePlayers();

	nLastFrameCount = *(int*)(adMBAABase + adFrameCount);
	nLastTrueFrameCount = *(int*)(adMBAABase + adTrueFrameCount);

}
