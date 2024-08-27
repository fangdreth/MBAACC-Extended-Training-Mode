#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "..\Common\Common.h"

int nLastFrameCount = 0; //Counts slower during slowdown
int nLastTrueFrameCount = 0; //Counts all frames during slowdown

int nBarCounter = 0;
int sBarScrolling = 0;
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

int nSaveTextTimer = 0;
int nClearTextTimer = 0;
int nClearSaveTimer = 0;

struct Player
{
	char Port = 0;

	DWORD adPlayerBase = 0x0;
	DWORD adInaction = 0x0;

	int nLastInactionableFrames = 0;

	DWORD dwColorBar1[BAR_MEMORY_SIZE][2];
	DWORD dwColorBar2[BAR_MEMORY_SIZE][2];

	int nNumBar[BAR_MEMORY_SIZE][2];

	int nActiveCounter = 0;
	int nInactiveMemory = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
	bool bLastOnRight = false;
	DWORD dwLastActivePointer = 0x0;
	char cLastHitstop = 0;
};

void UpdatePlayer(Player& P) {
	P.nLastInactionableFrames = *(int*)(P.adInaction);
	P.nLastFrameCount = *(int*)(P.adPlayerBase + adPlayerFrameCount);
	P.bLastOnRight = *(int*)(P.adPlayerBase + adOnRightFlag);
	P.dwLastActivePointer = *(DWORD*)(P.adPlayerBase + adAttackDataPointer);
	P.cLastHitstop = *(char*)(P.adPlayerBase + adHitstop);
}

Player P1{ 0, adMBAABase + adP1Base, adMBAABase + adP1Inaction };
Player P2{ 1, adMBAABase + adP2Base, adMBAABase + adP2Inaction };
Player P3{ 2, adMBAABase + adP3Base, adMBAABase + adP1Inaction };
Player P4{ 3, adMBAABase + adP4Base, adMBAABase + adP2Inaction };

Player* Player1 = &P1;
Player* Player2 = &P2;
Player* Player3 = &P3;
Player* Player4 = &P4;

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
			P1.nAdvantageCounter++;
		}
		else if (*(int*)(P2.adInaction) == 0 && *(int*)(P1.adInaction) != 0)
		{
			P2.nAdvantageCounter++;
		}
	}
}

void ResetBars(Player& P)
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	sBarScrolling = 0;
	bDoBarReset = false;
	nBarIntervalMax = DISPLAY_RANGE;
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

void UpdateBars(Player& P, Player& Assist)
{
	DWORD dwColor = 0x00000000;
	DWORD dwColor2 = 0x00000000;
	DWORD dwBar2Color0 = 0x00000000;
	DWORD dwBar2Color1 = 0x00000000;
	int nNumber = -1;
	int nNumFlag = 0;
	bool bClearNumFlag = true;
	bool bIsButtonPressed = *(char*)(P.adPlayerBase + adButtonInput) != 0 || *(char*)(P.adPlayerBase + adMacroInput) != 0;

	//Bar 1 - General action information
	if (*(int*)(P.adInaction) != 0) //Doing something with limited actionability
	{
		dwColor = 0xFF41C800;
		//sBarValue = std::format("{:2}", P.nInactionableFrames % 100);
		nNumber = *(int*)P.adInaction;
		if (*(int*)(P.adPlayerBase + adPattern) >= 35 && *(int*)(P.adPlayerBase + adPattern) <= 37) //Jump Startup
		{
			dwColor = 0xFFF1E084;
		}
		else if (*(int*)(P.adPlayerBase + adHitstunRemaining) != 0 && *(char*)(P.adPlayerBase + adBlockstunFlag) == 0) //Hitstun
		{
			dwColor = 0xFF8C8C8C;
			if (*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimStateDataPointer) + adStateStance) == 1) //Airborne
			{
				if (*(short*)(P.adPlayerBase + adUntechElapsed) < *(short*)(P.adPlayerBase + adUntechTotal)) //Still has untech remaining
				{
					//sBarValue = std::format("{:2}", (P.sUntechTotal - P.sUntechCounter) % 100);
					nNumber = *(short*)(P.adPlayerBase + adUntechTotal) - *(short*)(P.adPlayerBase + adUntechElapsed);
				}
			}
			else //Grounded
			{
				if (*(int*)(P.adPlayerBase + adHitstunRemaining) > 2) //Still has hitstun remaining
				{
					//sBarValue = std::format("{:2}", P.nHitstunRemaining - 1 % 100);
					nNumber = *(short*)(P.adPlayerBase + adHitstunRemaining) - 1;
				}
			}
		}
		else if (*(char*)(P.adPlayerBase + adBlockstunFlag)) //Blockstun
		{
			dwColor = 0xFFB4B4B4;
		}
		else if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0) //Attacking
		{
			dwColor = 0xFFFF0000;
			if (P.dwLastActivePointer == 0x0) //First active only
			{
				nNumFlag = 1;
			}
		}
	}
	else //Fully actionable
	{
		dwColor = 0xFF202020;
		//sBarValue = std::format("{:2}", P.nPattern % 100);

		if (bDoAdvantage) //Has advantage
		{
			//sBarValue = std::format("{:2}", P.nAdvantageCounter % 100);
			nNumber = P.nAdvantageCounter;
			if (P.nAdvantageCounter == 1)
			{
				bClearNumFlag = false;
			}
			if (P.nAdvantageCounter != 0)
			{
				dwColor = 0xFF101010;
			}
		}

		if (P.nLastInactionableFrames != 0) //Neutral frame
		{
			dwColor = 0xFF205A00;
		}
	}

	if (*(char*)(P.adPlayerBase + adThrowFlag) != 0) //Being thrown
	{
		dwColor = 0xFF6E6E6E;
		//sBarValue = " t";
	}
	else if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimBoxIndex) == 10) //Shield
	{
		dwColor = 0xFF91C2FF;
	}

	if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimBoxIndex) == 12) //Clash
	{
		dwColor2 = 0xFFE1B800;
	}
	else if (*(char*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimBoxIndex) <= 1 || //Various forms of invuln
		*(char*)(P.adPlayerBase + adStrikeInvuln) != 0 ||
		*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimStateDataPointer) + adStateInvuln) == 3)
	{
		dwColor2 = 0xFFFFFFFF;
	}

	if (*(char*)(adMBAABase + adGlobalFreeze) != 0 || //Screen is frozen
		*(int*)(adMBAABase + adP1Freeze) != 0 ||
		*(int*)(adMBAABase + adP2Freeze) != 0)
	{
		dwColor = 0xFF3C3C3C;
	}
	else if (*(char*)(P.adPlayerBase + adHitstop) != 0) //in hitstop
	{
		dwColor = 0xFF3C5080;
		if (*(char*)(P.adPlayerBase + adNotInCombo) == 0 && P.cLastHitstop == 0)
		{
			nNumFlag = 1;
		}
	}

	if (*(char*)(*(DWORD*)(*(DWORD*)(P.adPlayerBase + adAnimationDataPointer) + adAnimStateDataPointer) + adStateStance) == 1) //Airborne
	{
		dwBar2Color0 = 0xFFF1E084;
	}

	int nCharacterID = 0;
	if (P.Port % 2 == 0)
	{
		nCharacterID = *(char*)(adMBAABase + dwP1CharNumber) * 10 + *(char*)(adMBAABase + dwP1CharMoon);
	}
	else
	{
		nCharacterID = *(char*)(adMBAABase + dwP2CharNumber) * 10 + *(char*)(adMBAABase + dwP2CharMoon);
	}
	std::map<std::string, int> CharacterMap = MBAACC_Map[nCharacterID];
	for (int i = 0; i < 1000; i++) //Check Projectiles for active
	{
		bool validProj = true;
		if (*(char*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i) != 0 &&
			*(DWORD*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i + adAttackDataPointer) != 0 &&
			*(char*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i + adProjSource) >= 0 &&
			*(char*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i + adProjOwner) == P.Port &&
			*(int*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i + adPattern) >= 60)
		{
			for (auto const& [key, val] : CharacterMap)
			{
				if (*(int*)((adMBAABase + adProjectileBase) + dwProjectileStructSize * i + adPattern) == val)
				{
					validProj = false;
					break;
				}
			}
			if (validProj)
			{
				dwBar2Color1 = 0xFFFF0000;
			}
		}
	}

	if (*(DWORD*)(Assist.adPlayerBase + adAttackDataPointer) != 0) //Check Assist for active
	{
		dwBar2Color1 = 0xFFFF8000;
	}

	P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][0] = dwColor;
	if (dwColor2 != 0x0)
	{
		P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][1] = dwColor2;
	}
	else
	{
		P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][1] = dwColor;
	}

	P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][0] = dwBar2Color0;
	P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][1] = dwBar2Color1;

	P.nNumBar[nBarCounter % BAR_MEMORY_SIZE][0] = nNumber;
	P.nNumBar[nBarCounter % BAR_MEMORY_SIZE][1] = nNumFlag;
	if (nNumber >= 0 && bClearNumFlag && P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][1] == 0)
	{
		P.nNumBar[(nBarCounter - 1) % BAR_MEMORY_SIZE][0] = -1;
	}

}

void IncrementActive(Player& P)
{
	if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) != 0 && *(DWORD*)(P.adPlayerBase + adHitstop) == 0)
	{
		P.nActiveCounter++;
	}
	else if (*(DWORD*)(P.adPlayerBase + adAttackDataPointer) == 0)
	{
		P.nActiveCounter = 0;
	}
}

void HandleInactive(Player& P)
{
	if (*(int*)(P.adInaction) != 0)
	{
		P.nInactiveMemory = *(int*)(P.adInaction);
	}
}

void BarHandling(Player& P1, Player& P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool IsInput = (
		*(DWORD*)(P1.adPlayerBase + adRawInput) != 0 ||
		*(DWORD*)(P2.adPlayerBase + adRawInput) != 0
		); //True if player or dummy has any input
	bool DoBarUpdate = (
		*(int*)(P1.adInaction) != 0 ||
		*(int*)(P2.adInaction) ||
		*(DWORD*)(*(DWORD*)(*(DWORD*)(P1.adPlayerBase + adAnimationDataPointer) + adAnimStateDataPointer) + adStateFlagset2) != 0 ||
		*(DWORD*)(*(DWORD*)(*(DWORD*)(P2.adPlayerBase + adAnimationDataPointer) + adAnimStateDataPointer) + adStateFlagset2) != 0 ||
		P1.nActiveProjectileCount != 0 ||
		P2.nActiveProjectileCount != 0 ||
		*(DWORD*)(P1Assist.adPlayerBase + adAttackDataPointer) != 0 ||
		*(DWORD*)(P2Assist.adPlayerBase + adAttackDataPointer) != 0 ||
		P1Assist.nActiveProjectileCount != 0 ||
		P2Assist.nActiveProjectileCount != 0
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
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P1Assist);
		ResetBars(P2Assist);
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
				IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);
			}
			if (*(char*)(P2Assist.adPlayerBase))
			{
				IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);
			}
			nBarCounter++;
		}
	}
}

void FrameBar(Player& P1, Player& P2, Player& P3, Player& P4)
{
	bDisplayFreeze = *(char*)(adMBAABase + adSharedDisplayFreeze);
	bDisplayInputs = *(char*)(adMBAABase + adSharedDisplayInputs);
	sBarScrolling = *(short*)(adMBAABase + adSharedScrolling);

	Player1 = &P1;
	Player2 = &P2;
	Player3 = &P3;
	Player4 = &P4;
	if (*(char*)(P1.adPlayerBase + adTagFlag))
	{
		Player1 = &P3;
		Player3 = &P1;
	}
	if (*(char*)(P2.adPlayerBase + adTagFlag))
	{
		Player2 = &P4;
		Player4 = &P2;
	}

	if (*(int*)(adMBAABase + adTrueFrameCount) != nLastTrueFrameCount)
	{
		BarHandling(*Player1, *Player2, *Player3, *Player4);
	}

	if (*(int*)(adMBAABase + adTrueFrameCount) == 1)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P3);
		ResetBars(P4);
	}

	nPlayerAdvantage = (P1.nAdvantageCounter - P2.nAdvantageCounter) % 100;

	UpdatePlayer(P1);
	UpdatePlayer(P2);
	UpdatePlayer(P3);
	UpdatePlayer(P4);

	nLastFrameCount = *(int*)(adMBAABase + adFrameCount);
	nLastTrueFrameCount = *(int*)(adMBAABase + adTrueFrameCount);
}
